#include "CouplesFunctions.h"
#include "Triangles.h"
#include "Button.h"
#include "uvsqgraphics_2.h"

#define JAUNE couleur_RGB(255, 255, 0)
#define ROUGE couleur_RGB(255, 0, 0)
#define VERT couleur_RGB(0, 255, 100)

void Show_Images_On_Screen(IMAGE I, IMAGE I2) {
    Show_Image(I);
    Show_Image(I2);
}

LISTE_POINTS * Find_Node(LISTE_POINTS * head, int i) {
    LISTE_POINTS * temp = head;
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

COUPLES_POINTS first_user_couple(LISTE_POINTS * Head) {
    LISTE_POINTS * temp;
    POINT P1;
    POINT P2;
    COUPLES_POINTS couple;
    
    temp = Head;
    int i = 1;

    // Prendre le premier point créer par l'utilisateur
    while (temp) {
        if (i == 5) {
            P1.x = temp->CP.G.x;
            P1.y = temp->CP.G.y;

            P2.x = temp->CP.D.x;
            P2.y = temp->CP.D.y;

            couple.G = P1;
            couple.D = P2;
        }
        
        temp = temp->suiv;
        i = i + 1;
    }

    return couple;
}

TRIANGLE_HEAD Get_First_Triangles(TRIANGLE_HEAD TH, LISTE_POINTS * Head) {
    COUPLES_POINTS couple;
    couple = first_user_couple(Head);

    // Créer les triangles
    for (int i = 0; i <= 3; i+=3) {
        COUPLE_TRIANGLE couple_triangle;
        LISTE_POINTS *Node;
        POINT g1;
        POINT d1;

        Node = Find_Node(Head, i);
        g1 = Node->CP.G;
        d1 = Node->CP.D;
        for (int j = 1; j <= 2; j++) {
            LISTE_POINTS *Node2;
            POINT g2;
            POINT d2;

            Node2 = Find_Node(Head, j);
            g2 = Node2->CP.G;
            d2 = Node2->CP.D;

            couple_triangle.triangleGauche.P1 = couple.G;
            couple_triangle.triangleGauche.P2 = g1;
            couple_triangle.triangleGauche.P3 = g2;
            draw_triangle(couple.G, g1, g2, VERT);

            couple_triangle.triangleDroit.P1 = couple.D;
            couple_triangle.triangleDroit.P2 = d1;
            couple_triangle.triangleDroit.P3 = d2;
            draw_triangle(couple.D, d1, d2, VERT);

            TH = add_triangle(TH, couple_triangle.triangleGauche, couple_triangle.triangleDroit);
        }
    }

    return TH;
}

TRIANGLE_HEAD Get_Triangles(TRIANGLE_HEAD TH, LISTE_POINTS * Head) {
    TH = Get_First_Triangles(TH, Head);
    LISTE_POINTS *temp = Head;
    
    int i = 1;
    while (temp) {
        if (i > 5) {
            TRIANGLE Tg;
            TRIANGLE Td;
            Tg = in_triangle(TH, temp->CP.G);
            Td = in_triangle(TH, temp->CP.D);

            COUPLE_TRIANGLE CT;
            TRIANGLE Tg1 = {temp->CP.G, Tg.P1, Tg.P2};
            CT.triangleGauche = Tg1;
            draw_triangle(Tg1.P1, Tg1.P2, Tg1.P3, VERT);

            TRIANGLE Td1 = {temp->CP.D, Td.P1, Td.P2};
            CT.triangleDroit = Td1;
            draw_triangle(Td1.P1, Td1.P2, Td1.P3, VERT);

            TH = add_triangle(TH, CT.triangleGauche, CT.triangleDroit);

            COUPLE_TRIANGLE CT2;
            TRIANGLE Tg2 = {temp->CP.G, Tg.P2, Tg.P3};
            CT2.triangleGauche = Tg2;
            draw_triangle(Tg2.P1, Tg2.P2, Tg2.P3, VERT);

            TRIANGLE Td2 = {temp->CP.D, Td.P2, Td.P3};
            CT2.triangleDroit = Td2;
            draw_triangle(Td2.P1, Td2.P2, Td2.P3, VERT);

            TH = add_triangle(TH, CT2.triangleGauche, CT2.triangleDroit);

            COUPLE_TRIANGLE CT3;
            TRIANGLE Tg3 = {temp->CP.G, Tg.P1, Tg.P3};
            CT3.triangleGauche = Tg3;
            draw_triangle(Tg3.P1, Tg3.P2, Tg3.P3, VERT);

            TRIANGLE Td3 = {temp->CP.D, Td.P1, Td.P3};
            CT3.triangleDroit = Td3;
            draw_triangle(Td3.P1, Td3.P2, Td3.P3, VERT);

            TH = add_triangle(TH, CT3.triangleGauche, CT3.triangleDroit);
        }
        temp = temp->suiv;
        i++;
    }

    return TH;
}

int main() {
    // Declaration des struct IMAGE
    IMAGE I;
    IMAGE I2;

    // Declaration du struct LISTE_POINTS
    LISTE_POINTS * Head;
    Head = NULL;

    // Declaration du struct TRIANGLE_HEAD
    TRIANGLE_HEAD TH;
    TH = create_head_of_TRlist();

    // Declaration du struct BOUTON
    BOUTON sauver;
    sauver.nom = "Sauver";
    sauver.largeur = 150;
    sauver.hauteur = 40;
    sauver.x = 800;
    sauver.y = 450;
    sauver.couleur = JAUNE;
    BOUTON suppr;
    suppr.nom = "Supprimer dernier couple";
    suppr.largeur = 200;
    suppr.hauteur = 40;
    suppr.x = 800;
    suppr.y = 350;
    suppr.couleur = ROUGE;

    // Appel des fonctions d'images
    I = Read_Image("chat_chien_1.ppm", I);
    I2 = Read_Image("chat_chien_2.ppm", I2);
    I.decal_value = 0;
    I2.decal_value = INTER_IMAGE;

    // initialisation de la fenetre avec ses composantes(images, boutons...)
    init_graphics(LARGEUR, HAUTEUR);
    set_mode_CANVAS();
    creer_bouton(sauver);
    creer_bouton(suppr);
    Show_Images_On_Screen(I, I2);

    // Appel des fonctions pour les listes
    Head = Create_Couples_of_Points(Head, I, I2, sauver, suppr);
    Save_Point_Couples(Head);
    Read_Point_Couples("Couples_de_points.ppm");

    // Appel des fonctions triangulation
    TH = Get_Triangles(TH, Head);
    print_triangles(TH);

    //Fin du main
    wait_escape();

    for (int i = 0; i < I.hauteur; i++) free(I.P[i]);
    free(I.P);
    
    for (int i = 0; i < I2.hauteur; i++) free(I2.P[i]);
    free(I2.P);

    exit(0);
}