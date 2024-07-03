#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char registro[20];
int flag = 1;

// Strutture dati
typedef struct list {
    int data;
    struct list* next;
    struct list* prev;
} list;

typedef struct stazione {
    int key;
    int numeroauto;
    struct stazione* left;
    struct stazione* right;
    struct stazione* root;
    struct stazione* p;
    struct list* list;
} stazione;

// Dichiarazione delle funzioni
void distributore(FILE* file);
stazione* inseriscistazione(FILE *file, stazione *T);
stazione* inseriscimacchina(FILE *file, stazione *T);
stazione* cancellastazione(FILE *file, stazione *T);
stazione* cancellamacchina(FILE *file, stazione *T);
void cercapercorso(FILE *file, stazione *T);
stazione* treeinsert(stazione *T, FILE *file);
int treesearch(FILE *file, stazione *T, int distanza, int numero);
list* inserisciintesta(list* l, int dato);
list* inserisciinordine(list* testa, int n);
list* cancella(list* lista, int num, int *ris);
int treesearch2(FILE *file, stazione *T, int distanza, int numero);
stazione* TREE_MINIMUM(struct stazione* node);
stazione* TREE_DELETE(stazione* T, stazione* z);
void cancellalista(list* testa);
void treewalk2(stazione *T, int start, int stop, int lung, int mat[lung][4], int *i);
void treewalk(stazione *T, int start, int stop, int *lung);
void cancellaalbero(stazione *T);
list* cancellaTesta(list* l, int *j);

// Main
int main() {
    FILE* file;
    file = stdin;

    distributore(file);

    fclose(file);
    return 0;
}

// Gestore del distributore
void distributore(FILE *file) {
    // Creo puntatore all'albero
    struct stazione* T = NULL;

    // Finché non finisce il file eseguo
    while (fscanf(file, "%s", registro) != EOF) {
        flag = 1;
        if (flag) T = inseriscistazione(file, T);
        if (flag) T = inseriscimacchina(file, T);
        if (flag) T = cancellamacchina(file, T);
        if (flag) T = cancellastazione(file, T);
        if (flag) cercapercorso(file, T);
    }
    // cancellaalbero(T);
    return;
}

// Cancella l'intero albero
void cancellaalbero(stazione* T) {
    stazione* cestino = T;

    if (cestino != NULL) {
        cancellaalbero(cestino->left);
        cancellaalbero(cestino->right);
        cancellalista(cestino->list);
        free(cestino);
    }
}

// Inserisci una nuova stazione
stazione* inseriscistazione(FILE *file, stazione *T) {
    if (strcmp(registro, "aggiungi-stazione") != 0) {
        return T;
    } else {
        flag = 0;
        T = treeinsert(T, file);
        return T;
    }
}

// Inserisci una nuova macchina
stazione* inseriscimacchina(FILE *file, stazione *T) {
    if (strcmp(registro, "aggiungi-auto") != 0) {
        return T;
    } else {
        flag = 0;
        int distanza, ris, numero;
        fscanf(file, "%d", &distanza);
        fscanf(file, "%d", &numero);
        ris = treesearch(file, T, distanza, numero);
        printf(ris == 1 ? "aggiunta\n" : "non aggiunta\n");
    }
    return T;
}

// Cancella una macchina
stazione* cancellamacchina(FILE *file, stazione* T) {
    if (strcmp(registro, "rottama-auto") != 0) {
        return T;
    } else {
        flag = 0;
        int distanza, ris, numero;
        fscanf(file, "%d", &distanza);
        fscanf(file, "%d", &numero);
        ris = treesearch2(file, T, distanza, numero);
        printf(ris == 1 ? "rottamata\n" : "non rottamata\n");
    }
    return T;
}

// Cancella una stazione
stazione* cancellastazione(FILE *file, stazione *T) {
    if (strcmp(registro, "demolisci-stazione") != 0) {
        return T;
    } else {
        flag = 0;
        int distcancella;
        stazione* muovi = T;
        fscanf(file, "%d", &distcancella);

        while (muovi != NULL) {
            if (muovi->key == distcancella) {
                T = TREE_DELETE(T, muovi);
                printf("demolita\n");
                return T;
            }
            muovi = (muovi->key > distcancella) ? muovi->left : muovi->right;
        }
        printf("non demolita\n");
        return T;
    }
}

