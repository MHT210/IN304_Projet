#include "stdio.h"
#include <stdlib.h>
#include "ChainedList.h"

// La fonction INSERER AU DEBUT de la liste
LISTE_POINTS * insert_first(LISTE_POINTS *list, int x1, int y1, int x2, int y2) {
	LISTE_POINTS * temp;
	temp = malloc(sizeof(LISTE_POINTS));
	if (!temp) {fprintf(stderr, "pb malloc\n"); exit(12);}
	temp->CP.G.x = x1;
	temp->CP.G.y = y1;
	temp->CP.D.x = x2;
	temp->CP.D.y = y2;
	(*temp).suiv = list;
	return temp;
}

// La fonction INSERER A LA FIN de la liste
LISTE_POINTS * insert_last(LISTE_POINTS *list, int x1, int y1, int x2, int y2) {
	if (!list) {
		LISTE_POINTS * last = malloc(sizeof(LISTE_POINTS));
		if (!last) {fprintf(stderr, "pb malloc\n"); exit(12);}
		last->CP.G.x = x1;
		last->CP.G.y = y1;
		last->CP.D.x = x2;
		last->CP.D.y = y2;
		last->suiv = NULL;
		return last;
	}
	
	list->suiv = insert_last(list->suiv, x1, y1, x2, y2);
	return list;
}

void print_list(LISTE_POINTS *list) {
	int count = 0;
	while (list) {
		count += 1;
		printf("Couple numéro : %d\n", count);
		printf("premier point GAUCHE : %d %d\n", list->CP.G.x, list->CP.G.y );
		printf("deuxième point DROITE : %d %d\n", list->CP.D.x, list->CP.D.y );
		list = list->suiv;
	}
}