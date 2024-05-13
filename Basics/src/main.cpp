/***
 * main.cpp -
 * Basic Demo of MSP2401 display with pico
 * Jon Durrant
 * 13-May-2024
 *
 *
 */

#include "pico/stdlib.h"
#include <stdio.h>

#include <stdio.h>

#include "Ili9341.h"



int main(void) {
  stdio_init_all();
  sleep_ms(2000);
  printf("GO\n");

  Ili9341 *tft = new Ili9341(
  			spi1, 90 * MHz,
  			12,		//SDO(MISO)
  			9, 		//CS
  			10,		//SCK
  			11,		//SDI(MOSI)
  			15,  		//RESET
  			8,			//DC/RS
  			13, 		//LED,
  			true
  			);

  	tft->test();

  sleep_ms(5000);
  delete(tft);

  tft = new Ili9341(
   			spi1, 90 * MHz,
   			12,		//SDO(MISO)
   			9, 		//CS
   			10,		//SCK
   			11,		//SDI(MOSI)
   			15,  		//RESET
   			8,			//DC/RS
   			13, 		//LED,
   			false
   			);

   	tft->test();


    sleep_ms(5000);
    tft->clear();
    tft->putStr(
    		"Hello World",
    		10,50
    );
    tft->putStr(
    		"Hello World",
    		10,100,
			tft->RGBto16bit(0x30, 0x30, 0xFF)
    );
    tft->putStr(
       		"Hello World",
       		10,150,
   			tft->RGBto16bit(0xE0, 0x30, 0x3F)
       );
    tft->putStr(
			"Hello World",
			10,200,
			tft->RGBto16bit(0x10, 0xFF, 0x1F)
	  );





  	for (;;){
  		sleep_ms(3000);
  	}

  return 0;
}
