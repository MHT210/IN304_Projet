#ifndef COUPLES_FUNCTIONS_H
#define COUPLES_FUNCTIONS_H

#include "uvsqgraphics_2.h"
#include "ChainedList.h"
#include "Button.h"
#include "ImageFunctions.h"
#include "CouplesFunctions.h"

#define INTER_IMAGE 950
#define LARGEUR 1500
#define HAUTEUR 900
#define CIRCLE_RAYON 10
#define WHITE couleur_RGB(255,255,255)
#define RED couleur_RGB(255,0,0)

LISTE_POINTS * Create_Couples_of_Points(LISTE_POINTS * Head, IMAGE I, IMAGE I2, BOUTON stop, BOUTON suppr, BOUTON quitter);

void Save_Point_Couples(LISTE_POINTS * Head);

void Read_Point_Couples(char *filename);

#endif