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