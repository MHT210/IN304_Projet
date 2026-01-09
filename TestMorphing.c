#include <stdio.h>
#include "morphing_types.h"
#include "lire_ecrire.h"
#include "afficher.h"

void affiche_tab(Tab_point t){
    /*Affiche les valeurs contenues dans t*/
    printf("Nombres de points: %d\n", t.n);
    printf("Points du tableau de début:\n");
    for(int i=0; i<t.n; i++)
        printf("%d %d\n", t.deb[i].x, t.deb[i].y);
    printf("Points du tableau de fin:\n");
    for(int i=0; i<t.n; i++)
        printf("%d %d\n", t.fin[i].x, t.fin[i].y);
}

void libere_tab(Tab_point t_p, Tab_image t_i){
    /*Libère la mémoire qui a été allouée pour les tableaux de t*/
    free(t_p.deb);
    free(t_p.fin);
    free(t_i.t);
}

Tab_image init_tableau_image(int n){
    /*Initialise le tableau avec n images*/
    Tab_image t;
    t.n = n;
    t.t = malloc(n * sizeof(Image));
    if(!t.t){//alternative à t.t == NULL
        printf("Erreur malloc\n");
        exit(1);
    }
    return t;
}

void calcule_pixel_blancs(Pixel** mat, Tab_point t, int k, int n){
    /*met à blanc les pixels de l'image intermédiaire i
    dans la matrice de pixels mat
    à partir des tableaux de points t
    n est le nombre total d'images*/

    //pix est défini comme un pixel blanc:
    Pixel pix = {255, 255, 255};
    POINT p;
    for(int i=0; i<t.n; i++){
        //on calcule les coordonées du point à mettre à blanc
        p.x = (k*t.fin[i].x + (n-1-k)*t.deb[i].x) / (n - 1);
        p.y = (k*t.fin[i].y + (n-1-k)*t.deb[i].y) / (n - 1);
        //printf("%d %d %d %d\n", k, i, p.x, p.y);
        //on met à blanc le pixel correspondant de la matrice
        mat[p.x][p.y] = pix;
    }
}


void remplir_tab_image( Tab_image t_i, 
                        Tab_point t_p, 
                        int largeur, 
                        int hauteur, 
                        int range
                      )
{   /*calcul des images intermédiaires de largeur, hauteur et range données,
    à partir du tableau de points t_p,
    et stockées dans le tableau d'images t_i */

    for(int k=0; k<t_i.n; k++){
        t_i.t[k].largeur = largeur;
        t_i.t[k].hauteur = hauteur;
        t_i.t[k].range = range;
        //création de la matrice de pixels
        //tableau de pointeurs sur les lignes de la matrice
        t_i.t[k].mat = malloc(t_i.t[k].hauteur * sizeof(Pixel*));
        if (t_i.t[k].mat == NULL){
            printf("Erreur: malloc\n");
            exit(1);
        }
        //on crée les tableaux correspondant aux lignes de la matrice
        for(int i=0; i<t_i.t[k].hauteur; i++){
            t_i.t[k].mat[i] = malloc(t_i.t[k].largeur * sizeof(Pixel));
            if (t_i.t[k].mat[i] == NULL){
                printf("Erreur: malloc\n");
                exit(1);
            }
        }
        //a partir de là t_i.t[i].mat[i][j] va contenir un pixel noir
        Pixel p;
        for(int i=0; i<t_i.t[k].hauteur; i++){
            for(int j=0; j<t_i.t[k].largeur; j++){
                p.b = p.g = p.r = 0;
                t_i.t[k].mat[i][j] = p;
            }
        }
        //on calcule les pixels blancs
        calcule_pixel_blancs(t_i.t[k].mat, t_p, k, t_i.n);
    }
}



int main(int argc, char** argv){
    if(argc != 3){ 
        printf("Erreur argument de la ligne de commande\n");
        printf("Format attendu:\n");
        printf("./morphing fic_points nb_images\n");
        exit(1);
    }
    //on créé le fichier d'images
    system("rm -rf Images");
    system("rm -rf toto.mp4");
    system("mkdir Images");
    Tab_point t_p;
    Tab_image t_i;
    //on initialise le tableau d'images
    t_i = init_tableau_image(atoi(argv[2]));
    //on lit le fichier de points
    t_p = lire_fichier_points(argv[1]);
    //affiche_tab(t_p);
    //on calcule les images intermédiaires dans t_i
    remplir_tab_image(t_i, t_p, 512, 512, 256);

    init_graphics(1536, 512);
    affiche_auto_off();
    POINT p1, p2;
    char nom[128];
    //on affiche les images dans la fenêtre graphique,
    //et on les écrit dans un fichier
    for(int i=0; i<t_i.n; i++){
        //on affiche à gauche l'image de départ
        afficher_image(t_i.t[0], 0);
        //on affiche à droite l'image d'arrivée
        afficher_image(t_i.t[t_i.n - 1], 1024);
        afficher_image(t_i.t[i], 512);
        p2.x = p1.x = 512;
        p1.y = 0; p2.y = 512;
        draw_line(p1, p2, blanc);
        p2.x = p1.x = 1024;
        draw_line(p1, p2, blanc);
        affiche_all();
        sprintf(nom, "Images/img%04d.ppm", i);
        ecrire_fichier(t_i.t[i], nom);
    }
    wait_clic();

    libere_tab(t_p, t_i);
    sprintf(nom,"ffmpeg -framerate 20 -i Images/img%%04d.ppm -c:v libx264 -pix_fmt yuv420p toto.mp4");
    system(nom);

    return 0;
}
