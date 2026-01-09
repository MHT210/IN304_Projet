#ifndef MORPHING_H
#define MORPHING_H

#include "Triangles.h"
#include "ImageFunctions.h"
#include "ChainedList.h"
#include "CouplesFunctions.h"
#include "math.h"
#include "uvsqgraphics_2.h"

typedef struct POINTS_INT {
    POINT P;
    struct POINTS_INT *suiv;
} POINTS_INT;

POINTS_INT * getIntPoints(POINTS_INT * list_I, LISTE_POINTS * listD_G, float alpha);

void morphing(TRIANGLE_HEAD TH_I, TRIANGLE_HEAD TH_D, TRIANGLE_HEAD TH_A, float alpha, IMAGE I, IMAGE I2, IMAGE I3);

#endif