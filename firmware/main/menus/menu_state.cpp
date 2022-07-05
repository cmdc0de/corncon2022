#include "menu_state.h"
#include "../app.h"
#include "gui_list_processor.h"
#include <app/display_message_state.h>
#include <esp_log.h>
#include <math/rectbbox.h>
#include "setting_menu.h"

using libesp::ErrorType;
using libesp::BaseMenu;
using libesp::RGBColor;
using libesp::Point2Ds;

static StaticQueue_t InternalQueue;
static uint8_t InternalQueueBuffer[MenuState::QUEUE_SIZE*MenuState::MSG_SIZE] = {0};
static const char *LOGTAG = "MenuState";

MenuState::MenuState() :
	AppBaseMenu(), MenuList("Main Menu", Items, 0, 0, MyApp::get().getCanvasWidth(), MyApp::get().getCanvasHeight(), 0, ItemCount) {
	InternalQueueHandler = xQueueCreateStatic(QUEUE_SIZE,MSG_SIZE,&InternalQueueBuffer[0],&InternalQueue);
}

MenuState::~MenuState() {

}

ErrorType MenuState::onInit() {
	MyApp::get().getDisplay().fillScreen(RGBColor::BLACK);
  Items[0].id = 0;
	//if (DarkNet7::get().getContacts().getSettings().isNameSet()) {
//		Items[0].text = (const char *) "Settings";
//	} else {
		Items[0].text = (const char *) "Settings *";
//	}
	Items[1].id = 1;
	Items[1].text = (const char *) "Badge Pair";
	Items[2].id = 2;
	Items[2].text = (const char *) "Test Badge";
	Items[3].id = 3;
	Items[3].text = (const char *) "stuff";
  MyApp::get().getGUI().drawList(&this->MenuList);
	//MyApp::get().getTouch().removeObserver(InternalQueueHandler);
	return ErrorType();
}

libesp::BaseMenu::ReturnStateContext MenuState::onRun() {
	BaseMenu *nextState = this;

  char buf[32] = {'\0'};
  switch (MenuList.selectedItem) {
    case 0:
			//nextState = MyApp::get().getSettingMenu();
      break;
  } 
    
	return BaseMenu::ReturnStateContext(nextState);
}

ErrorType MenuState::onShutdown() {
	//MyApp::get().getTouch().removeObserver(InternalQueueHandler);
	return ErrorType();
}

