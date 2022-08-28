#include <stdlib.h>
#include "space_invaders.h"
#include <device/display/display_device.h>
#include "device/display/color.h"
#include "esp_netif_types.h"
#include "math/point.h"
#include "math/vec_math.h"
#include "menu_state.h"
#include "../app.h"
#include "freertos.h"
//#include "../art/sprits.h"
#include "../spaceinvader/sprits.h"
#include <etl/list.h>

using libesp::RGBColor;
using libesp::FreeRTOS;
using libesp::ErrorType;
using libesp::BaseMenu;
using libesp::DCImage;

static StaticQueue_t InternalQueue;
static uint8_t InternalQueueBuffer[SpaceInvaders::QUEUE_SIZE*SpaceInvaders::MSG_SIZE] = {0};


class Sprite {
   public:
      Sprite(uint16_t id, uint16_t x, uint16_t y, libesp::DCImage *i)
         : ID(id), Point(x,y), Image(i) {

   }
   void draw(libesp::DisplayDevice &d) {
      d.drawImage(Point.getX(),Point.getY(),(*Image));
   }
   void moveBy(libesp::Point2Dus &v) {
      Point+=v;
   }
   uint16_t getID() const {return ID;}
   const libesp::Point2Dus &getPosition() {return Point;}
   protected:
      uint16_t ID;
      libesp::Point2Dus Point;
      DCImage *Image;
};

static DCImage Invader1Image(getWidthinvader1(),getHeightinvader1(),2,getPixelDatainvader1());
static DCImage Invader1aImage(getWidthinvader1a(),getHeightinvader1a(),2,getPixelDatainvader1a());
static DCImage Invader2Image(getWidthinvader2(),getHeightinvader2(),2,getPixelDatainvader2());
static DCImage Invader2aImage(getWidthinvader2a(),getHeightinvader2a(),2,getPixelDatainvader2a());
static DCImage Invader3Image(getWidthinvader3(),getHeightinvader3(),2,getPixelDatainvader3());
static DCImage Invader3aImage(getWidthinvader3a(),getHeightinvader3a(),2,getPixelDatainvader3a());
static DCImage PlayerImage(getWidthplayer(), getHeightplayer(), 2, getPixelDataplayer());
static DCImage BarrierImage(getWidthbarrier(),getHeightbarrier(), 2, getPixelDatabarrier());
static Sprite SpriteRow1[6] = {
     Sprite(0, 2,45, &Invader1Image)
   , Sprite(1,26,45, &Invader1Image)
   , Sprite(2,50,45, &Invader1Image)
   , Sprite(3,74,45, &Invader1Image)
   , Sprite(4,98,45, &Invader1Image)
   , Sprite(5,122,45, &Invader1Image)
};
static Sprite SpriteRow2[6] = {
     Sprite(0, 2,25, &Invader2Image)
   , Sprite(1,26,25, &Invader2Image)
   , Sprite(2,50,25, &Invader2Image)
   , Sprite(3,74,25, &Invader2Image)
   , Sprite(4,98,25, &Invader2Image)
   , Sprite(5,122,25, &Invader2Image)
};
static Sprite SpriteRow3[6] = {
     Sprite(0, 2,5, &Invader3aImage)
   , Sprite(1,26,5, &Invader3aImage)
   , Sprite(2,50,5, &Invader3aImage)
   , Sprite(3,74,5, &Invader3aImage)
   , Sprite(4,98,5, &Invader3aImage)
   , Sprite(5,122,5, &Invader3aImage)
};
static const uint32_t NUM_SPRITES_ROW1 = sizeof(SpriteRow1)/sizeof(SpriteRow1[0]);
static const uint32_t NUM_SPRITES_ROW2 = sizeof(SpriteRow2)/sizeof(SpriteRow2[0]);
static const uint32_t NUM_SPRITES_ROW3 = sizeof(SpriteRow3)/sizeof(SpriteRow3[0]);

static Sprite Player(100,70,109,&PlayerImage);
static Sprite Barriers[3] = {
   Sprite(90,5,80,&BarrierImage)
   ,Sprite(90,65,80,&BarrierImage)
   ,Sprite(90,120,80,&BarrierImage)
};

typedef etl::list<Sprite*,6> SPRITE_ROW;
typedef SPRITE_ROW::iterator SPRITE_ROW_IT;
SPRITE_ROW Row1;
SPRITE_ROW Row2;
SPRITE_ROW Row3;

SpaceInvaders::SpaceInvaders() : AppBaseMenu() {
	InternalQueueHandler = xQueueCreateStatic(QUEUE_SIZE,MSG_SIZE,&InternalQueueBuffer[0],&InternalQueue);

}
SpaceInvaders::~SpaceInvaders() {

}

enum INTERNAL_STATE {
	INIT, RENDER
};

static INTERNAL_STATE InternalState = INIT;

ErrorType SpaceInvaders::onInit() {
	InternalState = INIT;
   for(int i=0;i<NUM_SPRITES_ROW1;++i) {
      Row1.push_back(&SpriteRow1[i]);
   }
   for(int i=0;i<NUM_SPRITES_ROW2;++i) {
      Row2.push_back(&SpriteRow2[i]);
   }
   for(int i=0;i<NUM_SPRITES_ROW3;++i) {
      Row3.push_back(&SpriteRow3[i]);
   }
   MyApp::get().getDisplay().fillScreen(RGBColor::BLACK);
   
	MyApp::get().getButtonMgr().addObserver(InternalQueueHandler);
	return ErrorType();
}


static int32_t spriteIndex = 0;
static uint32_t LastTime = 0;
static const uint32_t ANIMATION_TIME = 500;

BaseMenu::ReturnStateContext SpaceInvaders::onRun() {
   BaseMenu *nextState = this;
   ButtonManagerEvent *bme = nullptr;
	if(xQueueReceive(InternalQueueHandler, &bme, 0)) {
      nextState = MyApp::get().getMenuState();
   } 

   {
      SPRITE_ROW_IT it = Row1.begin();
      for(;it!=Row1.end();++it) {
         (*it)->draw(MyApp::get().getDisplay());
      }
   }
   {
      SPRITE_ROW_IT it = Row2.begin();
      for(;it!=Row2.end();++it) {
         (*it)->draw(MyApp::get().getDisplay());
      }
   }
   {
      SPRITE_ROW_IT it = Row3.begin();
      for(;it!=Row3.end();++it) {
         (*it)->draw(MyApp::get().getDisplay());
      }
   }
   for(int i=0;i<3;++i) {
      Barriers[i].draw(MyApp::get().getDisplay());
   }
   Player.draw(MyApp::get().getDisplay());
   

  uint32_t timeSinceLast = FreeRTOS::getTimeSinceStart()-LastTime;
   if(timeSinceLast>=ANIMATION_TIME) {
      LastTime = FreeRTOS::getTimeSinceStart();
      ++spriteIndex;
      if(spriteIndex>1) spriteIndex=0;
   } else {
      //ESP_LOGI(LOGTAG,"t");
   }
	return BaseMenu::ReturnStateContext(nextState);
}

ErrorType SpaceInvaders::onShutdown() {
	MyApp::get().getButtonMgr().removeObserver(InternalQueueHandler);
	return ErrorType();
}

