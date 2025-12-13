#include "stdio.h"
#include <stdlib.h>
#include "Triangles.h"

TRIANGLE_HEAD create_head_of_TRlist() {
    TRIANGLE_HEAD TR_head;

    TR_head.length = 0;

    return TR_head;
}

TRIANGLE_HEAD add_triangle(TRIANGLE_HEAD TR_head, TRIANGLE TR_points) {
    ELEMENT *e;

    e = malloc(sizeof(ELEMENT));
    if (!e) exit(5);

    e->points = TR_points;
    e->next = TR_head.head;
    TR_head.head = e;

    (TR_head.length)++;

    return TR_head;
}

// affiche les points des triangles
void print_triangle(TRIANGLE points, int i) {
    printf("\nTrianlge numéro %d :", i);
    printf("\nSommet : %d, %d;", points.P1.x, points.P1.y);
    printf("\nDeuxième points : %d, %d;", points.P2.x, points.P2.y);
    printf("\nTroisième points : %d, %d.", points.P3.x, points.P3.y);
}

void print_triangles(TRIANGLE_HEAD TR_head) {
    printf("\n###-- %d triangles au total --###", TR_head.length);

    ELEMENT *e;
    e = TR_head.head;

    int i = 1;

    while (e) {
        print_triangle(e->points, i);
        i++;
        e = e->next;
    }
}