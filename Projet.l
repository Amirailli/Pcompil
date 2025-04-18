%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "Projet.tab.h"
    #include "TS.h"
    extern int nb_ligne ;
    extern int  nb_colonne ;
    extern YYSTYPE yylval; 

    void afficher_erreur(const char *type, const char *entite, const char *message) {
        printf("ERREUR : %s a la ligne %d, colonne %d : %s (%s)\n", 
               type, nb_ligne, nb_colonne, message, entite);
    }
%}


lettre [a-z]
chiffre [0-9]
entier  {chiffre}+
entier_signe \([+-]{entier})\

flottant {chiffre}+.{chiffre}+
flottant_signe \([+-]{chiffre}+.{chiffre}+)\

IDF [a-zA-Z]({lettre}|{chiffre}|_)*
ERREUR_NUM_IDENT {chiffre}+({lettre}|{chiffre}|_)*
ERREUR_UNDERS_IDENT _({lettre}|{chiffre}|_)*

%%

\{--(.|\n)*?--\}   
"<!-"([^-]|-[^!])*"-!>"


"MainPrgm"      { printf("Mot-cle detecte : MainPrgm\n"); insererM(yytext, "MAINPRGM"); nb_colonne += yyleng; return MAINPRGM; }
"Var"           { printf("Mot-cle detecte : Var\n"); insererM(yytext, "VAR"); nb_colonne += yyleng; return VAR; }
"BeginPg"       { printf("Mot-cle detecte : BeginPg\n"); insererM(yytext, "BEGINPG"); nb_colonne += yyleng; return BEGINPG; }
"EndPg"         { printf("Mot-cle detecte : EndPg\n"); insererM(yytext, "ENDPG"); nb_colonne += yyleng; return ENDPG; } 
"let"           { printf("Mot-cle detecte : let\n"); insererM(yytext, "LET"); nb_colonne += yyleng;  return LET; }
"@define"       { printf("Mot-cle detecte : @define\n"); insererM(yytext, "ATDEF"); nb_colonne += yyleng; return ATDEF; }
"Const"         { printf("Mot-cle detecte : Const\n"); insererM(yytext, "CONST"); nb_colonne += yyleng; return CONST; }
"input"         { printf("Mot-cle detecte : input\n"); insererM(yytext, "INPUT"); nb_colonne += yyleng; return INPUT; }
"output"        { printf("Mot-cle detecte : output\n"); insererM(yytext, "OUTPUT"); nb_colonne += yyleng;  return OUTPUT; }

"Int"      { printf("Type Int detecte : %s\n", yytext); insererM(yytext, "TYPEINT");yylval.str=strdup(yytext); nb_colonne += yyleng; return TYPEINT; }
"Float"     { printf("Type Float detecte : %s\n", yytext); insererM(yytext, "TYPEFLOAT");yylval.str=strdup(yytext); nb_colonne += yyleng; return TYPEFLOAT; }

{entier}  { 
                yylval.entier = atoi(yytext); 
                if (yylval.entier < -32768 || yylval.entier > 32767) {
                    afficher_erreur("Valeur entiere hors limite", yytext, "Nombre trop grand ou trop petit");
                }else {
                   printf("Entier : %s\n", yytext);
                   nb_colonne += yyleng;
                   return ENTIER;}
            }

{entier_signe} { 
                    char copie[100];
                    strncpy(copie, yytext + 1, yyleng - 2);
                    copie[yyleng - 2] = '\0';
                    yylval.entier = atoi(copie);
                    if (yylval.entier < -32768 || yylval.entier > 32767) {
                        afficher_erreur("Valeur entiere hors limite", yytext, "Nombre trop grand ou trop petit");
                    }else {
                        printf("Entier signe : %s\n", yytext);
                        nb_colonne += yyleng;
                        return ENTIERSIGNE;}
                } 

[+-]{chiffre}+ { 
    afficher_erreur("Syntaxe invalide", yytext, "Un entier signe doit etre entre parentheses");  
}
[+-]{chiffre}+.{chiffre}+ { 
    afficher_erreur("Syntaxe invalide", yytext, "Un flottant signe doit etre entre parentheses");
}

{flottant}   { printf("float non signe %s\n", yytext);nb_colonne += yyleng;yylval.flottant = atof(yytext); return FLOAT; }

{flottant_signe} { printf("float signe %s\n", yytext); yylval.flottant = atof(yytext); nb_colonne += yyleng; return FLOAT;  }

"if"      { printf("Mot-cle : %s\n", yytext); insererM(yytext, "IF"); nb_colonne += yyleng;return IF;}
"then"    { printf("Mot-cle : %s\n", yytext);insererM(yytext, "THEN"); nb_colonne += yyleng;return THEN ;}
"else"    { printf("Mot-cle : %s\n", yytext); insererM(yytext,"ELSE");nb_colonne += yyleng;return ELSE ;}
"do"      { printf("Mot-cle : %s\n", yytext);insererM(yytext, "DO"); nb_colonne += yyleng;return DO;}
"while"   { printf("Mot-cle : %s\n", yytext);insererM(yytext, "WHILE"); nb_colonne += yyleng;return WHILE ;}
"for"     { printf("Mot-cle : %s\n", yytext); insererM(yytext, "FOR");nb_colonne += yyleng;return FOR;}
"from"    { printf("Mot-cle : %s\n", yytext); insererM(yytext, "FROM");nb_colonne += yyleng;return FROM;}
"to"      { printf("Mot-cle : %s\n", yytext); insererM(yytext, "TO");nb_colonne += yyleng;return TO;}
"step"    { printf("Mot-cle : %s\n", yytext); insererM(yytext, "STEP");nb_colonne += yyleng;return STEP;}

