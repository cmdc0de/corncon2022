#pragma once

#include "appbase_menu.h"

namespace libesp {
	class DisplayDevice;
	class RGBColor;
}


class SpaceInvaders : public AppBaseMenu {
public:
   enum INTERNAL_STATE {
	   INIT, RENDER
   };
   enum MOVEMENT {
      NONE = 0
      , RIGHT = 1
      , LEFT = 2
   };
public:
	SpaceInvaders();
	virtual ~SpaceInvaders();
public:
	static const int QUEUE_SIZE = 2;
	static const int MSG_SIZE = sizeof(ButtonManagerEvent*);
protected:
   void update(uint32_t );
   void draw(uint32_t );
protected:
	virtual libesp::ErrorType onInit();
	virtual libesp::BaseMenu::ReturnStateContext onRun();
	virtual libesp::ErrorType onShutdown();
private:
	QueueHandle_t InternalQueueHandler;
   uint32_t LastInvaderMoveTime;
   uint32_t Movement;
   uint32_t LastMovementTime;
   INTERNAL_STATE InternalState;
   static constexpr const char *LOGTAG = "SpaceInvader";
};

