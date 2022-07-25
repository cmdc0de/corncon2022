#include <stdlib.h>
#include <device/display/display_device.h>
#include "device/display/color.h"
#include "esp_netif_types.h"
#include "main_nav.h"
#include "menu_state.h"
#include "../app.h"
#include "freertos.h"

using libesp::RGBColor;
using libesp::FreeRTOS;
using libesp::ErrorType;
using libesp::BaseMenu;

static StaticQueue_t InternalQueue;
static uint8_t InternalQueueBuffer[MainNav::QUEUE_SIZE*MainNav::MSG_SIZE] = {0};
static libesp::DCImage MainMap;

MainNav::MainNav() : AppBaseMenu() {
	InternalQueueHandler = xQueueCreateStatic(QUEUE_SIZE,MSG_SIZE,&InternalQueueBuffer[0],&InternalQueue);
   MainMap.bytes_per_pixel = 2;
   MainMap.height = 128;
   MainMap.width = 110;
   extern char main_map_start[] asm("_binary_mainmap_map_start");
   extern char main_map_end[]   asm("_binary_mainmap_map_end");
   ESP_LOGI(LOGTAG,"size of mainmap %d",(main_map_end-main_map_start));
   MainMap.pixel_data = &main_map_start[0];
}

MainNav::~MainNav() {

}

enum INTERNAL_STATE {
	INIT, RENDER
};

static INTERNAL_STATE InternalState = INIT;

ErrorType MainNav::onInit() {
	InternalState = INIT;
   MyApp::get().getDisplay().fillScreen(RGBColor::BLACK);
	return ErrorType();
}

BaseMenu::ReturnStateContext MainNav::onRun() {
	BaseMenu::ReturnStateContext sr(this);
   MyApp::get().getDisplay().fillScreen(RGBColor::BLACK);
   MyApp::get().getDisplay().drawImage(0,0,MainMap);
	return sr;
}

ErrorType MainNav::onShutdown() {
	return ErrorType();
}

