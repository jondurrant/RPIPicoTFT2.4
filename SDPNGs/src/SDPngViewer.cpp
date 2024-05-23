/*
 * SDPngViewer.cpp
 *
 *  Created on: 24 Apr 2024
 *      Author: jondurrant
 */

#include "SDPngViewer.h"

#include "ff_headers.h"
#include "ff_sddisk.h"
#include "ff_stdio.h"
#include "ff_utils.h"
#include "ff_dir.h"
//
#include "hw_config.h"
#include "ff_file.h"

SDPngViewer::SDPngViewer() {
	init();

}

SDPngViewer::~SDPngViewer() {
	// TODO Auto-generated destructor stub
}



bool SDPngViewer::init(){
	pxDisk = FF_SDDiskInit("sd0");
	return pxDisk != NULL;
}


bool SDPngViewer::mount(){
	FF_Error_t xError = FF_SDDiskMount(pxDisk);
	if (FF_isERR(xError) != pdFALSE) {
		printf("FF_SDDiskMount: %s (%d)\n",
			  (const char *)FF_GetErrMessage(xError), xError);
		return false;
	} else {
		FF_FS_Add("/sd0", pxDisk);
		xDirScan = false;
		return true;
	}
}


bool SDPngViewer::unmount(){
	FF_FS_Remove("/sd0");
	FF_Unmount(pxDisk);
	FF_SDDiskDelete(pxDisk);
	return true;
}



int SDPngViewer::spngRead(spng_ctx *ctx, void *user, void *dst_src, size_t length){
	FF_FILE *pfi = (FF_FILE *) user;
	int count = 0;

	count = FF_Read( pfi,
	                1,
	                 length,
	                 (uint8_t *)dst_src );

	if (count >= 0){
		//printf("Read(%u) = %u\n", length, count);
		return 0;
	} else {
		//printf("Read(%u) Failed\n");
		return SPNG_IO_ERROR;
	}
}


bool SDPngViewer::displayPng( Ili9341 *pTft, const char * name){
		size_t size;
		struct spng_ihdr ihdr;
		int res;
		FF_FILE *pfi;
		uint32_t out[320];
		struct spng_row_info row_info;
		bool result = true;
		FF_Error_t error;

		pfi = ff_fopen(name, "r");
		if ( pfi ==  NULL ){
			printf("Failed to open %s\n", name);
			return false;
		}

		spng_ctx *ctx = spng_ctx_new(0);

		res = spng_set_png_stream(
				ctx,
				SDPngViewer::spngRead,
				pfi);

		res = spng_get_ihdr(ctx, &ihdr);
		if (ihdr.width > pTft->getWidth()){
			printf("Image is too wide %u > 240\n", ihdr.width);
			result = false;
		}
		if (ihdr.height > pTft->getHeight()){
			printf("Image is too high %u > 320\n", ihdr.height);
					result = false;
		}
		printf("(%d)PNG (%u, %u)\n", res, ihdr.width, ihdr.height);

		if (result){
			spng_decode_image(ctx, out, size, SPNG_FMT_RGB8, SPNG_DECODE_PROGRESSIVE);
			//spng_decode_image(ctx, out, size, SPNG_FMT_RGBA8, SPNG_DECODE_PROGRESSIVE);

			uint line = 0;
			for(;;){
				res = spng_decode_row( ctx,  out,  ihdr.width*3);
				//res = spng_decode_row( ctx,  out,  ihdr.width*4);
				//printf("(%d)Row decode\n", res);
				if (res == SPNG_EOI){
					printf("EOF Reached\n");
					break;
				}
				if (res != 0){
					printf("Read decode failed\n");
					result = false;
					break;
				}

				res = spng_get_row_info( ctx,  &row_info);
				//printf("(%d)Row Info %d \n", res, row_info.row_num);
				if (res == 0){
					line = (row_info.row_num  -1)% ihdr.width;


					pTft->drawBitmapRGB8(
							0,
							line,
							 ihdr.width,
							 1,
							(uint8_t *)out,
							false
							);
					/*
					pTft->drawBitmap32(
							0,
							line,
							 ihdr.width,
							 1,
							(uint32_t *)out
							);
						*/

				} else {
					result = false;
					break;
				}
			}
		}

		spng_ctx_free(ctx);
		ff_fclose( pfi);
		 return result;
}



bool SDPngViewer::displayFolder( Ili9341 *pTft, const char * folder){

	bool reset = false;
	char file[256];

	for (;;) {
		if (!xDirScan){
			if ( ff_findfirst( folder,  &xDirData ) != 0){
				return false;
			}
			xDirScan = true;
		} else if ( ff_findnext(  &xDirData ) != 0){
			//Need to reset
			if (reset){
				//Already reset once
				return false;
			}
			reset = true;

			if ( ff_findfirst( folder,  &xDirData ) != 0){
				return false;
			}
		}
		if (strlen(xDirData.pcFileName) > 4){
			const char *extension = xDirData.pcFileName + strlen(xDirData.pcFileName) - 4;
			if (
					(xDirData.pcFileName[0] != '.') &&
					(strcmp(extension, ".png") == 0)
					){
						sprintf(file, "%s/%s", folder, xDirData.pcFileName);
						printf("Display %s\n", file);
						return displayPng( pTft,  file );
			}
		}
		printf("Skip %s\n", xDirData.pcFileName);
	}

}

