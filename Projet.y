%{
    #include <stdio.h>
    int nb_ligne = 1;
    int nb_colonne = 1;
    char SauvType[20];
%}

%start Program

%union {
    int entier;
    char* str;
    float floatVal;
}

%type <entier> expression

%token MAINPRGM VAR BEGINPG ENDPG LET ATDEF CONST INPUT OUTPUT
%token <str> TYPEINT TYPEFLOAT 
%token <entier> ENTIER ENTIERSIGNE
%token <floatVal> FLOAT 
%token ENTIERERROR
%token <str> IDF 
/* Opérateurs arithmétiques */
%token PLUS MINUS TIMES DIV 

/* Opérateurs logiques */
%token AND OR NOT

/* Opérateurs de comparaison */
%token EQ NEQ IE SE I S

/* Symboles spéciaux */
%token OUVEREBLOC FERMETBLOC CROCHETOUVERT  CROCHETFERME PARENTHESEOUVERT PARENTHESEFERME 
%token PVG  DEUXPOINT VRG GUILLEMET EGAL AFFECTATION
%token ERREUR

%token IF THEN ELSE DO WHILE FOR FROM TO STEP
%token CHAINE VAR_IDF
/* Définition des priorités et associativités */
%left OR
%left AND
%left NOT
%left EQ NEQ IE SE I S
%left PLUS MINUS
%left TIMES DIV

%%

Program:
    MAINPRGM IDF PVG declaration_part BEGINPG OUVEREBLOC instructions FERMETBLOC ENDPG PVG
;

declaration_part:
    VAR declaration_list
;

declaration_list:
    declaration
    | declaration_list declaration
;

declaration:
    declaration_variables
    | declaration_tableau
    | declaration_constantes
;

declaration_variables:
    LET liste_idf DEUXPOINT type PVG
;

liste_idf:
    liste_idf VRG IDF  { 
                            if(rechercheType($3)==0) {
                                insererType($3,SauvType);
                            } else {
                                printf("Erreur Semantique: double declaration de %s, a la ligne %d\n", $3, nb_ligne);
                            } 
                       }
    |IDF  {      
                if (rechercheType($1)==0){
                    insererType($1,SauvType);
                } else {
                    printf("Erreur Semantique: double declaration de %s, a la ligne %d\n", $1, nb_ligne);
                }
         }
   ;

type:
    TYPEINT {strcpy(SauvType,"INT");}
    | TYPEFLOAT {strcpy(SauvType,"FLOAT");}
;

declaration_tableau:
    LET liste_idf  DEUXPOINT CROCHETOUVERT ENTIER CROCHETFERME DEUXPOINT type PVG
;

declaration_constantes:
    ATDEF CONST IDF DEUXPOINT type EGAL valeur_const PVG {
        if (rechercheType($3) == 0) {
            insererType($3, SauvType); 
        } else {
            printf("Erreur Semantique: double declaration de la constante %s, a la ligne %d\n", $3, nb_ligne);
        }
    }
;

valeur_const:
    ENTIER
    | FLOAT
    | ENTIERSIGNE
;

instructions : instructions instruction
             | instruction
             ;

instruction : affectation PVG
            | condition
            | boucle
            | input PVG
            | output PVG
            ;

affectation : IDF AFFECTATION expression {
                                           if (rechercheType($1) == 0) printf("Erreur semantique: %s non declare a la ligne %d\n", $1, nb_ligne);
                                         }
            | IDF CROCHETOUVERT expression CROCHETFERME AFFECTATION expression
                                  {
                                   if (rechercheType($1) == 0) printf("Erreur semantique: %s non declare a la ligne %d\n", $1, nb_ligne);
                                  }
            ;

condition : IF PARENTHESEOUVERT expression_logique PARENTHESEFERME THEN
            OUVEREBLOC instructions FERMETBLOC 
            | IF PARENTHESEOUVERT expression_logique PARENTHESEFERME THEN 
            OUVEREBLOC instructions FERMETBLOC ELSE OUVEREBLOC instructions FERMETBLOC
            ;
expression_logique : expression comparaison expression
                   | expression_logique AND expression_logique
                   | expression_logique OR expression_logique
                   | NOT expression_logique
                   | PARENTHESEOUVERT expression_logique PARENTHESEFERME
                   | expression  // Permet d'accepter directement un booléen (0 ou 1)
                   ;

comparaison : EQ 
            | NEQ
            | IE
            | SE
            | I
            | S
            ;

boucle : boucle_do
       | boucle_for
       ;

boucle_do : DO OUVEREBLOC instructions FERMETBLOC WHILE PARENTHESEOUVERT expression comparaison expression PARENTHESEFERME PVG
          ;

boucle_for : FOR IDF FROM expression TO expression STEP expression 
            OUVEREBLOC instructions FERMETBLOC
              {
                  if (rechercheType($2) == 0) printf("Erreur semantique: %s non declare a la ligne %d\n", $2, nb_ligne);
              }
          ;


input : INPUT PARENTHESEOUVERT VAR_IDF PARENTHESEFERME
      ;

output : OUTPUT PARENTHESEOUVERT contenu PARENTHESEFERME
       ;

contenu : CHAINE
        | VAR_IDF
        | IDF
        | CHAINE VRG VAR_IDF
        | CHAINE VRG IDF
        ;

expression : expression PLUS expression
           | expression MINUS expression
           | expression TIMES expression
           | expression DIV expression {
                                           if ($3 == 0) {
                                               printf("Erreur semantique à la ligne %d : division par 0 \n", nb_ligne);
                                           } else {
                                               $$ = $1 / $3;  // Traitement spécifique pour les entiers
                                           }
                                       }
           | PARENTHESEOUVERT expression PARENTHESEFERME { $$ = $2; }
           | IDF  { $$ = $1; if (rechercheType($1) == 0) printf("Erreur semantique: %s non declare a la ligne %d\n", $1, nb_ligne); }
           | ENTIERSIGNE { $$ = $1; }
           | FLOAT { $$ = $1; }
           | ENTIER { $$ = $1; }
           ;

%%

int main() {
    dynamicInit();

    yyparse();

    afficherTS();        
    afficherM();
    afficherS();

    libererMemoire();
}

int yyerror(char *msg) {
    printf("Erreur syntaxique à la ligne %d, colonne %d : %s\n", nb_ligne, nb_colonne, msg);
    return 1;
}
