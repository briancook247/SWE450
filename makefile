#
TARGET = milestone_3
ALT_DEVICE_FAMILY ?= soc_cv_av
SOCEDS_ROOT ?= $(SOCEDS_DEST_ROOT)
HWLIBS_ROOT = $(SOCEDS_ROOT)/ip/altera/hps/altera_hps/hwlib
CROSS_COMPILE = arm-linux-gnueabihf-
CFLAGS = -g -Wall   -D$(ALT_DEVICE_FAMILY) -I$(HWLIBS_ROOT)/include/$(ALT_DEVICE_FAMILY)   -I$(HWLIBS_ROOT)/include/  -std=c99
LDFLAGS =  -g -Wall -lrt
CC = $(CROSS_COMPILE)gcc
ARCH= arm

build: $(TARGET)

$(TARGET): app.o hardware.o lcd/terasic_lib.o lcd/LCD_Lib.o lcd/LCD_Driver.o lcd/lcd_graphic.o lcd/LCD_Hw.o lcd/font.o
	$(CC) $(LDFLAGS)   $^ -o $@  -lrt -lm

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(TARGET) *.a *.o *~