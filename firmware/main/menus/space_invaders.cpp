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
      static constexpr const char *LOGTAG="SPRITE";
   public:
      Sprite(uint16_t id, uint16_t x, uint16_t y, libesp::DCImage *i)
         : ID(id), Point(x,y), Image(i), NegXBounds(0),PosXBounds(0) {
      NegXBounds = Image->width/2;
      PosXBounds = 160-(Image->width/2);
      //ESP_LOGI(LOGTAG,"minx = %d maxx = %d",NegXBounds,PosXBounds);
   }
   void draw(libesp::DisplayDevice &d) {
      d.drawImage(Point.getX(),Point.getY(),(*Image));
   }
   void moveBy(libesp::Point2Dus &v) {
      Point+=v;
   }
   uint16_t getID() const {return ID;}
   const libesp::Point2Dus &getPosition() {return Point;}
   void moveXBy(int16_t v) {
      int16_t newX = Point.getX()+v;
      if(newX<NegXBounds) {
         Point.setX(NegXBounds);
      } else if(newX>PosXBounds) {
         Point.setX(PosXBounds);
      } else {
         Point.setX(newX);
      }
   }
   protected:
      uint16_t ID;
      libesp::Point2Dus Point;
      DCImage *Image;
      int16_t NegXBounds;
      int16_t PosXBounds;
};

class AnimatedSprite {
   public:
      AnimatedSprite(uint16_t id, uint16_t x, uint16_t y, libesp::DCImage *i1, libesp::DCImage *i2,
            uint32_t msBetweenAnimation)
         : ID(id), Point(x,y), Images(), MSBetweenAnimation(msBetweenAnimation), LastAnimationTime(0)
           ,Index(0), NegXBounds(0), PosXBounds(160) {
      Images[0] = i1;
      Images[1] = i2;
      NegXBounds = i1->width/2;
      PosXBounds = MyApp::get().getCanvasWidth()-(i1->width/2);
   }
   void draw(libesp::DisplayDevice &d) {
      uint32_t now =libesp::FreeRTOS::getTimeSinceStart();
      if(0==LastAnimationTime) {
         LastAnimationTime = now;
      }
      if((now-LastAnimationTime)>=MSBetweenAnimation) {
         Index= ++Index>1?0:Index;
         LastAnimationTime = now;
      }
      d.drawImage(Point.getX(),Point.getY(),(*Images[Index]));
   }
   void reset(libesp::Point2Dus &v) {
      LastAnimationTime = 0;
      Point = v;
   }
   void moveXBy(int16_t v) {
      int16_t newX = Point.getX()+v;
      if(newX<NegXBounds) {
         Point.setX(NegXBounds);
      } else if(newX>PosXBounds) {
         Point.setX(PosXBounds);
      } else {
         Point.setX(newX);
      }
   }
   void moveBy(libesp::Point2Dus &v) {
      Point+=v;
   }
   uint16_t getID() const {return ID;}
   const libesp::Point2Dus &getPosition() {return Point;}
   protected:
      uint16_t ID;
      libesp::Point2Dus Point;
      DCImage *Images[2];
      uint32_t MSBetweenAnimation;
      uint32_t LastAnimationTime;
      int16_t Index;
      int16_t NegXBounds;
      int16_t PosXBounds;
};

