#include "morphing.h"


POINTS_INT * insert_last_i(POINTS_INT * list_I, int x, int y) {
	if (!list_I) {
		POINTS_INT * last = malloc(sizeof(POINTS_INT));
		if (!last) {fprintf(stderr, "pb malloc\n"); exit(12);}
		last->P.x = x;
		last->P.y = y;
		last->suiv = NULL;
		return last;
	}
	
	list_I->suiv = insert_last_i(list_I->suiv, x, y);
	return list_I;
}

void free_list(POINTS_INT *head) {
    POINTS_INT *current = head;
    POINTS_INT *next;
    
    while (current != NULL) {
        next = current->suiv;
        free(current);
        current = next;
    }
}

POINTS_INT * getIntPoints(POINTS_INT * list_I, LISTE_POINTS * listD_G, float alpha) {
    LISTE_POINTS * temp = listD_G;

    while (temp) {
        int x = (1 - alpha) * temp->CP.G.x + alpha * temp->CP.D.x;
        int y = (1 - alpha) * temp->CP.G.y + alpha * temp->CP.D.y;
        list_I = insert_last_i(list_I, x, y);
        temp = temp->suiv;
    }

    return list_I;
}

void barycentricCoordinates(POINT A, POINT B, POINT C, POINT p, float *lambda, float *mu) {
    POINT AB = {B.x - A.x, B.y - A.y};
    POINT AC = {C.x - A.x, C.y - A.y};
    POINT AP = {p.x - A.x, p.y - A.y};

    float det = AB.x * AC.y - AB.y * AC.x;

    *lambda = (AP.x * AC.y - AP.y * AC.x) / det;
    *mu = (AB.x * AP.y - AB.y * AP.x) / det;
}

int InImageMorph(IMAGE I, POINT p) {
    return (p.x >= 0 && p.x < I.largeur && p.y >= 0 && p.y < I.hauteur);
}

IMAGE_INTER * morphing(POINTS_INT * Head_I, IMAGE_INTER * Images, TRIANGLE_HEAD TH_I, TRIANGLE_HEAD TH_D, TRIANGLE_HEAD TH_A, float alpha, IMAGE I, IMAGE I2, IMAGE I3) {
    int xInf = Head_I->P.x;
    int xSup = Head_I->suiv->P.x;
    int ySup = Head_I->suiv->suiv->P.y;

    I3.largeur = abs(xSup - xInf) + 1;
    I3.hauteur = ySup;
    I3.max_value_rgb = 255;
    I3.decal_value = 0;

    I3.P = calloc(I3.hauteur, sizeof(PIXEL *));
    if (!I3.P) {fprintf(stderr, "Erreur allocation I3.P\n"); exit(1);}

    for (int i=0; i < I3.hauteur; i++) {
        I3.P[i] = calloc(I3.largeur, sizeof(PIXEL));
        if (!I3.P[i]) {
            fprintf(stderr, "Erreur allocation I3.P[%d]\n", i);
            for (int j=0; j < i; j++) free(I3.P[j]);
            free(I3.P);
            exit(1);
        }
    }

    int pixelparcouru = 0;
    int pixeldansimage3 = 0;
    int pixeldansimage1_2 = 0;

    for (int y=0; y < I3.hauteur; y++) {
        for (int x = xInf; x <= xSup; x++) {
            POINT P = {x, y};
            pixelparcouru++;
            ELEMENT *eI = TH_I.head;
            int triangle_index = 0;
            int found_triangle = 0;
            
            while (eI && !found_triangle) {
                if (PointInTriangle(eI->triangle, P)) {
                    found_triangle = 1;
                } else {
                    triangle_index++;
                    eI = eI->next;
                }
            }
            
            if (found_triangle) {
                float lambda = 0.0f;
                float mu = 0.0f;
                POINT A = eI->triangle.P1;
                POINT B = eI->triangle.P2;
                POINT C = eI->triangle.P3;
                barycentricCoordinates(A, B, C, P, &lambda, &mu);
                
                ELEMENT *eD = TH_D.head;
                for (int i = 0; i < triangle_index; i++) {
                    eD = eD->next;
                }
                
                ELEMENT *eA = TH_A.head;
                for (int i = 0; i < triangle_index; i++) {
                    eA = eA->next;
                }
                
                POINT Ad = eD->triangle.P1;
                POINT Bd = eD->triangle.P2;
                POINT Cd = eD->triangle.P3;
                
                int Pd_x = Ad.x + lambda * (Bd.x - Ad.x) + mu * (Cd.x - Ad.x);
                int Pd_y = Ad.y + lambda * (Bd.y - Ad.y) + mu * (Cd.y - Ad.y);
                POINT Pd = {Pd_x, Pd_y};
                
                POINT Aa = eA->triangle.P1;
                POINT Ba = eA->triangle.P2;
                POINT Ca = eA->triangle.P3;
                
                int Pa_x = Aa.x + lambda * (Ba.x - Aa.x) + mu * (Ca.x - Aa.x);
                int Pa_y = Aa.y + lambda * (Ba.y - Aa.y) + mu * (Ca.y - Aa.y);
                POINT Pa = {Pa_x, Pa_y};
                POINT Pa_dans_image = {Pa_x - I2.decal_value, Pa_y};
                pixeldansimage3++;

                if (InImageMorph(I, Pd) && InImageMorph(I2, Pa_dans_image)) {
                    pixeldansimage1_2++;
                    if (Pd.y >= 0 && Pd.y < I.hauteur && Pd.x >= 0 && Pd.x < I.largeur &&
                        Pa_dans_image.y >= 0 && Pa_dans_image.y < I2.hauteur && 
                        Pa_dans_image.x >= 0 && Pa_dans_image.x < I2.largeur) {

                        int INT_red = (1-alpha) * I.P[Pd.y][Pd.x].R + alpha * I2.P[Pa_dans_image.y][Pa_dans_image.x].R;
                        int INT_green = (1-alpha) * I.P[Pd.y][Pd.x].G + alpha * I2.P[Pa_dans_image.y][Pa_dans_image.x].G;
                        int INT_blue = (1-alpha) * I.P[Pd.y][Pd.x].B + alpha * I2.P[Pa_dans_image.y][Pa_dans_image.x].B;

                        int idx_x = x - xInf;
                        if (idx_x >= 0 && idx_x < I3.largeur && y >= 0 && y < I3.hauteur) {
                            I3.P[y][idx_x].R = INT_red;
                            I3.P[y][idx_x].G = INT_green;
                            I3.P[y][idx_x].B = INT_blue;
                        } else {
                            printf("ERREUR: P=(%d,%d hors limites!\n", x, y);
                        }
                    } else {
                        printf("ERREUR: Pd=(%d,%d) Pa=(%d,%d) hors limites!\n", 
                            Pd.x, Pd.y, Pa_dans_image.x, Pa_dans_image.y);
                    }
                }
            }
        }
    }
    printf("\n pixel parcouru : %d \n", pixelparcouru);
    printf("\n pixel dans image I3 : %d \n", pixeldansimage3);
    printf("\n pixel dans image I et I2 : %d \n", pixeldansimage1_2);
    Images = InsertLastImage(Images, I3);
    return Images;
}
