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

class PlayerSprite {
public:
   static constexpr const char *LOGTAG="PLAYER_SPRITE";
public:
   PlayerSprite(uint16_t id, uint16_t x, uint16_t y, libesp::DCImage *i)
      : ID(id), Point(x,y), Image(i) {
   }
   void draw(libesp::DisplayDevice &d) {
      d.drawImage(Point.getX(),Point.getY(),(*Image));
   }
   void moveBy(libesp::Point2Dus &v) {
      Point+=v;
   }
   void set(const libesp::Point2Dus &v) {Point = v;}
   uint16_t getID() const {return ID;}
   const libesp::Point2Dus &getPosition() {return Point;}
   libesp::Point2Dus getTopMid() {
      return libesp::Point2Dus(Point.getX()+(Image->width/2),Point.getY()-(Image->height));
   }
   bool moveXBy(int16_t v) {
      bool retVal = false;
      int16_t newX = Point.getX()+v;
      if(newX<2) {
         Point.setX(2);
      } else if(newX>160-Image->width) {
         Point.setX(160-Image->width);
      } else {
         Point.setX(newX);
         retVal = true;
      }
      return retVal;
   }
protected:
   uint16_t ID;
   libesp::Point2Dus Point;
   DCImage *Image;
};

class Sprite {
   public:
      static constexpr const char *LOGTAG="SPRITE";
   public:
      Sprite(uint16_t id, uint16_t x, uint16_t y, libesp::DCImage *i, uint16_t maxXMovement=16)
         : ID(id), Point(x,y), Image(i), OriginalPt(x,y), MaxXMovement(maxXMovement) {
   }
   void draw(libesp::DisplayDevice &d) {
      if(!Hidden) {
         d.drawImage(Point.getX(),Point.getY(),(*Image));
      }
   }
   void moveBy(libesp::Point2Dus &v) {
      Point+=v;
   }
   void set(const libesp::Point2Dus &v) {Point = v;}
   uint16_t getID() const {return ID;}
   const libesp::Point2Dus &getPosition() {return Point;}
   bool moveYBy(int16_t v) {
      bool retVal = false;
      int16_t newY = Point.getY()+v;
      Point.setY(newY);
      return retVal;
   }
   bool moveXBy(int16_t v) {
      bool retVal = false;
      int16_t newX = Point.getX()+v;
      if(newX<OriginalPt.getX()) {
         Point.setX(OriginalPt.getX());
      } else if(newX>(OriginalPt.getX()+MaxXMovement)) {
         Point.setX(OriginalPt.getX()+MaxXMovement);
      } else {
         Point.setX(newX);
         retVal = true;
      }
      return retVal;
   }
   bool isHidden() {return Hidden;}
   void setHideen(bool b) {
      Hidden = b;
   }
   DCImage *getImage() {return Image;}
protected:
   uint16_t ID;
   libesp::Point2Dus Point;
   DCImage *Image;
   libesp::Point2Dus OriginalPt;
   uint16_t MaxXMovement;
   uint32_t Hidden:1;
};