// Pianifica il percorso
void cercapercorso(FILE *file, stazione *T) {
    if (strcmp(registro, "pianifica-percorso") != 0) {
        return;
    } else {
        int start, stop, lung = 0, i = 0;
        flag = 0;
        fscanf(file, "%d", &start);
        fscanf(file, "%d", &stop);

        if (start == stop) {
            printf("%d\n", start);
            return;
        } else {
            treewalk(T, start, stop, &lung);

            int mat[lung][4];
            treewalk2(T, start, stop, lung, mat, &i);

            if (start < stop) {
                list* memoria = NULL;
                list* coda = NULL;
                int trovato = 0, j = lung - 1, k, confronto;

                coda = inserisciintesta(coda, j);

                while (trovato == 0) {
                    coda = cancellaTesta(coda, &j);
                    memoria = inserisciintesta(memoria, mat[j][0]);
                    k = j;
                    confronto = memoria->data;

                    while (k >= 0) {
                        if (mat[k][1] >= (mat[j][0] - mat[k][0])) {
                            coda = inserisciintesta(coda, k);
                        }
                        k--;
                    }

                    if (mat[(coda->data)][0] == confronto) {
                        printf("nessun percorso\n");
                        return;
                    }
                    if (coda->data == 0) {
                        trovato = 1;
                        coda = cancellaTesta(coda, &j);
                        memoria = inserisciintesta(memoria, mat[j][0]);
                    }
                }

                while (memoria != NULL) {
                    if (memoria->next == NULL) {
                        printf("%d", memoria->data);
                    } else {
                        printf("%d ", memoria->data);
                    }
                    memoria = memoria->next;
                }
                printf("\n");
                return;
            } else {
                list* memoria = NULL;
                list* coda = NULL;
                int trovato = 0, j = 0, k, confronto, cont = 0;

                coda = inserisciintesta(coda, j);

                while (trovato == 0) {
                    coda = cancellaTesta(coda, &j);
                    memoria = inserisciintesta(memoria, mat[j][0]);
                    cont++;
                    k = j;
                    confronto = memoria->data;

                    while (k <= lung - 1) {
                        if (mat[k][1] >= (mat[k][0] - mat[j][0])) {
                            coda = inserisciintesta(coda, k);
                        }
                        k++;
                    }

                    if (mat[(coda->data)][0] == confronto) {
                        printf("nessun percorso\n");
                        return;
                    }
                    if (coda->data == lung - 1) {
                        trovato = 1;
                        coda = cancellaTesta(coda, &j);
                        memoria = inserisciintesta(memoria, mat[j][0]);
                        cont++;
                    }
                }

                int matrice2[cont][3], p, m = 0;
                while (memoria != NULL) {
                    for (p = lung - 1; p >= 0; p--) {
                        if (memoria->data == mat[p][0]) {
                            matrice2[m][0] = mat[p][0];
                            matrice2[m][1] = mat[p][1];
                            matrice2[m][2] = mat[p][2];
                        }
                    }
                    m++;
                    memoria = memoria->next;
                }

                int n, h, u, flag2 = 1;
                while (flag2 == 1) {
                    flag2 = 0;
                    for (n = cont - 2; n > 0; n--) {
                        h = matrice2[n][2];
                        if (h == 1) {
                            u = matrice2[n - 1][1] - (matrice2[n][0] - matrice2[n - 1][0]);
                            if (u < 0) {
                                matrice2[n - 1][2] = 1;
                                flag2 = 1;
                            }
                        }
                    }
                }

                int flag3 = 0;
                for (n = 0; n < cont; n++) {
                    if (matrice2[n][2] != 1) {
                        if (flag3 != 0) {
                            printf(" ");
                        }
                        printf("%d", matrice2[n][0]);
                        flag3 = 1;
                    }
                }
                printf("\n");
            }
        }
    }
}

// Funzione di ricerca
int treesearch(FILE *file, stazione *T, int distanza, int numero) {
    stazione* cercato = T;

    while (cercato != NULL) {
        if (cercato->key == distanza) {
            cercato->list = inserisciinordine(cercato->list, numero);
            if (cercato->list != NULL) {
                cercato->numeroauto++;
                return 1;
            }
        }
        cercato = (cercato->key > distanza) ? cercato->left : cercato->right;
    }
    return 0;
}

// Funzione di ricerca alternativa
int treesearch2(FILE *file, stazione *T, int distanza, int numero) {
    stazione* cercato = T;

    while (cercato != NULL) {
        if (cercato->key == distanza) {
            int ris;
            cercato->list = cancella(cercato->list, numero, &ris);
            if (ris == 1) {
                cercato->numeroauto--;
                return 1;
            }
        }
        cercato = (cercato->key > distanza) ? cercato->left : cercato->right;
    }
    return 0;
}

