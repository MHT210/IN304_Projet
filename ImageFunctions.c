#include "stdio.h"
#include <stdlib.h>
#include "ImageFunctions.h"

extern void add_pix(int x, int y, COULEUR coul);
extern void affiche_all_mode_CANVAS();

int clamp(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void Write_Image(char *nom, IMAGE I) {
    FILE *F;
    F = fopen(nom, "w");
    if (!F) {perror("Pd de fichier"); exit(20);}

    fprintf(F, "P3\n");
    fprintf(F, "%d %d\n", I.largeur, I.hauteur);
    fprintf(F, "%d\n", I.max_value_rgb);

    for (int i=0; i < I.hauteur; i++) {
        for (int j=0; j < I.largeur; j++) {
            int r = I.P[i][j].R;
            int g = I.P[i][j].G;
            int b = I.P[i][j].B;
            fprintf(F, "%d ", clamp(r, 0, 255));
            fprintf(F, "%d ", clamp(g, 0, 255));
            fprintf(F, "%d ", clamp(b, 0, 255));
        }
        fprintf(F, "\n");
    }
    
    fclose(F);
}

IMAGE Read_Image(char *nom, IMAGE I) {
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

    I.P = malloc(I.hauteur * sizeof(PIXEL *));
    
    int i,j;
    for (i=0; i < I.hauteur; i++) I.P[i] = malloc(I.largeur * sizeof(PIXEL));

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

void Show_Image(IMAGE I) {
    for (int y = 0; y < I.hauteur; y++) {
        for (int x = 0; x < I.largeur; x++) {
            COULEUR c = couleur_RGB(
                I.P[y][x].R, 
                I.P[y][x].G, 
                I.P[y][x].B
            );
            add_pix(I.decal_value + x, y, c);
        }
    }
    affiche_all_mode_CANVAS();
}

int Is_In_Image(IMAGE I, POINT p, int HEIGHT_OF_SDL) {
    if ((p.x >= I.decal_value && p.x <= I.largeur + I.decal_value) 
    && (p.y >= 0 && p.y <= HEIGHT_OF_SDL)) {
        return 1;
    } else {
        return 0;
    }
}

IMAGE_INTER * InsertLastImage(IMAGE_INTER * Images, IMAGE image) {
    if (!Images) {
		IMAGE_INTER * last_image = malloc(sizeof(IMAGE_INTER));
		if (!last_image) {fprintf(stderr, "pb malloc\n"); exit(12);}
		last_image->image = image;
		last_image->suiv = NULL;
		return last_image;
	}
	
	Images->suiv = InsertLastImage(Images->suiv, image);
	return Images;
}







