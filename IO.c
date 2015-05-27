#include "LPC17xx.h"
#include "IO.h"

// LED defintions
#define LED_ONBOARD_PORT LPC_GPIO1
#define LED_1_PORT LPC_GPIO2
#define LED_2_PORT LPC_GPIO2
#define LED_3_PORT LPC_GPIO2

#define LED_ONBOARD_PIN (1 << 18)
#define LED_1_PIN (1 << 2)
#define LED_2_PIN (1 << 0)
#define LED_3_PIN (1 << 1)

// Base board switch definitions
#define SW_1_PORT LPC_GPIO2
#define SW_2_PORT LPC_GPIO0
#define NAV_PUSH_PORT LPC_GPIO1
#define NAV_UP_PORT LPC_GPIO1
#define NAV_RIGHT_PORT LPC_GPIO1
#define NAV_DOWN_PORT LPC_GPIO1
#define NAV_LEFT_PORT LPC_GPIO1

#define SW_1_PIN (1 << 3)
#define SW_2_PIN (1 << 26)
#define NAV_PUSH_PIN (1 << 31)
#define NAV_UP_PIN (1 << 26)
#define NAV_RIGHT_PIN (1 << 27)
#define NAV_DOWN_PIN (1 << 29)
#define NAV_LEFT_PIN (1 << 28)

//====================================================================================
void IO_Init(void)
{
	// Init LEDs as outputs
	LED_ONBOARD_PORT->FIODIR |= LED_ONBOARD_PIN;
	LED_1_PORT->FIODIR |= LED_1_PIN;
	LED_2_PORT->FIODIR |= LED_2_PIN;
	LED_3_PORT->FIODIR |= LED_3_PIN;
	
	// Init switches as inputs
	SW_1_PORT->FIODIR &= ~SW_1_PIN;
	SW_2_PORT->FIODIR &= ~SW_2_PIN;
	NAV_PUSH_PORT->FIODIR &= ~NAV_PUSH_PIN;
	NAV_UP_PORT->FIODIR &= ~NAV_UP_PIN;
	NAV_RIGHT_PORT->FIODIR &= ~NAV_RIGHT_PIN;
	NAV_DOWN_PORT->FIODIR &= ~NAV_DOWN_PIN;
	NAV_LEFT_PORT->FIODIR &= ~NAV_LEFT_PIN;
	
	// Turn off LEDs
	IO_LED_Off(IO_LED_ONBOARD | IO_LED_1 | IO_LED_2 | IO_LED_3);
}

//====================================================================================
void IO_LED_On(uint8_t led)
{
	if(led & IO_LED_ONBOARD)
	{
		LED_ONBOARD_PORT->FIOSET = LED_ONBOARD_PIN;
	}
	
	if(led & IO_LED_1)
	{
		LED_1_PORT->FIOCLR = LED_1_PIN;
	}
	
	if(led & IO_LED_2)
	{
		LED_2_PORT->FIOCLR = LED_2_PIN;
	}
	
	if(led & IO_LED_3)
	{
		LED_3_PORT->FIOCLR = LED_3_PIN;
	}
}

//====================================================================================
void IO_LED_Off(uint8_t led)
{
	if(led & IO_LED_ONBOARD)
	{
		LED_ONBOARD_PORT->FIOCLR = LED_ONBOARD_PIN;
	}
	
	if(led & IO_LED_1)
	{
		LED_1_PORT->FIOSET = LED_1_PIN;
	}
	
	if(led & IO_LED_2)
	{
		LED_2_PORT->FIOSET = LED_2_PIN;
	}
	
	if(led & IO_LED_3)
	{
		LED_3_PORT->FIOSET = LED_3_PIN;
	}
}

//====================================================================================
uint8_t IO_GetSW(void)
{
	uint8_t res = 0;
	
	if((SW_1_PORT->FIOPIN & SW_1_PIN) == 0)
	{
		res |= IO_SW_1;
	}
	
	if((SW_2_PORT->FIOPIN & SW_2_PIN) == 0)
	{
		res |= IO_SW_2;
	}
	
	if((NAV_PUSH_PORT->FIOPIN & NAV_PUSH_PIN) == 0)
	{
		res |= IO_NAV_PUSH;
	}
	
	if((NAV_UP_PORT->FIOPIN & NAV_UP_PIN) == 0)
	{
		res |= IO_NAV_UP;
	}
	
	if((NAV_RIGHT_PORT->FIOPIN & NAV_RIGHT_PIN) == 0)
	{
		res |= IO_NAV_RIGHT;
	}
	
	if((NAV_DOWN_PORT->FIOPIN & NAV_DOWN_PIN) == 0)
	{
		res |= IO_NAV_DOWN;
	}
	
	if((NAV_LEFT_PORT->FIOPIN & NAV_LEFT_PIN) == 0)
	{
		res |= IO_NAV_LEFT;
	}
	
	return(res);
}
