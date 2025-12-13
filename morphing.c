#include "CouplesFunctions.h"
#include "Triangles.h"
#include "Button.h"

#define JAUNE couleur_RGB(255, 255, 0)
#define ROUGE couleur_RGB(255, 0, 0)

void Show_Images_On_Screen(IMAGE I, IMAGE I2) {
    Show_Image(I);
    Show_Image(I2);
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

    //Fin du main
    wait_escape();

    for (int i = 0; i < I.hauteur; i++) free(I.P[i]);
    free(I.P);
    
    for (int i = 0; i < I2.hauteur; i++) free(I2.P[i]);
    free(I2.P);

    exit(0);
}