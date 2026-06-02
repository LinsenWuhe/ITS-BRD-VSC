#ifndef _BMP_READER_H
#define _BMP_READER_H

#include "BMP_types.h"
#include "errors.h"

/**
*Liest und prüft BITMAPFILEHEADER und BITMAPINFOHEADER
* fh zeigt auf Bitmapfileheader
* ih zeigt auf Bitmapinfoheader
*/
int BMP_readHeaders(BITMAPFILEHEADER *fh, BITMAPFILEHEADER *ih);

/**
* Liest die Farbpalette (8-Bit)
* ih zeigt auf Bitmapinfoheader
* palette ist Array für Palette (max. 265 Einträge)
*/
int BMP_readPalette(BITMAPINFOHEADER *ih, RGBQUAD *palette);


/**
*Dekodiert das gesamte Bild und gibt Pixel auf Display aus
* fh ist zeiger auf Bitmapfileheader
* ih ist zeiger auf Bitmapinfoheader
* palette ist Farbpalette
*/
int BMP_decodeAndDisplay(BITMAPFILEHEADER *fh, BITMAPINFOHEADER *ih, RGBQUAD *palette);

#endif