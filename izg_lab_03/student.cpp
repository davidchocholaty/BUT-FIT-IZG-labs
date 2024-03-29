﻿/**
 * @file        student.cpp
 * @author      Ladislav Mosner, VUT FIT Brno, imosner@fit.vutbr.cz
 * @author      Petr Kleparnik, VUT FIT Brno, ikleparnik@fit.vutbr.cz
 * @author      Kamil Behun, VUT FIT Brno, ibehun@fit.vutbr.cz
 * @date        11.03.2018
 *
 * @brief       Deklarace funkci studentu. DOPLNUJI STUDENTI
 *
 */

#include "base.h"
#include "student.h"
#include "globals.h"
#include <math.h>

/**
 * @brief Vraci barvu pixelu z pozice [x, y]
 * @param[in] x X souradnice pixelu
 * @param[in] y Y souradnice pixelu
 * @return Barva pixelu na pozici [x, y] ve formatu RGBA
 */
RGBA getPixel(int x, int y)
{
    if (x >= width || y >= height || x < 0 || y < 0) {
        IZG_ERROR("Pristup do framebufferu mimo hranice okna\n");
    }
    return framebuffer[y * width + x];
}

/**
 * @brief Nastavi barvu pixelu na pozici [x, y]
 * @param[in] x X souradnice pixelu
 * @param[in] y Y souradnice pixelu
 * @param[in] color Barva pixelu ve formatu RGBA
 */
void putPixel(int x, int y, RGBA color)
{
    if (x >= width || y >= height || x < 0 || y < 0) {
        IZG_ERROR("Pristup do framebufferu mimo hranice okna\n");
    }
    framebuffer[y * width + x] = color;
}

/**
 * @brief Vykresli usecku se souradnicemi [x1, y1] a [x2, y2]
 * @param[in] x1 X souradnice 1. bodu usecky
 * @param[in] y1 Y souradnice 1. bodu usecky
 * @param[in] x2 X souradnice 2. bodu usecky
 * @param[in] y2 Y souradnice 2. bodu usecky
 * @param[in] color Barva pixelu usecky ve formatu RGBA
 * @param[in] arrow Priznak pro vykresleni sipky (orientace hrany)
 */
void drawLine(int x1, int y1, int x2, int y2, RGBA color, bool arrow = false)
{

    if (arrow) {
        // Sipka na konci hrany
        double vx1 = x2 - x1;
        double vy1 = y2 - y1;
        double length = sqrt(vx1 * vx1 + vy1 * vy1);
        double vx1N = vx1 / length;
        double vy1N = vy1 / length;
        double vx1NN = -vy1N;
        double vy1NN = vx1N;
        int w = 3;
        int h = 10;
        int xT = (int) (x2 + w * vx1NN - h * vx1N);
        int yT = (int) (y2 + w * vy1NN - h * vy1N);
        int xB = (int) (x2 - w * vx1NN - h * vx1N);
        int yB = (int) (y2 - w * vy1NN - h * vy1N);
        pinedaTriangle(Point(x2, y2), Point(xT, yT), Point(xB, yB), color, color, false);
    }

    bool steep = abs(y2 - y1) > abs(x2 - x1);

    if (steep) {
        SWAP(x1, y1);
        SWAP(x2, y2);
    }

    if (x1 > x2) {
        SWAP(x1, x2);
        SWAP(y1, y2);
    }

    const int dx = x2 - x1, dy = abs(y2 - y1);
    const int P1 = 2 * dy, P2 = P1 - 2 * dx;
    int P = 2 * dy - dx;
    int y = y1;
    int ystep = 1;
    if (y1 > y2) ystep = -1;

    for (int x = x1; x <= x2; x++) {
        if (steep) {
            if (y >= 0 && y < width && x >= 0 && x < height) {
                putPixel(y, x, color);
            }
        } else {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                putPixel(x, y, color);
            }
        }

        if (P >= 0) {
            P += P2;
            y += ystep;
        } else {
            P += P1;
        }
    }
}

/**
 * @brief Vyplni a vykresli trojuhelnik
 * @param[in] v1 Prvni bod trojuhelniku
 * @param[in] v2 Druhy bod trojuhelniku
 * @param[in] v3 Treti bod trojuhelniku
 * @param[in] color1 Barva vyplne trojuhelniku
 * @param[in] color2 Barva hranice trojuhelniku
 * @param[in] arrow Priznak pro vykresleni sipky (orientace hrany)
 *
 * SPOLECNY UKOL. Doplnuji studenti se cvicicim.
 */
