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

typedef struct {
    tnode *raiz;
    double (*cmp)(void *, void *);
} tarv;

void avl_constroi(tarv *parv, double (*cmp)(void *, void *));
void avl_insere(tarv *parv, void *reg);
void avl_remove(tnode **parv, titem reg);
void avl_destroi(tnode *parv);
tnode *sucessor(tnode *arv);

void _rd(tnode **pparv);
void _re(tnode **pparv);
void _avl_rebalancear(tnode **pparv);
void _avl_insere_node(tarv *parv, tnode **ppnode, tnode *pai, void *reg);

#endif