class AnimatedSprite {
   public:
      AnimatedSprite(uint16_t id, uint16_t x, uint16_t y, libesp::DCImage *i1, libesp::DCImage *i2,
            uint32_t msBetweenAnimation, uint16_t maxx=24)
         : ID(id), Point(x,y), Images(), MSBetweenAnimation(msBetweenAnimation), LastAnimationTime(0)
           ,Index(0), OriginalPt(x,y), MaxXMovement(maxx) {
      Images[0] = i1;
      Images[1] = i2;
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
   bool moveYBy(int16_t v) {
      bool retVal = false;
      int16_t newY = Point.getY()+v;
      Point.setY(newY);
      return retVal;
   }
   bool moveXBy(int16_t v) {
      bool retVal = false;
      int16_t newX = Point.getX()+v;
      if(newX<OriginalPt.getX()) {
         Point.setX(OriginalPt.getX());
      } else if(newX>(OriginalPt.getX()+MaxXMovement)) {
         Point.setX(OriginalPt.getX()+MaxXMovement);
      } else {
         Point.setX(newX);
         retVal = true;
      }
      return retVal;
   }
   void set(const libesp::Point2Dus &v) {Point = v;}
   void moveBy(libesp::Point2Dus &v) {
      Point+=v;
   }
   uint16_t getID() const {return ID;}
   const libesp::Point2Dus &getPosition() {return Point;}
   DCImage *getImage() {return Images[0];}
   protected:
      uint16_t ID;
      libesp::Point2Dus Point;
      DCImage *Images[2];
      uint32_t MSBetweenAnimation;
      uint32_t LastAnimationTime;
      int16_t Index;
      libesp::Point2Dus OriginalPt;
      uint16_t MaxXMovement;
};

static DCImage Invader1Image(getWidthinvader1(),getHeightinvader1(),2,getPixelDatainvader1());
static DCImage Invader1aImage(getWidthinvader1a(),getHeightinvader1a(),2,getPixelDatainvader1a());
static DCImage Invader2Image(getWidthinvader2(),getHeightinvader2(),2,getPixelDatainvader2());
static DCImage Invader2aImage(getWidthinvader2a(),getHeightinvader2a(),2,getPixelDatainvader2a());
static DCImage Invader3Image(getWidthinvader3(),getHeightinvader3(),2,getPixelDatainvader3());
static DCImage Invader3aImage(getWidthinvader3a(),getHeightinvader3a(),2,getPixelDatainvader3a());
static DCImage PlayerImage(getWidthplayer(), getHeightplayer(), 2, getPixelDataplayer());
//static DCImage BarrierImage(getWidthbarrier(),getHeightbarrier(), 2, getPixelDatabarrier());
static DCImage PlayerShotImage(getWidthplayer_shot(), getHeightplayer_shot(), 2, getPixelDataplayer_shot());

static constexpr const uint16_t Sprite1Distance = 24;
static constexpr const uint16_t Sprite1XStart = 2;
static constexpr const uint16_t Sprint1Y = 42;
static AnimatedSprite SpriteRow1[6] = {
     AnimatedSprite(0, 2,42, &Invader1Image, &Invader1aImage, 500)
   , AnimatedSprite(1,26,42, &Invader1Image, &Invader1aImage, 500)
   , AnimatedSprite(2,50,42, &Invader1Image, &Invader1aImage, 500)
   , AnimatedSprite(3,74,42, &Invader1Image, &Invader1aImage, 500)
   , AnimatedSprite(4,98,42, &Invader1Image, &Invader1aImage, 500)
   , AnimatedSprite(5,122,42, &Invader1Image, &Invader1aImage, 500)
};
static constexpr const uint16_t Sprite2Distance = 24;
static constexpr const uint16_t Sprite2XStart = 2;
static constexpr const uint16_t Sprint2Y = 26;
static AnimatedSprite SpriteRow2[6] = {
     AnimatedSprite(10, 2,26, &Invader2Image, &Invader2aImage, 500)
   , AnimatedSprite(11,26,26, &Invader2Image, &Invader2aImage, 500)
   , AnimatedSprite(12,50,26, &Invader2Image, &Invader2aImage, 500)
   , AnimatedSprite(13,74,26, &Invader2Image, &Invader2aImage, 500)
   , AnimatedSprite(14,98,26, &Invader2Image, &Invader2aImage, 500)
   , AnimatedSprite(15,122,26, &Invader2Image, &Invader2aImage, 500)
};
static constexpr const uint16_t Sprite3Distance = 24;
static constexpr const uint16_t Sprite3XStart = 2;
static constexpr const uint16_t Sprite3Y = 12;
static AnimatedSprite SpriteRow3[6] = {
     AnimatedSprite(20, 2,Sprite3Y, &Invader3Image, &Invader3aImage, 500)
   , AnimatedSprite(21,26,Sprite3Y, &Invader3Image, &Invader3aImage, 500)
   , AnimatedSprite(22,50,Sprite3Y, &Invader3Image, &Invader3aImage, 500)
   , AnimatedSprite(23,74,Sprite3Y, &Invader3Image, &Invader3aImage, 500)
   , AnimatedSprite(24,98,Sprite3Y, &Invader3Image, &Invader3aImage, 500)
   , AnimatedSprite(25,122,Sprite3Y,&Invader3Image, &Invader3aImage, 500)
};
static const uint32_t NUM_SPRITES_ROW1 = sizeof(SpriteRow1)/sizeof(SpriteRow1[0]);
static const uint32_t NUM_SPRITES_ROW2 = sizeof(SpriteRow2)/sizeof(SpriteRow2[0]);
static const uint32_t NUM_SPRITES_ROW3 = sizeof(SpriteRow3)/sizeof(SpriteRow3[0]);

static PlayerSprite Player(100,70,112,&PlayerImage);

//static Sprite Barriers[3] = {
//   Sprite(90,5,80,&BarrierImage)
//   ,Sprite(90,65,80,&BarrierImage)
//   ,Sprite(90,120,80,&BarrierImage)
//};
static Sprite PlayerShot(50,50, 110, &PlayerShotImage);

typedef etl::list<AnimatedSprite*,6> SPRITE_ROW;
typedef SPRITE_ROW::iterator SPRITE_ROW_IT;
SPRITE_ROW Row1;
SPRITE_ROW Row2;
SPRITE_ROW Row3;

SpaceInvaders::SpaceInvaders() : AppBaseMenu(), InternalQueueHandler(), LastInvaderMoveTime(0)
                                 , Movement(0), LastMovementTime(0), InternalState(INIT), Score(0) {
	InternalQueueHandler = xQueueCreateStatic(QUEUE_SIZE,MSG_SIZE,&InternalQueueBuffer[0],&InternalQueue);

}
SpaceInvaders::~SpaceInvaders() {

}


ErrorType SpaceInvaders::onInit() {
   for(int i=0;i<4;++i) {
      ButtonManagerEvent *bme = nullptr;
	   xQueueReceive(InternalQueueHandler, &bme, 0);
      delete bme;
   }
	InternalState = INIT;
   LastInvaderMoveTime = 0;
   Movement = 0; 
   LastMovementTime = 0;
   Score = 0;
   PlayerShot.setHideen(true);
   uint16_t x = Sprite1XStart;
   for(int i=0;i<NUM_SPRITES_ROW1;++i) {
      SpriteRow1[i].set(libesp::Point2Dus(x,Sprint1Y));
      Row1.push_back(&SpriteRow1[i]);
      x += Sprite1Distance;
   }
   x = Sprite2XStart;
   for(int i=0;i<NUM_SPRITES_ROW2;++i) {
      SpriteRow2[i].set(libesp::Point2Dus(x,Sprint2Y));
      Row2.push_back(&SpriteRow2[i]);
      x += Sprite2Distance;
   }
   x = Sprite3XStart;
   for(int i=0;i<NUM_SPRITES_ROW3;++i) {
      SpriteRow3[i].set(libesp::Point2Dus(x,Sprite3Y));
      Row3.push_back(&SpriteRow3[i]);
      x += Sprite3Distance;
   }
   Player.set(libesp::Point2Dus(70,112));
   MyApp::get().getDisplay().fillScreen(RGBColor::BLACK);
   
	MyApp::get().getButtonMgr().addObserver(InternalQueueHandler);
	return ErrorType();
}

static bool MoveRight = true;

bool testRowCollision(SPRITE_ROW &r) {
   SPRITE_ROW_IT toRemove = r.end();
   SPRITE_ROW_IT b = r.begin();
   if(PlayerShot.getPosition().getY()>=(*b)->getPosition().getY() &&
      PlayerShot.getPosition().getY()<=((*b)->getPosition().getY()+(*b)->getImage()->height)) {

      for(SPRITE_ROW_IT it=b;it!=r.end();++it) {
         if(PlayerShot.getPosition().getX()>(*it)->getPosition().getX() &&
               PlayerShot.getPosition().getX()<=((*it)->getPosition().getX()+(*it)->getImage()->width)) {
            toRemove = it;
            break;
         }
      }
   }
   if(toRemove!=r.end()) {
      r.erase(toRemove); 
      return true;
   }
   return false;
}

void SpaceInvaders::update(uint32_t time) {
   if(LastMovementTime==0) LastMovementTime = time;

   if((time-LastMovementTime)>50) {
      LastMovementTime = time;
      if(!PlayerShot.isHidden()) {
         if(PlayerShot.getPosition().getY()>2) {
            PlayerShot.moveYBy(-2);
            if(testRowCollision(Row1)) {
               PlayerShot.setHideen(true);
               Score+=10;
            } else if(testRowCollision(Row2)) {
               PlayerShot.setHideen(true);
               Score+=10;
            } else if(testRowCollision(Row3)) {
               PlayerShot.setHideen(true);
               Score+=10;
            }

         } else {
            PlayerShot.setHideen(true);
         }
      }
      switch(Movement) {
      case NONE:
         break;
      case LEFT:
         Player.moveXBy(-4);
         //ESP_LOGI(LOGTAG,"position %d",Player.getPosition().getX());
         break;
      case RIGHT:
         Player.moveXBy(4);
         break;
      }
   }

   if(0==LastInvaderMoveTime) LastInvaderMoveTime = time;
   if((time-LastInvaderMoveTime)>=500) {
      //ESP_LOGI(LOGTAG,"TIME");
      LastInvaderMoveTime = time;
      uint16_t x = MoveRight?2:-2;
      bool didMove = false;
      SPRITE_ROW_IT it = Row1.begin();
      for(;it!=Row1.end();++it) {
         if(!(*it)->moveXBy(x)) {
            if(!didMove) {
               didMove = true;
               MoveRight=!MoveRight; 
            }
            (*it)->moveYBy(2);
         } else {
            //ESP_LOGI(LOGTAG,"TRUE");
         }
      }
      it = Row2.begin();
      for(;it!=Row2.end();++it) {
         if(!(*it)->moveXBy(x)) {
            if(!didMove) {
               didMove = true;
               MoveRight=!MoveRight; 
            }
            (*it)->moveYBy(2);
         }
      }
      it = Row3.begin();
      for(;it!=Row3.end();++it) {
         if(!(*it)->moveXBy(x)) {
            if(!didMove) {
               didMove = true;
               MoveRight=!MoveRight; 
            }
            (*it)->moveYBy(2);
         }
      }
   }
}


void SpaceInvaders::draw(uint32_t time) {
   UNUSED_VAR(time);
   char buf[24];
   sprintf(&buf[0],"Score: %u",Score);
   MyApp::get().getDisplay().drawString(50,0,&buf[0],RGBColor::GREEN);
   /*
   for(int i=0;i<3;++i) {
      Barriers[i].draw(MyApp::get().getDisplay());
   }
   */
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
   PlayerShot.draw(MyApp::get().getDisplay());
}

void SpaceInvaders::playerFire() {
   if(PlayerShot.isHidden()) {
      libesp::Point2Dus p = Player.getTopMid();
      PlayerShot.set(p);
      PlayerShot.setHideen(false);
   }
}


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
         case PIN_NUM_FIRE_BTN:
            if(bme->wasReleased()) playerFire();
            break;
         default:
            break;
      }
      delete bme;
   } 

   uint32_t now = FreeRTOS::getTimeSinceStart();
   
   update(now);
   draw(now);
   Player.draw(MyApp::get().getDisplay());
   

	return BaseMenu::ReturnStateContext(nextState);
}

ErrorType SpaceInvaders::onShutdown() {
	MyApp::get().getButtonMgr().removeObserver(InternalQueueHandler);
   Row1.clear();
   Row2.clear();
   Row3.clear();
	return ErrorType();
}

