#ifndef _IO_H_
#define _IO_H_

enum
{
	IO_LED_ONBOARD	= 0x01,
	IO_LED_1		= 0x02,
	IO_LED_2		= 0x04,
	IO_LED_3		= 0x08,
};

enum
{
	IO_SW_1			= 0x01,
	IO_SW_2			= 0x02,
	IO_NAV_PUSH		= 0x04,
	IO_NAV_UP		= 0x08,
	IO_NAV_RIGHT	= 0x10,
	IO_NAV_DOWN		= 0x20,
	IO_NAV_LEFT		= 0x40,
};

void IO_Init(void);
void IO_LED_On(uint8_t led);
void IO_LED_Off(uint8_t led);
uint8_t IO_GetSW(void);

#endif // _IO_H_