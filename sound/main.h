#ifndef MAIN_H
#define MAIN_H

#include <string.h>
#include "MDR32Fx.h"
#include "MDR32F9Qx_usb_handlers.h"
#include "MDR32F9Qx_rst_clk.h"

#include "lcd.h"
static char string[21];

#include "fft.h"
#define SAMPLE_LENGTH 1024
static float re[SAMPLE_LENGTH];
static float im[SAMPLE_LENGTH];
static uint8_t out_sound[SAMPLE_LENGTH*2];
static uint32_t sample = 0;

#define BUFFER_LENGTH 2048

USB_Clock_TypeDef USB_Clock_InitStruct; 
USB_DeviceBUSParam_TypeDef USB_DeviceBUSParam;
static uint8_t Buffer[BUFFER_LENGTH];
#ifdef USB_CDC_LINE_CODING_SUPPORTED
	static USB_CDC_LineCoding_TypeDef LineCoding;
#endif
static void Setup_CPU_Clock(void);
static void Setup_USB(void);
static void VCom_Configuration(void);


#endif
