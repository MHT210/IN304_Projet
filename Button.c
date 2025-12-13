#include "Button.h"

void creer_bouton(BOUTON button)
{
    POINT p1 = { button.x - button.largeur/2, button.y - button.hauteur/2 };
    POINT p2 = { button.x + button.largeur/2, button.y + button.hauteur/2 };

  // Rectangle du bouton (rempli)
    draw_fill_rectangle(p1, p2, button.couleur);

  // Texte "Stop" centré
    int taille = 20;
    POINT txt;

    txt.x = p1.x + ( (p2.x - p1.x) - largeur_texte(button.nom, taille) ) / 2;
    txt.y = p1.y + ( (p2.y - p1.y) - hauteur_texte(button.nom, taille) ) / 2;

    aff_pol(button.nom, taille, txt, noir);
    affiche_all();
}

// --- Détection du clic dans le bouton Stop ---
int bouton_cliquer(POINT p, BOUTON button)
{
    if (p.x >= button.x - button.largeur/2 && p.x <= button.x + button.largeur/2 &&
        p.y >= button.y - button.hauteur/2 && p.y <= button.y + button.hauteur/2)
    {
         return 1;
    }
    return 0;
}

