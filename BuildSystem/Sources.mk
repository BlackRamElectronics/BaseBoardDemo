# Project Source Files
SOURCES = main.c
SOURCES += IO.c
SOURCES += retarget.c

# System Source Files
SOURCES += LPC175x_6x/System/system_LPC17xx.c

# Peripheral Source Files, comment in as required
SOURCES += LPC175x_6x/Peripherals/source/lpc17xx_adc.c
SOURCES += LPC175x_6x/Peripherals/source/lpc17xx_clkpwr.c
#SOURCES += LPC175x_6x/Peripherals/source/lpc17xx_i2c.c
SOURCES += LPC175x_6x/Peripherals/source/lpc17xx_pinsel.c
SOURCES += LPC175x_6x/Peripherals/source/lpc17xx_pwm.c
#SOURCES += LPC175x_6x/Peripherals/source/lpc17xx_systick.c
SOURCES += LPC175x_6x/Peripherals/source/lpc17xx_uart.c
