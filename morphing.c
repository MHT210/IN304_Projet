#include "uvsqgraphics_2.h"
#include "ChainedList.h"

extern void add_pix(int x, int y, COULEUR coul);
extern void affiche_all_mode_CANVAS();

#define INTER_IMAGE 950
#define LARGEUR 1500
#define HAUTEUR 900
#define CIRCLE_RAYON 20

/*
    Faire un bouton stop avec fonction qui renvoie 1 si appuyer
    Faire un fichier qui stocke tout les couples de points
        -Première ligne nbre de couples
        -4 premières lignes les couples de bases
        -Chaque ligne 4 entiers(les couples)
*/

struct pixel {
    int R, G, B;
};

struct image {
    int hauteur;
    int largeur;
    int max_value_rgb;
    int decal_value;
    struct pixel **P;
};

void Write_Image(char *nom, struct image I) {
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

struct image Read_Image(char *nom, struct image I) {
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

void Show_Image(struct image I) {
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

int Is_In_Image(struct image I, POINT p) {
    if ((p.x >= I.decal_value && p.x <= I.largeur + I.decal_value) 
    && (p.y >= 0 && p.y <= HAUTEUR)) {
        return 1;
    } else {
        return 0;
    }
}

LISTE_POINTS * Get_Pixel_Couple(LISTE_POINTS * Head, struct image I, struct image I2) {
    int keepGoing = 1;
    COULEUR redColor = couleur_RGB(255,0,0);
    
    while (keepGoing) {
        POINT p1 = wait_clic();
        POINT p2 = wait_clic();

        // Vérifier si p1 est dans l'image de gauche
        if (Is_In_Image(I, p1)) {
            draw_circle(p1, CIRCLE_RAYON, redColor);
            // Vérifier si p2 est bien dans l'image de droite
            if (Is_In_Image(I2, p2)) {
                draw_circle(p2, CIRCLE_RAYON, redColor);
                Head = insert_last(Head, p1.x, p1.y, p2.x, p2.y);
            } else {
                printf("choisir une autre image !\n");
                keepGoing = 0;
                return Head;
            }
        
        // Vérifier si p1 est dans l'image de droite
        } else if (Is_In_Image(I2, p1)) {
            draw_circle(p2, CIRCLE_RAYON, redColor);
            // Vérifier si p2 est bien dans l'image de gauche
            if (Is_In_Image(I, p2)) {
                draw_circle(p1, CIRCLE_RAYON, redColor);
                Head = insert_last(Head, p2.x, p2.y, p1.x, p1.y);
            } else {
                printf("choisir une autre image !\n");
                keepGoing = 0;
                return Head;
            }
        }
    }
    return Head;
}

LISTE_POINTS * Init_With_Couples_of_Base_Points(LISTE_POINTS * Head, struct image I, struct image I2) {
    COULEUR redColor = couleur_RGB(255,0,0);

    POINT g1;
    POINT d1;
    g1.x = I.decal_value;
    g1.y = 0;
    d1.x = I2.decal_value;
    d1.y = 0;
    Head = insert_last(Head, g1.x, g1.y, d1.x, d1.y);
    draw_circle(g1, CIRCLE_RAYON, redColor);
    draw_circle(d1, CIRCLE_RAYON, redColor);

    POINT g2;
    POINT d2;
    g2.x = I.largeur;
    g2.y = 0;
    d2.x = I2.largeur + I2.decal_value;
    d2.y = 0;
    Head = insert_last(Head, g2.x, g2.y, d2.x, d2.y);
    draw_circle(g2, CIRCLE_RAYON, redColor);
    draw_circle(d2, CIRCLE_RAYON, redColor);

    POINT g3;
    POINT d3;
    g3.x = I.decal_value;
    g3.y = I.hauteur;
    d3.x = I2.decal_value;
    d3.y = I2.hauteur;
    Head = insert_last(Head, g3.x, g3.y, d3.x, d3.y);
    draw_circle(g3, CIRCLE_RAYON, redColor);
    draw_circle(d3, CIRCLE_RAYON, redColor);

    POINT g4;
    POINT d4;
    g4.x = I.largeur;
    g4.y = I.hauteur;
    d4.x = I2.largeur + I2.decal_value;
    d4.y = I2.hauteur;
    Head = insert_last(Head, g4.x, g4.y, d4.x, d4.y);
    draw_circle(g4, CIRCLE_RAYON, redColor);
    draw_circle(d4, CIRCLE_RAYON, redColor);
    
    return Head;
}

LISTE_POINTS * Create_Couples_of_Points(LISTE_POINTS * Head, struct image I, struct image I2) {
    // initialiser avec les couples de base
    Head = Init_With_Couples_of_Base_Points(Head, I, I2);

    Head = Get_Pixel_Couple(Head, I, I2);
    
    Save_Point_Couples(Head);
    return Head;
}

void Save_Point_Couples(LISTE_POINTS * Head) {
    // parcourir la liste chainée et ecrire les coordonnées x et y des couples dans le fichier
    FILE *F;

    F = fopen("Couples_de_points.ppm", "w");
    if (!F) {perror("Pd de fichier"); exit(44);}

    fprintf(F, "%d\n",length_of_list(Head));
    
    while (Head) {
        fprintf(F, "%d %d  %d %d", Head->CP.G.x, Head->CP.G.y, 
            Head->CP.D.x, Head->CP.D.y);
        Head = Head->suiv;
    }
}

void Read_Point_Couples(char *filename) {
    FILE *F;
    
    F = fopen(filename, "r");
    if (!F) {
        perror("Erreur ouverture fichier");
        return;
    }
    
    // Lire le nombre de couples
    int nb_couples;
    if (fscanf(F, "%d", &nb_couples) != 1) {
        fprintf(stderr, "Erreur lecture nombre de couples\n");
        fclose(F);
        return;
    }
    
    printf("\n=== LECTURE DU FICHIER '%s' ===\n", filename);
    printf("Nombre de couples: %d\n\n", nb_couples);
    
    // Lire chaque couple
    for (int i = 0; i < nb_couples; i++) {
        int x1, y1, x2, y2;
        
        if (fscanf(F, "%d %d %d %d", &x1, &y1, &x2, &y2) != 4) {
            fprintf(stderr, "Erreur lecture couple %d\n", i);
            break;
        }
        
        printf("Couple %d: Gauche(%d, %d) -> Droite(%d, %d)\n", 
               i+1, x1, y1, x2, y2);
    }
    
    printf("===============================\n\n");
    
    fclose(F);
}

int main() {
    struct image I;
    struct image I2;

    I.decal_value = 0;
    I2.decal_value = INTER_IMAGE;

    LISTE_POINTS * Head;
    Head = NULL;

    char nom[] = "chat_chien_1.ppm";

    I = Read_Image(nom, I);
    I2 = Read_Image("chat_chien_2.ppm", I2);

    Write_Image("chat_chien_remake.ppm", I);
    Write_Image("chat_chien_remake_2.ppm", I2);

    init_graphics(LARGEUR, HAUTEUR);
    set_mode_CANVAS();

    Show_Image(I);
    Show_Image(I2);

    Head = Create_Couples_of_Points(Head, I, I2);
    Read_Point_Couples("Couples_de_points.ppm");
    print_list(Head);

    wait_escape();
    for (int i = 0; i < I.hauteur; i++) free(I.P[i]);
    free(I.P);
    
    for (int i = 0; i < I2.hauteur; i++) free(I2.P[i]);
    free(I2.P);

    exit(0);
}