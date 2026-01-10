#ifndef TRIANGLES_H
#define TRIANGLES_H

#include "uvsqgraphics_2.h"

//Les structures :
typedef struct triangle{
    POINT P1, P2, P3;
} TRIANGLE;

typedef struct element_node {
    struct triangle triangle;
    struct element_node *next;
} ELEMENT;

typedef struct the_triangles {
    int length;
    struct element_node *head;
} TRIANGLE_HEAD;


//Les fonctions :
TRIANGLE_HEAD create_head_of_TRlist(); // creation de la structure qui va contenir l'entête de la liste chainée avec le nombre total de noeud(=element_node)

TRIANGLE_HEAD add_triangle(TRIANGLE_HEAD TR_head, TRIANGLE T); // ajoute un triangle a la liste

TRIANGLE_HEAD del_triangle(TRIANGLE_HEAD TR_head, TRIANGLE T); // supprime le triangle T de la liste

TRIANGLE in_triangle(TRIANGLE_HEAD TR_head, POINT p); // retourne le triangle dans laquelle est le point p

void print_triangles(TRIANGLE_HEAD TR_head); // affiche la totalité des triangles avec la fonction precedente

int deter(POINT a, POINT b);

int PointInTriangle(TRIANGLE triangle, POINT p);

#endif