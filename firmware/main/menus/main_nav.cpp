#include <stdlib.h>
#include <device/display/display_device.h>
#include "esp_netif_types.h"
#include "main_nav.h"
#include "menu_state.h"
#include "../app.h"
#include "freertos.h"

using libesp::RGBColor;
using libesp::FreeRTOS;
using libesp::ErrorType;
using libesp::BaseMenu;

static StaticQueue_t InternalQueue;
static uint8_t InternalQueueBuffer[MainNav::QUEUE_SIZE*MainNav::MSG_SIZE] = {0};

MainNav::MainNav() : AppBaseMenu() {
	InternalQueueHandler = xQueueCreateStatic(QUEUE_SIZE,MSG_SIZE,&InternalQueueBuffer[0],&InternalQueue);
}

MainNav::~MainNav() {

}

enum INTERNAL_STATE {
	INIT, RENDER
};

static INTERNAL_STATE InternalState = INIT;

ErrorType MainNav::onInit() {
	InternalState = INIT;
	return ErrorType();
}

BaseMenu::ReturnStateContext MainNav::onRun() {
	BaseMenu::ReturnStateContext sr(this);
	switch (InternalState) {
		case INIT:
			break;
		case RENDER:
			break;
	}
	return sr;
}

ErrorType MainNav::onShutdown() {
	return ErrorType();
}

