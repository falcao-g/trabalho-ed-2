#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROXO "\e[38;5;92m"
#define PADRAO "\e[0m"

#include "../include/avl.h"
#include "../include/hash.h"

// struct da hash
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

// struct da avl (só preenchemos o que precisamos)
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

// usamos o tipo da avl para saber o que comparar
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

void leitor_json(FILE *arquivo, tarv *avl_nome, tarv *avl_lat, tarv *avl_long,
                 tarv *avl_uf, tarv *avl_ddd, thash hash_cod) {
    char linha[150];
    tmunicipio municipio;
    char codigo_ibge[10];
    while (fgets(linha, 150, arquivo)) {
        tcidade *nome = malloc(sizeof(tcidade));
        tcidade *latitude = malloc(sizeof(tcidade));
        tcidade *longitude = malloc(sizeof(tcidade));
        tcidade *codigo_uf = malloc(sizeof(tcidade));
        tcidade *ddd = malloc(sizeof(tcidade));

        // guardamos um municipio que armazena tudo e depois inserimos na hash
        // para cada campo relevante as avl, copiamos o código ibge e aquele
        // campo e inserimos na avl correspondente
        if (strstr(linha, "codigo_ibge")) {
            sscanf(linha, "    \"codigo_ibge\": %[^,],", municipio.codigo_ibge);
        } else if (strstr(linha, "nome")) {
            sscanf(linha, "    \"nome\": \"%[^\"]\",", municipio.nome);
            strcpy(nome->codigo_ibge, municipio.codigo_ibge);
            strcpy(nome->nome, municipio.nome);
            avl_insere(avl_nome, nome);
        } else if (strstr(linha, "latitude")) {
            sscanf(linha, "    \"latitude\": %f,", &municipio.latitude);
            strcpy(latitude->codigo_ibge, municipio.codigo_ibge);
            latitude->latitude = municipio.latitude;
            avl_insere(avl_lat, latitude);
        } else if (strstr(linha, "longitude")) {
            sscanf(linha, "    \"longitude\": %f,", &municipio.longitude);
            strcpy(longitude->codigo_ibge, municipio.codigo_ibge);
            longitude->longitude = municipio.longitude;
            avl_insere(avl_long, longitude);
        } else if (strstr(linha, "capital")) {
            sscanf(linha, "    \"capital\": %d,", &municipio.capital);
        } else if (strstr(linha, "codigo_uf")) {
            sscanf(linha, "    \"codigo_uf\": %d,", &municipio.codigo_uf);
            strcpy(codigo_uf->codigo_ibge, municipio.codigo_ibge);
            codigo_uf->codigo_uf = municipio.codigo_uf;
            avl_insere(avl_uf, codigo_uf);
        } else if (strstr(linha, "siafi_id")) {
            sscanf(linha, "    \"siafi_id\": %d,", &municipio.siafi_id);
        } else if (strstr(linha, "ddd")) {
            sscanf(linha, "    \"ddd\": %d,", &municipio.ddd);
            strcpy(ddd->codigo_ibge, municipio.codigo_ibge);
            ddd->ddd = municipio.ddd;
            avl_insere(avl_ddd, ddd);
        } else if (strstr(linha, "fuso_horario")) {
            sscanf(linha, "    \"fuso_horario\": \"%[^\"]\",",
                   municipio.fuso_horario);
            insere_cidade(hash_cod, &municipio);
        }
    }
}

void imprime_tabela_municipios(char **cods, int n, thash hash_cod) {
    if (cods == NULL || n <= 0) {
        printf("Nenhuma cidade encontrada\n");
        return;
    }

    // cabeçalho da tabela
    printf("%s%-8s | %-25s | %-9s | %-9s | %-7s | %-6s | %-8s | %-3s | "
           "%-15s\n%s",
           ROXO, "Cod IBGE", "Nome", "Latitude", "Longitude", "Capital",
           "Cod UF", "Siafi ID", "DDD", "Fuso Horário", PADRAO);
    printf("-------------------------------------------------------------------"
           "---------------------------------------------\n");

    // linhas da tabela
    for (int i = 0; i < n; i++) {
        tmunicipio *municipio = (tmunicipio *)hash_busca(hash_cod, cods[i]);

        printf("%-8s | %-25s | %-9.4f | %-9.4f | %-7d | %-6d | %-8d | %-3d | "
               "%-15s\n",
               municipio->codigo_ibge, municipio->nome, municipio->latitude,
               municipio->longitude, municipio->capital, municipio->codigo_uf,
               municipio->siafi_id, municipio->ddd, municipio->fuso_horario);
    }
}

