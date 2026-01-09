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

void barycentricCoordinates(POINT A, POINT B, POINT C, POINT p, float *alpha, float *beta, float *gamma) {
    POINT AB = {B.x - A.x, B.y - A.y};
    POINT AC = {C.x - A.x, C.y - A.y};
    POINT AP = {p.x - A.x, p.y - A.y};

    float denom = deter(AB, AC);
    *alpha = deter((POINT){p.x - B.x, p.y - B.y}, (POINT){C.x - B.x, C.y - B.y}) / denom;
    *beta = deter(AP, AC) / denom;
    *gamma = deter(AB, AP) / denom;
}

void morphing(TRIANGLE_HEAD TH_I, TRIANGLE_HEAD TH_D, TRIANGLE_HEAD TH_A, float alpha, IMAGE I, IMAGE I2, IMAGE I3) {
    ELEMENT *e = TH_I.head;
    ELEMENT *eD = TH_D.head;
    ELEMENT *eA = TH_A.head;

    I3.hauteur = (int)fmax(I.hauteur, I2.hauteur);
    I3.largeur = (int)fmax(I.largeur, I2.largeur);
    I3.max_value_rgb = 255;
    I3.decal_value = 0;

    I3.P = malloc(I3.hauteur * sizeof(PIXEL *));
    for (int i=0; i < I3.hauteur; i++) I3.P[i] = malloc(I3.largeur * sizeof(PIXEL));

    // Initialiser I3
    for (int y = 0; y < I3.hauteur; y++) {
        for (int x = 0; x < I3.largeur; x++) {
            I3.P[y][x].R = 0;
            I3.P[y][x].G = 0;
            I3.P[y][x].B = 0;
        }
    }

    for (int y = 0; y < I3.hauteur; y++) {
        for (int x = e->triangle.P1.x; x < I3.largeur; x++) {
            POINT P = {x, y};
            draw_circle(P, 5, couleur_RGB(255, 0, 0));
            int found = 0;

            while (e && eD && eA && !found) {
                POINT A = e->triangle.P1;
                POINT B = e->triangle.P2;
                POINT C = e->triangle.P3;
 
                float lambda, mu, alpha_bary;
                barycentricCoordinates(A, B, C, P, &alpha_bary, &lambda, &mu);

                if (alpha_bary >= 0 && lambda >= 0 && mu >= 0 && 
                    alpha_bary + lambda + mu <= 1.01) {

                    POINT Ad = eD->triangle.P1;
                    POINT Bd = eD->triangle.P2;
                    POINT Cd = eD->triangle.P3;

                    POINT Aa = eA->triangle.P1;
                    POINT Ba = eA->triangle.P2;
                    POINT Ca = eA->triangle.P3;

                    POINT Pd = {
                        Ad.x + lambda * (Bd.x - Ad.x) + mu * (Cd.x - Ad.x),
                        Ad.y + lambda * (Bd.y - Ad.y) + mu * (Cd.y - Ad.y)
                    };
                    
                    POINT Pa = {
                        Aa.x + lambda * (Ba.x - Aa.x) + mu * (Ca.x - Aa.x),
                        Aa.y + lambda * (Ba.y - Aa.y) + mu * (Ca.y - Aa.y)
                    };

                    int pd_x = (int)round(Pd.x);
                    int pd_y = (int)round(Pd.y);
                    int pa_x = (int)round(Pa.x);
                    int pa_y = (int)round(Pa.y);
                    
                    if (pd_x >= 0 && pd_x < I.largeur && pd_y >= 0 && pd_y < I.hauteur &&
                        pa_x >= 0 && pa_x < I2.largeur && pa_y >= 0 && pa_y < I2.hauteur) {

                        I3.P[y][x].R = (int)((1-alpha) * I.P[pd_y][pd_x].R + alpha * I2.P[pa_y][pa_x].R);
                        I3.P[y][x].G = (int)((1-alpha) * I.P[pd_y][pd_x].G + alpha * I2.P[pa_y][pa_x].G);
                        I3.P[y][x].B = (int)((1-alpha) * I.P[pd_y][pd_x].B + alpha * I2.P[pa_y][pa_x].B);

                        found = 1;
                    }
                }
                
                e = e->next;
                eD = eD->next;
                eA = eA->next;
            }
        }
    }
}

