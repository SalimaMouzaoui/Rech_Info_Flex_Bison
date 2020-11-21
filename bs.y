%{
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ts.h"
extern int ligne; 
extern int colonne;
extern FILE* yyin;
int res = 0;
%}
%union{
       int entier;
       char *chaine;
      } 
%token <chaine> ENTIER MOT POINT CROCH_OUVRT CROCH_FERMT TITLE AUTHORS ABSTRACT KEYWORDS ESPACE VIRGULE 
%token <chaine> INTRODUCTION RELATEDWORKS CONCEPTION EXPERIMENTALRESULTS CONCLUSION REFERENCES REF SAUT TABULATION CHIFFRE
%start program

%%
program : titre auteur resum key intro work concept result concl referenc ;
titre : TITLE ESPACE mottitre SAUT ;
mottitre : mottitre ESPACE MOT { if (NbMot($3) > 10) {yyerror("Erreur: le nombre de mots du titre doit etre <= 10\n");  return;}} | MOT { if (NbMot($1) > 10) {yyerror("Erreur: le nombre de mots du titre doit etre <= 10\n"); return; }};
auteur : AUTHORS ESPACE listnom POINT SAUT;
listnom : listnom VIRGULE ESPACE listmot | listmot ;
resum : ABSTRACT ESPACE motresum POINT SAUT ;
motresum : motresum ESPACE MOT { if (NbMot($3) > 100) {yyerror("Erreur: le nombre de mots de resume doit etre <= 100\n"); return;}} | MOT { if (NbMot($1) > 100) {yyerror("Erreur: le nombre de mots de resume doit etre <= 100\n"); return;}} ;
key : KEYWORDS ESPACE listnom POINT SAUT ;
intro : INTRODUCTION ESPACE paragraph POINT SAUT ;
work : RELATEDWORKS ESPACE paragraph POINT SAUT ;
concept : CONCEPTION ESPACE paragraph POINT SAUT ;
result : EXPERIMENTALRESULTS ESPACE paragraph POINT SAUT ;
concl : CONCLUSION ESPACE paragraph POINT SAUT ;
paragraph : paragraph ESPACE MOT | MOT | refer;
refer : paragraph ESPACE REF {inser_ref($3);};
referenc : REFERENCES SAUT listref ;
listref : listref SAUT refer | refer ;
refer : REF { if (verif_ref($1) == 0) {yyerror ("Erreur: la references n'existe pas dans la liste des references\n"); return; }} TABULATION listmot POINT  ;
listmot : listmot ESPACE MOT | MOT ;
%%
int yyerror(char *msg)
{
       printf("Erreur syntaxique à la ligne : %d , colonne : %d\n",ligne, colonne);
       printf("%s\n", msg);
	   res = 1;
       return 1;
}