// Inserisce in testa alla lista
list* inserisciintesta(list* l, int dato) {
    list* temp = (list*)malloc(sizeof(list));
    if (temp == NULL) {
        exit(1);
    }

    temp->data = dato;
    temp->next = l;
    temp->prev = NULL;

    if (l != NULL) {
        l->prev = temp;
    }

    return temp;
}

// Inserisce in ordine nella lista
list* inserisciinordine(list* testa, int n) {
    list* nuovo = malloc(sizeof(list));
    if (nuovo == NULL) {
        exit(1);
    }
    nuovo->data = n;

    if (testa == NULL || testa->data >= n) {
        nuovo->next = testa;
        if (testa != NULL) {
            testa->prev = nuovo;
        }
        nuovo->prev = NULL;
        return nuovo;
    }

    list* corrente = testa;
    while (corrente->next != NULL && corrente->next->data < n) {
        corrente = corrente->next;
    }

    nuovo->next = corrente->next;
    if (corrente->next != NULL) {
        corrente->next->prev = nuovo;
    }
    corrente->next = nuovo;
    nuovo->prev = corrente;

    return testa;
}

// Cancella elemento dalla lista
list* cancella(list* lista, int num, int* ris) {
    list* current = lista;

    while (current != NULL) {
        if (current->data == num) {
            if (current->prev != NULL) {
                current->prev->next = current->next;
            } else {
                lista = current->next;
            }

            if (current->next != NULL) {
                current->next->prev = current->prev;
            }

            free(current);
            *ris = 1;
            return lista;
        }
        current = current->next;
    }

    *ris = 0;
    return lista;
}

// Cancella la testa della lista
list* cancellaTesta(list* l, int* j) {
    list* temp = l;

    if (temp != NULL) {
        *j = temp->data;
        l = l->next;
        if (l != NULL) {
            l->prev = NULL;
        }
        free(temp);
    }

    return l;
}

// Inserisce una nuova stazione nell'albero
stazione* treeinsert(stazione* T, FILE* file) {
    stazione* z = (stazione*)malloc(sizeof(stazione));
    if (z == NULL) {
        exit(1);
    }

    fscanf(file, "%d", &z->key);
    fscanf(file, "%d", &z->numeroauto);
    z->left = z->right = z->p = NULL;
    z->root = T;
    z->list = NULL;

    if (T == NULL) {
        return z;
    }

    stazione* y = NULL;
    stazione* x = T;

    while (x != NULL) {
        y = x;
        x = (z->key < x->key) ? x->left : x->right;
    }

    z->p = y;
    if (z->key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }

    return T;
}

// Cancella nodo dall'albero
stazione* TREE_DELETE(stazione* T, stazione* z) {
    stazione* y;
    stazione* x;

    if (z->left == NULL || z->right == NULL) {
        y = z;
    } else {
        y = TREE_MINIMUM(z->right);
    }

    if (y->left != NULL) {
        x = y->left;
    } else {
        x = y->right;
    }

    if (x != NULL) {
        x->p = y->p;
    }

    if (y->p == NULL) {
        T = x;
    } else if (y == y->p->left) {
        y->p->left = x;
    } else {
        y->p->right = x;
    }

    if (y != z) {
        z->key = y->key;
        z->list = y->list;
        z->numeroauto = y->numeroauto;
    }

    if (y->list != NULL) {
        cancellalista(y->list);
    }

    free(y);
    return T;
}

// Trova il nodo minimo dell'albero
stazione* TREE_MINIMUM(stazione* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

// Cancella la lista
void cancellalista(list* testa) {
    list* current = testa;
    list* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

// Effettua il treewalk per trovare la lunghezza del percorso
void treewalk(stazione* T, int start, int stop, int* lung) {
    if (T != NULL) {
        if (T->key > start && T->key < stop) {
            *lung = *lung + 1;
        }
        treewalk(T->left, start, stop, lung);
        treewalk(T->right, start, stop, lung);
    }
}

// Effettua il treewalk per riempire la matrice del percorso
void treewalk2(stazione* T, int start, int stop, int lung, int mat[lung][4], int* i) {
    if (T != NULL) {
        treewalk2(T->left, start, stop, lung, mat, i);
        if (T->key > start && T->key < stop) {
            mat[*i][0] = T->key;
            mat[*i][1] = T->numeroauto;
            mat[*i][2] = 0;
            *i = *i + 1;
        }
        treewalk2(T->right, start, stop, lung, mat, i);
    }
}