// comparamos as listas de códigos ibges em pares
char **interseccao(char **a, char **b) {
    if (a == NULL) {
        return b;
    } else if (b == NULL) {
        return a;
    }

    int i = 0;
    int k = 0;
    int numOfNodes = 0;
    int capacity = 10;
    char **ret = malloc(sizeof(char *) * capacity);
    while (a[i] != NULL) {
        int j = 0;
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
    int capacity = 10;
    int numOfNodes = 0;
    char **ret = malloc(sizeof(char *) * capacity);
    tnode *pnode = avl_busca(parv, min);

    // se achamos um nó menor do que o que queremos, vamos para o próximo
    while (parv->cmp(pnode->item.reg, min, parv->type) < 0) {
        pnode = sucessor(pnode);
    }

    // chamamos o sucessor até que o nó seja maior do que o que queremos
    while (parv->cmp(pnode->item.reg, max, parv->type) <= 0) {
        if (capacity == numOfNodes) {
            capacity *= 2;
            ret = realloc(ret, sizeof(char *) * capacity);
        }

        if (parv->cmp(pnode->item.reg, max, parv->type) <= 0) {
            ret[numOfNodes] = ((tcidade *)pnode->item.reg)->codigo_ibge;
            numOfNodes++;

            if (pnode->item.prox != NULL) {
                titem *aux = pnode->item.prox;
                while (aux != NULL) {
                    ret[numOfNodes] = ((tcidade *)aux->reg)->codigo_ibge;
                    numOfNodes++;
                    if (capacity == numOfNodes) {
                        capacity *= 2;
                        ret = realloc(ret, sizeof(char *) * capacity);
                    }
                    aux = aux->prox;
                }
            }
        }

        pnode = sucessor(pnode);
        if (pnode == NULL) {
            break;
        }
    }

    return ret;
}

int main(void) {
    tarv avl_nome;
    tarv avl_lat;
    tarv avl_long;
    tarv avl_uf;
    tarv avl_ddd;

    avl_constroi(&avl_nome, 1, cmp);  // avl de nome
    avl_constroi(&avl_lat, 2, cmp);   // avl de latitude
    avl_constroi(&avl_long, 3, cmp);  // avl de longitude
    avl_constroi(&avl_uf, 4, cmp);    // avl de codigo_uf
    avl_constroi(&avl_ddd, 5, cmp);   // avl de ddd

    thash hash_cod;
    hash_constroi(&hash_cod, 12040, pega_codigo);

    leitor_json(fopen("./data/municipios.json", "r"), &avl_nome, &avl_lat,
                &avl_long, &avl_uf, &avl_ddd, hash_cod);

    // variáveis para a interface
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
        printf("Escolha uma opção: \n");
        int opcao;
        scanf("%d", &opcao);

        int i = 0;
        switch (opcao) {
            case 1:
                printf("--------------------\n");
                char nome_min[40];
                char nome_max[40];
                printf("Digite a menor palavra para a busca por nome: ");
                scanf(" %[^\n]s", nome_min);
                printf("Digite a maior palavra para a busca por nome: ");
                scanf(" %[^\n]s", nome_max);

                tcidade *cidade_nome_min = (tcidade *)malloc(sizeof(tcidade));
                strcpy(cidade_nome_min->nome, nome_min);
                tcidade *cidade_nome_max = (tcidade *)malloc(sizeof(tcidade));
                strcpy(cidade_nome_max->nome, nome_max);
                busca_nome = query(&avl_nome, cidade_nome_min, cidade_nome_max);

                qtde_nome = 0;
                while (busca_nome[i] != NULL) {
                    qtde_nome += 1;
                    i++;
                }
                break;

            case 2:
                printf("--------------------\n");
                float lat_min;
                float lat_max;
                printf("Digite a menor latitude para a busca por latitude: ");
                scanf("%f", &lat_min);
                printf("Digite a maior latitude para a busca por latitude: ");
                scanf("%f", &lat_max);

                tcidade *cidade_lat_min = (tcidade *)malloc(sizeof(tcidade));
                cidade_lat_min->latitude = lat_min;
                tcidade *cidade_lat_max = (tcidade *)malloc(sizeof(tcidade));
                cidade_lat_max->latitude = lat_max;
                busca_lat = query(&avl_lat, cidade_lat_min, cidade_lat_max);

                qtde_lat = 0;
                while (busca_lat[i] != NULL) {
                    qtde_lat += 1;
                    i++;
                }
                break;

            case 3:
                printf("--------------------\n");
                float long_min;
                float long_max;
                printf("Digite a menor longitude para a busca por longitude: ");
                scanf("%f", &long_min);
                printf("Digite a maior longitude para a busca por longitude: ");
                scanf("%f", &long_max);

                tcidade *cidade_long_min = (tcidade *)malloc(sizeof(tcidade));
                cidade_long_min->longitude = long_min;
                tcidade *cidade_long_max = (tcidade *)malloc(sizeof(tcidade));
                cidade_long_max->longitude = long_max;
                busca_long = query(&avl_long, cidade_long_min, cidade_long_max);

                qtde_long = 0;
                while (busca_long[i] != NULL) {
                    qtde_long += 1;
                    i++;
                }
                break;

            case 4:
                printf("--------------------\n");
                int uf_min;
                int uf_max;
                printf("Digite o menor código UF para a busca por código "
                       "UF: ");
                scanf("%d", &uf_min);
                printf("Digite o maior código UF para a busca por código "
                       "UF: ");
                scanf("%d", &uf_max);

                tcidade *cidade_uf_min = (tcidade *)malloc(sizeof(tcidade));
                cidade_uf_min->codigo_uf = uf_min;
                tcidade *cidade_uf_max = (tcidade *)malloc(sizeof(tcidade));
                cidade_uf_max->codigo_uf = uf_max;
                busca_uf = query(&avl_uf, cidade_uf_min, cidade_uf_max);

                qtde_uf = 0;
                while (busca_uf[i] != NULL) {
                    qtde_uf += 1;
                    i++;
                }
                break;

            case 5:
                printf("--------------------\n");
                int ddd_min;
                int ddd_max;
                printf("Digite o menor DDD para a busca por DDD: ");
                scanf("%d", &ddd_min);
                printf("Digite o maior DDD para a busca por DDD: ");
                scanf("%d", &ddd_max);

                tcidade *cidade_ddd_min = (tcidade *)malloc(sizeof(tcidade));
                cidade_ddd_min->ddd = ddd_min;
                tcidade *cidade_ddd_max = (tcidade *)malloc(sizeof(tcidade));
                cidade_ddd_max->ddd = ddd_max;
                busca_ddd = query(&avl_ddd, cidade_ddd_min, cidade_ddd_max);

                qtde_ddd = 0;
                while (busca_ddd[i] != NULL) {
                    qtde_ddd += 1;
                    i++;
                }
                break;

            case 6:
                printf("--------------------\n");
                printf("1 - Nome\n");
                printf("2 - Latitude\n");
                printf("3 - Longitude\n");
                printf("4 - Código UF\n");
                printf("5 - DDD\n");
                printf("Qual busca você deseja desativar?\n");

                int desativa;
                scanf("%d", &desativa);

                while (desativa < 1 || desativa > 5) {
                    printf("Digite um número entre 1 e 5\n");
                    scanf("%d", &desativa);
                }

                if (desativa == 1) {
                    qtde_nome = 0;
                    busca_nome = NULL;
                } else if (desativa == 2) {
                    qtde_lat = 0;
                    busca_lat = NULL;
                } else if (desativa == 3) {
                    qtde_long = 0;
                    busca_long = NULL;
                } else if (desativa == 4) {
                    qtde_uf = 0;
                    busca_uf = NULL;
                } else if (desativa == 5) {
                    qtde_ddd = 0;
                    busca_ddd = NULL;
                }
                break;

            case 7:
                printf("--------------------\n");
                printf("Intersecção das buscas:\n");

                imprime_tabela_municipios(intersec, qtde_total, hash_cod);
                break;

            default:
                hash_apaga(&hash_cod);
                avl_destroi(avl_nome.raiz);
                avl_destroi(avl_lat.raiz);
                avl_destroi(avl_long.raiz);
                avl_destroi(avl_uf.raiz);
                avl_destroi(avl_ddd.raiz);
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
