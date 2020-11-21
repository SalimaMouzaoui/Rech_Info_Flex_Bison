#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

typedef struct lst { char mot[50];
				     int freq;
				     char type[50];
				     struct lst *svt;} liste;

typedef struct { char mot[50];
				     int freq;
				     int etat;
				     char type[50];
                        liste *pointeur;} element;
						
typedef struct lst2 { int freq;
                      char nomDoc[50];} tab;
					  
typedef struct lst3 { char mot[50];
                      tab t[20];
				      struct lst3 *svt;} liste3;
					 
typedef struct { char mot[50];
				     int etat;
					 tab t[20];
                       liste3 *pointeur;} elementb;
				 
typedef struct { int freq;
                      char mot[50];} tabl;
typedef struct f { char nomDoc[50];} fich;

FILE* fichier;
DIR *d; 
struct dirent *rd;
char *tab_ref[20]; // un tableau de référence
fich tabFile[10];  // un tableau de fichiers de corpuss
int n = 1000;   
element TS[1000];  // la table de symboles
elementb Ind[1000];  // l'index
// on déclare une variable globale pour calculer le nombre de mot dans la table
int nbMot = 0;
//*************** Les fonctions de gestion de la table des symboles ************
int Fonction_Hachage(char *mot)
{   int i, valeur =0;
	for(i=0;i<strlen(mot);i++)
	   valeur = valeur + mot[i];
	 i = ( ( ( 123 * valeur ) + 139 ) * 173 ) % 1000;
	return i;
}


void insertion(char *mot, char *type)
{
long j;
  liste *p, *r, *q;
j = Fonction_Hachage(mot);
  if (TS[j].etat == 0){
      strcpy(TS[j].mot, mot);
      strcpy(TS[j].type, type);
      TS[j].freq = 1;
      TS[j].etat = 1;}
      else if (TS[j].pointeur == NULL){
           if (strcmp(TS[j].mot, mot) != 0) {
               TS[j].pointeur = (liste*)malloc(sizeof(liste));
               strcpy(TS[j].pointeur->mot, mot);
               strcpy(TS[j].pointeur->type, type);
               TS[j].pointeur->svt = NULL;
               TS[j].pointeur->freq=1;}
           else TS[j].freq++;   }
           else {
                if (strcmp(TS[j].mot, mot) != 0) {
                     for (r = TS[j].pointeur; (r->svt != NULL) && (strcmp(r->mot, mot) != 0) ; r=r->svt);
                       if (strcmp(r->mot, mot) != 0){
                            q = (liste*)malloc(sizeof(liste));
                            strcpy(q->mot, mot);
                            strcpy(q->type, type);
                            q->freq = 1;
                            q->svt = r->svt;
                            r->svt = q;}
                       else r->freq++; }
               else TS[j].freq++; }
  nbMot++;
}


void initialIndex(int nbDoc)
{
int i, k; 
for ( i = 1; i <= 1000 ; i++)
{
strcpy(Ind[i].mot, "");
Ind[i].etat = 0;
Ind[i].pointeur = NULL;
for (k = 0; k < nbDoc ; k++)
{
 Ind[i].t[k].freq == 0;
 strcpy(Ind[i].t[k].nomDoc, "");
}
}
}
void insertionIndex(char *fil, char *path, int nbDoc)
{
FILE* fichier = NULL;
    int freq;
    char ch[50];
    char buffer[50];
    int i, k, cle;
	long j;
  liste3 *p, *r, *q;
    fichier = fopen(fil, "r");
       while (fscanf(fichier, "%s %d", ch, &freq) != EOF)
         {
             j = Fonction_Hachage(ch);
	if (Ind[j].etat == 0 || (strcmp(Ind[j].mot, ch) == 0)){
  if (Ind[j].etat == 0){ 
      strcpy(Ind[j].mot, ch);
	  Ind[j].t[0].freq = freq;
	  strcpy(Ind[j].t[0].nomDoc, path);
	  Ind[j].etat = 1;
	  } else if (strcmp(Ind[j].mot, ch) == 0){
	  for (k = 0; k < nbDoc && (strcmp(Ind[j].t[k].nomDoc, "") != 0); k++);
                         if (strcmp(Ind[j].t[k].nomDoc, "") == 0){
                         Ind[j].t[k].freq = freq;
						 strcpy(Ind[j].t[k].nomDoc, path);
                        }
	  }} else if (Ind[j].pointeur == NULL){ 
           if (strcmp(Ind[j].mot, ch) != 0) {
               Ind[j].pointeur = (liste3*)malloc(sizeof(liste3));
               strcpy(Ind[j].pointeur->mot, ch);
               Ind[j].pointeur->svt = NULL;
			   Ind[j].pointeur->t[0].freq = freq;
			   strcpy(Ind[j].pointeur->t[0].nomDoc, path);
              } else  if (strcmp(Ind[j].mot, ch) == 0) {
			     for (k = 0; k < nbDoc && (strcmp(Ind[j].pointeur->t[k].nomDoc, "") != 0); k++);
                         if (strcmp(Ind[j].pointeur->t[k].nomDoc, "") == 0){
                         Ind[j].pointeur->t[k].freq = freq;
						 strcpy(Ind[j].pointeur->t[k].nomDoc, path);
                        }
			   }}
           else if (strcmp(Ind[j].mot, ch) != 0) { 
                     for (r = Ind[j].pointeur; r != NULL ; r=r->svt){
                       if (strcmp(r->mot, ch) != 0){
                            q = (liste3*)malloc(sizeof(liste3));
                            strcpy(q->mot, ch);
							q->t[0].freq = freq;
						    strcpy(q->t[0].nomDoc, path);	
                            q->svt = NULL;       
                            r->svt = q;
							} else if (strcmp(r->mot, ch) == 0){ 
							for (k = 0; k < nbDoc && (strcmp(r->t[k].nomDoc, "") != 0); k++);
                              if (strcmp(r->t[k].nomDoc , "") == 0){
                                r->t[k].freq = freq;
						        strcpy(r->t[k].nomDoc, path);
                              } 
					 }}}
}
}

