
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


ErrorType AppConfig::hasWiFiBeenSetup() {
  char data[64] = {'\0'};
  uint32_t len = sizeof(data);
  ErrorType et = Storage->getValue(WIFISID, &data[0],len);
  Sid = &data[0];
  ESP_LOGI(LOGTAG,"SID = %s",Sid.c_str());
   if(et.ok()) {
      len = sizeof(data);
      et = Storage->getValue(WIFIPASSWD, &data[0],len);
      if(et.ok()) {
         WifiPassword = &data[0];
         ESP_LOGI(LOGTAG,"P: %s",WifiPassword.c_str());
      } else {
         ESP_LOGI(LOGTAG,"failed to load password: %d %s", et.getErrT(), et.toString()); 
      }
  } else {
    ESP_LOGI(LOGTAG,"failed to load wifisid: %d %s", et.getErrT(), et.toString()); 
  }
  return et;
}

ErrorType AppConfig::clearConnectData() {
  ErrorType et = Storage->eraseKey(WIFISID);
  if(!et.ok()) {
    ESP_LOGI(LOGTAG,"failed to erase key ssid: %d %s", et.getErrT(), et.toString()); 
  } 
  et = Storage->eraseKey(WIFIPASSWD);
  if(!et.ok()) {
    ESP_LOGI(LOGTAG,"failed to erase key password: %d %s", et.getErrT(), et.toString()); 
  }
  return et;
}


ErrorType AppConfig::setWifiData(const char *sid, const char *password) {
   ErrorType et = Storage->setValue(WIFISID,sid);
   if(et.ok()) {
      Sid = sid;
      ESP_LOGI(LOGTAG,"Saving SID = %s", Sid.c_str());
      et = Storage->setValue(WIFIPASSWD,password);
      if(et.ok()) {
         WifiPassword = password;
         ESP_LOGI(LOGTAG,"Saving passwrd: %s",WifiPassword.c_str());
      }
   }
   return et;
}
