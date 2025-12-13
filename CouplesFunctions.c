#include "stdio.h"
#include <stdlib.h>
#include "CouplesFunctions.h"

POINT get_click_or_stop(BOUTON stop, BOUTON suppr) {
    // Fonction qui retourne la position du point sinon arrete le programme si stop est cliqué
    POINT p = wait_clic();
    if (bouton_cliquer(p, stop)) {
        printf("Couplage stopper\n");
        SDL_Quit();
        p.x = -1;
    } else if (bouton_cliquer(p, suppr)) {
        printf("suppression du dernier couples");
        p.y = -1;
    }
    return p;
}

int check_clic_position(POINT p1, POINT p2, IMAGE I, IMAGE I2) {
    // Vérifiez si p1 dans I et p2 dans I2
    if (Is_In_Image(I, p1, HAUTEUR) && Is_In_Image(I2, p2, HAUTEUR))
        return 1;

    // Vérifiez si p1 dans I2 et p2 dans I
    if (Is_In_Image(I2, p1, HAUTEUR) && Is_In_Image(I, p2, HAUTEUR))
        return 2;

    return 0;
}

void draw_circles_of_couple(POINT p1, POINT p2) {
    draw_circle(p1, CIRCLE_RAYON, RED);
    draw_circle(p2, CIRCLE_RAYON, RED);
}

LISTE_POINTS * Get_Pixel_Couple(LISTE_POINTS * Head, IMAGE I, IMAGE I2, BOUTON stop, BOUTON suppr) {
    int keepGoing = 1;

    while (keepGoing) {
        // Premier clic
        POINT p1 = get_click_or_stop(stop, suppr);
        if (p1.x == -1) {
            keepGoing = 0;
            SDL_Quit();
            return Head;
        } else if (p1.y == -1) { // appuie sur le bouton supprimer
            if (Head != NULL) {
                draw_circle(get_last_points(Head).G, CIRCLE_RAYON, WHITE);
                draw_circle(get_last_points(Head).D, CIRCLE_RAYON, WHITE);
                Head = delete_last(Head);
                Head = Get_Pixel_Couple(Head, I, I2, stop, suppr);
            }
            return Head;
        }

        // Deuxième clic
        POINT p2 = get_click_or_stop(stop, suppr);
        if (p2.x == -1) {
            keepGoing = 0;
            SDL_Quit();
            return Head;
        }

        int result = check_clic_position(p1, p2, I, I2);

        if (result == 1) {
            draw_circles_of_couple(p1, p2);
            Head = insert_last(Head, p1.x, p1.y, p2.x, p2.y);
        }
        else if (result == 2) {
            draw_circles_of_couple(p2, p1);
            Head = insert_last(Head, p2.x, p2.y, p1.x, p1.y);
        }
        else {  
            printf("choisir une autre image !\n");
            SDL_Quit();
            keepGoing = 0;
            return Head;
        }
    }
    return Head;
}

LISTE_POINTS * Init_With_Couples_of_Base_Points(LISTE_POINTS * Head, IMAGE I, IMAGE I2) {
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

LISTE_POINTS * Create_Couples_of_Points(LISTE_POINTS * Head, IMAGE I, IMAGE I2, BOUTON stop, BOUTON suppr) {
    // initialiser avec les couples de base
    Head = Init_With_Couples_of_Base_Points(Head, I, I2);

    Head = Get_Pixel_Couple(Head, I, I2, stop, suppr);

    return Head;
}

void Save_Point_Couples(LISTE_POINTS * Head) {
    // parcourir la liste chainée et ecrire les coordonnées x et y des couples dans le fichier
    FILE *F;

    F = fopen("Couples_de_points.ppm", "w");
    if (!F) {perror("fopen"); exit(1);}

    fprintf(F, "%d\n",length_of_list(Head));
    
    while (Head) {
        fprintf(F, "%d %d  %d %d\n", Head->CP.G.x, Head->CP.G.y, 
            Head->CP.D.x, Head->CP.D.y);
        Head = Head->suiv;
    }

    fclose(F);
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