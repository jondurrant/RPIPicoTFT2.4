/*
 * Ili9341.h
 *
 * Partial library to control an Ili9341 TFT Screen of 240x320.
 * Based on some of the work of https://github.com/RPiks/pico-touchscr-sdk
 *
 *
 *  Created on: 6 Apr 2024
 *      Author: jondurrant
 */

#ifndef SRC_ILI9341_H_
#define SRC_ILI9341_H_

#include "pico/stdlib.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"


#define MHz                 1000000L

class Ili9341 {
public:
	/***
	 *
	 * @param pspi_port - SPI Device
	 * @param spi_clock_freq SPI clock freq (SCK pin).
	 * @param gpio_MISO - Master input slave output pin.
	 * @param gpio_CS - Chip select pin, active low.
	 * @param gpio_SCK - Serial clock pin.
	 * @param gpio_MOSI - Master output slave input pin.
	 * @param gpio_RS - Reset pin, active low.
	 * @param gpio_DC - Data/command switch pin.
	 * @param led - led Pin
	 * @param portrait = true for portrait
	 */
	Ili9341(
			spi_inst_t *pspi_port,
	        int spi_clock_freq,
			uint8_t gpio_MISO,
			uint8_t gpio_CS,
	        uint8_t gpio_SCK,
			uint8_t gpio_MOSI,
			uint8_t gpio_RS,
			uint8_t gpio_DC,
			uint8_t led,
			bool portrait = true);
	virtual ~Ili9341();

	/***
	 * Convert RGB into 16bit colour for screen
	 * @param r
	 * @param g
	 * @param b
	 * @return
	 */
	uint16_t RGBto16bit(uint8_t r, uint8_t g, uint8_t b);

	/***
	 * Convert RGB + Alpha into 16bit colour for screen
	 * @param r
	 * @param g
	 * @param b
	 * @param a
	 * @return
	 */
	uint16_t RGBAto16bit(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	/***
	 * Draw a rectangle of a specified colour
	 * @param x top left
	 * @param y top left
	 * @param wid width
	 * @param hei height
	 * @param colour in 16bit
	 */
	void drawRect(
			uint16_t x,
			uint16_t y,
			uint16_t wid,
			uint16_t hei,
			uint16_t colour
			);

	/***
	 * Draw a 16Bit Bitmap
	 * @param x top left
	 * @param y top left
	 * @param wid width
	 * @param hei height
	 * @param bitmap array of 16bit values
	 */
	void drawBitmap16(
			uint16_t x,
			uint16_t y,
			uint16_t wid,
			uint16_t hei,
			const uint16_t * bitmap
			);

	/***
	 * Draw a 1 bit bitmap
	 * @param x top left
	 * @param y top left
	 * @param wid width
	 * @param hei height
	 * @param bitmap data in 1bit format
	 * @param fg - foreground colour for values set to 1
	 * @param bg - background colour for values set to 0
	 */
	void drawBitmap1(
			uint16_t x,
			uint16_t y,
			uint16_t wid,
			uint16_t hei,
			const uint8_t * bitmap,
			uint16_t fg = 0xffff,
			uint16_t bg = 0x0000
			);

	/***
	 * Draw a 32bit bitmap on screen. Reduce to 16Bit
	 * @param x top left
	 * @param y top left
	 * @param wid width
	 * @param hei height
	 * @param bitmap 32bit array
	 */
	void drawBitmap32(
			uint16_t x,
			uint16_t y,
			uint16_t wid,
			uint16_t hei,
			const uint32_t * bitmap
			);


	void drawBitmapRGB8(
			uint16_t x,
			uint16_t y,
			uint16_t wid,
			uint16_t hei,
			const uint8_t * bitmap,
			bool whiteIsBlack = false
			);

	/***
	 * put character at position using default font (32bit font)
	 * @param c character
	 * @param x top left
	 * @param y top left
	 * @param fg foreground colour
	 * @param bg background colour
	 * @return width of the charater printed (<0 if error)
	 */
	int putChar(
			char c,
			uint16_t x,
			uint16_t y,
			uint16_t fg = 0xffff,
			uint16_t bg = 0x0000
			);


	/***
	 * put string at position using default font (32bit font)
	 * @param s null terminated string
	 * @param x top left
	 * @param y top left
	 * @param fg foreground colour
	 * @param bg background colour
	 * @return x possition after printing (<0 if error occured)
	 */
	int putStr(
			const char *s,
			uint16_t x,
			uint16_t y,
			uint16_t fg = 0xffff,
			uint16_t bg = 0x0000
			);


	/***
	 * Clear the screen
	 */
	void clear();


	/***
	 * Display a test pattern.
	 * For development purposes only
	 */
	void test();



	bool isPortrait();

	uint16_t getWidth();
	uint16_t getHeight();


protected:
	/***
	 * Initialise
	 * @param pspi_port - SPI Device
	 * @param spi_clock_freq SPI clock freq (SCK pin).
	 * @param gpio_MISO - Master input slave output pin.
	 * @param gpio_CS - Chip select pin, active low.
	 * @param gpio_SCK - Serial clock pin.
	 * @param gpio_MOSI - Master output slave input pin.
	 * @param gpio_RS - Reset pin, active low.
	 * @param gpio_DC - Data/command switch pin.
	 * @param led - led Pin
	 */
	void  init(
			spi_inst_t *pspi_port,
	        int spi_clock_freq,
			uint8_t gpio_MISO,
			uint8_t gpio_CS,
	        uint8_t gpio_SCK,
			uint8_t gpio_MOSI,
			uint8_t gpio_RS,
			uint8_t gpio_DC,
			uint8_t led);


	/***
	 * Set chip select
	 * @param state
	 */
	void ILI9341_CS_Set( int state) ;

	/***
	 * Set command for TFT parameters
	 * @param cmd
	 */
	void ILI9341_SetCommand(uint8_t cmd) ;

	/***
	 * Set command parameter
	 * @param data
	 */
	void ILI9341_CommandParam(uint8_t data);

	/***
	 * Write to screen
	 * @param start_col
	 * @param end_col
	 * @param start_page
	 * @param end_page
	 */
	void ILI9341_SetOutWriting(
	                            const int start_col,
								const int end_col,
	                            const int start_page,
								const int end_page);

	/***
	 * Write data to screen/
	 * @param buffer
	 * @param bytes
	 */
	void ILI9341_WriteData(const void *buffer,int bytes);


	/***
	 * Draw a clipped image from a 1bit bitmap
	 * @param x
	 * @param y
	 * @param wid
	 * @param dataWid
	 * @param hei
	 * @param bitmap
	 * @param fg
	 * @param bg
	 */
	void intDrawBitmap1(
			uint16_t x,
			uint16_t y,
			uint16_t wid,
			uint16_t dataWid,
			uint16_t hei,
			const uint8_t * bitmap,
			uint16_t fg,
			uint16_t bg
			);


private:
	spi_inst_t *pSpi;
	int xSpiClockFreq;
	uint8_t xGpMISO;
	uint8_t xGpCS;
	uint8_t xGpSCK;
	uint8_t xGpMOSI;
	uint8_t xGpRS;
	uint8_t xGpDC;
	uint8_t xGpLed;

	bool xPortrait;
	uint16_t xWidth;
	uint16_t xHeight;


};

#endif /* SRC_ILI9341_H_ */
