#include "CouplesFunctions.h"
#include "Triangles.h"
#include "Button.h"

/* Faire un bouton pour visualiser les triangles*/

#define JAUNE couleur_RGB(255, 255, 0)
#define ROUGE couleur_RGB(255, 0, 0)

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

TRIANGLE_HEAD Get_Triangles(TRIANGLE_HEAD TH, char *couples_file, LISTE_POINTS * Head) {
    FILE *F;
    COUPLES_POINTS couple;
    couple = first_user_couple(Head);

    // Ouvrir le fichier pour recupérer les 4 premiers couples de base
    F = fopen(couples_file, "r");
    if (!F) {perror("Erreur ouverture fichier"); exit(1);}

    int n;
    if (fscanf(F, "%d", &n) != 1) {perror("Nombre max de couples pas trouver"); exit(1);}

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

            couple_triangle.triangleDroit.P1 = couple.D;
            couple_triangle.triangleDroit.P2 = d1;
            couple_triangle.triangleDroit.P3 = d2;
            TH = add_triangle(TH, couple_triangle.triangleGauche, couple_triangle.triangleDroit);
        }
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
    BOUTON stop;
    stop.nom = "Sauver et quitter";
    stop.largeur = 150;
    stop.hauteur = 40;
    stop.x = 800;
    stop.y = 450;
    stop.couleur = JAUNE;
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
    creer_bouton(stop);
    creer_bouton(suppr);
    Show_Images_On_Screen(I, I2);

    // Appel des fonctions pour les listes
    Head = Create_Couples_of_Points(Head, I, I2, stop, suppr);
    Save_Point_Couples(Head);
    Read_Point_Couples("Couples_de_points.ppm");

    // Appel des fonctions triangulation
    TH = Get_Triangles(TH, "Couples_de_points.ppm", Head);
    print_triangles(TH);

    //Fin du main
    wait_escape();

    for (int i = 0; i < I.hauteur; i++) free(I.P[i]);
    free(I.P);
    
    for (int i = 0; i < I2.hauteur; i++) free(I2.P[i]);
    free(I2.P);

    exit(0);
}