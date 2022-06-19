/*
 * app.cpp
 *
 * Author: cmdc0de
 */

#include "app.h"
#include <esp_log.h>
#include <system.h>
#include <spibus.h>

#include <driver/uart.h>
#include <driver/gpio.h>
#include <device/display/frame_buffer.h>
#include <device/display/display_device.h>
#include <device/display/fonts.h>
#include <device/display/gui.h>
#include <device/touch/XPT2046.h>
#include "menus/menu_state.h"
#include "menus/calibration_menu.h"
#include "menus/game_of_life.h"
#include "menus/wifi_menu.h"
#include "menus/setting_menu.h"
#include "menus/menu3d.h"
#include <app/display_message_state.h>
#include "device/leds/apa102.h"
#include "spibus.h"
#include "freertos.h"
#include "fatfsvfs.h"
#include "pinconfig.h"
#include <device/sensor/dht11.h>
#include "appmsg.h"
#include <math/point.h>
#include <esp_spiffs.h>
#include <time.h>

using libesp::ErrorType;
using libesp::System;
using libesp::FreeRTOS;
using libesp::RGBB;
using libesp::RGBColor;
using libesp::APA102c;
using libesp::SPIBus;
using libesp::DisplayILI9341;
using libesp::XPT2046;
using libesp::GUI;
using libesp::DisplayMessageState;
using libesp::BaseMenu;
using libesp::Point2Ds;

const char *MyApp::LOGTAG = "AppTask";
const char *MyApp::sYES = "Yes";
const char *MyApp::sNO = "No";

#define START_ROT libesp::DisplayILI9341::LANDSCAPE_TOP_LEFT
static const uint16_t PARALLEL_LINES = 1;

libesp::DisplayILI9341 Display(MyApp::DISPLAY_WIDTH,MyApp::DISPLAY_HEIGHT,START_ROT, PIN_NUM_DISPLAY_BACKLIGHT, PIN_NUM_DISPLAY_RESET);

static uint16_t BkBuffer[MyApp::FRAME_BUFFER_WIDTH*MyApp::FRAME_BUFFER_HEIGHT];
static uint16_t *BackBuffer = &BkBuffer[0];

uint16_t ParallelLinesBuffer[MyApp::DISPLAY_WIDTH*PARALLEL_LINES] = {0};

libesp::ScalingBuffer FrameBuf(&Display, MyApp::FRAME_BUFFER_WIDTH, MyApp::FRAME_BUFFER_HEIGHT, uint8_t(16), MyApp::DISPLAY_WIDTH
    ,MyApp::DISPLAY_HEIGHT, PARALLEL_LINES, (uint8_t*)&BackBuffer[0],(uint8_t*)&ParallelLinesBuffer[0]);

static GUI MyGui(&Display);
static CalibrationMenu MyCalibrationMenu("nvs");
static WiFiMenu MyWiFiMenu;

static libesp::AABBox2D Close(Point2Ds(185,7),6);
static libesp::Button CloseButton((const char *)"X", MyApp::CLOSE_BTN_ID, &Close,RGBColor::RED, RGBColor::BLUE);

const char *MyErrorMap::toString(int32_t err) {
	return "TODO";
}

MyApp MyApp::mSelf;
static StaticQueue_t InternalQueue;
static uint8_t InternalQueueBuffer[MyApp::QUEUE_SIZE*MyApp::MSG_SIZE] = {0};
static StaticSemaphore_t xMutexDisplayTouchBuffer;

MyApp &MyApp::get() {
	return mSelf;
}

MyApp::MyApp() : AppErrors(), CurrentMode(ONE), LastTime(0) ,DHT22T()
                 , InternalQueueHandler(0), Temperature(0.0f), Humidity(0.0f), MHZ19T(), CO2(0)
                 , NVSStorage("appdata","data",false), LSensorResult(), DisplayTouchSemaphore(nullptr)
                 , ConfigStore(&NVSStorage), LastConnectCheck(0) {
	ErrorType::setAppDetail(&AppErrors);
}

MyApp::~MyApp() {

}

libesp::Button &MyApp::getCloseButton() {
  return CloseButton;
}

uint8_t *MyApp::getBackBuffer() {
	return (uint8_t *)&BackBuffer[0];
}

uint32_t MyApp::getBackBufferSize() {
	return MyApp::FRAME_BUFFER_WIDTH*MyApp::FRAME_BUFFER_HEIGHT*2;
}
  
static RGBB leds[256];
static size_t NumLEDs = sizeof(leds)/sizeof(leds[0]);
libesp::APA102c LedControl;

