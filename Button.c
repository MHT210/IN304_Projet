#include "Button.h"

// --- Couleur jaune ---
#define JAUNE couleur_RGB(255, 255, 0)

void bouton_stop(int largeur, int hauteur)
{
    POINT p1 = { largeur/2 - 150/2, hauteur/2 - 20/2 };  // coin haut gauche du bouton
    POINT p2 = { largeur/2 + 150/2, hauteur/2 + 20/2 };  // coin bas droit du bouton

  // Rectangle du bouton (rempli)
    draw_fill_rectangle(p1, p2, JAUNE);

  // Texte "Stop" centré
    char texte[] = "Stop";
    int taille = 20;
    POINT txt;

    txt.x = p1.x + ( (p2.x - p1.x) - largeur_texte(texte, taille) ) / 2;
    txt.y = p1.y + ( (p2.y - p1.y) - hauteur_texte(texte, taille) ) / 2;

    aff_pol(texte, taille, txt, noir);
    affiche_all();
}

// --- Détection du clic dans le bouton Stop ---
int bouton_stop_cliquer(int largeur, int hauteur, POINT p)
{
    if (p.x >= largeur/2 - 150/2 && p.x <= largeur/2 + 150/2 &&
        p.y >= hauteur/2 - 20/2 && p.y <= hauteur/2 + 20/2)
    {
         return 1;
    }
    return 0;
}

