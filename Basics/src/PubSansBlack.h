/*
 * PubSansBlack.h
 *
 *  Created on: 6 Apr 2024
 *      Author: jondurrant
 */

#ifndef SRC_PUBSANSBLACK_H_
#define SRC_PUBSANSBLACK_H_

#include "pico/stdlib.h"


struct font_dsc{
	uint8_t w_px;
	uint16_t glyph_index;
};

typedef struct font_dsc font_dsc_t;

extern   const uint8_t PubSansBlack_glyph_bitmap[];

extern   const font_dsc_t PubSansBlack_glyph_dsc[];



#endif /* SRC_PUBSANSBLACK_H_ */
