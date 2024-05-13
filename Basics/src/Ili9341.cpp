/*
 * Ili9341.cpp
 *
 *  Created on: 6 Apr 2024
 *      Author: jondurrant
 */

#include "Ili9341.h"
#include "ili9341hw.h"

#include "hardware/pwm.h"
#include "ColorUtils.h"

#include "PubSansBlack.h"

Ili9341::Ili9341(
		spi_inst_t *pspi_port,
        int spi_clock_freq,
		uint8_t gpio_MISO,
		uint8_t gpio_CS,
        uint8_t gpio_SCK,
		uint8_t gpio_MOSI,
		uint8_t gpio_RS,
		uint8_t gpio_DC,
		uint8_t led,
		bool portrait) {

	xPortrait = portrait;
	if (xPortrait){
		xWidth = 240;
		xHeight = 320;
	} else {
		xWidth = 320;
		xHeight = 240;
	}

	init(
			pspi_port,
            spi_clock_freq,
			gpio_MISO,
			gpio_CS,
            gpio_SCK,
			gpio_MOSI,
			gpio_RS,
			gpio_DC,
			led
			);



}

Ili9341::~Ili9341() {
	// TODO Auto-generated destructor stub
}

bool Ili9341::isPortrait(){
	return xPortrait;
}

uint16_t Ili9341::getWidth(){
	return xWidth;
}

uint16_t Ili9341::getHeight(){
	return xHeight;
}


void  Ili9341::init(
		spi_inst_t *pspi_port,
        int spi_clock_freq,
		uint8_t gpio_MISO,
		uint8_t gpio_CS,
        uint8_t gpio_SCK,
		uint8_t gpio_MOSI,
		uint8_t gpio_RS,
		uint8_t gpio_DC,
		uint8_t led)
{


	pSpi = pspi_port;
	xGpMISO = gpio_MISO;
	xGpCS = gpio_CS;
	xGpSCK = gpio_SCK;
	xGpMOSI = gpio_MOSI;
	xGpRS = gpio_RS;
	xGpDC = gpio_DC;
	xGpLed = led;


	gpio_init(xGpLed);
	gpio_set_function(xGpLed, GPIO_FUNC_PWM);
	pwm_set_gpio_level(xGpLed, 0xFFFF);
	uint sliceNum = pwm_gpio_to_slice_num(xGpLed);
	pwm_set_enabled(sliceNum, true);


    spi_init(pSpi, spi_clock_freq);
    const int baudrate = spi_set_baudrate(pSpi, spi_clock_freq);

    gpio_set_function(xGpMISO, GPIO_FUNC_SPI);
    gpio_set_function(xGpSCK, GPIO_FUNC_SPI);
    gpio_set_function(xGpMOSI, GPIO_FUNC_SPI);

    gpio_init(xGpCS);
    gpio_set_dir(xGpCS, GPIO_OUT);
    gpio_put(xGpCS, 1);

    gpio_init(xGpRS);
    gpio_set_dir(xGpRS, GPIO_OUT);
    gpio_put(xGpRS, 1);

    gpio_init(xGpDC);
    gpio_set_dir(xGpDC, GPIO_OUT);
    gpio_put(xGpDC, 0);

    sleep_ms(10);
    //vTaskDelay(10);
    gpio_put(xGpRS, 0);
    sleep_ms(10);
    //vTaskDelay(10);
    gpio_put(xGpRS, 1);

    ILI9341_SetCommand(0x01);  // Do reset.
    sleep_ms(100);
    //vTaskDelay(100);

    ILI9341_SetCommand(ILI9341_GAMMASET);
    //ILI9341_CommandParam(0x01);
    //Best mode for this display
    ILI9341_CommandParam(0x04);

    // positive gamma correction.
    ILI9341_SetCommand(ILI9341_GMCTRP1);
    {
		uint8_t data[15] = { 0x0f, 0x31, 0x2b, 0x0c, 0x0e, 0x08, 0x4e, 0xf1,
				0x37, 0x07, 0x10, 0x03, 0x0e, 0x09, 0x00};
		ILI9341_WriteData(
				data,
				15);
    }
    /*
    ILI9341_WriteData((uint8_t[15])
        { 0x0f, 0x31, 0x2b, 0x0c, 0x0e, 0x08, 0x4e, 0xf1,
          0x37, 0x07, 0x10, 0x03, 0x0e, 0x09, 0x00
        }, 15);
        */

    // negative gamma correction.
    {
		ILI9341_SetCommand(ILI9341_GMCTRN1);
		uint8_t data[15]= { 0x00, 0x0e, 0x14, 0x03, 0x11, 0x07, 0x31, 0xc1,
				0x48, 0x08, 0x0f, 0x0c, 0x31, 0x36, 0x0f
			  };
		ILI9341_WriteData(
					data,
					15);
    }
    /*
    ILI9341_WriteData((uint8_t[15])
        { 0x00, 0x0e, 0x14, 0x03, 0x11, 0x07, 0x31, 0xc1,
          0x48, 0x08, 0x0f, 0x0c, 0x31, 0x36, 0x0f
        }, 15);
        */

    ILI9341_SetCommand(ILI9341_MADCTL);
    //MV = 1 - Row/Column Exchange
    //BGR = 1 - BGR Panel?
    //ILI9341_CommandParam(0x48)
    if (isPortrait()){
    	ILI9341_CommandParam(0x08);
    } else  {
    	//ILI9341_CommandParam(0x28);
    	ILI9341_CommandParam(0x68);
    }

    ILI9341_SetCommand(ILI9341_PIXFMT);
    ILI9341_CommandParam(0x55);    // 16-bit pixel format.

    ILI9341_SetCommand(ILI9341_FRMCTR1);
    ILI9341_CommandParam(0x00);
    ILI9341_CommandParam(0x1B);

    // Set display on.
    ILI9341_SetCommand(ILI9341_SLPOUT);
    ILI9341_SetCommand(ILI9341_DISPON);
}


