#ifndef APP_BASE_MENU_H
#define APP_BASE_MENU_H

#include <app/basemenu.h>
#include <device/display/gui.h>
#include <device/hwbutton/buttonmanager.h>
#include <freertos.h>

namespace libesp {
class GUIListData;
}

class AppBaseMenu : public libesp::BaseMenu {
public:
   typedef libesp::ButtonManager<10,2,6,2>::ButtonEvent ButtonManagerEvent;
	static const char *LOGTAG;
   static const uint32_t RowLength = 64;
	static const uint32_t NumRows = 16;
public:
	AppBaseMenu() : libesp::BaseMenu() {}
	virtual ~AppBaseMenu(){}
protected:
	static void clearListBuffer();
	static char *getRow(uint8_t row);
protected:
	bool processTouch(QueueHandle_t &queH, libesp::GUIListData &guiList, uint16_t itemCount, bool &penUp, bool &headerHit);
private:
	static char ListBuffer[NumRows][RowLength]; //height then width
};

#endif
