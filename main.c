#include "CouplesFunctions.h"
#include "Triangles.h"
#include "Button.h"
#include "Triangulate.h"
#include "ImageFunctions.h"
#include "morphing.h"
#include "uvsqgraphics_2.h"

void Show_Images_On_Screen(IMAGE I, IMAGE I2) {
    Show_Image(I);
    Show_Image(I2);
}

int main() {
    // Declaration des struct IMAGE
    IMAGE I;
    IMAGE I2;
    IMAGE I3;

    // Declaration du struct LISTE_POINTS
    LISTE_POINTS * Head;
    Head = NULL;

    // Declaration du struct TRIANGLE_HEAD
    TRIANGLE_HEAD TH;
    TH = create_head_of_TRlist();
    TRIANGLE_HEAD TH_D;
    TH_D = create_head_of_TRlist();
    TRIANGLE_HEAD TH_A;
    TH_A = create_head_of_TRlist();

    // Declaration du struct POINTS_INT
    POINTS_INT * Head_I;
    Head_I = NULL;
    POINTS_INT * Head_D;
    Head_D = NULL;
    POINTS_INT * Head_A;
    Head_A = NULL;

    // Declaration du struct BOUTON
    BOUTON trianguler;
    trianguler.nom = "sauver et trianguler";
    trianguler.largeur = 200;
    trianguler.hauteur = 40;
    trianguler.x = 800;
    trianguler.y = 450;
    trianguler.couleur = VERT;
    BOUTON suppr;
    suppr.nom = "Supprimer dernier couple";
    suppr.largeur = 250;
    suppr.hauteur = 40;
    suppr.x = 800;
    suppr.y = 350;
    suppr.couleur = ROUGE;
    BOUTON quitter;
    quitter.nom = "Quitter";
    quitter.largeur = 150;
    quitter.hauteur = 40;
    quitter.x = 800;
    quitter.y = 250;
    quitter.couleur = JAUNE;

    // Appel des fonctions d'images
    I = Read_Image("chat_chien_1.ppm", I);
    I2 = Read_Image("chat_chien_2.ppm", I2);
    I.decal_value = 0;
    I2.decal_value = INTER_IMAGE;

    // initialisation de la fenetre avec ses composantes(images, boutons...)
    init_graphics(LARGEUR, HAUTEUR);
    set_mode_CANVAS();
    creer_bouton(trianguler);
    creer_bouton(suppr);
    creer_bouton(quitter);
    Show_Images_On_Screen(I, I2);

    // Appel des fonctions pour les listes
    Head = Create_Couples_of_Points(Head, I, I2, trianguler, suppr, quitter);
    Save_Point_Couples(Head);
    Read_Point_Couples("Couples_de_points.ppm");

    // Appel des fonctions pour les points de bases intermediaires
    Head_I = getIntPoints(Head_I, Head, 0.5);
    Head_D = getIntPoints(Head_D, Head, 0);
    Head_A = getIntPoints(Head_A, Head, 1);

    // Appel des fonctions triangulation
    TH = Get_Triangles(TH, Head_I);
    TH_D = Get_Triangles(TH_D, Head_D);
    TH_A = Get_Triangles(TH_A, Head_A);
    print_triangles(TH);

    morphing(TH, TH_D, TH_A, 0.5, I, I2, I3);

    Write_Image("frame_00.ppm", I3);

    // A la fin attendre un clic sur le bouton quitter
    int keepgoing = 1;
    while (keepgoing == 1) {
        POINT p = wait_clic();
        if (bouton_cliquer(p, quitter)) {
            keepgoing = 0;
            SDL_Quit();
        }
    }
    //Fin du main
    wait_escape();

    for (int i = 0; i < I.hauteur; i++) free(I.P[i]);
    free(I.P);
    
    for (int i = 0; i < I2.hauteur; i++) free(I2.P[i]);
    free(I2.P);

    exit(0);
}