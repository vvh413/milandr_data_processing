#include "main.h"

int main(void){
	
	LCD_Init();
	LCD_ClearScreen();
	LCD_PutString("sample: 0", 1);
	LCD_PutString("size: 0 KB", 3);
	
	VCom_Configuration();
	USB_CDC_Init(Buffer, BUFFER_LENGTH, SET);
	Setup_CPU_Clock();
	Setup_USB();
	
	while(1)  {}
}

void Setup_CPU_Clock(void){
	RST_CLK_HSEconfig(RST_CLK_HSE_ON);
	if (RST_CLK_HSEstatus() != SUCCESS){
		while (1){} 
	}
	RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul10);
		RST_CLK_CPU_PLLcmd(ENABLE);
	if (RST_CLK_CPU_PLLstatus() != SUCCESS){
		while  (1){}
	}
	RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);
	RST_CLK_CPU_PLLuse(ENABLE);
	RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);
}

void Setup_USB(void){
	RST_CLK_PCLKcmd(RST_CLK_PCLK_USB, ENABLE);
	USB_Clock_InitStruct.USB_USBC1_Source = USB_C1HSEdiv2;
	USB_Clock_InitStruct.USB_PLLUSBMUL    = USB_PLLUSBMUL12;
	USB_DeviceBUSParam.MODE  = USB_SC_SCFSP_Full;
	USB_DeviceBUSParam.SPEED = USB_SC_SCFSR_12Mb;
	USB_DeviceBUSParam.PULL  = USB_HSCR_DP_PULLUP_Set;
	USB_DeviceInit(&USB_Clock_InitStruct, &USB_DeviceBUSParam);
	USB_SetSIM(USB_SIS_Msk);
	USB_DevicePowerOn();
	#ifdef USB_INT_HANDLE_REQUIRED
		NVIC_EnableIRQ(USB_IRQn);
	#endif
	USB_DEVICE_HANDLE_RESET;
}

static void VCom_Configuration(void){
	#ifdef USB_CDC_LINE_CODING_SUPPORTED
		LineCoding.dwDTERate = 115200;
		LineCoding.bCharFormat = 0;
		LineCoding.bParityType = 0;
		LineCoding.bDataBits = 8;
	#endif
}


void sound(uint8_t* Buffer){
	USB_Result result;

	uint16_t temp;
	for(uint16_t i=0; i<SAMPLE_LENGTH; i++){
		temp = (Buffer[i*2+1] << 8) + Buffer[i*2];
		re[i] = (float)temp;
		im[i] = 0.0;
	}
	fft(re, im, SAMPLE_LENGTH, 10, DIRECT);
	
//	shift(2, re, im, 1024);
	
	fft(re, im, SAMPLE_LENGTH, 10, INVERSE);
	for(uint16_t i=0; i<SAMPLE_LENGTH; i++){
		temp = (int16_t)re[i];
		out_sound[i*2+1] = (temp >> 8);
		out_sound[i*2] = temp & 0x08;
		sprintf(string, "size: %.2f KB", (sample*SAMPLE_LENGTH + i + 1)/512.0);
		LCD_PutString(string, 3);
	}
	result = USB_CDC_SendData(out_sound, SAMPLE_LENGTH*2);
	sample++;
	sprintf(string, "sample: %d", sample);
	LCD_PutString(string, 1);
}


USB_Result USB_CDC_RecieveData(uint8_t* Buffer, uint32_t Length){
	
	sound(Buffer);
	
	return USB_SUCCESS;
}

#ifdef USB_CDC_LINE_CODING_SUPPORTED

USB_Result USB_CDC_GetLineCoding(uint16_t wINDEX, USB_CDC_LineCoding_TypeDef* DATA){
	assert_param(DATA);
	if (wINDEX != 0){
		return USB_ERR_INV_REQ;
	}
	*DATA = LineCoding;
	return USB_SUCCESS;
}

USB_Result USB_CDC_SetLineCoding(uint16_t wINDEX, const USB_CDC_LineCoding_TypeDef* DATA){
	assert_param(DATA);
	if (wINDEX != 0){
		return USB_ERR_INV_REQ;
	}
	LineCoding = *DATA;
	return USB_SUCCESS;
}

#endif
