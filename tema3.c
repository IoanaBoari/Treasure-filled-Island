/*BOARI Ioana-Ruxandra - 312CD*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tema3.h"
#define MAX 1000

int sort_cost(const void *p, const void *q) {
    const int *a = (const int *)p;
    const int *b = (const int *)q;

    if (*a > *b) {
        return 1;
    }
    if (*a < *b) {
        return -1;
    }
    return 0;
}  // functia de sortare folosita pentru qsort

int main (int argc, char const *argv[]) {
    if (argc == 2) {
        if (strcmp(argv[1], "1") == 0) {
            printf("Rezolva cerinta 1\n");
            FILE *in = fopen("tema3.in", "rt");
            FILE *out = fopen("tema3.out", "wt");
            int N, M;
            char varf1[MAX][MAX];  
            // vector de siruri de caractere in care retin primul varf al unei muchii
            char varf2[MAX][MAX];
            // vector de siruri de caractere in care retin al doilea varf al unei muchii
            int cost[MAX];  // vector in care retin costul drumului
            if (in == NULL) {  // verifica daca fisierul s-a deschis cu succes
            printf("Eroare la deschiderea fisierului.\n");
            exit(1);
            }
            fscanf(in, "%d %d", &N, &M);
            fseek(in, 1, SEEK_CUR);
            for (int i = 0; i < M; i++) {
                fscanf(in, "%s", varf1[i]);
                fscanf(in, "%s", varf2[i]);
                fscanf(in, "%d", &cost[i]);
                fseek(in, 1, SEEK_CUR);
            }
            Harta* harta = malloc (sizeof(Harta));
            harta->obiective = (char**)malloc(N * sizeof(char*));
            for (int i = 0; i < N; i++) {
                harta->obiective[i] = (char*)calloc(MAX, sizeof(char));
            }
            TGraphL* graph = createGraphAdjList(N);
            for (int k = 0; k < N;) {
                for (int i = 0; i < M; i++) {
                    int exists_1 = 0;
                    int exists_2 = 0;
                    for (int j = 0; j < N; j++) {
                        if (strcmp(varf1[i], harta->obiective[j]) == 0) {
                            exists_1 = 1;
                        }
                        if (strcmp(varf2[i], harta->obiective[j]) == 0) {
                            exists_2 = 1;
                        }
                    }
                    if (!exists_1) {
                        strcpy(harta->obiective[k], varf1[i]);
                        harta->key[k] = k;
                        k++;
                    }
                    if (!exists_2) {
                        strcpy(harta->obiective[k], varf2[i]);
                        harta->key[k] = k;
                        k++;
                    }
                }
            }  // adaug toate obiectivele distincte in vectorul harta->obiective
            // generez o codificare pentru fiecare obiectiv
            int key1;
            int key2;
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    if (strcmp(varf1[i], harta->obiective[j]) == 0) {
                        key1 = harta->key[j];
                    }
                    if (strcmp(varf2[i], harta->obiective[j]) == 0) {
                        key2 = harta->key[j];
                    }
                }
                addEdgeList(graph, key1, key2, cost[i]);  // creez muchiile din graf
            }

            int nrZone = countConnectedComponents(graph, N);
            fprintf(out, "%d\n", nrZone);
            int* parent = malloc(graph->nn * sizeof(int));
            for (int i = 0; i < graph->nn; i++) {
                parent[i] = -1;
            }
            // folosesc acest vector in algoritmul Prim pentru a sti ce noduri
            // nu au fost incluse deja intr-un arbore minim de acoperire
            int* cost_total = malloc(nrZone * sizeof(int));
            // in acest vector retin costurile totale minime pentru fiecare zona
            if (cost_total == NULL) {
                exit(1);
            }
            int idx = 0;
            for (int i = 0; i < graph->nn; i++) {
                int cost_drum = 0;
                if (parent[i] == -1) {
                    cost_drum = Prim(*graph, i, parent);
                    cost_total[idx++] = cost_drum;
                }
            }
            qsort(cost_total, nrZone, sizeof(int), sort_cost);
            for (int i = 0; i < nrZone; i++) {
                fprintf(out, "%d\n", cost_total[i]);
            }
            free(cost_total);
            free(parent);
            destroyGraphAdjList(graph);
            eliberareHarta(harta, N);
            // eliberez memoria alocata
            fclose(in);
            fclose(out);
        }
        if (strcmp(argv[1], "2") == 0) {
            printf("Rezolva cerinta 2\n");
            FILE *in = fopen("tema3.in", "rt");
            FILE *out = fopen("tema3.out", "wt");
            int N, M;
            char varf1[MAX][MAX];
            char varf2[MAX][MAX];
            char nod[MAX][MAX];
            int cost[MAX];
            int adancime[MAX];
            int comoara;
            if (in == NULL) { // verifica daca fisierul s-a deschis cu succes
            printf("Eroare la deschiderea fisierului.\n");
            exit(1);
            }
            fscanf(in, "%d %d", &N, &M);
            fseek(in, 1, SEEK_CUR);
            //fprintf(out, "N= %d\n", N);
            //fprintf(out, "M= %d\n", M);
            for (int i = 0; i < M; i++) {
                fscanf(in, "%s", varf1[i]);
                fscanf(in, "%s", varf2[i]);
                fscanf(in, "%d", &cost[i]);
                fseek(in, 1, SEEK_CUR);
            }
            for (int i = 0; i < N; i++) {
                fscanf(in, "%s", nod[i]);
                fscanf(in, "%d", &adancime[i]);
                fseek(in, 1, SEEK_CUR);
            }
            fscanf(in, "%d", &comoara);
            printf("comoara: %d\n", comoara);
            Harta* harta = malloc (sizeof(Harta));
            harta->obiective = (char**)malloc(N * sizeof(char*));
            for (int i = 0; i < N; i++) {
                harta->obiective[i] = (char*)malloc(MAX * sizeof(char));
            }
            TGraphL* graph = createGraphAdjList(N);
            for (int k = 0; k < N;) {
                for (int i = 0; i < M; i++) {
                    int exists_1 = 0;
                    int exists_2 = 0;
                    for (int j = 0; j < N; j++) {
                        if (strcmp(varf1[i], harta->obiective[j]) == 0) {
                            exists_1 = 1;
                        }
                        if (strcmp(varf2[i], harta->obiective[j]) == 0) {
                            exists_2 = 1;
                        }
                    }
                    if (!exists_1) {
                        strcpy(harta->obiective[k], varf1[i]);
                        harta->key[k] = k;
                        for (int idx = 0; idx < N; idx++) {
                            if (strcmp(harta->obiective[k], nod[idx]) == 0) {
                                harta->adancime[k] = adancime[idx];
                            }
                        }
                        k++;
                    }
                    if (!exists_2) {
                        strcpy(harta->obiective[k], varf2[i]);
                        harta->key[k] = k;
                        for (int idx = 0; idx < N; idx++) {
                            if (strcmp(harta->obiective[k], nod[idx]) == 0) {
                                harta->adancime[k] = adancime[idx];
                            }
                        }
                        k++;
                    }
                }
            }
            int key1;
            int key2;
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    if (strcmp(varf1[i], harta->obiective[j]) == 0) {
                        key1 = harta->key[j];
                    }
                    if (strcmp(varf2[i], harta->obiective[j]) == 0) {
                        key2 = harta->key[j];
                    }
                }
                addEdgeListOrientated(graph, key1, key2, cost[i]);

            }
            fclose(in);
            fclose(out);
        }
    } else {
        printf("Fisierul primeste ca argument 1 sau 2!\n");
    }
    return 0;
}