#ifndef TRIANGULATE_H
#define TRIANGULATE_H

#define JAUNE couleur_RGB(255, 255, 0)
#define ROUGE couleur_RGB(255, 0, 0)
#define VERT couleur_RGB(0, 255, 100)

#include "Button.h"
#include "uvsqgraphics_2.h"
#include "CouplesFunctions.h"
#include "Triangles.h"
#include "morphing.h"

TRIANGLE_HEAD Get_Triangles(TRIANGLE_HEAD TH, POINTS_INT * Head);

POINTS_INT * CreateHeadList(LISTE_POINTS * HeadCouple, POINTS_INT * Head, int cote);

#endif