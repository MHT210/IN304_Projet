#ifndef IMAGE_H
#define IMAGE_H

#include "uvsqgraphics_2.h"

// Les types de structures : //
typedef struct pixel { //structures qui contient les valeurs RGB d'un pixel
    int R, G, B;
} PIXEL;

typedef struct image { // structure qui contient ttes les infos d'une image y compris tout les pixels de l'image(adaptés au fichier ppm)
    int hauteur;
    int largeur;
    int max_value_rgb;
    int decal_value;
    PIXEL **P;
} IMAGE;

typedef struct Image_Inter { // structure de liste chainée qui va contenir tout les images(frames) créer par la fonction morphing
    IMAGE image;
    struct Image_Inter * suiv;
} IMAGE_INTER;

// Les fonctions pour les images : //
void Write_Image(char *nom, IMAGE I);

IMAGE Read_Image(char *nom, IMAGE I);

int Is_In_Image(IMAGE I, POINT p, int HEIGHT_OF_SDL);

void Show_Image(IMAGE I);

IMAGE_INTER * InsertLastImage(IMAGE_INTER * Images, IMAGE image);

#endif