void pinedaTriangle(const Point &v1, const Point &v2, const Point &v3, const RGBA &color1, const RGBA &color2, bool arrow)
{
    int minX, maxX, minY, maxY;
    int e1minX, e2minX, e3minX;
    int deltaX1, deltaY1, deltaX2, deltaY2, deltaX3, deltaY3;
    int e1, e2, e3;

    // Nalezeni obalky (minX, maxX), (minY, maxY) trojuhleniku.

    minX = MIN(v1.x, MIN(v2.x, v3.x));
    maxX = MAX(v1.x, MAX(v2.x, v3.x));

    minY = MIN(v1.y, MIN(v2.y, v3.y));
    maxY = MAX(v1.y, MAX(v2.y, v3.y));


    // Oriznuti obalky (minX, maxX, minY, maxY) trojuhleniku podle rozmeru okna.

    minX = MAX(minX, 0);
    maxX = MIN(maxX, width-1);

    minY = MAX(minY, 0);
    maxY = MIN(maxY, height-1);

    // Spocitani parametru hranove funkce (deltaX, deltaY) pro kazdou hranu.
    // Hodnoty deltaX, deltaY jsou souradnicemi vektoru, ktery ma pocatek
    // v prvnim vrcholu hrany, konec v druhem vrcholu.
    // Vypocet prvnotni hodnoty hranove funkce.

    deltaX1 = v2.x - v1.x;
    deltaY1 = v2.y - v1.y;
	
    deltaX2 = v3.x - v2.x;
    deltaY2 = v3.y - v2.y;
	
    deltaX3 = v1.x - v3.x;
    deltaY3 = v1.y - v3.y;

    e1minX = (minY - v1.y)*deltaX1 - (minX - v1.x)*deltaY1;
    e2minX = (minY - v2.y)*deltaX2 - (minX - v2.x)*deltaY2;
    e3minX = (minY - v3.y)*deltaX3 - (minX - v3.x)*deltaY3;

    // Vyplnovani: Cyklus pres vsechny body (x, y) v obdelniku (minX, minY), (maxX, maxY).
    // Pro aktualizaci hodnot hranove funkce v bode P (x +/- 1, y) nebo P (x, y +/- 1)
    // vyuzijte hodnoty hranove funkce E (x, y) z bodu P (x, y).

    for (int y = minY; y <= maxY; y++)
    {
	    e1 = e1minX;
	    e2 = e2minX;
	    e3 = e3minX;
	    
	    for(int x = minX; x <= maxX; x++)
	    {
		    if (e1 >= 0 && e2 >= 0 && e3 >= 0)
		    {
			    putPixel(x, y, color1);
		    }
		    
		    e1 -= deltaY1;
		    e2 -= deltaY2;
		    e3 -= deltaY3;
	    }
	    
	    e1minX += deltaX1;
	    e2minX += deltaX2;
	    e3minX += deltaX3;
    }

    // Prekresleni hranic trojuhelniku barvou color2.
    drawLine(v1.x, v1.y, v2.x, v2.y, color2, arrow);
    drawLine(v2.x, v2.y, v3.x, v3.y, color2, arrow);
    drawLine(v3.x, v3.y, v1.x, v1.y, color2, arrow);
}

/**
 * @brief Vyplni a vykresli polygon
 * @param[in] points Pole bodu polygonu
 * @param[in] size Pocet bodu polygonu (velikost pole "points")
 * @param[in] color1 Barva vyplne polygonu
 * @param[in] color2 Barva hranice polygonu
 *
 * SAMOSTATNY BODOVANY UKOL. Doplnuji pouze studenti.
 */
