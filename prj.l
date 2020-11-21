%{
#include<stdio.h>
#include "bs.tab.h"
int ligne = 1;
int colonne = 0;
extern YYSTYPE yylval;
%}
chiffre [1-9][0-9]*
chiffrebis [0-9]+
entier {chiffre}+|{chiffrebis}+
lettre [a-zA-Z]
lettrebis [a-zA-Z]+[0-9]*
saut [\n]
mot {lettre}+|{lettrebis}+|{lettre}+"-"{lettre}+|{lettrebis}+"-"{lettrebis}+
title "TITLE" 
authors "AUTHORS" 
abstract "ABSTRACT"
keywords "KEYWORDS" 
intro "INTRODUCTION" 
works "RELATEDWORKS"
concept "CONCEPTION"
result "EXPERIMENTALRESULTS"
conclusion "CONCLUSION"
refer "REFERENCES"
tabulation [\t]
croch_ouvrt "["
croch_fermt "]"
point "."
virgule ","
espace " "+
ref {croch_ouvrt}{chiffre}{croch_fermt}
ch {chiffre}
%%
{title} {colonne+=yyleng; 
         insertion(yytext, "Mot cle");
		 return TITLE;}
{authors} {colonne+=yyleng; 
           insertion(yytext, "Mot cle");
		   return AUTHORS;}
{abstract} {colonne+=yyleng; 
            insertion(yytext, "Mot cle");
			return ABSTRACT;}
{keywords} {colonne+=yyleng; 
            insertion(yytext, "Mot cle");
			return KEYWORDS;} 
{intro} {colonne+=yyleng; 
         insertion(yytext, "Mot cle");
		 return INTRODUCTION;}
{works} {colonne+=yyleng; 
         insertion(yytext, "Mot cle");
		 return RELATEDWORKS;}
{concept} {colonne+=yyleng; 
           insertion(yytext, "Mot cle");
		   return CONCEPTION;}
{result} {colonne+=yyleng; 
          insertion(yytext, "Mot cle");
		  return EXPERIMENTALRESULTS;}
{conclusion} {colonne+=yyleng; 
              insertion(yytext, "Mot cle");
			  return CONCLUSION;}
{refer} {colonne+=yyleng; 
         insertion(yytext, "Mot cle");
		 return REFERENCES;}
{entier} {colonne += yyleng; 
          yylval.entier = atoi(yytext);	  
		  return ENTIER;}
{mot} {colonne += yyleng; 
       yylval.chaine = yytext; 
	   insertion(yytext, "Mot");
	   return MOT;}
{espace} {colonne += yyleng; return ESPACE;}
{point} {colonne += yyleng; return POINT;}
{saut} {ligne++; colonne = 0; return SAUT;}
{croch_ouvrt} {colonne+=yyleng; return CROCH_OUVRT;}
{croch_fermt} {colonne+=yyleng; return CROCH_FERMT;}
{tabulation} {colonne+=yyleng; return TABULATION;}
{ref} {colonne+=yyleng;
       yylval.chaine = yytext; 
       insertion(yytext, "Reference");
	   return REF;}	  

{virgule} {colonne+=yyleng; 
		   return VIRGULE;}
. { printf("Erreur à la ligne %d la colonne %d \n", ligne , colonne ); colonne += yyleng;}


%%
int yywrap(){
return 1;
}
