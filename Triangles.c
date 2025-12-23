#include "stdio.h"
#include <stdlib.h>
#include "Triangles.h"

TRIANGLE_HEAD create_head_of_TRlist() {
    TRIANGLE_HEAD TR_head;

    TR_head.length = 0;

    return TR_head;
}

TRIANGLE_HEAD add_triangle(TRIANGLE_HEAD TR_head, TRIANGLE pointsGauche, TRIANGLE pointsDroit) {
    ELEMENT *e;

    e = malloc(sizeof(ELEMENT));
    if (!e) exit(5);

    e->couple_triangle.triangleGauche = pointsGauche;
    e->couple_triangle.triangleDroit = pointsDroit;
    e->next = TR_head.head;
    TR_head.head = e;

    (TR_head.length)++;

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
        POINT A = e->couple_triangle.triangleGauche.P1;
        POINT B = e->couple_triangle.triangleGauche.P2;
        POINT C = e->couple_triangle.triangleGauche.P3;

        POINT Ad = e->couple_triangle.triangleDroit.P1;
        POINT Bd = e->couple_triangle.triangleDroit.P2;
        POINT Cd = e->couple_triangle.triangleDroit.P3;

        /* Vecteurs triangle gauche : */
        POINT AB = {B.x - A.x, B.y - A.y};
        POINT AC = {C.x - A.x, C.y - A.y};
        POINT AP = {p.x - A.x, p.y - A.y};

        if (deter(AB, AC) == 0) {return TNull;}

        float beta = (float)deter(AP, AC) / deter(AB, AC);
        float gamma = (float)deter(AB, AP) / deter(AB, AC);

        /* Vecteurs triangle droit : */
        POINT ABd = {Bd.x - Ad.x, Bd.y - Ad.y};
        POINT ACd = {Cd.x - Ad.x, Cd.y - Ad.y};
        POINT APd = {p.x - Ad.x, p.y - Ad.y};

        if (deter(ABd, ACd) == 0) {return TNull;}

        float betad = (float)deter(APd, ACd) / deter(ABd, ACd);
        float gammad = (float)deter(ABd, APd) / deter(ABd, ACd);

        /* Verifier les conditions barycentrique */
        if (beta >= 0 && gamma >= 0 && beta + gamma <= 1) {
            return e->couple_triangle.triangleGauche;
        }
        else if (betad >= 0 && gammad >= 0 && betad + gammad <= 1) {
            return e->couple_triangle.triangleDroit;
        }
        e = e->next;
    }
    return TNull;
}

// affiche les points des triangles
void print_triangle(COUPLE_TRIANGLE couple_triangle, int i) {
    printf("\n-Couple de triangles numéro %d :-", i);

    printf("\n Triangle Gauche :");
    printf("\nSommet : %d, %d;", couple_triangle.triangleGauche.P1.x, couple_triangle.triangleGauche.P1.y);
    printf("\nDeuxième points : %d, %d;", couple_triangle.triangleGauche.P2.x, couple_triangle.triangleGauche.P2.y);
    printf("\nTroisième points : %d, %d.", couple_triangle.triangleGauche.P3.x, couple_triangle.triangleGauche.P3.y);
    printf("\n");
    printf("\n Triangle Droit :");
    printf("\nSommet : %d, %d;", couple_triangle.triangleDroit.P1.x, couple_triangle.triangleDroit.P1.y);
    printf("\nDeuxième points : %d, %d;", couple_triangle.triangleDroit.P2.x, couple_triangle.triangleDroit.P2.y);
    printf("\nTroisième points : %d, %d.\n", couple_triangle.triangleDroit.P3.x, couple_triangle.triangleDroit.P3.y);
}

void print_triangles(TRIANGLE_HEAD TR_head) {
    printf("\n###-- %d couples de triangles au total --###", TR_head.length);

    ELEMENT *e;
    e = TR_head.head;

    int i = 1;

    while (e) {
        print_triangle(e->couple_triangle, i);
        i++;
        e = e->next;
    }
}