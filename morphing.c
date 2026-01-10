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

// Fonction pour lire un pixel avec interpolation bilinéaire
PIXEL interpoler_pixel(IMAGE img, float x, float y) {
    PIXEL result = {0, 0, 0};
    
    // Clamping
    if (x < 0) x = 0;
    if (x >= img.largeur - 1) x = img.largeur - 1.001;
    if (y < 0) y = 0;
    if (y >= img.hauteur - 1) y = img.hauteur - 1.001;
    
    int x0 = (int)floor(x);
    int y0 = (int)floor(y);
    int x1 = x0 + 1;
    int y1 = y0 + 1;
    
    float fx = x - x0;
    float fy = y - y0;
    
    // Vérification supplémentaire
    if (x1 >= img.largeur) x1 = img.largeur - 1;
    if (y1 >= img.hauteur) y1 = img.hauteur - 1;
    
    // Interpolation bilinéaire
    result.R = (int)(
        (1-fx) * (1-fy) * img.P[y0][x0].R +
        fx * (1-fy) * img.P[y0][x1].R +
        (1-fx) * fy * img.P[y1][x0].R +
        fx * fy * img.P[y1][x1].R
    );
    
    result.G = (int)(
        (1-fx) * (1-fy) * img.P[y0][x0].G +
        fx * (1-fy) * img.P[y0][x1].G +
        (1-fx) * fy * img.P[y1][x0].G +
        fx * fy * img.P[y1][x1].G
    );
    
    result.B = (int)(
        (1-fx) * (1-fy) * img.P[y0][x0].B +
        fx * (1-fy) * img.P[y0][x1].B +
        (1-fx) * fy * img.P[y1][x0].B +
        fx * fy * img.P[y1][x1].B
    );
    
    return result;
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

IMAGE morphing(TRIANGLE_HEAD TH_I, TRIANGLE_HEAD TH_D, TRIANGLE_HEAD TH_A, float alpha, IMAGE I, IMAGE I2, IMAGE I3) {
    FILE *F;
    F = fopen("frame_00.ppm", "w");
    if (!F) {perror("Pd de fichier"); exit(20);}

    I3.hauteur = (int)fmax(I.hauteur, I2.hauteur);
    I3.largeur = (int)fmax(I.largeur, I2.largeur);
    I3.max_value_rgb = 255;
    I3.decal_value = 0;

    fprintf(F, "P3\n");
    fprintf(F, "%d %d\n", I3.largeur, I3.hauteur);
    fprintf(F, "%d\n", I3.max_value_rgb);

    I3.P = malloc(I3.hauteur * sizeof(PIXEL *));
    for (int i=0; i < I3.hauteur; i++) I3.P[i] = malloc(I3.largeur * sizeof(PIXEL));
    int k = 1;
    int pixelparcouru = 0;
    int pixeldansimage3 = 0;
    for (int y=0; y < 900; y++) {
        for (int x=0; x < 1550; x++) {
            int keepGoing = 1;
            ELEMENT *eI = TH_I.head;
            ELEMENT *eD = TH_D.head;
            ELEMENT *eA = TH_A.head;
            while (eI && eD && eA && keepGoing) {
                POINT P = {x, y};
                // draw_circle(P, 15, couleur_RGB(255, 0, 255));
                pixelparcouru++;
                float lambda, mu;

                POINT A = eI->triangle.P1;
                POINT B = eI->triangle.P2;
                POINT C = eI->triangle.P3;

                POINT Ad = eD->triangle.P1;
                POINT Bd = eD->triangle.P2;
                POINT Cd = eD->triangle.P3;

                POINT Aa = eA->triangle.P1;
                POINT Ba = eA->triangle.P2;
                POINT Ca = eA->triangle.P3;
                if (lambda < 0.0f && mu < 0.0f && (lambda + mu) > 1.0f) {printf("P pas dans image !");}
                barycentricCoordinates(A, B, C, P, &lambda, &mu);
                if (lambda >= 0.0f && mu >= 0.0f && (lambda + mu) <= 1.0f) {
                    pixeldansimage3++;
                    float Pd_x = Ad.x + lambda * (Bd.x - Ad.x) + mu * (Cd.x - Ad.x);
                    float Pd_y = Ad.y + lambda * (Bd.y - Ad.y) + mu * (Cd.y - Ad.y);

                    float Pa_x = Aa.x + lambda * (Ba.x - Aa.x) + mu * (Ca.x - Aa.x);
                    float Pa_y = Aa.y + lambda * (Ba.y - Aa.y) + mu * (Ca.y - Aa.y);

                    // CLAMPING : Forcer dans les limites
                    int pd_x = (int)round(Pd_x);
                    int pd_y = (int)round(Pd_y);
                    int pa_x = (int)round(Pa_x);
                    int pa_y = (int)round(Pa_y);

                    // Clipper aux bornes de l'image
                    pd_x = (pd_x < 0) ? 0 : (pd_x >= I.largeur) ? I.largeur - 1 : pd_x;
                    pd_y = (pd_y < 0) ? 0 : (pd_y >= I.hauteur) ? I.hauteur - 1 : pd_y;
                    pa_x = (pa_x < 0) ? 0 : (pa_x >= I2.largeur) ? I2.largeur - 1 : pa_x;
                    pa_y = (pa_y < 0) ? 0 : (pa_y >= I2.hauteur) ? I2.hauteur - 1 : pa_y;

                    POINT Pd = {pd_x, pd_y};
                    // draw_circle(Pd, 15, couleur_RGB(0, 255, 255));

                    POINT Pa = {pa_x, pa_y};
                    // draw_circle(Pa, 15, couleur_RGB(255, 255, 0)); 

                    if (!InImageMorph(I, Pd)) {printf("Pd pas dans image !\n");}
                    if (!InImageMorph(I2, Pa)) {printf("Pa pas dans image !\n");}
                    
                    if (InImageMorph(I, Pd) && InImageMorph(I2, Pa)) {
                        int INT_red = (1-alpha) * I.P[(int)Pd.y][(int)Pd.x].R + alpha * I2.P[(int)Pa.y][(int)Pa.x].R;
                        int INT_green = (1-alpha) * I.P[(int)Pd.y][(int)Pd.x].G + alpha * I2.P[(int)Pa.y][(int)Pa.x].G;
                        int INT_blue = (1-alpha) * I.P[(int)Pd.y][(int)Pd.x].B + alpha * I2.P[(int)Pa.y][(int)Pa.x].B;
                        I3.P[y][x].R = INT_red;
                        I3.P[y][x].G = INT_green;
                        I3.P[y][x].B = INT_blue;
                        k++;
                        fprintf(F, "%d ", I3.P[y][x].R);
                        fprintf(F, "%d ", I3.P[y][x].G);
                        fprintf(F, "%d ", I3.P[y][x].B);
                        if (k/8 == 1) {
                            fprintf(F, "\n");
                            k = 1;
                        }
                    }
                    keepGoing = 0;
                }
                eI = eI->next;
                eD = eD->next;
                eA = eA->next;
            }
        }
    }
    printf("\n pixel parcouru : %d \n", pixelparcouru);
    printf("\n pixel dans image I3 : %d \n", pixeldansimage3);
    fclose(F);
    return I3;
}