{ERREUR_NUM_IDENT}  { afficher_erreur("Identificateur invalide", yytext, "Un IDF ne peut pas commencer par un chiffre"); 
                    }
{ERREUR_UNDERS_IDENT} {
                       afficher_erreur("Identificateur invalide", yytext, "Un IDF ne doit pas commencer par '_'");
                      }

"+"   { printf("Operateur arithmetique : %s\n", yytext);insererS(yytext, "PLUS");nb_colonne += yyleng; return PLUS; }
"-"   { printf("Operateur arithmetique : %s\n", yytext);insererS(yytext, "MINUS");nb_colonne += yyleng; return MINUS; }
"*"   { printf("Operateur arithmetique : %s\n", yytext);insererS(yytext, "TIMES");nb_colonne += yyleng; return TIMES; }
"/"   { printf("Operateur arithmetique : %s\n", yytext);insererS(yytext, "DIV");nb_colonne += yyleng; return DIV; }

"AND" { printf("Operateur logique : %s\n", yytext);insererS(yytext, "AND");nb_colonne += yyleng; return AND; }
"OR"  { printf("Operateur logique : %s\n", yytext);insererS(yytext, "OR");nb_colonne += yyleng; return OR; }
"!"   { printf("Operateur NOT : %s\n", yytext);insererS(yytext, "NOT"); nb_colonne += yyleng; return NOT; }

"=="  { printf("Operateur de comparaison : %s\n", yytext);insererS(yytext, "EQ");nb_colonne += yyleng; return EQ; }
"!="  { printf("Operateur de comparaison : %s\n", yytext);insererS(yytext, "NEQ");nb_colonne += yyleng; return NEQ; }
"<="  { printf("Operateur de comparaison : %s\n", yytext);insererS(yytext, "IE");nb_colonne += yyleng; return IE; }
">="  { printf("Operateur de comparaison : %s\n", yytext);insererS(yytext, "SE");nb_colonne += yyleng; return SE; }
"<"   { printf("Operateur de comparaison : %s\n", yytext);insererS(yytext, "I");nb_colonne += yyleng; return I; }
">"   { printf("Operateur de comparaison : %s\n", yytext);insererS(yytext, "S");nb_colonne += yyleng; return S; }

"{"   { printf("Ouverture de bloc\n");insererS(yytext, "OUVEREBLOC");nb_colonne += yyleng; return OUVEREBLOC; }
"}"   { printf("Fermeture de bloc\n");insererS(yytext, "FERMETBLOC");nb_colonne += yyleng; return FERMETBLOC; }
"["   { printf("Crochet ouvrant\n");insererS(yytext, "CROCHETOUVERT");nb_colonne += yyleng; return CROCHETOUVERT; }
"]"   { printf("Crochet fermant\n");insererS(yytext, "CROCHETFERME"); nb_colonne += yyleng; return  CROCHETFERME; }
"("   { printf("Parenthese ouvrante\n");insererS(yytext, "PARENTHESEOUVERT"); nb_colonne += yyleng; return PARENTHESEOUVERT; }
")"   { printf("Parenthese fermante\n");insererS(yytext, "PARENTHESEFERME"); nb_colonne += yyleng; return  PARENTHESEFERME; }

";"   { printf("Point virgule\n");insererS(yytext, "PVG");nb_colonne += yyleng; return PVG; }
":"   { printf("Deux points\n");insererS(yytext, "DEUXPOINT");nb_colonne += yyleng; return DEUXPOINT; }
","   { printf("Virgule\n");insererS(yytext, "VRG");nb_colonne += yyleng; return VRG; }
"="   { printf("Egal\n");insererS(yytext, "EGAL");nb_colonne += yyleng; return EGAL; }
":="  { printf("Affectation\n");insererS(yytext, " AFFECTATION");nb_colonne += yyleng; return AFFECTATION ;}

{lettre}     { printf("Lettre: %s\n", yytext); yylval.str=strdup(yytext);insererTS(yytext, "IDF");nb_colonne += yyleng; return IDF; }


{IDF}       { 
                if (yyleng > 14) {
                     afficher_erreur("Identificateur trop long", yytext, "Un IDF ne peut pas dépasser 14 caracteres");
                }else if (strstr(yytext, "__") != NULL) {
                    afficher_erreur("Identificateur invalide", yytext, "Un IDF ne doit pas contenir '__'");
                }else if (yytext[yyleng - 1] == '_') {
                    afficher_erreur("Identificateur invalide", yytext, "Un IDF ne doit pas se terminer par '_'");
                }else {
                     printf("Identificateur valide : %s\n", yytext); 
                     yylval.str=strdup(yytext);
                     insererTS(yytext, "IDF");
                     nb_colonne += yyleng;
                     return IDF;}
            }  


[a-zA-Z][a-zA-Z0-9]*  {printf("var_idf: %s\n", yytext);return VAR_IDF;}

\"[^\"]*\"  {printf("chaine: %s\n", yytext);return CHAINE; }

[ \t]+      {nb_colonne += yyleng;}
\n          { nb_ligne++,nb_colonne = 1; }
.           { afficher_erreur("Erreur lexicale", yytext, "Caractere inconnu");  
            }

%%