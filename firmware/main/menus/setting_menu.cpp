/*
 * setting_state.cpp
 *
 *      Author: cmdc0de
 */

#include "setting_menu.h"
#include <device/display/display_device.h>
#include "menu_state.h"
#include "../app.h"
#include <esp_log.h>
//#include "wifi_menu.h"
#include <app/display_message_state.h>
#include "game_of_life.h"
#include "menu3d.h"
#include <system.h>
#include "../vkeyboard.h"

using libesp::ErrorType;
using libesp::BaseMenu;
using libesp::RGBColor;
using libesp::Button;

static StaticQueue_t ButtonQueue;
static uint8_t QueueBuffer[SettingMenu::QUEUE_SIZE*SettingMenu::MSG_SIZE] = {0};
const char *SettingMenu::LOGTAG = "SettingMenu";
VKeyboard VB;

SettingMenu::SettingMenu() : AppBaseMenu(), QueueHandle() 
	, InternalState(INTERNAL_STATE::SHOW_ALL), MenuList("Setting (jump saves)", Items, 0, 0, MyApp::get().getCanvasWidth()
   , MyApp::get().getCanvasHeight(), 0, ItemCount), Name(), Position(0) {
	
	QueueHandle = xQueueCreateStatic(QUEUE_SIZE,MSG_SIZE,&QueueBuffer[0],&ButtonQueue);
}

SettingMenu::~SettingMenu() {

}

enum STATES {
  INIT
  , ENTER_NAME
  , ENTER_NUMBER
  , ENTER_BOOL
};
static STATES State = INIT;
static bool BValue = true;

ErrorType SettingMenu::onInit() {
   memset(&Name[0],0,sizeof(Name));
   if(MyApp::get().getConfig().isNameSet()) {
      strcpy(&Name[0],MyApp::get().getConfig().getName());
   }
   BValue = MyApp::get().getConfig().ledsEnabled();
	ButtonManagerEvent* *pe = nullptr;
	for(int i=0;i<2;i++) {
		if(xQueueReceive(QueueHandle, &pe, 0)) {
			delete pe;
		}
	}
   Items[0].id = 0;
   sprintf(getRow(0),"Name: %s", &Name[0]);
   Items[0].text = getRow(0);
	Items[1].id = 1;
   sprintf(getRow(1),"Set Sleep Time: %d", MyApp::get().getConfig().getSleepMin());
	Items[1].text = getRow(1);
	Items[2].id = 2;
   sprintf(getRow(2),"LEDs Enabled: %s", (const char *)(MyApp::get().getConfig().ledsEnabled()?"No":"Yes"));
	Items[2].text = getRow(2);
	MyApp::get().getDisplay().fillScreen(RGBColor::BLACK);
   MyApp::get().getGUI().drawList(&this->MenuList);
	MyApp::get().getButtonMgr().addObserver(QueueHandle);
   State = INIT;
	return ErrorType();
}


