#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef union {
    int valeurInt;
    float valeurFloat;
} Valeur;
// Structures pour TS (Table des symboles)
typedef struct TSNode {
    char NomEntite[20];
    char CodeEntite[20];
    char TypeEntite[20];  
    // union {
    //     int valeurInt;
    //     float valeurFloat;
    // } valeur;
    Valeur valeur;
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
//  fonction pour obtenir le type
const char* obtenirTypeVariable(char* entite) {
    TSNode* courant = tsHead;
    while (courant != NULL) {
        if (strcmp(courant->NomEntite, entite) == 0) {
            return courant->TypeEntite;
        }
        courant = courant->suivant;
    }
    return "non_declare";
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

// float getValeur(char* idf) {
//     TSNode* courant = tsHead;
//     while (courant != NULL) {
//         if (strcmp(courant->NomEntite, idf) == 0) {
//             if (strcmp(courant->TypeEntite, "int") == 0)
//                 return (float)obtenirValeurInt(idf);
//             else if (strcmp(courant->TypeEntite, "float") == 0)
//                 return obtenirValeurFloat(idf);
//             else
//                 return 0.0f;
//         }
//         courant = courant->suivant;
//     }
//     printf("Erreur sémantique : Variable '%s' non déclarée\n", idf);
//     return 0.0f;
// }
float getValeur(char* idf) {
    TSNode* courant = tsHead;
    while (courant != NULL) {
        if (strcmp(courant->NomEntite, idf) == 0) {
            if (strcmp(courant->TypeEntite, "Int") == 0) {
                return (float)courant->valeur.valeurInt;
            } else if (strcmp(courant->TypeEntite, "Float") == 0) {
                return courant->valeur.valeurFloat;
            }
        }
        courant = courant->suivant;
    }
    printf("Erreur semantique : Variable '%s' non declaree\n", idf);
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


// TSNode* trouverTS(char* nomEntite) {
//     TSNode* courant = tsHead;
//     while (courant != NULL) {
//         if (strcmp(courant->NomEntite, nomEntite) == 0)
//             return courant;
//         courant = courant->suivant;
//     }
//     return NULL;
// }

// void update_value(char* nomEntite, Valeur valeur) {
//     TSNode* node = trouverTS(nomEntite);
//     if (node != NULL) {
//         if (strcmp(node->TypeEntite, "Int") == 0) {
//             node->valeur.valeurInt = valeur.valeurInt;
//         } else if (strcmp(node->TypeEntite, "Float") == 0) {
//             node->valeur.valeurFloat = valeur.valeurFloat;
//         }
//     }

// }


// void afficherTS() {
//     printf("\n/*************** Table des symboles (TS) ******************/\n");
//     printf("__________________________________________________________________\n");
//     printf("| %-12s | %-14s | %-12s | %-10s |\n", "NomEntite", "CodeEntite", "TypeEntite", "valEntitie");
//     printf("__________________________________________________________________\n");
    
//     TSNode* current = tsHead;
//     // Parcours dans l'ordre inverse (pour afficher comme dans l'exemple)
//     // On va d'abord compter le nombre d'éléments
//     int count = 0;
//     int i, j;  // Déclaration des variables de boucle avant (pour compatibilité pre-C99)
//     TSNode* temp = current;
//     while (temp != NULL) {
//         count++;
//         temp = temp->suivant;
//     }
    
//     // On parcourt en partant de la fin
//     for (i = count - 1; i >= 0; i--) {
//         temp = current;
//         for (j = 0; j < i; j++) {
//             temp = temp->suivant;
//         }
        
//         char valueStr[20];
//         if (strcmp(temp->TypeEntite, "int") == 0) {
//             sprintf(valueStr, "%d", temp->valeur.valeurInt);
//         } else if (strcmp(temp->TypeEntite, "float") == 0) {
//             sprintf(valueStr, "%.2f", temp->valeur.valeurFloat);
//         } else {
//             strcpy(valueStr, "N/A");
//         }
        
//         printf("| %-12s | %-14s | %-12s | %-10s |\n",
//                temp->NomEntite,
//                temp->CodeEntite,
//                temp->TypeEntite,
//                valueStr);
//     }
//     printf("__________________________________________________________________\n");
// }
void afficherTS() {
    printf("\n/*************** Table des symboles (TS) ******************/\n");
    printf("__________________________________________________________________\n");
    printf("| %-12s | %-14s | %-12s | %-10s |\n", "NomEntite", "CodeEntite", "TypeEntite", "valEntitie");
    printf("__________________________________________________________________\n");

    TSNode* courant = tsHead;
    while (courant != NULL) {
        char valeurStr[20];

        // Si la variable est de type int ou float et non initialisée
        if (!courant->estInitialise &&
            (strcmp(courant->TypeEntite, "int") == 0 || strcmp(courant->TypeEntite, "float") == 0)) {

            printf(">> Entrez la valeur de '%s' (%s) : ", courant->NomEntite, courant->TypeEntite);
            if (strcmp(courant->TypeEntite, "int") == 0) {
                int val;
                scanf("%d", &val);
                courant->valeur.valeurInt = val;
            } else {
                float val;
                scanf("%f", &val);
                courant->valeur.valeurFloat = val;
            }
            courant->estInitialise = 1;
        }

        if (strcmp(courant->TypeEntite, "int") == 0)
            sprintf(valeurStr, "%d", courant->valeur.valeurInt);
        else if (strcmp(courant->TypeEntite, "float") == 0)
            sprintf(valeurStr, "%.2f", courant->valeur.valeurFloat);
        else
            strcpy(valeurStr, "N/A");

        printf("| %-12s | %-14s | %-12s | %-10s |\n",
               courant->NomEntite,
               courant->CodeEntite,
               courant->TypeEntite,
               valeurStr);

        courant = courant->suivant;
    }

    printf("__________________________________________________________________\n");
}

// void afficherTS() {
//     printf("\n/*************** Table des symboles (TS) ******************/\n");
//     printf("__________________________________________________________________\n");
//     printf("| %-12s | %-14s | %-12s | %-10s |\n", "NomEntite", "CodeEntite", "TypeEntite", "Valeur");
//     printf("__________________________________________________________________\n");

//     TSNode* courant = tsHead;

//     while (courant != NULL) {
//         char valeurStr[20];

//         if (strcmp(courant->TypeEntite, "int") == 0) {
//             if (!courant->estInitialise) {
//                 int val;
//                 printf("Entrez la valeur entière pour %s : ", courant->NomEntite);
//                 scanf("%d", &val);
//                 courant->valeur.valeurInt = val;
//                 courant->estInitialise = 1;
//             }
//             sprintf(valeurStr, "%d", courant->valeur.valeurInt);
//         } else if (strcmp(courant->TypeEntite, "float") == 0) {
//             if (!courant->estInitialise) {
//                 float val;
//                 printf("Entrez la valeur flottante pour %s : ", courant->NomEntite);
//                 scanf("%f", &val);
//                 courant->valeur.valeurFloat = val;
//                 courant->estInitialise = 1;
//             }
//             sprintf(valeurStr, "%.2f", courant->valeur.valeurFloat);
//         } else {
//             strcpy(valeurStr, "N/A");
//         }

//         printf("| %-12s | %-14s | %-12s | %-10s |\n",
//                courant->NomEntite,
//                courant->CodeEntite,
//                courant->TypeEntite,
//                valeurStr);

//         courant = courant->suivant;
//     }

//     printf("__________________________________________________________________\n");
// }


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
