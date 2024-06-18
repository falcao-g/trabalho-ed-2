#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/avl.h"

typedef struct {
    char codigo_ibge[10];
    char nome[40];
} tnome;

typedef struct {
    char codigo_ibge[10];
    float latitude;
} tlatitude;

typedef struct {
    char codigo_ibge[10];
    float longitude;
} tlongitude;

typedef struct {
    char codigo_ibge[10];
    int codigo_uf;
} tcodigouf;

typedef struct {
    char codigo_ibge[10];
    int ddd;
} tddd;

double cmp_nome(void *a, void *b) {
    return strcmp(((tnome *)a)->nome, ((tnome *)b)->nome);
}

double cmp_latitude(void *a, void *b) {
    return ((tlatitude *)a)->latitude - ((tlatitude *)b)->latitude;
}

double cmp_longitude(void *a, void *b) {
    return ((tlongitude *)a)->longitude - ((tlongitude *)b)->longitude;
}

double cmp_codigo_uf(void *a, void *b) {
    return ((tcodigouf *)a)->codigo_uf - ((tcodigouf *)b)->codigo_uf;
}

double cmp_ddd(void *a, void *b) {
    return ((tddd *)a)->ddd - ((tddd *)b)->ddd;
}

// what a mess
void leitor_json(FILE *arquivo, tarv *arv, tarv *arv2, tarv *arv3, tarv *arv4,
                 tarv *arv5) {
    char linha[150];
    tnome nome;
    tlatitude latitude;
    tlongitude longitude;
    tcodigouf codigo_uf;
    tddd ddd;
    char codigo_ibge[10];
    while (fgets(linha, 150, arquivo)) {
        if (strstr(linha, "codigo_ibge")) {
            sscanf(linha, "    \"codigo_ibge\": %[^,],", codigo_ibge);
        } else if (strstr(linha, "nome")) {
            sscanf(linha, "    \"nome\": \"%[^\"]\",", nome.nome);
            strcpy(nome.codigo_ibge, codigo_ibge);
            tnome *nome_cidade = (tnome *)malloc(sizeof(tnome));
            memcpy(nome_cidade, &nome, sizeof(tnome));
            avl_insere(arv, nome_cidade);
        } else if (strstr(linha, "latitude")) {
            sscanf(linha, "    \"latitude\": %f,", &latitude.latitude);
            strcpy(latitude.codigo_ibge, codigo_ibge);
            tlatitude *latitude_cidade = (tlatitude *)malloc(sizeof(tlatitude));
            memcpy(latitude_cidade, &latitude, sizeof(tlatitude));
            avl_insere(arv2, latitude_cidade);
        } else if (strstr(linha, "longitude")) {
            sscanf(linha, "    \"longitude\": %f,", &longitude.longitude);
            strcpy(longitude.codigo_ibge, codigo_ibge);
            tlongitude *longitude_cidade =
                (tlongitude *)malloc(sizeof(tlongitude));
            memcpy(longitude_cidade, &longitude, sizeof(tlongitude));
            avl_insere(arv3, longitude_cidade);
        } else if (strstr(linha, "codigo_uf")) {
            sscanf(linha, "    \"codigo_uf\": %d,", &codigo_uf.codigo_uf);
            strcpy(codigo_uf.codigo_ibge, codigo_ibge);
            tcodigouf *codigo_uf_cidade =
                (tcodigouf *)malloc(sizeof(tcodigouf));
            memcpy(codigo_uf_cidade, &codigo_uf, sizeof(tcodigouf));
            avl_insere(arv4, codigo_uf_cidade);
        } else if (strstr(linha, "ddd")) {
            sscanf(linha, "    \"ddd\": %d,", &ddd.ddd);
            strcpy(ddd.codigo_ibge, codigo_ibge);
            tddd *ddd_cidade = (tddd *)malloc(sizeof(tddd));
            memcpy(ddd_cidade, &ddd, sizeof(tddd));
            avl_insere(arv5, ddd_cidade);
        }
    }
}

int main(void) {
    tarv arv;
    tarv arv2;
    tarv arv3;
    tarv arv4;
    tarv arv5;

    avl_constroi(&arv, cmp_nome);
    avl_constroi(&arv2, cmp_latitude);
    avl_constroi(&arv3, cmp_longitude);
    avl_constroi(&arv4, cmp_codigo_uf);
    avl_constroi(&arv5, cmp_ddd);
    leitor_json(fopen("./data/municipios.json", "r"), &arv, &arv2, &arv3, &arv4,
                &arv5);

    printf("%s\n", ((tnome *)arv.raiz->item.reg)->nome);
    printf("%s\n", ((tnome *)arv.raiz->dir->item.reg)->nome);
    printf("%f\n", ((tlatitude *)arv2.raiz->item.reg)->latitude);
    printf("%f\n", ((tlatitude *)arv2.raiz->dir->item.reg)->latitude);
    printf("%f\n", ((tlongitude *)arv3.raiz->item.reg)->longitude);
    printf("%f\n", ((tlongitude *)arv3.raiz->dir->item.reg)->longitude);
    printf("%d\n", ((tcodigouf *)arv4.raiz->item.reg)->codigo_uf);
    printf("%d\n", ((tcodigouf *)arv4.raiz->dir->item.reg)->codigo_uf);
    printf("%d\n", ((tddd *)arv5.raiz->item.reg)->ddd);
    printf("%d\n", ((tddd *)arv5.raiz->esq->item.reg)->ddd);
    printf("%s\n", ((tddd *)arv5.raiz->esq->esq->item.prox->prox->prox->prox
                        ->prox->prox->prox->reg)
                       ->codigo_ibge);

    return 0;
}
