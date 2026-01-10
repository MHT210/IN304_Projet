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
    IMAGE_INTER * Images;
    Images = NULL;

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
    Head_D = CreateHeadList(Head, Head_D, 1);
    Head_A = CreateHeadList(Head, Head_A, 2);

    // Appel des fonctions triangulation
    TH_D = Get_Triangles(TH_D, Head_D, 1);
    TH_A = Get_Triangles(TH_A, Head_A, 1);
    print_triangles(TH);

    for (float alpha = 0.0f; alpha<=1.0f; alpha = alpha + 0.01f) {
        free_list(Head_I);
        Head_I = NULL;
        Head_I = getIntPoints(Head_I, Head, alpha);
        free_list_TH(TH.head);
        TH = create_head_of_TRlist();
        TH = Get_Triangles(TH, Head_I, 0);
        Images = morphing(Head_I, Images, TH, TH_D, TH_A, alpha, I, I2, I3);
    }
    int i = 0;
    while (Images) {
        char filename[256];
        sprintf(filename, "FRAMES/frame_%02d.ppm", i);
        Write_Image(filename, Images->image);
        Images = Images->suiv;
        i++;
    }

    printf("Génération de la vidéo...\n");
    int result = system("ffmpeg -y -framerate 10 -i FRAMES/frame_%02d.ppm "
                    "-vf \"scale='trunc(iw/2)*2:trunc(ih/2)*2'\" "
                    "-c:v libx264 -pix_fmt yuv420p morphing.mp4");

    if (result == 0) {
        printf("Vidéo créée avec succès : morphing.mp4\n");
    } else {
        fprintf(stderr, "Erreur lors de la création de la vidéo\n");
    }
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

    free_list(Head_I);
    free_list_TH(TH.head);

    for (int i = 0; i < I.hauteur; i++) free(I.P[i]);
    free(I.P);
    
    for (int i = 0; i < I2.hauteur; i++) free(I2.P[i]);
    free(I2.P);

    exit(0);
}