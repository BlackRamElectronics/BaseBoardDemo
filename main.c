#include "LPC17xx.h"
#include "system_LPC17xx.h"
#include "IO.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_pwm.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_adc.h"

volatile uint32_t msTicks = 0; // counter for 1ms SysTicks

void FlashLEDs(void);
void HardwareInit(void);

//====================================================================================
void main()
{
	uint32_t timer_mark;
	uint16_t adc_res0, adc_res1;
	uint8_t button_active = 0;
	uint32_t pwm1, pwm2;
	
	// Configure the peripherals
	HardwareInit();
	
	// Config GPIO lines
	IO_Init();
	
	// Toggle each LED in turn
	FlashLEDs();
	
	// Send welcome message to console
	printf("<==== Black Ram Demo ====>\r\n");
	
	/*for(;;)
	{
		timer_mark = msTicks;				// Take timer snapshot 
		while(msTicks < (timer_mark + 100));	// Wait until 10ms has passed
		
		
		adc_res0 = ADC_ChannelGetData(LPC_ADC, 0);
		adc_res1 = ADC_ChannelGetData(LPC_ADC, 1);
		
		pwm1 = ((25000/4096) * adc_res0) + 25000;
		pwm2 = ((25000/4096) * adc_res1) + 25000;
		
		PWM_MatchUpdate(LPC_PWM1, 3, pwm1, PWM_MATCH_UPDATE_NOW);
		PWM_MatchUpdate(LPC_PWM1, 4, pwm2, PWM_MATCH_UPDATE_NOW);
	}*/
	
	
	// Loop forever processing inputs
	for(;;)
	{
		timer_mark = msTicks;				// Take timer snapshot 
		while(msTicks < (timer_mark + 100));	// Wait until 10ms has passed

		adc_res0 = ADC_ChannelGetData(LPC_ADC, 0);
		adc_res1 = ADC_ChannelGetData(LPC_ADC, 1);
		printf("ADC0: %d,\tADC1: %d\r\n", adc_res0, adc_res1);
		
		/*pwm1 = ((25000/4096) * adc_res0) + 25000;
		pwm2 = ((25000/4096) * adc_res1) + 25000;
		PWM_MatchUpdate(LPC_PWM1, 3, ((25000/4096) * adc_res0) + 25000, PWM_MATCH_UPDATE_NOW);
		*/
		
		printf("PWM1: %d,\tPWM2: %d\r\n", pwm1, pwm2);
		
		pwm1 = ((25000/4096) * adc_res0) + 25000;
		pwm2 = ((25000/4096) * adc_res1) + 25000;
		
		PWM_MatchUpdate(LPC_PWM1, 3, pwm1, PWM_MATCH_UPDATE_NOW);
		PWM_MatchUpdate(LPC_PWM1, 4, pwm2, PWM_MATCH_UPDATE_NOW);
		
		// If a button has been pressed send to serial console
		if(IO_GetSW())
		{
			// Was the button already pressed
			if(button_active == 0)
			{
				printf("Button press: 0x%2.2X\r\n", IO_GetSW());
				IO_LED_On(IO_LED_ONBOARD);
				button_active = 1;
			}
		}
		else
		{
			if(button_active == 1)
			{
				IO_LED_Off(IO_LED_ONBOARD);
				button_active = 0;
			}
		}
	}
}

