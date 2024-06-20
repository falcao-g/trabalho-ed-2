#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/avl.h"

typedef struct {
    char codigo_ibge[10];
    char nome[40];
    float latitude;
    float longitude;
    int codigo_uf;
    int ddd;
} tcidade;

double cmp(void *a, void *b, int type) {
    if (type == 1) {
        return strcmp(((tcidade *)a)->nome, ((tcidade *)b)->nome);
    } else if (type == 2) {
        return ((tcidade *)a)->latitude - ((tcidade *)b)->latitude;
    } else if (type == 3) {
        return ((tcidade *)a)->longitude - ((tcidade *)b)->longitude;
    } else if (type == 4) {
        return ((tcidade *)a)->codigo_uf - ((tcidade *)b)->codigo_uf;
    } else {
        return ((tcidade *)a)->ddd - ((tcidade *)b)->ddd;
    }
}

// what a mess
void leitor_json(FILE *arquivo, tarv *arv, tarv *arv2, tarv *arv3, tarv *arv4,
                 tarv *arv5) {
    char linha[150];
    tcidade nome;
    tcidade latitude;
    tcidade longitude;
    tcidade codigo_uf;
    tcidade ddd;
    char codigo_ibge[10];
    while (fgets(linha, 150, arquivo)) {
        if (strstr(linha, "codigo_ibge")) {
            sscanf(linha, "    \"codigo_ibge\": %[^,],", codigo_ibge);
        } else if (strstr(linha, "nome")) {
            sscanf(linha, "    \"nome\": \"%[^\"]\",", nome.nome);
            strcpy(nome.codigo_ibge, codigo_ibge);
            tcidade *nome_cidade = (tcidade *)malloc(sizeof(tcidade));
            memcpy(nome_cidade, &nome, sizeof(tcidade));
            avl_insere(arv, nome_cidade);
        } else if (strstr(linha, "latitude")) {
            sscanf(linha, "    \"latitude\": %f,", &latitude.latitude);
            strcpy(latitude.codigo_ibge, codigo_ibge);
            tcidade *latitude_cidade = (tcidade *)malloc(sizeof(tcidade));
            memcpy(latitude_cidade, &latitude, sizeof(tcidade));
            avl_insere(arv2, latitude_cidade);
        } else if (strstr(linha, "longitude")) {
            sscanf(linha, "    \"longitude\": %f,", &longitude.longitude);
            strcpy(longitude.codigo_ibge, codigo_ibge);
            tcidade *longitude_cidade = (tcidade *)malloc(sizeof(tcidade));
            memcpy(longitude_cidade, &longitude, sizeof(tcidade));
            avl_insere(arv3, longitude_cidade);
        } else if (strstr(linha, "codigo_uf")) {
            sscanf(linha, "    \"codigo_uf\": %d,", &codigo_uf.codigo_uf);
            strcpy(codigo_uf.codigo_ibge, codigo_ibge);
            tcidade *codigo_uf_cidade = (tcidade *)malloc(sizeof(tcidade));
            memcpy(codigo_uf_cidade, &codigo_uf, sizeof(tcidade));
            avl_insere(arv4, codigo_uf_cidade);
        } else if (strstr(linha, "ddd")) {
            sscanf(linha, "    \"ddd\": %d,", &ddd.ddd);
            strcpy(ddd.codigo_ibge, codigo_ibge);
            tcidade *ddd_cidade = (tcidade *)malloc(sizeof(tcidade));
            memcpy(ddd_cidade, &ddd, sizeof(tcidade));
            avl_insere(arv5, ddd_cidade);
        }
    }
}

tnode **query(tarv *parv, tnode **ppnode, void *min, void *max) {
    int numOfNodes = 10;
    int capacity = 0;
    tnode **ret = malloc(sizeof(tnode *) * numOfNodes);
    ppnode = avl_busca(parv, ppnode, min);

    while (parv->cmp((*ppnode)->item.reg, min, parv->type) < 0) {
        ppnode = sucessor(ppnode);
    }

    while (parv->cmp((*ppnode)->item.reg, max, parv->type) <= 0) {
        if (capacity == numOfNodes) {
            numOfNodes *= 2;
            ret = realloc(ret, sizeof(tnode *) * numOfNodes);
        }

        if (parv->cmp((*ppnode)->item.reg, max, parv->type) <= 0) {
            ret[capacity] = *ppnode;
            capacity++;
        }

        ppnode = sucessor(ppnode);
    }

    return ret;
}

int main(void) {
    tarv arv;
    tarv arv2;
    tarv arv3;
    tarv arv4;
    tarv arv5;

    avl_constroi(&arv, 1, cmp);
    avl_constroi(&arv2, 2, cmp);
    avl_constroi(&arv3, 3, cmp);
    avl_constroi(&arv4, 4, cmp);
    avl_constroi(&arv5, 5, cmp);
    leitor_json(fopen("./data/municipios.json", "r"), &arv, &arv2, &arv3, &arv4,
                &arv5);

    tcidade *cidade1 = (tcidade *)malloc(sizeof(tcidade));
    strcpy(cidade1->nome, "Assis");
    tcidade *cidade2 = (tcidade *)malloc(sizeof(tcidade));
    strcpy(cidade2->nome, "Assis Chateaubriand");

    tnode **busca = query(&arv, &arv.raiz, cidade1, cidade2);

    int i = 0;
    while (busca[i] != NULL) {
        printf("%s\n", ((tcidade *)busca[i]->item.reg)->nome);
        i++;
    }

    // tnode **busca_bet = sucessor(&busca);
    // tnode **busca_bet2 = sucessor(busca_bet);

    // printf("%d\n", ((tcidade *)(*busca_bet)->item.reg)->ddd);
    // printf("%d\n", ((tcidade *)(*busca_bet2)->item.reg)->ddd);

    // tnode *busca2 = avl_busca(&arv5, &arv5.raiz, cidade2);
    // printf("%d\n", ((tcidade *)busca2->item.reg)->ddd);

    //   tnode **busca = avl_busca_intervalo(&arv, &arv.raiz, cidade1,
    //   cidade2);
    //    printf("%s\n", ((tcidade *)(*busca)->item.reg)->ddd);

    return 0;
}
