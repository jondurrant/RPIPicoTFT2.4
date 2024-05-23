/*
 * SDPngViewer.h
 *
 *  Created on: 24 Apr 2024
 *      Author: jondurrant
 */

#ifndef SRC_SDPNGVIEWER_H_
#define SRC_SDPNGVIEWER_H_


#include "pico/stdlib.h"
#include <cstdio>
#include "Ili9341.h"
#include "ff_headers.h"
#include "ff_sddisk.h"
#include "ff_stdio.h"
#include <spng.h>


class SDPngViewer {
public:
	SDPngViewer();
	virtual ~SDPngViewer();

	bool init();

	bool mount();

	bool unmount();

	bool displayPng( Ili9341 *pTft, const char * name);

	bool displayFolder( Ili9341 *pTft, const char * folder);

private:
		static int spngRead(spng_ctx *ctx, void *user, void *dst_src, size_t length);



		FF_Disk_t *pxDisk = NULL;

		FF_FindData_t xDirData;
		bool xDirScan = false;
};

#endif /* SRC_SDPNGVIEWER_H_ */