void Ili9341::ILI9341_CS_Set( int state) {
    asm volatile("nop \n nop \n nop");
    gpio_put(xGpCS, state);
   asm volatile("nop \n nop \n nop");
}

void Ili9341::ILI9341_SetCommand(uint8_t cmd) {
    ILI9341_CS_Set(CS_ENABLE);
    gpio_put(xGpDC, 0);
    asm volatile("nop \n nop \n nop");
    spi_write_blocking(pSpi, &cmd, 1);
    gpio_put(xGpDC, 1);
    ILI9341_CS_Set(CS_DISABLE);
}

void Ili9341::ILI9341_CommandParam(uint8_t data){
    ILI9341_CS_Set(CS_ENABLE);
    spi_write_blocking(pSpi, &data, 1);
    ILI9341_CS_Set(CS_DISABLE);
}

/// @brief Starts HW writing session of arbitrary area of screen.
/// @param pconfig Control structure.
/// @param start_col Start column of the area.
/// @param end_col Finish column of the area.
/// @param start_page Start row of the area.
/// @param end_page Finish row of the area.
void Ili9341::ILI9341_SetOutWriting(
                            const int start_col,
							const int end_col,
                            const int start_page,
							const int end_page){
    // Column address set.
    ILI9341_SetCommand(ILI9341_CASET);

    ILI9341_CommandParam((start_col >> 8) & 0xFF);
    ILI9341_CommandParam(start_col & 0xFF);

    ILI9341_CommandParam((end_col >> 8) & 0xFF);
    ILI9341_CommandParam(end_col & 0xFF);

    // Page address set.
    ILI9341_SetCommand(ILI9341_PASET);

    ILI9341_CommandParam((start_page >> 8) & 0xFF);
    ILI9341_CommandParam(start_page & 0xFF);

    ILI9341_CommandParam((end_page >> 8) & 0xFF);
    ILI9341_CommandParam(end_page & 0xFF);

    // Start writing.
    ILI9341_SetCommand(ILI9341_RAMWR);

}

/// @brief Writes a data buffer to display.
/// @param pconfig Control structure of display.
/// @param buffer Data buffer.
/// @param bytes Size of the buffer in bytes.
void Ili9341::ILI9341_WriteData( const  void *buffer,int bytes){
    ILI9341_CS_Set(CS_ENABLE);
    spi_write_blocking(pSpi, (const uint8_t * )buffer, bytes);
    ILI9341_CS_Set(CS_DISABLE);
}

uint16_t Ili9341::RGBto16bit(uint8_t r, uint8_t g, uint8_t b) {
    r >>= 3;
    g >>= 2;
    b >>= 3;
    uint16_t res =  (((uint16_t) b & 31) << 11) | (((uint16_t) g & 63) << 5) | ((uint16_t) r & 31);
     return (res >> 8) | (res << 8);
}

