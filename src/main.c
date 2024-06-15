#include <stdio.h>
#include <stdlib.h>
#include "../include/avl.h"

int main(void)
{
    tnode *root = NULL;
    avl_insere(&root, 1);
    avl_insere(&root, 2);

    printf("%d\n", root->item);
    printf("%d\n", root->dir->item);
    printf("%d\n", root->dir->pai->item);

    return 0;
}
