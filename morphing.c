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

struct couples_points {
    POINT G;
    POINT D;
};

struct List_Points {
    struct couples_points CP;
    struct List_Points *suiv;
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

struct List_Points * get_coordinates_of_image(struct List_Points * Head, struct image I) {
    return Head;
}

int main() {
    struct image I;
    struct image I2;
    struct List_Points * Head;

    char nom[] = "chat_chien_1.ppm";

    I = read_image(nom, I);
    I2 = read_image("chat_chien_2.ppm", I2);
    write_image("chat_chien_remake.ppm", I);
    write_image("chat_chien_remake_2.ppm", I2);

    init_graphics(1500,900);

    show_image(I, 0);
    show_image(I2, (1500-I.largeur) + INTER_IMAGE);

    Head = get_coordinates_of_image()

    affiche_all();

    wait_escape();
    exit(0);
}