int main(int argc, char *argv[])
{
int i = 1;
int j, nbDoc = 0;
int max;
char *sortie; 
FILE *fichier = NULL;
init();
int nbff = 0;
int somme = 0;
clock_t dbt, fin;
 double moy_tps_exe1, moy_tps_exe2;
char *nomFile;
char path[20] = "results\\";
 char sh[2] = "\\";
if (argc != 2) {
printf ("argument manquant veuillez entrer le nom de votre corpus et reessaye a nouveau \n"); return; }
else {
 d = opendir(argv[1]);
while ((rd = readdir(d)) != NULL)
{
    if ((strcmp(rd->d_name, ".") != 0) && (strcmp(rd->d_name, "..") != 0)){
	ligne = 1; 
    colonne = 0;
	nbMot = 0;
	nomFile = strtok(argv[1], sh); 
	strtok (path, sh);
    strcat (nomFile, sh);
	strcat (path, "\\");
    strcat (nomFile, rd->d_name);
	strcat (path, rd->d_name);
    yyin = fopen(nomFile, "r");	
	yyrestart(yyin);
	dbt = clock();
	yyparse();
	fin = clock();
    moy_tps_exe1 += fin - dbt; 	
	fclose (yyin);
	if (res == 0) {  nbDoc++; printf ("\n\n document %s est valide \n\n", nomFile);
	fichier = fopen(path, "wt"); 
	Construct_File(fichier);
	fclose (fichier);
	//Afficher();
	somme += nbMot;
	nbff++;
	}
	else if (res = 1) { printf ("\n\n document %s est invalide \n\n", nomFile);}
	res = 0;
	 }
	 
}
}
 closedir(d);
char *nomf;
 initialIndex(nbDoc); 
 char pathh[20] = "results";
 d = opendir(pathh); 
int nb;
while ((rd = readdir(d)) != NULL)
{
    if ((strcmp(rd->d_name, ".") != 0) && (strcmp(rd->d_name, "..") != 0) ){
    nomf = strtok(pathh, sh); 
    strcat(nomf, sh);	
    strcat(nomf, rd->d_name);
	strcpy(tabFile[i].nomDoc, rd->d_name);
	i++;
	dbt = clock();
	insertionIndex(nomf, rd->d_name, nbDoc); 	
	fin = clock();
    moy_tps_exe2 += fin - dbt;
	}
}
closedir(d);
char *maxMot[4];
int maxT[4];
	nb = nbrMotSingle(nbDoc);
int c;
	fichier = fopen("index.txt", "w");
	RemplissageIndex(nbDoc, fichier); 	 
    fclose(fichier);
do {
printf (" ************** MENU **************** \n");
printf (" 1 ** le nombre de mots total du corpus \n");
printf (" 2 ** le nombre de mots figurant juste une seule fois dans le corpus\n");
printf (" 3 ** la frequence d'apparition de chaque mot dans le corpus\n");
printf (" 4 ** les 4 mots les plus utilises dans le corpus\n");
printf (" 5 ** le temps d'execution ecoule pour pouvoir analyser tous les documents du corpus\n");
printf (" 6 ** le temps d'execution ecoule lors de la generation de l'index\n");
printf (" 7 ** le temps de traitement d'une requete\n");
printf (" 8 ************ QUITTER ************* \n");
printf (" ************************************ \n");
printf (" veuillez tapper un choix : \n");
 scanf ("%d", &c);
char mot[50];
 switch (c)
 {
     case 1 : printf ("\nle nombre de mots total du corpus = %d\n\n", somme);
              break;
     case 2 : printf ("\nle nombre de mots figurant juste une seule fois dans le corpus = %d\n\n", nb);
              break;
     case 3 : printf ("\nla frequence d'apparition de chaque mot dans le corpus\n\n");
	          AfficherIndex(nbDoc);
              break;
     case 4 : printf ("\nles 4 mots les plus utilises dans le corpus sont : \n\n");
	          max4Mots (maxT, maxMot, nbDoc);
			  for ( i=0; i < 4; i++)
			  {printf ("le mot %s apparait %d fois\n", maxMot[i], maxT[i]);			   
			  printf ("\n");}
              break;
     case 5 : printf ("\nle temps d'execution ecoule pour pouvoir analyser tous les documents du corpus = %f \n\n", (float)(moy_tps_exe1) / CLOCKS_PER_SEC * nbff);
              break;
     case 6 : printf ("le temps d'execution ecoule lors de la generation de l'index = %f\n\n", (float)(moy_tps_exe2) / CLOCKS_PER_SEC * nbDoc);
              break;
     case 7 : printf ("\nle temps de traitement d'une requete\n");
	          printf ("\ndonner un mot a recherche afin de trouver le document le plus partinent : \n\n");
			  scanf ("%s", &mot);
	          dbt = clock();
	          RechercheIndex(mot, &sortie, &max, nbDoc);
	          fin = clock();
	printf("le temps d'execution ecoule pour pouvoir traiter une requete = %f \n\n", (float)(fin - dbt) / CLOCKS_PER_SEC);
              break;
			  default : break;
			  }
 }
 while (c != 8);
 return 0;
}