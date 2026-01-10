#include "stdio.h"
#include <stdlib.h>
#include "Triangulate.h"

POINTS_INT * Find_Node(POINTS_INT * head, int i) {
    POINTS_INT * temp = head;
    int j = 0;
    while (temp) {
        if (j == i) {
            return temp;
        }
        temp = temp->suiv;
        j += 1;
    }

    return head;
}

POINT first_user_point(POINTS_INT * Head) {
    POINTS_INT * temp;
    POINT P1;

    temp = Head;
    int i = 1;

    // Prendre le premier point créer par l'utilisateur
    while (temp) {
        if (i == 5) {
            P1.x = temp->P.x;
            P1.y = temp->P.y;
        }
        
        temp = temp->suiv;
        i = i + 1;
    }

    return P1;
}

TRIANGLE_HEAD Get_First_Triangles(TRIANGLE_HEAD TH, POINTS_INT * Head, int draw) {
    POINT p1 = first_user_point(Head);

    // Créer les triangles
    for (int i = 0; i <= 3; i+=3) {
        TRIANGLE T;
        POINTS_INT *Node;
        POINT p2;

        Node = Find_Node(Head, i);
        p2 = Node->P;
        for (int j = 1; j <= 2; j++) {
            POINTS_INT * Node2;
            POINT p3;

            Node2 = Find_Node(Head, j);
            p3 = Node2->P;

            T.P1 = p1;
            T.P2 = p2;
            T.P3 = p3;
            if (draw) {draw_triangle(p1, p2, p3, VERT);}

            TH = add_triangle(TH, T);
        }
    }

    return TH;
}

TRIANGLE_HEAD create_3_triangles(TRIANGLE_HEAD TH, TRIANGLE T, POINTS_INT * temp, int draw) {
    TRIANGLE T1 = {temp->P, T.P1, T.P2};
    if (draw) {draw_triangle(T1.P1, T1.P2, T1.P3, VERT);}
    TH = add_triangle(TH, T1);

    TRIANGLE T2 = {temp->P, T.P2, T.P3};
    if (draw) {draw_triangle(T2.P1, T2.P2, T2.P3, VERT);}
    TH = add_triangle(TH, T2);

    TRIANGLE T3 = {temp->P, T.P1, T.P3};
    if (draw) {draw_triangle(T3.P1, T3.P2, T3.P3, VERT);}
    TH = add_triangle(TH, T3);

    return TH;
}

TRIANGLE_HEAD Get_Triangles(TRIANGLE_HEAD TH, POINTS_INT * Head, int draw) {
    TH = Get_First_Triangles(TH, Head, draw);
    POINTS_INT *temp = Head;
    
    int i = 1;
    while (temp) {
        if (i > 5) {
            TRIANGLE T;
            T = in_triangle(TH, temp->P);

            TH = create_3_triangles(TH, T, temp, draw);
            TH = del_triangle(TH, T);
        }
        temp = temp->suiv;
        i++;
    }

    return TH;
}

POINTS_INT * CreateHeadList(LISTE_POINTS * HeadCouple, POINTS_INT * Head, int cote) {
    LISTE_POINTS *temp = HeadCouple;

    while (temp) {
        if (cote == 1) {
            Head = insert_last_i(Head, temp->CP.G.x, temp->CP.G.y);
            temp = temp->suiv;
        }
        if (cote == 2) {
            Head = insert_last_i(Head, temp->CP.D.x, temp->CP.D.y);
            temp = temp->suiv;
        }
    }

    return Head;
}