void RechercheIndex(char ch[50], char *sortie[], int *max, int nbDoc)
{
FILE* fichier = NULL;
    int freq;
    int i, k, cle;
	long j;
	 *max = 0; int pos;
  liste3 *p, *r, *q;      
  j = Fonction_Hachage(ch);
  if (Ind[j].etat != 0){
  if (strcmp(Ind[j].mot, ch) == 0){   
	  for (k = 0; k < nbDoc; k++){
	  if (Ind[j].t[k].freq > *max)
	  *max = Ind[j].t[k].freq;
	  pos = k;}
	  *sortie = Ind[j].t[pos].nomDoc;
	  }
      else if (strcmp(Ind[j].mot, ch) != 0){
                     for (r = Ind[j].pointeur; (r->svt != NULL) && (strcmp(r->mot, ch) != 0) ; r=r->svt);
                       if (strcmp(r->mot, ch) == 0){ 
					   for (k = 0; k < nbDoc; k++){
	                   if (r->t[k].freq > *max)
	                   *max = r->t[k].freq;
	                   pos = k;}
	                   *sortie = r->t[pos].nomDoc;
	                   }
                        }                         
    }
}

void AfficherIndex(int nbDoc)
{
int i, k;
liste3 *q;

printf("\n **************** AFFICHAGE ***************\n\n");

for(i = 1; i <= 1000; i++){
  if(Ind[i].etat!=0)
    {
     printf(" %s" ,Ind[i].mot);
	 
for (k = 0; k < nbDoc ; k++)
{
 printf (" ------> %d 	", Ind[i].t[k].freq);
 printf (" -----> %s\n", Ind[i].t[k].nomDoc);
}
    for(q = Ind[i].pointeur; q != NULL; q = q->svt)
        {
          printf("   %s" ,q->mot);
		  for (k = 0; k < nbDoc ; k++)
{
 printf (" ------> %d 	", q->t[k].freq);
 printf (" -----> %s\n", q->t[k].nomDoc);
}
        }
		printf("\n");
   }
}
}

void RemplissageIndex(int nbDoc, FILE *fichier)
{
int i, j, k, l, m;
liste3 *q;
fprintf(fichier, "\n-------------------------------------------------\n");
fprintf (fichier, "|  	  		  ");
for (i= 1; i <= nbDoc; i++)
{
fprintf (fichier, "| 	%s		", tabFile[i].nomDoc);
}
fprintf(fichier, "|\n-------------------------------------------------\n");
for(m = 1; m <= 1000; m++){
  if(Ind[m].etat != 0)
    {
fprintf (fichier, "|  %s		 ", Ind[m].mot);
for (k = 0; k < nbDoc ; k++)
{ 
 for (l = 1; l <= nbDoc && (strcmp(Ind[m].t[k].nomDoc, tabFile[l].nomDoc) != 0); l++);
 if (strcmp(Ind[m].t[k].nomDoc, tabFile[l].nomDoc) == 0)
 {
fprintf (fichier, "| 		%d		 ", Ind[m].t[k].freq);
 }
 }
fprintf(fichier, "|\n-------------------------------------------------\n");
    for(q = Ind[m].pointeur; q != NULL; q = q->svt)
        { 
		fprintf (fichier, "|  %s		 ", q->mot);
for (k = 0; k < nbDoc ; k++)
{ 
 for (l = 1; l <= nbDoc && (strcmp(q->t[k].nomDoc, tabFile[l].nomDoc) != 0); l++);
 if (strcmp(q->t[k].nomDoc,tabFile[l].nomDoc) == 0)
 {
 fprintf (fichier, "| 		%d	  ", q->t[k].freq);
 }
}
fprintf(fichier, "|\n-------------------------------------------------\n");
}
}
}
}
//******************************************************************************

