int main(int argc, char** argv){
    if(argc != 3){ 
        printf("Erreur argument de la ligne de commande\n");
        printf("Format attendu:\n");
        exit(1);
    }
    //on créé le fichier d'images
    system("rm -rf Images");
    system("rm -rf film.mp4");
    system("mkdir Images");
    Tab_point t_p;
    Tab_image t_i;
    //on initialise le tableau d'images
    t_i = init_tableau_image(atoi(argv[2]));
    //on lit le fichier de points
    t_p = lire_fichier_points(argv[1]);
    //affiche_tab(t_p);
    //on calcule les images intermédiaires dans t_i
    remplir_tab_image(t_i, t_p, 512, 512, 256);

    init_graphics(1536, 512);
    affiche_auto_off();
    POINT p1, p2;
    char nom[128];
    //on affiche les images dans la fenêtre graphique,
    //et on les écrit dans un fichier
    for(int i=0; i<t_i.n; i++){
        //on affiche à gauche l'image de départ
        afficher_image(t_i.t[0], 0);
        //on affiche à droite l'image d'arrivée
        afficher_image(t_i.t[t_i.n - 1], 1024);
        afficher_image(t_i.t[i], 512);
        p2.x = p1.x = 512;
        p1.y = 0; p2.y = 512;
        draw_line(p1, p2, blanc);
        p2.x = p1.x = 1024;
        draw_line(p1, p2, blanc);
        affiche_all();
        sprintf(nom, "Images/img%04d.ppm", i);
        ecrire_fichier(t_i.t[i], nom);
    }
    wait_clic();

    libere_tab(t_p, t_i);
    sprintf(nom,"ffmpeg -framerate 20 -i Images/img%%04d.ppm -c:v libx264 -pix_fmt yuv420p toto.mp4");
    system(nom);

    return 0;
}