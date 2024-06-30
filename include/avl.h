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
    int type;
    double (*cmp)(void *, void *, int);
} tarv;

void avl_constroi(tarv *parv, int type, double (*cmp)(void *, void *, int));
void avl_insere(tarv *parv, void *reg);
tnode *avl_busca(tarv *parv, void *reg);
tnode *avl_busca_node(tarv *parv, tnode *pnode, void *reg);
void avl_destroi(tnode *parv);
tnode *sucessor(tnode *arv);

void _avl_insere_lista(titem *pitem, void *item);
void _rd(tnode **pparv);
void _re(tnode **pparv);
void _avl_rebalancear(tnode **pparv);
void _avl_insere_node(tarv *parv, tnode **ppnode, tnode *pai, void *reg);

#endif
