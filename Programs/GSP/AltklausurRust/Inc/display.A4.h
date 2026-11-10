#ifndef DISPLAY_H
#define DISPLAY_H

/**
 * Initiallisert das Display inklusive der Touch-Funktionalität
 */
void initDisplay();

/**
Initialisiert die Ausgabe des Zahlen-Puzzles
 */
void initPuzzle(void);

/**
Jedem Block werden Koordinaten zugewiesen, die für das ganze Spiel konstan sind
*/
void initPiecePosition(int pos);

/**
Zeichnet ein weißes Rechteck an der entsprechenden Position
* @param pos Die Position 1-8 zeichnen weiße Rechtecke, 0 zeichnet ein schwarzes Rechteck
*/
void drawPiece(int pos);

/**
Das gesamte Puzzel wird ausgegeben
*/
void drawPuzzle(void);

#endif