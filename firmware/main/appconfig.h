#pragma once

#include <error_type.h>

namespace libesp {
   class NVS;
}

class AppConfig {
public:
   static constexpr const uint32_t MAX_NAME_LENGTH = 14;
   static constexpr const char *NAME_KEY="BADGE_NAME";
   static constexpr const char *SLEEP_KEY="SLEEP_KEY";
   static constexpr const char *FLAGS_KEY="FLAGS_KEY";
   static constexpr const char *LOGTAG = "AppConfig";
public:
   AppConfig(libesp::NVS *s);
   ~AppConfig();
public:
   libesp::ErrorType init();
public:
   bool isNameSet() {return Name[0]!='\0';}
   const char *getName() {return &Name[0];}
   uint16_t getSleepMin() {return SleepTime;}
   bool ledsEnabled() {return LedEnabled;}
public:
   libesp::ErrorType setName(const char *name);
   libesp::ErrorType setSleepMin(uint16_t s);
   libesp::ErrorType setLedsEnable(bool b);
private:
   libesp::NVS *Storage;
   char Name[MAX_NAME_LENGTH];
   uint16_t SleepTime;
   union {
      struct {
         uint32_t LedEnabled:1;
      };
      uint32_t Flags;
   };
};
