#ifndef IMAGE_H
#define IMAGE_H

#include "uvsqgraphics_2.h"

// Les types de structures : //
typedef struct pixel {
    int R, G, B;
} PIXEL;

typedef struct image {
    int hauteur;
    int largeur;
    int max_value_rgb;
    int decal_value;
    PIXEL **P;
} IMAGE;

// Les fonctions pour les images : //
void Write_Image(char *nom, IMAGE I);

IMAGE Read_Image(char *nom, IMAGE I);

int Is_In_Image(IMAGE I, POINT p, int HEIGHT_OF_SDL);

void Show_Image(IMAGE I);

#endif