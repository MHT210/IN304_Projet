#include "stdio.h"
#include <stdlib.h>
#include "Triangles.h"

TRIANGLE_HEAD create_head_of_TRlist() {
    TRIANGLE_HEAD TR_head;

    TR_head.length = 0;

    return TR_head;
}

TRIANGLE_HEAD add_triangle(TRIANGLE_HEAD TR_head, TRIANGLE T) {
    ELEMENT *e;

    e = malloc(sizeof(ELEMENT));
    if (!e) exit(5);

    e->triangle = T;
    e->next = TR_head.head;
    TR_head.head = e;

    (TR_head.length)++;

    return TR_head;
}

int compareTriangles(TRIANGLE T1, TRIANGLE T2) {
    if (T1.P1.x == T2.P1.x && T1.P1.y == T2.P1.y
    && T1.P2.x == T2.P2.x && T1.P2.y == T2.P2.y
    && T1.P3.x == T2.P3.x && T1.P3.y == T2.P3.y) {
        return 1;
    }
    else {
        return 0;
    }
}

TRIANGLE_HEAD del_triangle(TRIANGLE_HEAD TR_head, TRIANGLE T) {
    ELEMENT * e = TR_head.head;
    ELEMENT * prev_e = NULL;  // ← NULL au lieu de e

    // Chercher le triangle
    while (e && !compareTriangles(e->triangle, T)) {
        prev_e = e;
        e = e->next;
    }

    // Si trouvé
    if (e) {
        if (prev_e == NULL) {
            // C'est le premier élément
            TR_head.head = e->next;
        } else {
            // C'est un élément quelconque
            prev_e->next = e->next;
        }
        free(e);
        (TR_head.length)--;
    }

    return TR_head;
}

int deter(POINT a, POINT b) { /* calcul du determinant pour savoir si on a bien un triangle */
    return a.x * b.y - b.x * a.y;
}

TRIANGLE in_triangle(TRIANGLE_HEAD TR_head, POINT p) {
    ELEMENT *e;
    e = TR_head.head;

    TRIANGLE TNull;
    TNull.P1.x = 0, TNull.P1.y = 0;
    TNull.P2.x = 0, TNull.P2.y = 0;
    TNull.P3.x = 0, TNull.P3.y = 0;

    while (e) { /* Utilisation des coordonnées barycentrique pour determiner si p est dans triangle */
        POINT A = e->triangle.P1;
        POINT B = e->triangle.P2;
        POINT C = e->triangle.P3;

        /* Vecteurs triangle gauche : */
        POINT AB = {B.x - A.x, B.y - A.y};
        POINT AC = {C.x - A.x, C.y - A.y};
        POINT AP = {p.x - A.x, p.y - A.y};

        if (deter(AB, AC) == 0) {return TNull;}

        float beta = (float)deter(AP, AC) / deter(AB, AC);
        float gamma = (float)deter(AB, AP) / deter(AB, AC);

        /* Verifier les conditions barycentrique */
        if (beta >= 0 && gamma >= 0 && beta + gamma <= 1) {
            return e->triangle;
        }
        e = e->next;
    }
    return TNull;
}

int PointInTriangle(TRIANGLE triangle, POINT p) {
    POINT A = triangle.P1;
    POINT B = triangle.P2;
    POINT C = triangle.P3;

    /* Vecteurs triangle gauche : */
    POINT AB = {B.x - A.x, B.y - A.y};
    POINT AC = {C.x - A.x, C.y - A.y};
    POINT AP = {p.x - A.x, p.y - A.y};

    if (deter(AB, AC) == 0) {return 0;}

    float beta = (float)deter(AP, AC) / deter(AB, AC);
    float gamma = (float)deter(AB, AP) / deter(AB, AC);

    /* Verifier les conditions barycentrique */
    if (beta >= 0 && gamma >= 0 && beta + gamma <= 1) {
        return 1;
    } else {
        return 0;
    }
}

// affiche les points des triangles
void print_triangle(TRIANGLE T, int i) {
    printf("\n Triangle %d:", i);
    printf("\nSommet : %d, %d;", T.P1.x, T.P1.y);
    printf("\nDeuxième points : %d, %d;", T.P2.x, T.P2.y);
    printf("\nTroisième points : %d, %d.", T.P3.x, T.P3.y);
    printf("\n");
}

void print_triangles(TRIANGLE_HEAD TR_head) {
    printf("\n###-- %d triangles au total --###", TR_head.length);

    ELEMENT *e;
    e = TR_head.head;

    int i = 1;

    while (e) {
        print_triangle(e->triangle, i);
        i++;
        e = e->next;
    }
}