static DCImage Invader1Image(getWidthinvader1(),getHeightinvader1(),2,getPixelDatainvader1());
static DCImage Invader1aImage(getWidthinvader1a(),getHeightinvader1a(),2,getPixelDatainvader1a());
static DCImage Invader2Image(getWidthinvader2(),getHeightinvader2(),2,getPixelDatainvader2());
static DCImage Invader2aImage(getWidthinvader2a(),getHeightinvader2a(),2,getPixelDatainvader2a());
static DCImage Invader3Image(getWidthinvader3(),getHeightinvader3(),2,getPixelDatainvader3());
static DCImage Invader3aImage(getWidthinvader3a(),getHeightinvader3a(),2,getPixelDatainvader3a());
static DCImage PlayerImage(getWidthplayer(), getHeightplayer(), 2, getPixelDataplayer());
static DCImage BarrierImage(getWidthbarrier(),getHeightbarrier(), 2, getPixelDatabarrier());
static AnimatedSprite SpriteRow1[6] = {
     AnimatedSprite(0, 2,42, &Invader1Image, &Invader1aImage, 500)
   , AnimatedSprite(1,26,42, &Invader1Image, &Invader1aImage, 500)
   , AnimatedSprite(2,50,42, &Invader1Image, &Invader1aImage, 500)
   , AnimatedSprite(3,74,42, &Invader1Image, &Invader1aImage, 500)
   , AnimatedSprite(4,98,42, &Invader1Image, &Invader1aImage, 500)
   , AnimatedSprite(5,122,42, &Invader1Image, &Invader1aImage, 500)
};
static AnimatedSprite SpriteRow2[6] = {
     AnimatedSprite(0, 2,26, &Invader2Image, &Invader2aImage, 500)
   , AnimatedSprite(1,26,26, &Invader2Image, &Invader2aImage, 500)
   , AnimatedSprite(2,50,26, &Invader2Image, &Invader2aImage, 500)
   , AnimatedSprite(3,74,26, &Invader2Image, &Invader2aImage, 500)
   , AnimatedSprite(4,98,26, &Invader2Image, &Invader2aImage, 500)
   , AnimatedSprite(5,122,26, &Invader2Image, &Invader2aImage, 500)
};
static AnimatedSprite SpriteRow3[6] = {
     AnimatedSprite(0, 2,10, &Invader3Image, &Invader3aImage, 500)
   , AnimatedSprite(1,26,10, &Invader3Image, &Invader3aImage, 500)
   , AnimatedSprite(2,50,10, &Invader3Image, &Invader3aImage, 500)
   , AnimatedSprite(3,74,10, &Invader3Image, &Invader3aImage, 500)
   , AnimatedSprite(4,98,10, &Invader3Image, &Invader3aImage, 500)
   , AnimatedSprite(5,122,10,&Invader3Image, &Invader3aImage, 500)
};
static const uint32_t NUM_SPRITES_ROW1 = sizeof(SpriteRow1)/sizeof(SpriteRow1[0]);
static const uint32_t NUM_SPRITES_ROW2 = sizeof(SpriteRow2)/sizeof(SpriteRow2[0]);
static const uint32_t NUM_SPRITES_ROW3 = sizeof(SpriteRow3)/sizeof(SpriteRow3[0]);

static Sprite Player(100,70,112,&PlayerImage);
static Sprite Barriers[3] = {
   Sprite(90,5,80,&BarrierImage)
   ,Sprite(90,65,80,&BarrierImage)
   ,Sprite(90,120,80,&BarrierImage)
};

typedef etl::list<AnimatedSprite*,6> SPRITE_ROW;
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

enum MOVEMENT {
   NONE = 0
   , RIGHT = 1
   , LEFT = 2
};
static uint32_t Movement = 0;
static uint32_t LastMovementTime = 0;

BaseMenu::ReturnStateContext SpaceInvaders::onRun() {
   BaseMenu *nextState = this;
   ButtonManagerEvent *bme = nullptr;
   MyApp::get().getDisplay().fillScreen(RGBColor::BLACK);
	if(xQueueReceive(InternalQueueHandler, &bme, 0)) {
      switch(bme->getButton()) {
         case PIN_NUM_JUMP_BTN:
            if(bme->wasReleased()) nextState = MyApp::get().getMenuState();
            break;
         case PIN_NUM_LEFT_BTN:
            if(bme->wasReleased()) Movement = NONE;
            else Movement = LEFT;
            break;
         case PIN_NUM_RIGHT_BTN:
            if(bme->wasReleased()) Movement = NONE;
            else Movement = RIGHT;
            break;
         default:
            break;
      }
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
   uint32_t now = FreeRTOS::getTimeSinceStart();
   if(LastMovementTime==0) {
      LastMovementTime = now;
   }
   if((now-LastMovementTime)>50) {
      LastMovementTime = now;
      switch(Movement) {
      case NONE:
         break;
      case LEFT:
         Player.moveXBy(-4);
         ESP_LOGI(LOGTAG,"position %d",Player.getPosition().getX());
         break;
      case RIGHT:
         Player.moveXBy(4);
         break;
      }
   }
   Player.draw(MyApp::get().getDisplay());
   

	return BaseMenu::ReturnStateContext(nextState);
}

ErrorType SpaceInvaders::onShutdown() {
	MyApp::get().getButtonMgr().removeObserver(InternalQueueHandler);
	return ErrorType();
}

