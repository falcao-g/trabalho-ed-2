#ifndef __AVL__
#define __AVL__
typedef struct _item {
    void *reg;
    struct _item *prox;
} titem;

typedef struct _node {
    titem item;
    struct _node *pai;
    struct _node *esq;
    struct _node *dir;
    int h;
} tnode;

void avl_insere_node(tnode **parv, tnode *pai, titem reg);
void avl_insere(tnode **parv, titem reg);
void avl_remove(tnode **parv, titem reg);
void avl_destroi(tnode *parv);

void _rd(tnode **pparv);
void _re(tnode **pparv);
void _avl_rebalancear(tnode **pparv);

#endif
