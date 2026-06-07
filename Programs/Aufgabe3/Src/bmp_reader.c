#include "bmp_reader.h"
#include "BMP_types.h"
#include "LCD_GUI.h"
#include "LCD_general.h"
#include "errors.h"
#include "input.h"
#include <stdio.h>



/**
* fh zeigt auf Bitmapfileheader
* ih zeigt auf Bitmapinfoheader
*/
int BMP_readHeaders(BITMAPFILEHEADER *fh, BITMAPINFOHEADER *ih)
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
    //Breite und Höhe aus Info Header lesen
    int width   = ih->biWidth;
    int height  = ih->biHeight;

    //Zeilen müssen durch 4 bytes teilbar sein -> padding bytes berechnen
    int rowSize = (((width * ih->biBitCount) + 31) / 32) * 4;
    int padding = rowSize - (width * ih->biBitCount / 8);

    // unkomprimiert
    if (ih->biCompression == BI_RGB) 
    {
        //äußere schleife: höhe durchlaufen
        for (int row = 0; row < height; row++) 
        {
            //innere schleife: zeilen durchlaufen
            for (int col = 0; col < width; col++) 
            {
                // nächsten Pixel lesen
                int index = nextChar();
                // Farbe aus Palette holen
                RGBQUAD color = palette[index];
                // y-Koordinate spiegeln - BMP speichert von unten nach oben -> row 0 ist unterste zeile im Bild - wird an y gespiegelt
                int y = height - 1 - row;
                int x = col;
                // nur zeichnen wenn innerhalb Display
                if (x < LCD_WIDTH && y < LCD_HEIGHT) 
                {
                    //farbkonvertierung von rgb (3x8 Bit) auf 16 Bit LCD-Format
                    COLOR c = ((color.rgbRed >> 3) << 11) | //Rot 8 Bit -> 5 bit >> 3, dann in Bits 15-11 (<<11)
                              ((color.rgbGreen >> 2) << 5) | //Grün: 8-Bit → 6-Bit (>> 2), dann in Bits 10-5 (<< 5)
                              (color.rgbBlue >> 3);         //Blau: 8-Bit → 5-Bit (>> 3), bleibt in Bits 4-0
                    //Pixel zeichnen
                    Coordinate crd = {x, y};
                    GUI_drawPoint(crd, c, DOT_PIXEL_1X1, DOT_FILL_AROUND);
                }
            }
            // Padding-Bytes überspringen
            for (int p = 0; p < padding; p++) 
            {
                nextChar();
            }
        }
    } 

    //wenn komprimiert ist
    else if (ih->biCompression == BI_RLE8) 
    {
        //Startposition wieder unten links
        int x = 0;
        int y = height - 1;  //von unten starten

        while (1) 
        {
            //RLE liest 2 Bytes und entscheidet, was zu tun ist
            int b1 = nextChar();  // erstes Byte
            int b2 = nextChar();  // zweites Byte

            if (b1 > 0) 
            {
                // Fall 1 : Encoded Mode: b1 mal Farbe b2 zeichnen
                for (int i = 0; i < b1; i++) 
                {
                    if (x < LCD_WIDTH && y >= 0 && y < LCD_HEIGHT) 
                    {
                        RGBQUAD color = palette[b2];
                        COLOR c = ((color.rgbRed >> 3) << 11) |
                                ((color.rgbGreen >> 2) << 5) |
                                (color.rgbBlue >> 3);
                        Coordinate crd = {x, y};
                        GUI_drawPoint(crd, c, DOT_PIXEL_1X1, DOT_FILL_AROUND);
                    }
                    x++;
                }
            } 
            else 
            {
                //Fall 2: b1 == 0 -> Escape
                if (b2 == 0) 
                {
                    // nächste Zeile, x wieder auf 0 setzen, y eine Zeile höher
                    x = 0;
                    y--;
                }
                else if (b2 == 1) 
                {
                    //Fertig, Schleife beenden
                    break;
                } 
                else if (b2 == 2) 
                {
                    // cursor um x einheiten nach rechts und y einheiten nach oben verschieben -> nächsten 2 Bytes geben versatz an
                    x += nextChar();
                    y -= nextChar();
                } 
                else 
                {
                    //Nächste 2 Bytes sind einzelne Palettenindizes, jeder ein eigener Pixel - Danach mus wieder auf Wortgrenze aufgefüll werden - falls 2.byte ungerade ist, kommt ein extra Padding byte hinzu
                    for (int i = 0; i < b2; i++) 
                    {
                        int index = nextChar();
                        if (x < LCD_WIDTH && y >= 0 && y < LCD_HEIGHT) 
                        {
                            RGBQUAD color = palette[index];
                            COLOR c = ((color.rgbRed >> 3) << 11) |
                                    ((color.rgbGreen >> 2) << 5) |
                                    (color.rgbBlue >> 3);
                            Coordinate crd = {x, y};
                            GUI_drawPoint(crd, c, DOT_PIXEL_1X1, DOT_FILL_AROUND);
                        }
                        x++;
                    }
                    // Word-Padding: Anzahl Bytes muss gerade sein
                    if (b2 % 2 != 0) 
                    {
                        nextChar();
                    }
                }
            }
        }
    }
    return OK;
}