BaseMenu::ReturnStateContext SettingMenu::onRun() {
	BaseMenu *nextState = this;
   ButtonManagerEvent *bme = nullptr;
	if(xQueueReceive(QueueHandle, &bme, 0)) {
		//ESP_LOGI(LOGTAG,"que");
      if(bme->wasReleased()) {
         switch(bme->getButton()) {
            case PIN_NUM_FIRE_BTN:
               {
                  switch(State) {
                  case INIT:
                     Position = 0;
                     memset(&Name[0],0,sizeof(Name));
                     switch(MenuList.getSelectedItemID()) {
                        case 0:
                           State = ENTER_NAME;
                           VB.init(VKeyboard::K1,7);
                           break;
                        case 1:
                           State = ENTER_NUMBER;
                           VB.init(VKeyboard::N1,5);
                           break;
                        case 2:
                           State = ENTER_BOOL;
                           break;
                     }
                     break;
                  case ENTER_NAME:
                     Name[Position] = VB.getSelection();
                     if(++Position==AppConfig::MAX_NAME_LENGTH-1) {
                        Name[Position] = '\0';
                        ErrorType et = MyApp::get().getConfig().setName(&Name[0]);
                        if(!et.ok()) ESP_LOGE(LOGTAG,"Error saving name %s",et.toString());
                        nextState = MyApp::get().getDisplayMessageState(this, "Max Name Length reached", 2000);
                     } 
                     sprintf(getRow(0),"Name: %s", &Name[0]);
                     break;
                  case ENTER_NUMBER: 
                     {
                     int32_t v = VB.getSelection() - '0';
                     sprintf(getRow(1),"Set Sleep Time: %d", v);
                     break;
                     }
                  case ENTER_BOOL:
                     BValue = !BValue;
                     sprintf(getRow(2),"Disable LEDs: %s", (const char *)(BValue?"No":"Yes"));
                     break;
                  }
               }
            break;
            case PIN_NUM_UP_BTN: 
            {
               switch(State) {
                  case INIT:
                     MenuList.moveUp();
                     break;
                  default:
                     VB.reset();
                     break;
               }
            }
            break;
            case PIN_NUM_DOWN_BTN:
            {
               switch(State) {
                  case INIT:
                     MenuList.moveDown();
                     break;
                  default:
                     VB.reset();
                     break;
               }
            }
            break;
            case PIN_NUM_LEFT_BTN:
            {
               switch(State) {
                  case INIT:
                     MenuList.selectTop();
                     break;
                  default:
                     VB.moveLeft();
                     break;
               }
            }
            break;
            case PIN_NUM_RIGHT_BTN:
            {
               switch(State) {
                  case INIT:
                     break;
                  default:
                     VB.moveRight();
                     break;
               }
               break;
            }
            case PIN_NUM_JUMP_BTN:
            {
               switch(State) {
               case INIT:
                  nextState = MyApp::get().getMenuState();
                  break;
               case ENTER_NAME: 
                  {
                        ErrorType et = MyApp::get().getConfig().setName(&Name[0]);
                        if(!et.ok()) {
                           ESP_LOGE(LOGTAG,"Error saving name %s",et.toString());
                           nextState = MyApp::get().getDisplayMessageState(this, "Failed to save name", 2000);
                        } else {
                           nextState = MyApp::get().getDisplayMessageState(this, "Name Saved Successfully", 2000);
                        }
                  }
                  break;
               case ENTER_NUMBER: 
                  {
                        ErrorType et = MyApp::get().getConfig().setSleepMin(uint16_t(VB.getSelection()-'0'));
                        if(!et.ok()) {
                           ESP_LOGE(LOGTAG,"Error saving sleep time %s",et.toString());
                           nextState = MyApp::get().getDisplayMessageState(this, "Failed to save sleep time", 2000);
                        } else {
                           nextState = MyApp::get().getDisplayMessageState(this, "Sleep Time Saved\nSuccessfully", 2000);
                        }

                  }
                  break;
               case ENTER_BOOL:
                  {
                        ErrorType et = MyApp::get().getConfig().setLedsEnable(BValue);
                        if(!et.ok()) {
                           ESP_LOGE(LOGTAG,"Error saving led enable %s",et.toString());
                           nextState = MyApp::get().getDisplayMessageState(this, "Failed to save\nLED Enable", 2000);
                        } else {
                           nextState = MyApp::get().getDisplayMessageState(this, "LED Enable Saved\nSuccessfully", 2000);
                        }

                  }
                  break;
               default:
                  break;
               }
               break;
            }
            default:
            break;
         }
      }
   }
   MyApp::get().getGUI().drawList(&this->MenuList);
   if(State!=INIT && State!=ENTER_BOOL) {
      VB.draw(MyApp::get().getDisplay(),50, 80);
   }

	return ReturnStateContext(nextState);
}

ErrorType SettingMenu::onShutdown() {
	MyApp::get().getButtonMgr().removeObserver(QueueHandle);
	return ErrorType();
}


