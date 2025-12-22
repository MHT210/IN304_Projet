#ifndef TRIANGLES_H
#define TRIANGLES_H

#include "uvsqgraphics_2.h"

//Les structures :
typedef struct triangle{
    POINT P1, P2, P3;
} TRIANGLE;

typedef struct couple_triangle{
    struct triangle triangleGauche;
    struct triangle triangleDroit;
} COUPLE_TRIANGLE;

typedef struct element_node {
    struct couple_triangle couple_triangle;
    struct element_node *next;
} ELEMENT;

typedef struct the_triangles {
    int length;
    struct element_node *head;
} TRIANGLE_HEAD;


//Les fonctions :
TRIANGLE_HEAD create_head_of_TRlist(); // creation de la structure qui va contenir l'entête de la liste chainée avec le nombre total de noeud(=element_node)

TRIANGLE_HEAD add_triangle(TRIANGLE_HEAD TR_head, TRIANGLE pointsGauche, TRIANGLE pointsDroit); // ajoute un triangle a la liste

void print_triangles(TRIANGLE_HEAD TR_head); // affiche la totalité des triangles avec la fonction precedente

#endif