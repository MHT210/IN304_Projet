#ifndef CHAINED_LIST_H
#define CHAINED_LIST_H

#include "uvsqgraphics_2.h"

// Les types de structures : //
typedef struct couples_points {
    POINT G;
    POINT D;
} COUPLES_POINTS;

typedef struct LISTE_POINTS {
    COUPLES_POINTS CP;
    struct LISTE_POINTS *suiv;
} LISTE_POINTS;

// Les fonctions pour les listes chainées : //
LISTE_POINTS * insert_first(LISTE_POINTS *list, int x1, int y1, int x2, int y2);

LISTE_POINTS * insert_last(LISTE_POINTS *list, int x1, int y1, int x2, int y2);

void print_list(LISTE_POINTS *list);

#endif