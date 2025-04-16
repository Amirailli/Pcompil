%{
    #include <stdio.h>
    int nb_ligne = 1;
    int nb_colonne = 1;
    char SauvType[20];
    // ajouter le tableau et le compteur
    char liste_idfs[100][20];
    int nb_idfs = 0;
    extern int estConst(char* idf); 
    char currentVarType[20];   // Type de la variable en cours d'affectation
    char currentExprType[20];  // Type de l'expression courante

%}

%start Program

%union {
    int entier;
    char* str;
    float flottant ;
}


%token MAINPRGM VAR BEGINPG ENDPG LET ATDEF CONST INPUT OUTPUT
%token <str> TYPEINT TYPEFLOAT 
%token <entier> ENTIER ENTIERSIGNE ENTIERERROR
%token <flottant> FLOAT

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
%type <flottant> expression 
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
    {
      int i;
      for (i = 0; i < nb_idfs; i++) {
        if (rechercheType(liste_idfs[i]) == 0) {
            insererType(liste_idfs[i], SauvType);
        } else {
            printf("Erreur Semantique: double declaration de %s, ligne %d\n", liste_idfs[i], nb_ligne);
        }
      }
      nb_idfs = 0; // reset la liste temporaire
    }
;

type:
    TYPEINT {strcpy(SauvType,$1);}
    | TYPEFLOAT {strcpy(SauvType,$1);}
;

liste_idf:
    liste_idf VRG IDF  { 
                             strcpy(liste_idfs[nb_idfs], $3);
                             nb_idfs++;
                       }
    |IDF  {      
                 strcpy(liste_idfs[nb_idfs], $1);
                 nb_idfs++;
         }
   ;


declaration_tableau:
    LET liste_idf DEUXPOINT CROCHETOUVERT TYPEINT PVG ENTIER CROCHETFERME PVG
    {  
        int i;
      for (i = 0; i < nb_idfs; i++) {
        if (rechercheType(liste_idfs[i]) == 0) {
            insererType(liste_idfs[i], SauvType);
        } else {
            printf("Erreur Semantique: double declaration de %s, ligne %d\n", liste_idfs[i], nb_ligne);
        }
      }
      nb_idfs = 0; 
      if($7 <= 0){printf("ERROR,Taille du tableau = %d\n", $7);}
    }
;

declaration_constantes:
    ATDEF CONST IDF DEUXPOINT type EGAL valeur_const PVG {
        if (rechercheType($3) == 0) {
            insererType($3, SauvType); 
            marquerCommeConstante($3); 
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
                                          printf("la valeur est %f\n" , $3);
                                           if (!variable_declaree($1)) {
                                          printf("Erreur semantique : Variable '%s' non declaree a la ligne %d\n", $1, nb_ligne);
         
                                              }else if (estConst($1)) {
                                          printf("Erreur semantique : Tentative de modification de la constante '%s' à la ligne %d\n", $1, nb_ligne);
                                           }
                                           if (rechercheType($1) == 0){ 
                                            printf("Erreur semantique: %s non declare a la ligne %d\n", $1, nb_ligne);

                                            }
                                            // Vérification type
                                            else {
                                             strcpy(currentVarType, obtenirTypeVariable($1));
            
                                       // Conversion autorisée : int -> float
                                     if (strcmp(currentVarType, "Float") == 0 && strcmp(currentExprType, "Int") == 0) {
                                      // Conversion implicite autorisée
                                     }
                                 // Conversion interdite : float -> int
                                else if (strcmp(currentVarType, "Int") == 0 && strcmp(currentExprType, "Float") == 0) {
                               printf("Erreur semantique non compatibilite du type (ligne %d): Conversion float->int impossible pour '%s'\n", 
                                   nb_ligne, $1);
                                    }
                                
                                           }
                                            

                                         }
            | IDF CROCHETOUVERT expression CROCHETFERME AFFECTATION expression
                                  {
                                   if (rechercheType($1) == 0) {printf("Erreur semantique: %s non declare a la ligne %d\n", $1, nb_ligne);}
                                   if (estConst($1)) {
                                          printf("Erreur semantique : Tentative de modification de la constante '%s' à la ligne %d\n", $1, nb_ligne);
                                           }
                                  }
            ;

condition : IF PARENTHESEOUVERT expression_logique PARENTHESEFERME THEN 
            OUVEREBLOC instructions FERMETBLOC ELSE OUVEREBLOC instructions FERMETBLOC
            ;
expression_logique : expression comparaison expression
                   | expression_logique AND expression_logique
                   | expression_logique OR expression_logique
                   | NOT expression_logique
                   | PARENTHESEOUVERT expression_logique PARENTHESEFERME
                   | expression  
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

boucle_do : DO OUVEREBLOC instructions FERMETBLOC WHILE PARENTHESEOUVERT expression_logique PARENTHESEFERME PVG
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

contenu : CHAINE VRG VAR_IDF
        | CHAINE VRG IDF
        ;

expression : expression PLUS expression {
     $$ = $1 + $3; 
     // Détermination du type résultat
        if (strcmp(currentExprType, "Float") == 0 || strcmp(currentExprType + 2, "Float") == 0)
            strcpy(currentExprType, "Float");
        else
            strcpy(currentExprType, "Int");
}
            | expression MINUS expression { $$ = $1 - $3; }
            | expression TIMES expression { $$ = $1 * $3; }
            | expression DIV expression {
                  if ($3 == 0 ) {
                      printf("Erreur semantique : division par zero a la ligne %d\n" , nb_ligne);
                      $$ = 0;
                  } else {
                      $$ = $1 / $3;
                  }
              }
            | PARENTHESEOUVERT expression PARENTHESEFERME { $$ = $2; }
            | IDF { $$ = getValeur($1);
                    if (rechercheType($1) != 0)
            strcpy(currentExprType, obtenirTypeVariable($1));
        else
            strcpy(currentExprType, "erreur");
            }
            | ENTIERSIGNE { $$ = $1; 
            }
            | FLOAT { $$ = $1; 
                       strcpy(currentExprType, "Float");
                     
            }
            | ENTIER { $$ = $1; 
                    strcpy(currentExprType, "Int");
            }
            ;
%%

int main() {

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