//====================================================================================
void HardwareInit(void)
{
	// Config structures
	UART_CFG_Type uart_config;
	PINSEL_CFG_Type pinsel_config;
	PWM_TIMERCFG_Type pwm_timer_config;
	PWM_MATCHCFG_Type pwm_match_config;
	
	// ----- Init SysTick -----
	SysTick_Config(SystemCoreClock / 1000);		// Generate interrupt every 1 ms

	// ----- Enable UART to PC for debug ----
	UART_ConfigStructInit(&uart_config);
	uart_config.Baud_rate = 115200;
	
	UART_Init(LPC_UART0, &uart_config);
	UART_TxCmd(LPC_UART0, ENABLE);
	
	pinsel_config.Portnum = PINSEL_PORT_0;
	pinsel_config.Funcnum = PINSEL_FUNC_1;
	pinsel_config.Pinmode = PINSEL_PINMODE_TRISTATE;
	pinsel_config.OpenDrain = PINSEL_PINMODE_NORMAL;
	
	pinsel_config.Pinnum = PINSEL_PIN_2;
	PINSEL_ConfigPin(&pinsel_config);
	
	pinsel_config.Pinnum = PINSEL_PIN_3;
	PINSEL_ConfigPin(&pinsel_config);
	
	// ----- Configure the ADC to read the pots -----
	ADC_Init(LPC_ADC, 100000);	// Enable clock and power to the ADC and set the ADC clock to 100kHz
	
	pinsel_config.Portnum = PINSEL_PORT_0;
	pinsel_config.Funcnum = PINSEL_FUNC_1;
	pinsel_config.Pinnum = PINSEL_PIN_23;
	PINSEL_ConfigPin(&pinsel_config);
	pinsel_config.Pinnum = PINSEL_PIN_24;
	PINSEL_ConfigPin(&pinsel_config);
	
	ADC_ChannelCmd(LPC_ADC, 0, ENABLE);	// Enable channel ADC0.0
	ADC_ChannelCmd(LPC_ADC, 1, ENABLE);	// Enable channel ADC0.1
	
	ADC_BurstCmd(LPC_ADC, ENABLE);	// Enable bust mode, we will simply read the data when we want

	// ----- PWN configuration -----
	pwm_timer_config.PrescaleOption = PWM_TIMER_PRESCALE_TICKVAL;
	pwm_timer_config.PrescaleValue = 1;
	PWM_Init(LPC_PWM1, PWM_MODE_TIMER, (void *) &pwm_timer_config);
	
	pinsel_config.Portnum = PINSEL_PORT_2;
	pinsel_config.Funcnum = PINSEL_FUNC_1;
	pinsel_config.Pinmode = PINSEL_PINMODE_TRISTATE;
	pinsel_config.OpenDrain = PINSEL_PINMODE_NORMAL;
	
	pinsel_config.Pinnum = PINSEL_PIN_2;
	PINSEL_ConfigPin(&pinsel_config);
	
	pinsel_config.Pinnum = PINSEL_PIN_3;
	PINSEL_ConfigPin(&pinsel_config);
	
	// Set match value for PWM match channel 0 = 256, update immediately
	PWM_MatchUpdate(LPC_PWM1, 0, 500000, PWM_MATCH_UPDATE_NOW);
	
	pwm_match_config.IntOnMatch = DISABLE;
	pwm_match_config.MatchChannel = 0;
	pwm_match_config.ResetOnMatch = ENABLE;
	pwm_match_config.StopOnMatch = DISABLE;
	PWM_ConfigMatch(LPC_PWM1, &pwm_match_config);
	
	PWM_ChannelConfig(LPC_PWM1, 2, PWM_CHANNEL_SINGLE_EDGE);
	PWM_ChannelConfig(LPC_PWM1, 3, PWM_CHANNEL_SINGLE_EDGE);
	
	// Set up match value
	PWM_MatchUpdate(LPC_PWM1, 3, 37500, PWM_MATCH_UPDATE_NOW);
	// Configure match option
	pwm_match_config.IntOnMatch = DISABLE;
	pwm_match_config.MatchChannel = 3;
	pwm_match_config.ResetOnMatch = DISABLE;
	pwm_match_config.StopOnMatch = DISABLE;
	PWM_ConfigMatch(LPC_PWM1, &pwm_match_config);
	
	// Set up match value
	PWM_MatchUpdate(LPC_PWM1, 4, 37500, PWM_MATCH_UPDATE_NOW);
	// Configure match option
	pwm_match_config.IntOnMatch = DISABLE;
	pwm_match_config.MatchChannel = 4;
	pwm_match_config.ResetOnMatch = DISABLE;
	pwm_match_config.StopOnMatch = DISABLE;
	PWM_ConfigMatch(LPC_PWM1, &pwm_match_config);
	
	// Enable PWM Channel Output
	PWM_ChannelCmd(LPC_PWM1, 3, ENABLE);
	PWM_ChannelCmd(LPC_PWM1, 4, ENABLE);
	
	// Reset and Start counter
	PWM_ResetCounter(LPC_PWM1);
	PWM_CounterCmd(LPC_PWM1, ENABLE);

	// Start PWM
	PWM_Cmd(LPC_PWM1, ENABLE);
}

//====================================================================================
void FlashLEDs(void)
{
	uint32_t timer_mark;

	IO_LED_On(IO_LED_ONBOARD);
	timer_mark = msTicks;					// Take timer snapshot 
	while(msTicks < (timer_mark + 200));	// Wait until 100ms has passed
	IO_LED_Off(IO_LED_ONBOARD);
	
	IO_LED_On(IO_LED_1);
	timer_mark = msTicks;					// Take timer snapshot 
	while(msTicks < (timer_mark + 200));	// Wait until 100ms has passed
	IO_LED_Off(IO_LED_1);
	
	IO_LED_On(IO_LED_2);
	timer_mark = msTicks;					// Take timer snapshot 
	while(msTicks < (timer_mark + 200));	// Wait until 100ms has passed
	IO_LED_Off(IO_LED_2);
	
	IO_LED_On(IO_LED_3);
	timer_mark = msTicks;					// Take timer snapshot 
	while(msTicks < (timer_mark + 200));	// Wait until 100ms has passed
	IO_LED_Off(IO_LED_3);
}

//====================================================================================
void GetPotVals(uint16_t pot0, uint16_t pot1)
{

}

//====================================================================================
void SysTick_Handler(void)
{
	msTicks++;
}

//#ifdef  DEBUG
/*******************************************************************************
* @brief		Reports the name of the source file and the source line number
* 				where the CHECK_PARAM error has occurred.
* @param[in]	file Pointer to the source file name
* @param[in]    line assert_param error line source number
* @return		None
*******************************************************************************/
void check_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while(1);
}
//#endif

