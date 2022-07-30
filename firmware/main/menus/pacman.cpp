#include <stdlib.h>
#include <device/display/display_device.h>
#include "device/display/color.h"
#include "esp_netif_types.h"
#include "pacman.h"
#include "menu_state.h"
#include "../app.h"
#include "freertos.h"

using libesp::RGBColor;
using libesp::FreeRTOS;
using libesp::ErrorType;
using libesp::BaseMenu;

static StaticQueue_t InternalQueue;
static uint8_t InternalQueueBuffer[Pacman::QUEUE_SIZE*Pacman::MSG_SIZE] = {0};
static libesp::DCImage PacmanMap;

Pacman::Pacman() : AppBaseMenu() {
	InternalQueueHandler = xQueueCreateStatic(QUEUE_SIZE,MSG_SIZE,&InternalQueueBuffer[0],&InternalQueue);
   PacmanMap.bytes_per_pixel = 2;
   PacmanMap.height = 120;
   PacmanMap.width = 126;
   extern char pac_map_start[] asm("_binary_pacmanlevel1_map_start");
   extern char pac_map_end[]   asm("_binary_pacmanlevel1_map_end");
   ESP_LOGI(LOGTAG,"****size of pacmap %d ****************",(pac_map_end-pac_map_start));
   PacmanMap.pixel_data = &pac_map_start[0];
}

Pacman::~Pacman() {

}

enum INTERNAL_STATE {
	INIT, RENDER
};

static INTERNAL_STATE InternalState = INIT;

ErrorType Pacman::onInit() {
	InternalState = INIT;
   MyApp::get().getDisplay().fillScreen(RGBColor::BLACK);
	return ErrorType();
}

BaseMenu::ReturnStateContext Pacman::onRun() {
	BaseMenu::ReturnStateContext sr(this);
   MyApp::get().getDisplay().fillScreen(RGBColor::BLACK);
   MyApp::get().getDisplay().drawImage(0,0,PacmanMap);
	return sr;
}

ErrorType Pacman::onShutdown() {
	return ErrorType();
}

