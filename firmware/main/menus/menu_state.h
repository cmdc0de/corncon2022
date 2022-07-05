#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "appbase_menu.h"
#include <device/display/layout.h>

class MenuState: public AppBaseMenu {
public:
	static const int QUEUE_SIZE = 5;
	static const int MSG_SIZE = sizeof(ButtonManagerEvent*);
	MenuState();
	virtual ~MenuState();
protected:
	virtual libesp::ErrorType onInit();
	virtual libesp::BaseMenu::ReturnStateContext onRun();
	virtual libesp::ErrorType onShutdown();
private:
	libesp::GUIListData MenuList;
	libesp::GUIListItemData Items[4];
	QueueHandle_t InternalQueueHandler;
	static const uint16_t ItemCount = uint16_t(sizeof(Items) / sizeof(Items[0]));
};

#endif
