/**
 * IZG - Zaklady pocitacove grafiky - FIT VUT
 * Lab 01 - Redukce barevneho prostoru
 *
 * Copyright (c) 2022 by Michal Vlnas, ivlnas@fit.vutbr.cz
 *
 * Tato sablona slouzi vyhradne pro studijni ucely, sireni kodu je bez vedomi autora zakazano.
 */

#include "base.h"

void ImageTransform::grayscale()
{    
    uint8_t I;
    RGB color;

    for (uint32_t y = 0; y < cfg->height; y++)
    {
       for (uint32_t x = 0; x < cfg->width; x++)
       {
            /* Get pixel */
            color = getPixel(x, y);
           
            /* Calculate grayscale */
            I = std::round(0.299 * color.r + 0.587 * color.g + 0.114 * color.b);
            color.r = color.g = color.b = I;

            /* Set pixel with grayscale values*/
            setPixel(x, y, color);
       }
   }
}

void ImageTransform::threshold()
{
    RGB color;

    grayscale();

    for (uint32_t y = 0; y < cfg->height; y++)
    {
        for (uint32_t x = 0; x < cfg->width; x++)
        {
            /* Get pixel */
            color = getPixel(x, y);
            
            /* Calculate value with threshold */
            color = color.r > cfg->threshold ? COLOR_WHITE : COLOR_BLACK;

            /* Set pixel */
            setPixel(x, y, color);
        }        
    }    
}

void ImageTransform::randomDithering()
{
    grayscale();

    for (uint32_t y = 0; y < cfg->height; y++)
    {
        for (uint32_t x = 0; x < cfg->width; x++)
        {
            auto p = getPixel(x, y);
            uint8_t value = p.r > getRandom() ? 255 : 0;

            setPixel(x, y, RGB(value));
        }
    }
}

void ImageTransform::orderedDithering()
{
    uint32_t i, j;
    RGB color;

    uint8_t threshMatrix[] =
    {
          0, 204,  51, 255,
         68, 136, 187, 119,
         34, 238,  17, 221,
        170, 102, 153,  85
    };

    grayscale();

    for (uint32_t y = 0; y < cfg->height; y++)
    {
        for (uint32_t x = 0; x < cfg->width; x++)
        {
            /* Get pixel */
            color = getPixel(x, y);
            /* Calculate value with matrix threshold */
            i = x % m_side;
            j = y % m_side;

            color = color.r > threshMatrix[j * m_side + i] ? COLOR_WHITE : COLOR_BLACK; 
            /* Set pixel */
            setPixel(x, y, color);
        }
    }
}

void ImageTransform::updatePixelWithError(uint32_t x, uint32_t y, float err)
{	
    RGB color;
    float tmp;

    if (y < cfg->height && x < cfg->width)
    {
        color = getPixel(x, y);

        tmp = color.g + err;
        
        if ((tmp = std::max(0.0f, tmp)) != 0.0f)
        {
            tmp = std::min(255.0f, tmp);
        }

        setPixel(x, y, RGB(std::round(tmp)));
    }
}

void ImageTransform::errorDistribution()
{
    uint8_t tmp;
    int16_t err;
    RGB color;

    grayscale();

    for (uint32_t y = 0; y < cfg->height; y++)
    {
        for (uint32_t x = 0; x < cfg->width; x++)
        {
            /* Get pixel */
            color = getPixel(x, y);

            /* Store previous intensity value */
            tmp = color.r;

            /* Calculate value with threshold */
            color = color.r > 127 ? COLOR_WHITE : COLOR_BLACK;

            /* Calculate error */
            err = tmp - color.r;

            /* Set pixel */
            setPixel(x, y, color);

            /* Distribute error */
            /*
             * 3/8 = 0.375
             * 2/8 = 0.25
             */

            updatePixelWithError(x+1, y  , (float)err * 0.375);
            updatePixelWithError(x  , y+1, (float)err * 0.375);
            updatePixelWithError(x+1, y+1, (float)err * 0.25 );
        }
    }
}