uint16_t Ili9341::RGBAto16bit(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	//printf("%X,", a);
	uint16_t res ;

	uint16_t red = r >> 3; // 5 bits
	uint16_t grn =g >> 2; // 6 bits
	uint16_t blu = b >> 3; // 5 bits

	if (a != 0xFF){
		red = (float)red * ((float)a / 0xFF);
		grn = (float)grn * ((float)a / 0xFF);
		blu = (float)blu * ((float)a / 0xFF);
	}

	res = (blu & 0x1F) << 3;
	res = res | (red & 0x1F) << 8;

	res = res |  ((grn & 0x38) >> 3)   |  ((grn & 0x07) << 13);
	//res = res |   ((grn & 0x03) < 14) | ((grn & 0x3C) >> 2);

	return res;
}



void Ili9341::drawRect(
		uint16_t x,
		uint16_t y,
		uint16_t wid,
		uint16_t hei,
		uint16_t colour
		){
	uint16_t sBufLine[PIX_HEIGHT];

	for (int i=0; i < wid; i++){
		sBufLine[i] = colour;
	}


	 ILI9341_SetOutWriting(x, x + wid-1, y, y + hei-1);
	 for (int i=0; i < hei; i++){
		 ILI9341_WriteData(
				 sBufLine,
		          wid * sizeof(uint16_t)
		 );
	 }
}

void Ili9341::drawBitmap16(
		uint16_t x,
		uint16_t y,
		uint16_t wid,
		uint16_t hei,
		const uint16_t * bitmap
		){

	 ILI9341_SetOutWriting(x, x + wid-1, y, y + hei-1);
	 for (int i=0; i < hei; i++){
		 uint16_t line = wid * i;
		 ILI9341_WriteData(
				 &bitmap[line],
				  wid * sizeof(uint16_t)
		 );
	 }

}

void Ili9341::drawBitmap1(
		uint16_t x,
		uint16_t y,
		uint16_t wid,
		uint16_t hei,
		const uint8_t * bitmap,
		uint16_t fg,
		uint16_t bg
		){

	intDrawBitmap1(x, y, wid, wid, hei, bitmap, fg, bg);
}

void Ili9341::intDrawBitmap1(
		uint16_t x,
		uint16_t y,
		uint16_t wid,
		uint16_t dataWid,
		uint16_t hei,
		const uint8_t * bitmap,
		uint16_t fg,
		uint16_t bg
		){
	uint16_t sBufLine[PIX_HEIGHT];

	ILI9341_SetOutWriting(x , x  + wid-1, y  , y + hei-1);

	 for (int yy=0; yy < hei; yy++){
		for (int xx=0; xx < wid; xx++){
			uint16_t index = yy*dataWid + xx;
			uint16_t byteI = index / 8;
			uint8_t bitI = index % 8;
			uint8_t mask = 0x80 >> bitI;

			//printf("byte %u bit %u mask %X\n", byteI, bitI, mask);
			if ((bitmap[byteI] & mask) > 0 ){
				sBufLine[xx] = fg;
			} else {
				sBufLine[xx] = bg;
			}

			//printf("%X,", sBufLine[xx]);

		}
		//printf("\n");



		 ILI9341_WriteData(
				 sBufLine,
				  wid * sizeof(uint16_t)
		 );
	 }

}


void Ili9341::drawBitmap32(
		uint16_t x,
		uint16_t y,
		uint16_t wid,
		uint16_t hei,
		const uint32_t * bitmap
		){

	uint16_t sBufLine[PIX_HEIGHT];

	ILI9341_SetOutWriting(x , x  + wid-1, y  , y + hei-1);

	 for (int yy=0; yy < hei; yy++){
		for (int xx=0; xx < wid; xx++){
			uint16_t index = yy*wid + xx;

			/*
			uint8_t red = (bitmap[index] & 0x00FF0000) >> 16;
			uint8_t grn = (bitmap[index] & 0x0000FF00) >> 8;
			uint8_t blu = (bitmap[index] & 0x000000FF) ;
			*/
			uint8_t alp = (bitmap[index] & 0xFF000000) >> 24;
			uint8_t blu = (bitmap[index] & 0x00FF0000) >> 16;
			uint8_t grn = (bitmap[index] & 0x0000FF00) >> 8 ;
			uint8_t red = (bitmap[index] & 0x000000FF) ;


			sBufLine[xx] = RGBAto16bit(red, grn, blu, alp);

			//printf("%u: %X = %x\n",  index, bitmap[index], sBufLine[index]);
		}

		 ILI9341_WriteData(
				 sBufLine,
				  wid * sizeof(uint16_t)
		 );
	 }
}