int nbrMotSingle(int nbDoc)
{
int i, k;
liste3 *q;
int nbMotSingle = 0;

for(i = 1; i <= 1000; i++){
  if(Ind[i].etat!=0)
    {	 
    for (k = 0; k < nbDoc && (Ind[i].t[k].freq == 1); k++);
    if (k == nbDoc) nbMotSingle++;
    for(q = Ind[i].pointeur; q != NULL; q = q->svt)
        {
		  for (k = 0; k < nbDoc && (q->t[k].freq == 1); k++);
		  if (k == nbDoc) nbMotSingle++;
        }
   }
}
return nbMotSingle;
}

void Construct_File (FILE* fichier)
{
int i;
liste *q;

for(i = 1; i < 1000; i++)
{
  if(TS[i].etat != 0)
    {
     fprintf(fichier, " %s" ,TS[i].mot);
     fprintf(fichier, "  			   %d  " ,TS[i].freq);
     fprintf(fichier, "\n");
    for(q = TS[i].pointeur; q != NULL; q = q->svt)
        {
          fprintf(fichier, " %s" ,q->mot);
          fprintf(fichier, "  			   %d  " ,q->freq);
          fprintf(fichier, "\n");
        }
    }
}
}

int NbMot(char mot[])
{
    return nbMot;
}

void max4Mots (int max[4], char *maxMot[4], int nbDoc)
{
int i, k;
liste3 *q;
max[0] = 0; 
max[1] = 0; 
max[2] = 0; 
max[3] = 0; 
for(i = 1; i <= 1000; i++){
  if(Ind[i].etat!=0)
    {	 
for (k = 0; k < nbDoc ; k++)
{
if (Ind[i].t[k].freq > max[0])
{ max[3] = max[2]; max[2] = max[1]; max[1] = max[0]; max[0] = Ind[i].t[k].freq; 
  maxMot[0] = Ind[i].mot;}
else if (Ind[i].t[k].freq > max[1])
{ max[3] = max[2]; max[2] = max[1]; max[1] = Ind[i].t[k].freq; 
  maxMot[1] = Ind[i].mot;}
else if (Ind[i].t[k].freq > max[2])
{ max[3] = max[2]; max[2] = Ind[i].t[k].freq; 
  maxMot[2] = Ind[i].mot; }
else if (Ind[i].t[k].freq > max[3])
{ max[3] = Ind[i].t[k].freq; maxMot[3] = Ind[i].mot;}
}
    for(q = Ind[i].pointeur; q != NULL; q = q->svt)
        {
		  for (k = 0; k < nbDoc ; k++)
{
if (q->t[k].freq > max[0])
{ max[3] = max[2]; max[2] = max[1]; max[1] = max[0]; max[0] = q->t[k].freq; 
  maxMot[0] = q->mot;}
else if (q->t[k].freq > max[1])
{ max[3] = max[2]; max[2] = max[1]; max[1] = q->t[k].freq; 
  maxMot[1] = q->mot;}
else if (q->t[k].freq > max[2])
{ max[3] = max[2]; max[2] = q->t[k].freq; 
  maxMot[2] = q->mot; }
else if (q->t[k].freq > max[3])
{ max[3] = Ind[i].t[k].freq; maxMot[3] = q->mot;}
}
        }
   }
}
}

//******************************************************************************

void init()
{
int i;
for (i = 0; i < 50; i++)
tab_ref[i] = "";
}

/* fonction d'insertion des références dans le tableau */
void inser_ref(char *ref)
{
int i;
for (i = 0; i < 20 && (strcmp(tab_ref[i], "") != 0); i++);
if (strcmp(tab_ref[i], "") == 0) 
{
tab_ref[i] = (char*) malloc (sizeof(char));
strcpy(tab_ref[i], ref); 
}
}
/* fonction de vérification d'existence de référence dans le tableau */
int verif_ref( char *ref)
{
int i;
for (i = 0; i < 20 && (strcmp(tab_ref[i], ref) != 0); i++);
if (strcmp(tab_ref[i], ref) == 0) 
return 1;
else { if (i == 20) return 0;}
 
}
/* fonction d'affichage de la table de symboles */
void Afficher()
{
   liste *q;
   int i;
   printf("----------------------------------------------------------\n");
   printf("| 		Mot 	 	 | 	type	| 	frequence	|");
   	printf("\n-------------------------------------------------------\n");
   for(i = 1; i <= 1000; i++){
  if(TS[i].etat != 0)
    {
	printf("-------------------------------------------------------\n");
	 printf("|  	%s  		 " ,TS[i].mot);
     printf("|  %s  	 " ,TS[i].type);
     printf("|  %d  	 |" ,TS[i].freq);
    for(q = TS[i].pointeur; q != NULL; q = q->svt)
        {
		  printf("\n-------------------------------------------------\n");
          printf("|  	%s 		  " ,q->mot);
          printf("|  %s  	 " ,q->type);
          printf("|  %d  	 |" ,q->freq);
		  printf("\n-------------------------------------------------\n");
        }
		printf("\n");
		}
}
}
