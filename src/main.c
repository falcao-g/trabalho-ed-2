#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROXO "\e[38;5;92m"
#define PADRAO "\e[0m"

#include "../include/avl.h"
// why the include to the header file is not working?
#include "../src/hash.c"

typedef struct {
    char codigo_ibge[10];
    char nome[40];
    float latitude;
    float longitude;
    int capital;
    int codigo_uf;
    int siafi_id;
    int ddd;
    char fuso_horario[40];
} tmunicipio;

typedef struct {
    char codigo_ibge[10];
    char nome[40];
    float latitude;
    float longitude;
    int codigo_uf;
    int ddd;
} tcidade;

char *pega_codigo(void *reg) {
    return ((tmunicipio *)reg)->codigo_ibge;
}

double cmp(void *a, void *b, int type) {
    if (type == 1) {
        return strcasecmp(((tcidade *)a)->nome, ((tcidade *)b)->nome);
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

void insere_cidade(thash hash_cod, tmunicipio *municipio) {
    tmunicipio *municipio_aux = malloc(sizeof(tmunicipio));
    memcpy(municipio_aux, municipio, sizeof(tmunicipio));
    hash_insere(&hash_cod, municipio_aux);
}

// what a huge mess
void leitor_json(FILE *arquivo, tarv *arv, tarv *arv2, tarv *arv3, tarv *arv4,
                 tarv *arv5, thash hash_cod) {
    char linha[150];
    tcidade nome;
    tcidade latitude;
    tcidade longitude;
    tcidade codigo_uf;
    tcidade ddd;
    tmunicipio municipio;
    char codigo_ibge[10];
    while (fgets(linha, 150, arquivo)) {
        if (strstr(linha, "codigo_ibge")) {
            sscanf(linha, "    \"codigo_ibge\": %[^,],", municipio.codigo_ibge);
        } else if (strstr(linha, "nome")) {
            sscanf(linha, "    \"nome\": \"%[^\"]\",", municipio.nome);
            strcpy(nome.codigo_ibge, municipio.codigo_ibge);
            strcpy(nome.nome, municipio.nome);
            tcidade *nome_cidade = (tcidade *)malloc(sizeof(tcidade));
            memcpy(nome_cidade, &nome, sizeof(tcidade));
            avl_insere(arv, nome_cidade);
        } else if (strstr(linha, "latitude")) {
            sscanf(linha, "    \"latitude\": %f,", &municipio.latitude);
            strcpy(latitude.codigo_ibge, municipio.codigo_ibge);
            latitude.latitude = municipio.latitude;
            tcidade *latitude_cidade = (tcidade *)malloc(sizeof(tcidade));
            memcpy(latitude_cidade, &latitude, sizeof(tcidade));
            avl_insere(arv2, latitude_cidade);
        } else if (strstr(linha, "longitude")) {
            sscanf(linha, "    \"longitude\": %f,", &municipio.longitude);
            strcpy(longitude.codigo_ibge, municipio.codigo_ibge);
            longitude.longitude = municipio.longitude;
            tcidade *longitude_cidade = (tcidade *)malloc(sizeof(tcidade));
            memcpy(longitude_cidade, &longitude, sizeof(tcidade));
            avl_insere(arv3, longitude_cidade);
        } else if (strstr(linha, "capital")) {
            // sscanf(linha, "    \"capital\": %d,", &municipio.capital);
        } else if (strstr(linha, "codigo_uf")) {
            sscanf(linha, "    \"codigo_uf\": %d,", &municipio.codigo_uf);
            strcpy(codigo_uf.codigo_ibge, municipio.codigo_ibge);
            codigo_uf.codigo_uf = municipio.codigo_uf;
            tcidade *codigo_uf_cidade = (tcidade *)malloc(sizeof(tcidade));
            memcpy(codigo_uf_cidade, &codigo_uf, sizeof(tcidade));
            avl_insere(arv4, codigo_uf_cidade);
        } else if (strstr(linha, "siafi_id")) {
            // sscanf(linha, "    \"siafi_id\": %d,", &municipio.siafi_id);
        } else if (strstr(linha, "ddd")) {
            sscanf(linha, "    \"ddd\": %d,", &municipio.ddd);
            strcpy(ddd.codigo_ibge, municipio.codigo_ibge);
            ddd.ddd = municipio.ddd;
            tcidade *ddd_cidade = (tcidade *)malloc(sizeof(tcidade));
            memcpy(ddd_cidade, &ddd, sizeof(tcidade));
            avl_insere(arv5, ddd_cidade);
        } else if (strstr(linha, "fuso_horario")) {
            sscanf(linha, "    \"fuso_horario\": \"%[^\"]\",",
                   municipio.fuso_horario);
            insere_cidade(hash_cod, &municipio);
        }
    }
}

void imprime_municipio(tmunicipio *municipio) {
    if (municipio == NULL) {
        printf("Cidade não encontrada\n");
        return;
    }
    printf("%sCódigo IBGE: %s%s\n", ROXO, PADRAO, municipio->codigo_ibge);
    printf("%sNome: %s%s\n", ROXO, PADRAO, municipio->nome);
    printf("%sLatitude: %s%f\n", ROXO, PADRAO, municipio->latitude);
    printf("%sLongitude: %s%f\n", ROXO, PADRAO, municipio->longitude);
    printf("%sCapital: %s%d\n", ROXO, PADRAO, municipio->capital);
    printf("%sCódigo UF: %s%d\n", ROXO, PADRAO, municipio->codigo_uf);
    printf("%sSiafi ID: %s%d\n", ROXO, PADRAO, municipio->siafi_id);
    printf("%sDDD: %s%d\n", ROXO, PADRAO, municipio->ddd);
    printf("%sFuso Horário: %s%s\n", ROXO, PADRAO, municipio->fuso_horario);
}

char **interseccao(char **a, char **b) {
    int i = 0;
    int j = 0;
    int k = 0;
    int numOfNodes = 0;
    int capacity = 10;
    char **ret = malloc(sizeof(char *) * capacity);
    while (a[i] != NULL) {
        while (b[j] != NULL) {
            if (strcmp(a[i], b[j]) == 0) {
                ret[k] = a[i];
                numOfNodes++;
                if (numOfNodes == capacity) {
                    capacity *= 2;
                    ret = realloc(ret, sizeof(char *) * capacity);
                }
                k++;
                break;
            }
            j++;
        }
        i++;
    }
    return ret;
}

char **query(tarv *parv, tnode **ppnode, void *min, void *max) {
    int numOfNodes = 10;
    int capacity = 0;
    char **ret = malloc(sizeof(char *) * numOfNodes);
    ppnode = avl_busca(parv, ppnode, min);

    while (parv->cmp((*ppnode)->item.reg, min, parv->type) < 0) {
        ppnode = sucessor(ppnode);
    }

    while (parv->cmp((*ppnode)->item.reg, max, parv->type) <= 0) {
        if (capacity == numOfNodes) {
            numOfNodes *= 2;
            ret = realloc(ret, sizeof(char *) * numOfNodes);
        }

        if (parv->cmp((*ppnode)->item.reg, max, parv->type) <= 0) {
            ret[capacity] = ((tcidade *)(*ppnode)->item.reg)->codigo_ibge;
            capacity++;

            if ((*ppnode)->item.prox != NULL) {
                titem *aux = (*ppnode)->item.prox;
                while (aux != NULL) {
                    ret[capacity] = ((tcidade *)aux->reg)->codigo_ibge;
                    capacity++;
                    if (capacity == numOfNodes) {
                        numOfNodes *= 2;
                        ret = realloc(ret, sizeof(char *) * numOfNodes);
                    }
                    aux = aux->prox;
                }
            }
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

    thash hash_cod;
    hash_constroi(&hash_cod, 12040, pega_codigo);

    leitor_json(fopen("./data/municipios.json", "r"), &arv, &arv2, &arv3, &arv4,
                &arv5, hash_cod);

    // tcidade *cidade1 = (tcidade *)malloc(sizeof(tcidade));
    // strcpy(cidade1->nome, "AB");
    // tcidade *cidade2 = (tcidade *)malloc(sizeof(tcidade));
    // strcpy(cidade2->nome, "AC");

    tcidade *cidade1 = (tcidade *)malloc(sizeof(tcidade));
    cidade1->longitude = -60;
    tcidade *cidade2 = (tcidade *)malloc(sizeof(tcidade));
    cidade2->longitude = -40;

    char **busca = query(&arv3, &arv3.raiz, cidade1, cidade2);

    int i = 0;
    while (busca[i] != NULL) {
        tmunicipio *municipio = (tmunicipio *)hash_busca(hash_cod, busca[i]);
        imprime_municipio(municipio);
        // printf("%s%s%s\n", ROXO, busca[i], PADRAO);
        i++;
    }

    return 0;
}