void pinedaPolygon(const Point *points, const int size, const RGBA &color1, const RGBA &color2)
{
    int minX, maxX, minY, maxY;
    int dx1, dy1, dx2, dy2;
    int zCrossProduct;
    bool sign;
    bool drawPixel;
    // Pri praci muzete vyuzit pro vas predpripravene datove typy z base.h., napriklad:
    //
    //      Pro ukladani parametru hranovych funkci muzete vyuzit prichystany vektor parametru hranovych funkci "EdgeParams":
    //
    //          EdgeParams edgeParams(size)                         // Vytvorite vektor (pole) "edgeParams" parametru hranovych funkci o velikosti "size".
    //          edgeParams[i].deltaX, edgeParams[i].deltaY          // Pristup k parametrum (deltaX, deltaY) hranove funkce v poli "edgeParams" na indexu "i".
    //
    //      Pro ukladani hodnot hranovych funkci muzete vyuzit prichystany vektor hodnot hranovych funkci "EdgeFncValues":
    //
    //          EdgeFncValues edgeFncValues(size)                   // Vytvorite vektor (pole) "edgeFncValues" hodnot hranovych funkci o velikosti "size".
    //          edgeFncValues[i]                                    // Pristup k hodnote hranove funkce v poli "edgeFncValues" na indexu "i".
    //
    
    
    EdgeParams edgeParams(size);
    EdgeFncValues edgeFncValuesMinX(size);
    EdgeFncValues edgeFncValues(size);
    
    // Nalezeni obalky (minX, maxX), (minY, maxY) polygonu.
    
    minX = maxX = points[0].x;
    minY = maxY = points[0].y;
    
    for (int i = 0; i < size; i++)
    {
        minX = MIN(minX, points[i].x);
        maxX = MAX(maxX, points[i].x);
        minY = MIN(minY, points[i].y);
        maxY = MAX(maxY, points[i].y);
    }

    // Oriznuti obalky (minX, maxX), (minY, maxY) polygonu podle rozmeru okna

    minX = MAX(minX, 0);
    maxX = MIN (maxX, width -1);

    minY = MAX(minY, 0);
    maxY = MIN (maxY, height -1);

    // Spocitani parametru (deltaX, deltaY) hranove funkce pro kazdou hranu.
    // Hodnoty deltaX, deltaY jsou souradnicemi vektoru, ktery ma pocatek
    // v prvnim vrcholu hrany, konec v druhem vrcholu.
    // Vypocet prvnotnich hodnot hranovych funkci pro jednotlive hrany.

    for (int i = 0; i < size; i++)
    {
	    edgeParams[i].deltaX = points[(i+1)%size].x - points[i].x;
	    edgeParams[i].deltaY = points[(i+1)%size].y - points[i].y;
    }	
	    
    for (int i = 0; i < size; i++)
    {
	    edgeFncValuesMinX[i] = (minY - points[i].y)*edgeParams[i].deltaX -
                               (minX - points[i].x)*edgeParams[i].deltaY;		
    }

    // Test konvexnosti polygonu    

    if (size >= 4)
    {	
	    sign = false;
		    
	    for (int i = 0; i < size; i++)
	    {
		    dx1 = points[(i+1)%size].x - points[i].x;
		    dy1 = points[(i+1)%size].y - points[i].y;
		    dx2 = points[(i+2)%size].x - points[(i+1)%size].x;
		    dy2 = points[(i+2)%size].y - points[(i+1)%size].y;
		    zCrossProduct = dx1 * dy2 - dy1 * dx2;
		    
		    if (i == 0)
		    {
			    sign = zCrossProduct > 0;
		    }
		    else if ((zCrossProduct > 0) != sign)
		    {				
			    IZG_INFO("Polygon is non convex!\n");				
			    return;
		    }
	    }
    }
    
    // Vyplnovani: Cyklus pres vsechny body (x, y) v obdelniku (minX, minY), (maxX, maxY).
    // Pro aktualizaci hodnot hranove funkce v bode P (x +/- 1, y) nebo P (x, y +/- 1)
    // vyuzijte hodnoty hranove funkce E (x, y) z bodu P (x, y) 
    
    for (int y = minY; y <= maxY; y++)
    {
	    for (int i = 0; i < size; i++)
	    {
		    edgeFncValues[i] = edgeFncValuesMinX[i];
	    }
	    
	    for (int x = minX; x <= maxX; x++)
	    {
		    drawPixel = true;
					    
		    for (int i = 0; i < size; i++)
		    {
			    if (edgeFncValues[i] < 0)
			    {
				    drawPixel = false;
				    break;
			    }
		    }
		    
		    if (drawPixel)
		    {
			    putPixel(x, y, color1);
		    }
		    
		    for (int i = 0; i < size; i++)
		    {
			    edgeFncValues[i] -= edgeParams[i].deltaY;
		    }
        }
	    
	    for (int i = 0; i < size; i++)
	    {
		    edgeFncValuesMinX[i] += edgeParams[i].deltaX;
	    }
	    
    }

    // Prekresleni hranic polygonu barvou color2.
    for (int i = 0; i < size; i++) {
        drawLine(points[i].x, points[i].y, points[(i + 1) % size].x, points[(i + 1) % size].y, color2/*, true*/);
    }
    
}
