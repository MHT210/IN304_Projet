#ifndef BUTTON_H
#define BUTTON_H

#include "uvsqgraphics_2.h"

#define INTER_IMAGE 950
#define LARGEUR 1550
#define HAUTEUR 900
#define CIRCLE_RAYON 10

// le struct
typedef struct bouton {
    char *nom;
    int largeur;
    int hauteur;
    int x;
    int y;
    COULEUR couleur;
} BOUTON;

// les fonctions
void creer_bouton(BOUTON button);

int bouton_cliquer(POINT p, BOUTON button);
#endif