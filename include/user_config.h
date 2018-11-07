#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

// UART config
#define SERIAL_BAUD_RATE COM_SPEED_SERIAL

// ESP SDK config
#define LWIP_OPEN_SRC
#define USE_US_TIMER

// Default types
#define __CORRECT_ISO_CPP_STDLIB_H_PROTO
#include <limits.h>
#include <stdint.h>

// Override c_types.h include and remove buggy espconn
#define _C_TYPES_H_
#define _NO_ESPCON_

// Updated, compatible version of c_types.h
// Just removed types declared in <stdint.h>
#include <espinc/c_types_compatible.h>

// System API declarations
#include <esp_systemapi.h>

// C++ Support
#include <esp_cplusplus.h>
// Extended string conversion for compatibility
#include <stringconversion.h>
// Network base API
#include <espinc/lwip_includes.h>

#ifdef __cplusplus
}
#endif



//-------------------------
// define for WIFI_Op_Mode
enum
{
	WIFI_OP_MODE_NONE,
	WIFI_OP_MODE_SSID_REQUESTING,
	WIFI_OP_MODE_SSID_OK,
	WIFI_OP_MODE_SSID_RETRY,
	WIFI_OP_MODE_SSID_ERROR,
	WIFI_OP_MODE_SSID_STOP,
	WIFI_OP_MODE_CONNECT_REQUESTING,
	WIFI_OP_MODE_CONNECT_OK,
	WIFI_OP_MODE_CONNECT_ERROR,
	WIFI_OP_MODE_CONNECT_STOP,
	WIFI_OP_MODE_CONNECT_STANDBY
};


#define NARODM_PORT 33381
#define SERIAL_BAUDRATE				115200
//#define SERIAL_BAUDRATE				720000
//#define SERIAL_BAUDRATE				230400
//#define SERIAL_BAUDRATE				115200



// define for SPI

#define PIN_RADIO_DO 4/* Master Out Slave In */
#define PIN_RADIO_DI 5	/* Master In Slave Out */
#define PIN_RADIO_CK 12	/* Serial Clock */
#define PIN_RADIO_SS 13	/* Slave Select */


#define delay_ms(x)		os_delay_us(1000 * x )

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;

//----------------------------------------------------------
// definitions for  Hardware SPI
/*
 * GND      (GND)         GND
 * VCC      (VCC)         3.3v
 * D0       (CLK)         GPIO14
 * D1       (MOSI)        GPIO13
 * CS       (CS)          GPI015
 * RES      (RESET)       GPIO16
 * DC       (DC)          GPIO2
 */
#define CAM_SCLK 14 // HW SPI pins - don't change
#define CAM_MOSI 13
#define CAM_MISO 12

#define CAM_CS 16 // this pins are free to change

//----------------------------------------------------------

#define LED_PIN 2 // GPIO2



#endif
