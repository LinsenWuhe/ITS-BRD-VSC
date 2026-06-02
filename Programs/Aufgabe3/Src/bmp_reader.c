#include "bmp_reader.h"
#include "BMP_types.h"
#include "LCD_GUI.h"
#include "errors.h"
#include "input.h"
#include <stdio.h>



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
        // ih->biBitCount    = 8 oder 24
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
    // Bestimmen, wie viele Farben tatsächlich gelesen werden müssen
    int numColors = ih->biClrUsed;
    
    if (numColors == 0 && ih->biBitCount <= 8) 
    {
        // Wenn 0 eingetragen ist, gilt das Maximum für 8-Bit (256 Farben)
        numColors = 256; 
    }

    // Falls biBitCount > 8 ist (24-Bit), gibt es keine Palette
    if (numColors == 0)
    {
        return OK; // Keine Palette vorhanden
    }

    // palette-struct einlesen
    COMread((char *) palette, sizeof(RGBQUAD), numColors);


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
    //Holt sich Infos aus den gespeicherten headern zu Breite und Höhe
    int width = ih->biWidth;
    int height = ih->biHeight;

//padding berechnen, sodass jede zeile ein Vielfaches von 4 ist
    //zuerst: wie viele Bits brauchen wir? -> width ist anzahl pixel und ih->biCount die anzahl von bits pro pixel
    //dann: nach oben aufrunden mit +31, damit rest bei division nciht abgeschnitten wird -> /32 - wie viele 32 bit blöcke 4 Byte
    //dann: *4 um Anzahl der Bytes zu haben
    //casts wegen compiler warnungen
    int rowSize = (int) ((((unsigned int) width * ih->biBitCount +31)/32) *4);
    int paddingBytes = rowSize - width;

    //zu Pixeldaten vorspringen
    unsigned int numColors = ih->biClrUsed;




        return 0;
}