void Ili9341::drawBitmapRGB8(
		uint16_t x,
		uint16_t y,
		uint16_t wid,
		uint16_t hei,
		const uint8_t * bitmap,
		bool whiteIsBlack
		){

	uint16_t sBufLine[PIX_HEIGHT];

	ILI9341_SetOutWriting(x , x  + wid-1, y  , y + hei-1);

	 for (int yy=0; yy < hei; yy++){
		for (int xx=0; xx < wid; xx++){
			uint16_t index = (yy*wid + xx) * 3;

			uint8_t red = bitmap[index];
			uint8_t grn = bitmap[index + 1];
			uint8_t blu = bitmap[index + 2] ;

			if (whiteIsBlack && (red == 0xFF) && (grn == 0xFF) && (blu == 0xFF)){
				red = 0;
				grn = 0;
				blu = 0;
			}

			sBufLine[xx] = RGBto16bit(red, grn, blu);

			//printf("%u: %X, %X, %X\n",  index, red, grn, blu);
		}

		 ILI9341_WriteData(
				 sBufLine,
				  wid * sizeof(uint16_t)
		 );
	 }

}





void Ili9341::test(){
	   clear();
		int w = getWidth();
		int h = getHeight();

		//Top Left
		drawRect(0,  0 , 50,50, RGBto16bit(0, 0xff, 0));

		//Bottom Left
		drawRect(0, h-50 , 50,50, RGBto16bit(0, 0, 0xff));

		//Top right
		drawRect(w-50,  0 , 50,50, RGBto16bit(0xff, 0, 0));

		//Bottom right
		drawRect(w-50, h-50 , 50,50, RGBto16bit(0x84, 0x45, 0x13));

		int bar = (w -100)/4;
		for (int y=0; y < h; y++){
			float p = (float)y / (float)h;
			uint8_t c = p * 255.0;
			drawRect(50,  y , bar, 1, RGBto16bit(c, 0, 0));
			drawRect(50+bar,  y , bar, 1, RGBto16bit(0, c, 0));
			drawRect(50+bar*2,  y , bar, 1, RGBto16bit(0, 0, c));

			uint8_t r, g, b;
			ColorUtils::valToRGB(p, r, g, b);
			drawRect(50+bar*3,  y , bar, 1, RGBto16bit(r, g, b));
		}
}


void Ili9341::clear(){
	drawRect(0, 0, getWidth(), getHeight(), 0);
}

int Ili9341::putChar(
		char c,
		uint16_t x,
		uint16_t y,
		uint16_t fg,
		uint16_t bg) {
	if ((c < 32)  > ( c > 0x7E)){
		return -1;
	}
	uint8_t index = c - 32;
	uint16_t glyphI = PubSansBlack_glyph_dsc[index].glyph_index;
	uint8_t trueWid = PubSansBlack_glyph_dsc[index].w_px;

	if ((x+ trueWid) > getWidth()){
		return -1;
	}
	if ((y + 32) > getHeight()){
		return -1;
	}


	uint16_t wid;
	if ( (trueWid % 8) == 0){
		wid = trueWid;
	} else {
		wid = 8 * ((trueWid/8) + 1);
	}

	//printf("%c %u %u %u\n", c, glyphI, trueWid, wid);

	intDrawBitmap1(
			x,
			y,
			trueWid,
			wid,
			32,
			&PubSansBlack_glyph_bitmap[glyphI],
			fg,
			bg
			);

	return trueWid + 1;

}


int Ili9341::putStr(
			const char *s,
			uint16_t x,
			uint16_t y,
			uint16_t fg,
			uint16_t bg
			){

	int len = strlen(s);
	uint16_t xx = x;
	for (int i=0; i < len; i++){
		int res = putChar(
				s[i],
				xx,
				y,
				fg,
				bg);
		if (res < 0){
			res;
		} else {
			xx += res;
		}
	}
	return xx;

}

