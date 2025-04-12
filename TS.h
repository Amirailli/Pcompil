#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structures pour TS (Table des symboles)
typedef struct TSNode {
    char NomEntite[20];
    char CodeEntite[20];
    char TypeEntite[20];
    union {
        int valeurInt;
        float valeurFloat;
    } valeur;
    int estInitialise;
   int estConstante;
    struct TSNode* suivant;
} TSNode;

// Structures pour SM (mots-clés et séparateurs)
typedef struct SMNode {
    char NomEntite[20];
    char CodeEntite[20];
    struct SMNode* suivant;
} SMNode;

TSNode* tsHead = NULL;
SMNode* motsClesHead = NULL;
SMNode* separateursHead = NULL;

// Insertion TS
void insererTS(char entite[], char code[]) {
    if (rechercheTS(entite) == -1) {
        TSNode* nouv = (TSNode*)malloc(sizeof(TSNode));
        strcpy(nouv->NomEntite, entite);
        strcpy(nouv->CodeEntite, code);
        strcpy(nouv->TypeEntite, "");
        nouv->estInitialise = 0;
        nouv->estConstante = 0;
        nouv->suivant = tsHead;
        tsHead = nouv;
    }
}

// Insertion Mots-clés
void insererM(char entite[], char code[]) {
    if (rechercheM(entite) == -1) {
        SMNode* nouv = (SMNode*)malloc(sizeof(SMNode));
        strcpy(nouv->NomEntite, entite);
        strcpy(nouv->CodeEntite, code);
        nouv->suivant = motsClesHead;
        motsClesHead = nouv;
    }
}

// Insertion Séparateurs
void insererS(char entite[], char code[]) {
    if (rechercheS(entite) == -1) {
        SMNode* nouv = (SMNode*)malloc(sizeof(SMNode));
        strcpy(nouv->NomEntite, entite);
        strcpy(nouv->CodeEntite, code);
        nouv->suivant = separateursHead;
        separateursHead = nouv;
    }
}

// Recherche TS (retourne 1 si trouvé, -1 sinon)
int rechercheTS(char entite[]) {
    TSNode* courant = tsHead;
    while (courant != NULL) {
        if (strcmp(courant->NomEntite, entite) == 0)
            return 1;
        courant = courant->suivant;
    }
    return -1;
}

// Recherche Mots-clés
int rechercheM(char entite[]) {
    SMNode* courant = motsClesHead;
    while (courant != NULL) {
        if (strcmp(courant->NomEntite, entite) == 0)
            return 1;
        courant = courant->suivant;
    }
    return -1;
}

// Recherche Séparateurs
int rechercheS(char entite[]) {
    SMNode* courant = separateursHead;
    while (courant != NULL) {
        if (strcmp(courant->NomEntite, entite) == 0)
            return 1;
        courant = courant->suivant;
    }
    return -1;
}

// RechercheType
int rechercheType(char entite[]) {
    TSNode* courant = tsHead;
    while (courant != NULL) {
        if (strcmp(courant->NomEntite, entite) == 0) {
            return strcmp(courant->TypeEntite, "") == 0 ? 0 : 1;
        }
        courant = courant->suivant;
    }
    return -1;
}

// Insertion type
void insererType(char entite[], char type[]) {
    TSNode* courant = tsHead;
    while (courant != NULL) {
        if (strcmp(courant->NomEntite, entite) == 0) {
            strcpy(courant->TypeEntite, type);
            printf("le type de %s est %s\n", entite, type);
            return;
        }
        courant = courant->suivant;
    }
}



int estConst(char* idf) {
    TSNode* courant = tsHead;
    while (courant != NULL) {
        if (strcmp(courant->NomEntite, idf) == 0) {

            return courant->estConstante;
        }
        courant = courant->suivant;
    }
    return 0;
}

void marquerCommeConstante(char* entite) {
    TSNode* courant = tsHead;
    while (courant != NULL) {
        if (strcmp(courant->NomEntite, entite) == 0) {
            courant->estConstante = 1;  // Marque comme constante
            return;
        }
        courant = courant->suivant;
    }
}

float obtenirValeurFloat(char* idf);
int obtenirValeurInt(char* idf);

