#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROXO "\e[38;5;92m"
#define PADRAO "\e[0m"

#include "../include/avl.h"
#include "../include/hash.h"

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
    if (a == NULL) {
        return b;
    } else if (b == NULL) {
        return a;
    }

    int i = 0;
    int j = 0;
    int k = 0;
    int numOfNodes = 0;
    int capacity = 10;
    char **ret = malloc(sizeof(char *) * capacity);
    while (a[i] != NULL) {
        while (b[j] != NULL) {
            if (strcasecmp(a[i], b[j]) == 0) {
                ret[k] = a[i];
                numOfNodes++;
                if (numOfNodes == capacity) {
                    capacity *= 2;
                    ret = realloc(ret, sizeof(char *) * capacity);
                }
                k++;
            }
            j++;
        }
        i++;
    }
    return ret;
}

char **query(tarv *parv, void *min, void *max) {
    int numOfNodes = 10;
    int capacity = 0;
    char **ret = malloc(sizeof(char *) * numOfNodes);
    tnode *ppnode = avl_busca(parv, min);

    while (parv->cmp(ppnode->item.reg, min, parv->type) < 0) {
        ppnode = sucessor(ppnode);
    }

    while (parv->cmp(ppnode->item.reg, max, parv->type) <= 0) {
        if (capacity == numOfNodes) {
            numOfNodes *= 2;
            ret = realloc(ret, sizeof(char *) * numOfNodes);
        }

        if (parv->cmp(ppnode->item.reg, max, parv->type) <= 0) {
            ret[capacity] = ((tcidade *)ppnode->item.reg)->codigo_ibge;
            capacity++;

            if (ppnode->item.prox != NULL) {
                titem *aux = ppnode->item.prox;
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

    avl_constroi(&arv, 1, cmp);   // avl de nome
    avl_constroi(&arv2, 2, cmp);  // avl de latitude
    avl_constroi(&arv3, 3, cmp);  // avl de longitude
    avl_constroi(&arv4, 4, cmp);  // avl de codigo_uf
    avl_constroi(&arv5, 5, cmp);  // avl de ddd

    thash hash_cod;
    hash_constroi(&hash_cod, 12040, pega_codigo);

    leitor_json(fopen("./data/municipios.json", "r"), &arv, &arv2, &arv3, &arv4,
                &arv5, hash_cod);

    int qtde_nome = 0;
    int qtde_lat = 0;
    int qtde_long = 0;
    int qtde_uf = 0;
    int qtde_ddd = 0;
    int qtde_total = 0;
    char **busca_nome = NULL;
    char **busca_lat = NULL;
    char **busca_long = NULL;
    char **busca_uf = NULL;
    char **busca_ddd = NULL;
    char **intersec = NULL;
    while (1) {
        printf("-----------Cidades-----------\n");
        printf("Há %d cidades na busca por nome\n", qtde_nome);
        printf("Há %d cidades na busca por latitude\n", qtde_lat);
        printf("Há %d cidades na busca por longitude\n", qtde_long);
        printf("Há %d cidades na busca por codigo_uf\n", qtde_uf);
        printf("Há %d cidades na busca por ddd\n", qtde_ddd);
        printf("Há %d cidades na intersecção\n", qtde_total);
        printf("----------------------------\n");
        printf("(1) NOME (2) LATITUDE (3) LONGITUDE (4) CODIGO UF (5) DDD (6) "
               "LIMPAR BUSCA (7) LISTAR CIDADES (0) SAIR\n");
        printf("Escolha o que você deseja editar ou adicionar:\n");
        int opcao;
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("--------------------\n");
                char nome1[40];
                char nome2[40];
                printf("Digite a primeira string para a query de nome: ");
                scanf(" %[^\n]s", nome1);
                printf("Digite a segunda string para a query de nome: ");
                scanf(" %[^\n]s", nome2);
                tcidade *cidade1 = (tcidade *)malloc(sizeof(tcidade));
                strcpy(cidade1->nome, nome1);
                tcidade *cidade2 = (tcidade *)malloc(sizeof(tcidade));
                strcpy(cidade2->nome, nome2);
                busca_nome = query(&arv, cidade1, cidade2);
                int i = 0;
                qtde_nome = 0;

                while (busca_nome[i] != NULL) {
                    qtde_nome += 1;
                    i++;
                }

                break;

            case 2:
                printf("--------------------\n");
                float lat1;
                float lat2;
                printf("Digite a primeira latitude para a query de latitude: ");
                scanf("%f", &lat1);
                printf("Digite a segunda latitude para a query de latitude: ");
                scanf("%f", &lat2);
                tcidade *cidade3 = (tcidade *)malloc(sizeof(tcidade));
                cidade3->latitude = lat1;
                tcidade *cidade4 = (tcidade *)malloc(sizeof(tcidade));
                cidade4->latitude = lat2;
                busca_lat = query(&arv2, cidade3, cidade4);
                int j = 0;
                qtde_lat = 0;

                while (busca_lat[j] != NULL) {
                    qtde_lat += 1;
                    j++;
                }

                break;

            case 3:
                printf("--------------------\n");
                float long1;
                float long2;
                printf(
                    "Digite a primeira longitude para a query de longitude: ");
                scanf("%f", &long1);
                printf(
                    "Digite a segunda longitude para a query de longitude: ");
                scanf("%f", &long2);
                tcidade *cidade5 = (tcidade *)malloc(sizeof(tcidade));
                cidade5->longitude = long1;
                tcidade *cidade6 = (tcidade *)malloc(sizeof(tcidade));
                cidade6->longitude = long2;
                busca_long = query(&arv3, cidade5, cidade6);
                int k = 0;
                qtde_long = 0;

                while (busca_long[k] != NULL) {
                    qtde_long += 1;
                    k++;
                }

                break;

            case 4:
                printf("--------------------\n");
                int uf1;
                int uf2;
                printf("Digite o primeiro código de UF para a query de codigo "
                       "de UF: ");
                scanf("%d", &uf1);
                printf(
                    "Digite o segundo código de UF para a query de codigo de "
                    "UF: ");
                scanf("%d", &uf2);
                tcidade *cidade7 = (tcidade *)malloc(sizeof(tcidade));
                cidade7->codigo_uf = uf1;
                tcidade *cidade8 = (tcidade *)malloc(sizeof(tcidade));
                cidade8->codigo_uf = uf2;
                busca_uf = query(&arv4, cidade7, cidade8);
                int l = 0;
                qtde_uf = 0;

                while (busca_uf[l] != NULL) {
                    qtde_uf += 1;
                    l++;
                }

                break;

            case 5:
                printf("--------------------\n");
                int ddd1;
                int ddd2;
                printf("Digite o primeiro DDD para a query de DDD: ");
                scanf("%d", &ddd1);
                printf("Digite o segundo DDD para a query de DDD: ");
                scanf("%d", &ddd2);
                tcidade *cidade9 = (tcidade *)malloc(sizeof(tcidade));
                cidade9->ddd = ddd1;
                tcidade *cidade10 = (tcidade *)malloc(sizeof(tcidade));
                cidade10->ddd = ddd2;
                busca_ddd = query(&arv5, cidade9, cidade10);
                int m = 0;
                qtde_ddd = 0;

                while (busca_ddd[m] != NULL) {
                    qtde_ddd += 1;
                    m++;
                }

                break;

            case 6:
                printf("--------------------\n");
                printf("Qual query deseja desativar?\n");
                printf("1 - Nome\n");
                printf("2 - Latitude\n");
                printf("3 - Longitude\n");
                printf("4 - Código UF\n");
                printf("5 - DDD\n");
                int desativa;
                scanf("%d", &desativa);
                if (desativa == 1) {
                    qtde_nome = 0;
                } else if (desativa == 2) {
                    qtde_lat = 0;
                } else if (desativa == 3) {
                    qtde_long = 0;
                } else if (desativa == 4) {
                    qtde_uf = 0;
                } else if (desativa == 5) {
                    qtde_ddd = 0;
                }
                break;

            case 7:
                printf("--------------------\n");
                int n = 0;
                printf("Intersecção das queries:\n");

                if (intersec != NULL) {
                    while (intersec[n] != NULL) {
                        tmunicipio *municipio =
                            (tmunicipio *)hash_busca(hash_cod, intersec[n]);
                        imprime_municipio(municipio);
                        n++;
                    }
                }
                break;

            default:
                hash_apaga(&hash_cod);
                avl_destroi(arv.raiz);
                avl_destroi(arv2.raiz);
                avl_destroi(arv3.raiz);
                avl_destroi(arv4.raiz);
                avl_destroi(arv5.raiz);
                exit(0);
                break;
        }
        intersec = interseccao(busca_nome, busca_lat);
        intersec = interseccao(intersec, busca_long);
        intersec = interseccao(intersec, busca_uf);
        intersec = interseccao(intersec, busca_ddd);
        int n = 0;
        qtde_total = 0;

        if (intersec != NULL) {
            while (intersec[n] != NULL) {
                qtde_total += 1;
                n++;
            }
        }
    }

    return 0;
}
