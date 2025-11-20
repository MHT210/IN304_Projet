#include "uvsqgraphics_2.h"
#include "ChainedList.h"

#define INTER_IMAGE 100

struct pixel {
    int R, G, B;
};

struct image {
    int hauteur;
    int largeur;
    int max_value_rgb;
    struct pixel **P;
};

void write_image(char *nom, struct image I) {
    FILE *F;
    F = fopen(nom, "w");
    if (!F) {perror("Pd de fichier"); exit(20);}

    fprintf(F, "P3\n");
    fprintf(F, "%d %d\n", I.largeur, I.hauteur);
    fprintf(F, "%d\n", I.max_value_rgb);

    for (int i=0; i < I.hauteur; i++) {
        for (int j=0; j < I.largeur; j++) {
            fprintf(F, "%d ", I.P[i][j].R);
            fprintf(F, "%d ", I.P[i][j].G);
            fprintf(F, "%d ", I.P[i][j].B);
        }
        fprintf(F, "\n");
    }
    
    fclose(F);
}

struct image read_image(char *nom, struct image I) {
    FILE *F;
    F = fopen(nom, "r");
    if (!F) {printf("Pb de fichier"); exit(20);}
    
    char c;
    if (fscanf(F, "%c", &c) != 1) {fprintf(stderr,"erreur fichier"); exit(24);}
    if (c != 'P') {fprintf(stderr,"erreur fichier"); exit(21);}
    if (fscanf(F, "%c", &c) != 1) {fprintf(stderr,"erreur fichier"); exit(25);}
    if (c != '3') {fprintf(stderr,"erreur fichier"); exit(22);}
    if (fscanf(F, "%c", &c) != 1) {fprintf(stderr,"erreur fichier"); exit(26);};
    if (c != '\n') {fprintf(stderr,"erreur fichier"); exit(23);}

    if (fscanf(F, "%d", &(I.largeur)) != 1) {fprintf(stderr,"erreur fichier"); exit(27);}
    if (fscanf(F, "%d", &(I.hauteur)) != 1) {fprintf(stderr,"erreur fichier"); exit(28);}
    if (fscanf(F, "%d", &(I.max_value_rgb)) != 1) {fprintf(stderr,"erreur fichier"); exit(29);}
    printf("%d, %d, %d \n", I.largeur, I.hauteur, I.max_value_rgb);

    I.P = malloc(I.hauteur * sizeof(struct pixel *));
    
    int i,j;
    for (i=0; i < I.hauteur; i++) I.P[i] = malloc(I.largeur * sizeof(struct pixel));

    for (i=0; i < I.hauteur; i++) {
        for (j=0; j < I.largeur; j++) {
            if (fscanf(F, "%d", &(I.P[i][j].R)) != 1) {fprintf(stderr,"erreur fichier"); exit(21);}
            if (fscanf(F, "%d", &(I.P[i][j].G)) != 1) {fprintf(stderr,"erreur fichier"); exit(21);}
            if (fscanf(F, "%d", &(I.P[i][j].B)) != 1) {fprintf(stderr,"erreur fichier"); exit(21);}
        }
    }

    fclose(F);

    return I;
}

void show_image(struct image I, int decalh) {
    set_mode_WINDOW();
    for (int y = 0; y < I.hauteur; y++) {
        for (int x = 0; x < I.largeur; x++) {
            COULEUR c = couleur_RGB(
                I.P[y][x].R,
                I.P[y][x].G,
                I.P[y][x].B
            );
            POINT p = {decalh + x, y};
            draw_pixel(p, c);
        }
    }
} 

LISTE_POINTS * Get_Pixel_Couple(LISTE_POINTS * Head, struct image I, struct image I2) {
    
    POINT p1 = wait_clic();
    POINT p2 = wait_clic();

    // Vérifier si p1 est dans l'image de gauche
    if ((p1.x <= I.largeur && p1.x >= 0) && (p1.y <= I.hauteur && p1.y >= 0)) {
        // Vérifier si p2 est bien dans l'image de droite
        if ((p2.x <= 1500 && p2.x >= (1500-I2.largeur)) && (p2.y <= I2.hauteur && p2.y >= 0)) {
            Head = insert_first(Head, p1.x, p1.y, p2.x, p2.y);
        } else {printf("choisir une autre image !\n");}
        // Vérifier si p1 est dans l'image de droite
    } else if ((p1.x <= 1500 && p1.x >= (1500-I2.largeur)) && (p1.y <= I2.hauteur && p1.y >= 0)) {
        // Vérifier si p2 est bien dans l'image de gauche
        if ((p2.x <= I.largeur && p2.x >= 0) && (p2.y <= I.hauteur && p2.y >= 0)) {
            Head = insert_first(Head, p2.x, p2.y, p1.x, p1.y);
        } else {printf("choisir une autre image !\n");}
    } else {
        printf("Le clic n'est pas dans une image !\n");
    }
    
    return Head;
}

// faire fonction qui prend le nombre de couples voulu en utilisant la fonction Get_Pixel_Couple()
LISTE_POINTS * Create_Couples_of_Base_Points(LISTE_POINTS * Head, struct image I, struct image I2) {
    
    Head = Get_Pixel_Couple(Head, I, I2);
    
    return Head;
}


int main() {
    struct image I;
    struct image I2;

    LISTE_POINTS * Head;
    Head = NULL;

    char nom[] = "chat_chien_1.ppm";

    I = read_image(nom, I);
    I2 = read_image("chat_chien_2.ppm", I2);

    write_image("chat_chien_remake.ppm", I);
    write_image("chat_chien_remake_2.ppm", I2);

    init_graphics(1500,900);

    show_image(I, 0);
    show_image(I2, (1500-I.largeur) + INTER_IMAGE);

    affiche_all();

    Head = Create_Couples_of_Base_Points(Head, I, I2);
    print_list(Head);
    wait_escape();
    
    for (int i = 0; i < I.hauteur; i++) free(I.P[i]);
    free(I.P);
    
    for (int i = 0; i < I2.hauteur; i++) free(I2.P[i]);
    free(I2.P);

    exit(0);
}