#include <stdio.h>
#include <stdlib.h>

#include "../include/avl.h"

int main(void) {
    tnode *root = NULL;
    titem *item = (titem *)malloc(sizeof(titem));
    item->reg = 4;
    item->prox = NULL;

    titem *item2 = (titem *)malloc(sizeof(titem));
    item2->reg = 3;
    item2->prox = NULL;

    titem *item3 = (titem *)malloc(sizeof(titem));
    item3->reg = 2;
    item3->prox = NULL;

    titem *item4 = (titem *)malloc(sizeof(titem));
    item4->reg = 5;
    item4->prox = NULL;

    titem *item5 = (titem *)malloc(sizeof(titem));
    item5->reg = 6;
    item5->prox = NULL;

    avl_insere(&root, *item);
    avl_insere(&root, *item2);
    avl_insere(&root, *item4);
    avl_insere(&root, *item3);
    avl_insere(&root, *item5);

    printf("%d\n", root->item);
    printf("%d\n", root->dir->item);
    printf("%d\n", root->dir->dir->item);
    printf("%d\n", root->esq->item);
    printf("%d\n", root->esq->esq->item);

    printf("%d\n", sucessor(root->esq)->item.reg);

    return 0;
}