int BMP_decodeAndDisplayWithWriteLine(BITMAPFILEHEADER *fh, BITMAPINFOHEADER *ih, RGBQUAD *palette)
{
    //Breite und Höhe aus Info Header lesen
    int width   = ih->biWidth;
    int height  = ih->biHeight;


    //Zeilen müssen durch 4 bytes teilbar sein -> padding bytes berechnen
    int rowSize = (((width * ih->biBitCount) + 31) / 32) * 4;
    int padding = rowSize - (width * ih->biBitCount / 8); // ?

    // unkomprimiert
    if (ih->biCompression == BI_RGB) 
    {
        COLOR colors[width];

        //äußere schleife: höhe durchlaufen
        for (int row = 0; row < height; row++) 
        {
            //innere schleife: zeilen durchlaufen
            for (int col = 0; col < width; col++) 
            {
                // nächsten Pixel lesen
                int index = nextChar();
                // Farbe aus Palette holen
                RGBQUAD color = palette[index];
                // y-Koordinate spiegeln - BMP speichert von unten nach oben -> row 0 ist unterste zeile im Bild - wird an y gespiegelt
                int y = height - 1 - row;
                int x = col;
                // nur zeichnen wenn innerhalb Display
                if (x < LCD_WIDTH && y < LCD_HEIGHT) 
                {
                    //farbkonvertierung von rgb (3x8 Bit) auf 16 Bit LCD-Format
                    COLOR c = ((color.rgbRed >> 3) << 11) | //Rot 8 Bit -> 5 bit >> 3, dann in Bits 15-11 (<<11)
                              ((color.rgbGreen >> 2) << 5) | //Grün: 8-Bit → 6-Bit (>> 2), dann in Bits 10-5 (<< 5)
                              (color.rgbBlue >> 3);         //Blau: 8-Bit → 5-Bit (>> 3), bleibt in Bits 4-0
                    //Pixel zeichnen
                    Coordinate crd = {0, y};
                    colors[col] = c;
                    GUI_WriteLine(crd, width, colors);
                }
            }
            // Padding-Bytes überspringen
            for (int p = 0; p < padding; p++) 
            {
                nextChar();
            }
        }
    } 

    //wenn komprimiert ist
    else if (ih->biCompression == BI_RLE8) 
    {
        COLOR colors[width];

        //Startposition wieder unten links
        int x = 0;
        int y = height - 1;  //von unten starten

        while (1) 
        {
            //RLE liest 2 Bytes und entscheidet, was zu tun ist
            int b1 = nextChar();  // erstes Byte
            int b2 = nextChar();  // zweites Byte

            if (b1 > 0) 
            {
                // Fall 1 : Encoded Mode: b1 mal Farbe b2 zeichnen
                for (int i = 0; i < b1; i++) 
                {
                    if (x < LCD_WIDTH && y >= 0 && y < LCD_HEIGHT)
                    {
                        RGBQUAD color = palette[b2];
                        COLOR c = ((color.rgbRed >> 3) << 11) |
                                ((color.rgbGreen >> 2) << 5) |
                                (color.rgbBlue >> 3);
                        
                        // Color Array für Zeile auffüllen
                        Coordinate crd = {0, y};
                        colors[x] = c;
                    }
                    x++;
                }
            } 
            else 
            {
                //Fall 2: b1 == 0 -> Escape
                if (b2 == 0) 
                {
                    // nächste Zeile, x wieder auf 0 setzen, y eine Zeile höher
                    if (y < LCD_HEIGHT)
                    {
                        Coordinate crd = {0, y};
                        GUI_WriteLine(crd, width, colors);
                    }
                    
                    x = 0;
                    y--;
                }
                else if (b2 == 1) 
                {
                    //Fertig, Schleife beenden
                    Coordinate crd = {0, y};
                    GUI_WriteLine(crd, width, colors);
                    break;
                } 
                else if (b2 == 2) 
                {
                    // cursor um x einheiten nach rechts und y einheiten nach oben verschieben -> nächsten 2 Bytes geben versatz an
                    x += nextChar();
                    y -= nextChar();
                } 
                else 
                {
                    //Nächste b2 Bytes sind einzelne Palettenindizes, jeder ein eigener Pixel - Danach mus wieder auf Wortgrenze aufgefüll werden - falls b2 ungerade ist, kommt ein extra Padding byte hinzu, dies passeirt bei "// Word-Padding:..."
                    for (int i = 0; i < b2; i++) 
                    {
                        int index = nextChar();
                        if (x < LCD_WIDTH && y >= 0 && y < LCD_HEIGHT) 
                        {
                            RGBQUAD color = palette[index];
                            COLOR c = ((color.rgbRed >> 3) << 11) |
                                    ((color.rgbGreen >> 2) << 5) |
                                    (color.rgbBlue >> 3);

                            colors[x] = c;
                        }
                        x++;
                    }
                    // Word-Padding: Anzahl Bytes muss gerade sein
                    if (b2 % 2 != 0) 
                    {
                        nextChar();
                    }
                }
            }
        }
    }
    return OK;
}

