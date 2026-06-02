#include "bmp_reader.h"
#include "BMP_types.h"
#include "errors.h"
#include "input.h"


typedef struct {
    LONG  width;        // Breite in Pixeln
    LONG  height;       // Höhe in Pixeln
    WORD  bitCount;     // 8 oder 24
    DWORD compression;  // BI_RGB oder BI_RLE8
    BOOL  hasPalette;   // hat das Bild eine Palette?
    int   padding;      // Padding-Bytes pro Zeile
} BMP_Info;


/**
* fh zeigt auf Bitmapfileheader
* ih zeigt auf Bitmapinfoheader
*/
int BMP_readHeaders(BITMAPFILEHEADER *fh, BITMAPFILEHEADER *ih)
{
    //schreibt 14 Bytes in Struct - mit Umwandlung - fh ist Zeiger auf Bitmapfileheader-Struct, aber COM-Read erwaretet ein Char Zeiger
    COMread((char *) fh, sizeof(BITMAPFILEHEADER), 1); //fh - speicherort von buffer, Elementgröße von BMP-FH, 1 Element der Größe lesen
        // jetzt ist fh gefüllt:
        // fh->bfType    = 0x4D42  ("BM")
        // fh->bfSize    = Dateigröße
        // fh->bfOffBits = Offset zu Pixeldaten

    if (fh->bfType != 0x4D42) 
    {
        return KEINE_BMP_DATEI;
    }

    // 2. InfoHeader lesen — COMread schreibt 40 Bytes in die Struct
    COMread((char*) ih, sizeof(BITMAPINFOHEADER), 1);
        // jetzt ist ih gefüllt:
        // ih->biWidth       = Breite
        // ih->biHeight      = Höhe
        // ih->biBitCount    = 8
        // ih->biCompression = BI_RGB oder BI_RLE8
        // ih->biClrUsed     = Anzahl Palettenfarben

    


    return 0;
}

/**
* Liest die Farbpalette (8-Bit)
* ih zeigt auf Bitmapinfoheader
* palette ist Array für Palette (max. 265 Einträge)
*/
int BMP_readPalette(BITMAPINFOHEADER *ih, RGBQUAD *palette)
{
    return 0;
}


/**
*Dekodiert das gesamte Bild und gibt Pixel auf Display aus
* fh ist zeiger auf Bitmapfileheader
* ih ist zeiger auf Bitmapinfoheader
* palette ist Farbpalette
*/
int BMP_decodeAndDisplay(BITMAPFILEHEADER *fh, BITMAPINFOHEADER *ih, RGBQUAD *palette)
{
    return 0;
}


