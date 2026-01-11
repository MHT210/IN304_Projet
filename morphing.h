#ifndef MORPHING_H
#define MORPHING_H

#include "Triangles.h"
#include "ImageFunctions.h"
#include "ChainedList.h"
#include "CouplesFunctions.h"
#include "math.h"
#include "uvsqgraphics_2.h"
#include "stdlib.h"
#include "math.h"

typedef struct POINTS_INT { // structure qui va stocker tout les points intermédiaires créer a partir des couples de points
    POINT P;
    struct POINTS_INT *suiv;
} POINTS_INT;

POINTS_INT * getIntPoints(POINTS_INT * list_I, LISTE_POINTS * listD_G, float alpha);

IMAGE_INTER * morphing(POINTS_INT * Head_I, IMAGE_INTER * Images, TRIANGLE_HEAD TH_I, TRIANGLE_HEAD TH_D, TRIANGLE_HEAD TH_A, float alpha, IMAGE I, IMAGE I2, IMAGE I3);

void test_I3(IMAGE I3);

POINTS_INT * insert_last_i(POINTS_INT * list_I, int x, int y);

void free_list(POINTS_INT *head);

#endif