ErrorType MyApp::initFS() {
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/www",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = false
    };
    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(LOGTAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(LOGTAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(LOGTAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return ESP_FAIL;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(LOGTAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    } else {
        ESP_LOGI(LOGTAG, "Partition size: total: %d, used: %d", total, used);
    }
    return ESP_OK;
}

uint32_t LastMotionDetect = 0;

static void gpio_isr_handler(void* arg) {
  LastMotionDetect = FreeRTOS::getTimeSinceStart();
  printf("motion");
}

bool MyApp::wasMotion() {
  return ((FreeRTOS::getTimeSinceStart()-LastMotionDetect)<TIME_MOTION_DETECT);
}

libesp::ErrorType MyApp::initMotionSensor() {
	gpio_config_t io_conf;
  memset(&io_conf,0,sizeof(io_conf));
	//interrupt of rising edge
	io_conf.intr_type = GPIO_INTR_POSEDGE;
	io_conf.pin_bit_mask = (1ULL << PIN_NUM_IR_OUTPUT_IRQ);
	//set as input mode    
	io_conf.mode = GPIO_MODE_INPUT;
	//enable pull-up mode
	io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
	io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
	gpio_config(&io_conf);
	//start gpio task
	gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
	//hook isr handler for specific gpio pin
	return gpio_isr_handler_add(PIN_NUM_IR_OUTPUT_IRQ, gpio_isr_handler, (void*) (1ULL << PIN_NUM_IR_OUTPUT_IRQ));
}


libesp::ErrorType MyApp::onInit() {
	ErrorType et;

	InternalQueueHandler = xQueueCreateStatic(QUEUE_SIZE,MSG_SIZE,&InternalQueueBuffer[0],&InternalQueue);

	ESP_LOGI(LOGTAG,"OnInit: Free: %u, Min %u", System::get().getFreeHeapSize(),System::get().getMinimumFreeHeapSize());
  et = getConfig().init();
  if(!et.ok()) {
    ESP_LOGE(LOGTAG,"Failed to init config: %d %s", et.getErrT(), et.toString());
  }

	ESP_LOGI(LOGTAG,"OnInit: Free: %u, Min %u", System::get().getFreeHeapSize(),System::get().getMinimumFreeHeapSize());

  DisplayTouchSemaphore = xSemaphoreCreateMutexStatic(&xMutexDisplayTouchBuffer);
	ESP_LOGI(LOGTAG,"OnInit: Free: %u, Min %u", System::get().getFreeHeapSize(),System::get().getMinimumFreeHeapSize());
  initFS();
	ESP_LOGI(LOGTAG,"OnInit: Free: %u, Min %u", System::get().getFreeHeapSize(),System::get().getMinimumFreeHeapSize());
	et = NVSStorage.init();
	if(!et.ok()) {
		ESP_LOGI(LOGTAG, "1st InitNVS failed bc %s\n", et.toString());
		et = NVSStorage.initStorage();
		if(et.ok()) {
      ESP_LOGI(LOGTAG, "initStorage succeeded");
			et = NVSStorage.init();
      if(et.ok()) {
        ESP_LOGI(LOGTAG, "NVSSTorage init successful");
      } else {
		    ESP_LOGE(LOGTAG, "2nd InitNVS failed bc %s\nTHIS IS A PROBLEM\n", et.toString());
      }
		} else {
			ESP_LOGI(LOGTAG, "initStorage failed %s\n", et.toString());
		}
	}
	ESP_LOGI(LOGTAG,"OnInit: Free: %u, Min %u", System::get().getFreeHeapSize(),System::get().getMinimumFreeHeapSize());

  et = APA102c::initAPA102c(PIN_NUM_LEDS_MOSI, PIN_NUM_LEDS_CLK, SPI2_HOST, SPI_DMA_CH1);
  if(!et.ok()) {
    return et;
  } else {
    ESP_LOGI(LOGTAG,"APA102c inited");
  }

	ESP_LOGI(LOGTAG,"OnInit: Free: %u, Min %u", System::get().getFreeHeapSize(),System::get().getMinimumFreeHeapSize());

	ESP_LOGI(LOGTAG,"OnInit: Free: %u, Min %u", System::get().getFreeHeapSize(),System::get().getMinimumFreeHeapSize());
	SPIBus *vbus = SPIBus::get(SPI2_HOST);
  et = LedControl.initDevice(vbus);

  et = MyCalibrationMenu.initNVS();
	if(!et.ok()) {
		ESP_LOGE(LOGTAG,"failed to init nvs for calibration");
		return et;
	}

  //NOT initializing XPT as we don't need to set up spi bus since we are sharing with display
	et = XPT2046::initTouch(PIN_NUM_TOUCH_MISO, PIN_NUM_TOUCH_MOSI, PIN_NUM_TOUCH_CLK, SPI3_HOST, SPI_DMA_DISABLED);
	if(!et.ok()) {
		ESP_LOGE(LOGTAG,"failed to touch");
		return et;
	}

  //this will init the SPI bus and the display
  DisplayILI9341::initDisplay(PIN_NUM_DISPLAY_MISO, PIN_NUM_DISPLAY_MOSI,
    PIN_NUM_DISPLAY_SCK, SPI_DMA_CH2, PIN_NUM_DISPLAY_DATA_CMD, PIN_NUM_DISPLAY_RESET,
    PIN_NUM_DISPLAY_BACKLIGHT, SPI3_HOST);

  ESP_LOGI(LOGTAG,"After Display: Free: %u, Min %u", System::get().getFreeHeapSize()
    ,System::get().getMinimumFreeHeapSize());

  SPIBus *hbus = libesp::SPIBus::get(SPI3_HOST);

	FrameBuf.createInitDevice(hbus,PIN_NUM_DISPLAY_CS,PIN_NUM_DISPLAY_DATA_CMD, DisplayTouchSemaphore);
	
	ESP_LOGI(LOGTAG,"After FrameBuf: Free: %u, Min %u", System::get().getFreeHeapSize(),System::get().getMinimumFreeHeapSize());

	ESP_LOGI(LOGTAG,"start display init");
	et=Display.init(libesp::DisplayILI9341::FORMAT_16_BIT, &Font_6x10, &FrameBuf);

  if(et.ok()) {
		ESP_LOGI(LOGTAG,"display init OK");
		Display.fillScreen(libesp::RGBColor::BLACK);
		Display.swap();
		ESP_LOGI(LOGTAG,"fill black done");
		Display.fillRec(0,0,FRAME_BUFFER_WIDTH/4,10,libesp::RGBColor::RED);
		Display.swap();
		vTaskDelay(500 / portTICK_RATE_MS);
		Display.fillRec(0,15,FRAME_BUFFER_WIDTH/2,10,libesp::RGBColor::WHITE);
		Display.swap();
		vTaskDelay(500 / portTICK_RATE_MS);
		Display.fillRec(0,30,FRAME_BUFFER_WIDTH,10,libesp::RGBColor::BLUE);
		Display.swap();
		vTaskDelay(500 / portTICK_RATE_MS);
		Display.drawRec(0,60,FRAME_BUFFER_WIDTH/2,20, libesp::RGBColor::GREEN);
		Display.drawString(15,110,"Color Validation.",libesp::RGBColor::RED);
		Display.drawString(30,120,"Sensor Clock",libesp::RGBColor::BLUE, libesp::RGBColor::WHITE,1,false);
		Display.swap();

		vTaskDelay(1000 / portTICK_RATE_MS);
		ESP_LOGI(LOGTAG,"After Display swap:Free: %u, Min %u",System::get().getFreeHeapSize(),System::get().getMinimumFreeHeapSize());

	} else {
		ESP_LOGE(LOGTAG,"failed display init");
	}

	ESP_LOGI(LOGTAG,"After Touch Task starts: Free: %u, Min %u", System::get().getFreeHeapSize(),System::get().getMinimumFreeHeapSize());

  if(DHT22T.init(PIN_NUM_DHT_DATA).ok()) {
    DHT22T.start();
	  ESP_LOGI(LOGTAG,"After DHT22 Task starts: Free: %u, Min %u", System::get().getFreeHeapSize(),System::get().getMinimumFreeHeapSize());
  } else {
    ESP_LOGE(LOGTAG,"Failed to initialize DHT22 Task");
  }


  //init MHZ19
  //PIN_NUM_ESP_TX_MHZ19B_RX
  //PIN_NUM_ESP_RX_MHZ19B_TX
  if(!MHZ19T.init(UART_NUM_2, PIN_NUM_ESP_RX_MHZ19B_TX, PIN_NUM_ESP_TX_MHZ19B_RX).ok()) {
    ESP_LOGE(LOGTAG,"Failed to initialize MHZ19B ");
  } else {
    ESP_LOGI(LOGTAG,"MHZ19 initialized");
    MHZ19T.start();
  }

#define USE_MOTION_SENSOR
#ifdef  USE_MOTION_SENSOR
  et = initMotionSensor();
  if(!et.ok()) {
    ESP_LOGI(LOGTAG,"failed to start motion sensor");
  } else {
    ESP_LOGI(LOGTAG,"motion sensor started");
  }
#endif

  et = MyWiFiMenu.initWiFi();
  if(et.ok()) {
    ESP_LOGI(LOGTAG,"OnInit:After MyWiFiMenu::initWiFi: Free: %u, Min %u", System::get().getFreeHeapSize(),System::get().getMinimumFreeHeapSize());
  } else {
    ESP_LOGE(LOGTAG,"Error Num :%d Msg: %s", et.getErrT(), et.toString());
  }

  if(!MyCalibrationMenu.hasBeenCalibrated()) {
		setCurrentMenu(getCalibrationMenu());
	} else {
    ESP_LOGI(LOGTAG,"***********************************************************");
    if(MyWiFiMenu.hasWiFiBeenSetup().ok()) {
      et = MyWiFiMenu.connect();
  		setCurrentMenu(getMenuState());
    } else {
      setCurrentMenu(getSettingMenu());
    }
	}

	return et;
}

void MyApp::handleMessages() {
  MyAppMsg *msg = nullptr;
  int counter = 0;
	while(counter++<5 && xQueueReceive(InternalQueueHandler, &msg, 0)) {
    if(msg!=nullptr) {
      if(msg->handleMessage(this)) {
  		  delete msg;
      }
    }
  }
}

static uint8_t MinIndex[60] = 
{
   4, 5 ,6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,
  34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59, 0, 1, 2, 3,  
};

static uint8_t SecIndex[60] = 
{
//   1,  2,  3,  4, 5, 6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
    64, 65, 66, 67,68,69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 
    94, 95, 96, 97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119, 60, 61, 62, 63
};

ErrorType MyApp::onRun() {
  ErrorType et;
  handleMessages();
	libesp::BaseMenu::ReturnStateContext rsc = getCurrentMenu()->run();
	Display.swap();

	if (rsc.Err.ok()) {
		if (getCurrentMenu() != rsc.NextMenuToRun) {
			setCurrentMenu(rsc.NextMenuToRun);
			ESP_LOGI(LOGTAG,"on Menu swap: Free: %u, Min %u",
				System::get().getFreeHeapSize(),System::get().getMinimumFreeHeapSize());
		} else {
		}
	} 

  uint32_t timeSinceLast = FreeRTOS::getTimeSinceStart()-LastTime;

  if(timeSinceLast>=TIME_BETWEEN_PULSES) {
    LastTime = FreeRTOS::getTimeSinceStart();

    switch(CurrentMode) {
    case ONE:
      {
        CurrentMode = TWO;
      }
      break;
    case TWO:
      {
        CurrentMode = THREE;
      }
      break;
    case THREE:
      {
        CurrentMode = FOUR;
      }
      break;
    case FOUR:
      {
      }
      break;
    }
  }
	return et;
}

uint16_t MyApp::getCanvasWidth() {
	return FrameBuf.getBufferWidth(); 
}

uint16_t MyApp::getCanvasHeight() {
	return FrameBuf.getBufferHeight();
}

uint16_t MyApp::getLastCanvasWidthPixel() {
	return getCanvasWidth()-1;
}

uint16_t MyApp::getLastCanvasHeightPixel() {
	return getCanvasHeight()-1;
}

libesp::DisplayDevice &MyApp::getDisplay() {
	return Display;
}

libesp::GUI &MyApp::getGUI() {
	return MyGui;
}

MenuState MyMenuState;
libesp::DisplayMessageState DMS;
SettingMenu MySettingMenu;
GameOfLife GOL;
Menu3D Menu3DRender( uint8_t(float(MyApp::FRAME_BUFFER_WIDTH)*0.8f) , uint8_t(float(MyApp::FRAME_BUFFER_HEIGHT)*0.8f));

Menu3D *MyApp::getMenu3D() {
  return &Menu3DRender;
}

GameOfLife *MyApp::getGameOfLife() {
  return &GOL;
}

MenuState *MyApp::getMenuState() {
	return &MyMenuState;
}

SettingMenu *MyApp::getSettingMenu() {
	return &MySettingMenu;
}

DisplayMessageState *MyApp::getDisplayMessageState(BaseMenu *bm, const char *msg, uint32_t msDisplay) {
	DMS.setMessage(msg);
	DMS.setNextState(bm);
	DMS.setTimeInState(msDisplay);
	DMS.setDisplay(&Display);
	return &DMS;
}

