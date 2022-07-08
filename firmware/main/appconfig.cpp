
#include "appconfig.h"
#include "error_type.h"
#include <nvs_memory.h>
#include <string.h>

using libesp::NVS;
using libesp::ErrorType;

AppConfig::AppConfig(libesp::NVS *s) :
   Storage(s), Name(), SleepTime(3), Flags(0) {

      LedEnabled= 1;
      memset(&Name[0],0,sizeof(Name));
}


AppConfig::~AppConfig() {

}

libesp::ErrorType AppConfig::init() {
   ErrorType et;
   {
      uint32_t len = static_cast<uint32_t>(sizeof(Name));
      et = Storage->getValue(NAME_KEY,&Name[0],len);
      if(!et.ok()) {
         ESP_LOGI(LOGTAG,"Failed to load name %s", et.toString());
      }
      et = Storage->getValue(SLEEP_KEY,SleepTime);
      if(!et.ok()) {
         ESP_LOGI(LOGTAG,"Failed to load sleep time %s", et.toString());
      }
      et = Storage->getValue(FLAGS_KEY,Flags);
      if(!et.ok()) {
         ESP_LOGI(LOGTAG,"Failed to load light enable %s", et.toString());
      }
   }
   return et;
}

libesp::ErrorType AppConfig::setName(const char *name) {
   ErrorType et;
   strncpy(&Name[0],name,sizeof(Name));
   et = Storage->setValue(NAME_KEY,name);
   return et;
}

libesp::ErrorType AppConfig::setSleepMin(uint16_t s) {
   ErrorType et;
   SleepTime = s;
   et = Storage->setValue(SLEEP_KEY,s);
   return et;
}

libesp::ErrorType AppConfig::setLedsEnable(bool b) {
   ErrorType et;
   LedEnabled = b?1:0;
   et = Storage->setValue(FLAGS_KEY,Flags);
   return et;
}