float getValeur(char* idf) {
    TSNode* courant = tsHead;
    while (courant != NULL) {
        if (strcmp(courant->NomEntite, idf) == 0) {
            if (strcmp(courant->TypeEntite, "int") == 0)
                return (float)obtenirValeurInt(idf);
            else if (strcmp(courant->TypeEntite, "float") == 0)
                return obtenirValeurFloat(idf);
            else
                return 0.0f;
        }
        courant = courant->suivant;
    }
    printf("Erreur sémantique : Variable '%s' non déclarée\n", idf);
    return 0.0f;
}

int variable_declaree(char *var) {
    return rechercheType(var);
}

int obtenirValeurInt(char* idf) {
    TSNode* courant = tsHead;
    while (courant != NULL) {
        if (strcmp(courant->NomEntite, idf) == 0) {
            if (strcmp(courant->TypeEntite, "int") == 0) {
                if (courant->estInitialise)
                    return courant->valeur.valeurInt;
                else {
                    printf("Erreur : la variable entière '%s' n'est pas initialisée.\n", idf);
                    exit(EXIT_FAILURE);
                }
            } else {
                printf("Erreur : la variable '%s' n'est pas de type entier.\n", idf);
                exit(EXIT_FAILURE);
            }
        }
        courant = courant->suivant;
    }
    printf("Erreur : variable '%s' non déclarée.\n", idf);
    exit(EXIT_FAILURE);
}

float obtenirValeurFloat(char* idf) {
    TSNode* courant = tsHead;
    while (courant != NULL) {
        if (strcmp(courant->NomEntite, idf) == 0) {
            if (strcmp(courant->TypeEntite, "float") == 0) {
                if (courant->estInitialise)
                    return courant->valeur.valeurFloat;
                else {
                    printf("Erreur : la variable flottante '%s' n'est pas initialisée.\n", idf);
                    exit(EXIT_FAILURE);
                }
            } else {
                printf("Erreur : la variable '%s' n'est pas de type flottant.\n", idf);
                exit(EXIT_FAILURE);
            }
        }
        courant = courant->suivant;
    }
    printf("Erreur : variable '%s' non déclarée.\n", idf);
    exit(EXIT_FAILURE);
}

// Fonctions récursives pour l'affichage inversé
void afficherTSRecursif(TSNode* node) {
    if (node == NULL) return;
    afficherTSRecursif(node->suivant);
    printf("\t|%10s |%12s |%12s |\n", node->NomEntite, node->CodeEntite, node->TypeEntite);
}

void afficherMRecursif(SMNode* node) {
    if (node == NULL) return;
    afficherMRecursif(node->suivant);
    printf("\t|%10s |%12s  |\n", node->NomEntite, node->CodeEntite);
}

void afficherSRecursif(SMNode* node) {
    if (node == NULL) return;
    afficherSRecursif(node->suivant);
    printf("\t|%10s |%12s  |\n", node->NomEntite, node->CodeEntite);
}

// Affichage TS
void afficherTS() {
    printf("\n/*************** Table des symboles (TS) ******************/\n");
    printf("_________________________________________________\n");
    printf("\t| NomEntite |  CodeEntite  | TypeEntite |\n");
    printf("_________________________________________________\n");
    afficherTSRecursif(tsHead);
}

// Affichage Mots-clés
void afficherM() {
    printf("\n/*************** Table des mots-clés ******************/\n");
    printf("____________________________________\n");
    printf("\t| NomEntite |  CodeEntite  |\n");
    printf("____________________________________\n");
    afficherMRecursif(motsClesHead);
}

// Affichage Séparateurs
void afficherS() {
    printf("\n/*************** Table des séparateurs ******************/\n");
    printf("____________________________________\n");
    printf("\t| NomEntite |  CodeEntite  |\n");
    printf("____________________________________\n");
    afficherSRecursif(separateursHead);
}

// Libération mémoire
void libererMemoire() {
    TSNode* tmp;
    while (tsHead) {
        tmp = tsHead;
        tsHead = tsHead->suivant;
        free(tmp);
    }

    while (motsClesHead) {
        tmp = (TSNode*)motsClesHead;
        motsClesHead = motsClesHead->suivant;
        free(tmp);
    }

    while (separateursHead) {
        tmp = (TSNode*)separateursHead;
        separateursHead = separateursHead->suivant;
        free(tmp);
    }
}
