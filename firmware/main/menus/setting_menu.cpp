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
	, InternalState(INTERNAL_STATE::SHOW_ALL), MenuList("Setting", Items, 0, 0, MyApp::get().getCanvasWidth()
   , MyApp::get().getCanvasHeight(), 0, ItemCount), Name() {
	
	QueueHandle = xQueueCreateStatic(QUEUE_SIZE,MSG_SIZE,&QueueBuffer[0],&ButtonQueue);
}

SettingMenu::~SettingMenu() {

}

ErrorType SettingMenu::onInit() {
   memset(&Name[0],0,sizeof(Name));
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
	Items[1].text = (const char *) "Set Sleep Time: ";
	Items[2].id = 2;
	Items[2].text = (const char *) "Disable LEDs: ";
	MyApp::get().getDisplay().fillScreen(RGBColor::BLACK);
   MyApp::get().getGUI().drawList(&this->MenuList);
	MyApp::get().getButtonMgr().addObserver(QueueHandle);
	return ErrorType();
}

enum STATES {
  INIT
  , ENTER_NAME
  , ENTER_NUMBER
  , ENTER_BOOL
};
static STATES State = INIT;

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
                     switch(MenuList.getSelectedItemID()) {
                        case 0:
                           State = ENTER_NAME;
                           VB.init(VKeyboard::K1,7);
                           break;
                        case 1:
                           State = ENTER_NUMBER;
                           break;
                        case 2:
                           State = ENTER_BOOL;
                           break;
                     }
                     break;
                  case ENTER_NAME:
                     //save Name
                     //sprintf(getRow(0),"Name: %s", &Name[0]);
                     State = INIT;
                     break;
                  case ENTER_NUMBER:
                     //save
                     State = INIT;
                     break;
                  case ENTER_BOOL:
                     State = INIT;
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
            }
            break;
            default:
            break;
         }
      }
   }
   MyApp::get().getGUI().drawList(&this->MenuList);
   if(State!=INIT) {
      VB.draw(MyApp::get().getDisplay(),50, 80);
   }

	return ReturnStateContext(nextState);
}

ErrorType SettingMenu::onShutdown() {
	MyApp::get().getButtonMgr().removeObserver(QueueHandle);
	return ErrorType();
}


