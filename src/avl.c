#include "../include/avl.h"

#include <stdio.h>
#include <stdlib.h>

int max(int a, int b) {
    return a > b ? a : b;
}

int altura(tnode *arv) {
    int ret;
    if (arv == NULL) {
        ret = -1;
    } else {
        ret = arv->h;
    }
    return ret;
}

void avl_constroi(tarv *parv, int type, double (*cmp)(void *, void *, int)) {
    parv->raiz = NULL;
    parv->type = type;
    parv->cmp = cmp;
}

void _avl_insere_lista(titem *pitem, void *item) {
    if (pitem->prox == NULL) {
        pitem->prox = (titem *)malloc(sizeof(titem));
        pitem->prox->reg = item;
        pitem->prox->prox = NULL;
    } else {
        _avl_insere_lista(pitem->prox, item);
    }
}

void _avl_insere_node(tarv *parv, tnode **ppnode, tnode *pai, void *item) {
    if (*ppnode == NULL) {
        *ppnode = (tnode *)malloc(sizeof(tnode));
        (*ppnode)->pai = pai;
        (*ppnode)->item.reg = item;
        (*ppnode)->item.prox = NULL;
        (*ppnode)->esq = NULL;
        (*ppnode)->dir = NULL;
        (*ppnode)->h = 0;
    } else if (parv->cmp((*ppnode)->item.reg, item, parv->type) > 0) {
        _avl_insere_node(parv, &(*ppnode)->esq, *ppnode, item);
    } else if (parv->cmp((*ppnode)->item.reg, item, parv->type) < 0) {
        _avl_insere_node(parv, &(*ppnode)->dir, *ppnode, item);
    } else {
        // se achou o nó, insere na lista encadeada
        _avl_insere_lista(&(*ppnode)->item, item);
        return;
    }
    (*ppnode)->h = max(altura((*ppnode)->esq), altura((*ppnode)->dir)) + 1;
    _avl_rebalancear(ppnode);
}

void avl_insere(tarv *parv, void *item) {
    _avl_insere_node(parv, &parv->raiz, NULL, item);
}

void _rd(tnode **ppnode) {
    tnode *y = *ppnode;
    tnode *x = y->esq;
    tnode *A = x->esq;
    tnode *B = x->dir;
    tnode *C = y->dir;

    y->esq = B;
    x->dir = y;
    *ppnode = x;

    if (B != NULL) {
        B->pai = y;
    }
    x->pai = y->pai;
    y->pai = x;

    y->h = max(altura(B), altura(C)) + 1;
    x->h = max(altura(A), altura(y)) + 1;
}

void _re(tnode **ppnode) {
    tnode *x = *ppnode;
    tnode *y = x->dir;
    tnode *A = x->esq;
    tnode *B = y->esq;
    tnode *C = y->dir;

    x->dir = B;
    y->esq = x;
    *ppnode = y;

    if (B != NULL) {
        B->pai = x;
    }
    y->pai = x->pai;
    x->pai = y;

    x->h = max(altura(A), altura(B)) + 1;
    y->h = max(altura(x), altura(C)) + 1;
}

void _avl_rebalancear(tnode **ppnode) {
    int fb;
    int fbf;
    tnode *filho;
    fb = altura((*ppnode)->esq) - altura((*ppnode)->dir);

    if (fb == -2) {
        filho = (*ppnode)->dir;
        fbf = altura(filho->esq) - altura(filho->dir);
        if (fbf <= 0) {  // Caso 1  --> ->
            _re(ppnode);
        } else {  // Caso 2  --> <-
            _rd(&(*ppnode)->dir);
            _re(ppnode);
        }
    } else if (fb == 2) {
        filho = (*ppnode)->esq;
        fbf = altura(filho->esq) - altura(filho->dir);
        if (fbf >= 0) {  // Caso 3  <-- <-
            _rd(ppnode);
        } else {  // Caso 4  <-- ->
            _re(&(*ppnode)->esq);
            _rd(ppnode);
        }
    }
}

tnode *avl_busca_node(tarv *parv, tnode *pnode, void *reg) {
    if (parv->cmp(pnode->item.reg, reg, parv->type) > 0) {
        if (pnode->esq == NULL) {
            return pnode;
        }

        return avl_busca_node(parv, pnode->esq, reg);
    } else if (parv->cmp(pnode->item.reg, reg, parv->type) < 0) {
        if (pnode->dir == NULL) {
            return pnode;
        }

        return avl_busca_node(parv, pnode->dir, reg);
    } else {
        return pnode;
    }
}

tnode *avl_busca(tarv *parv, void *reg) {
    return avl_busca_node(parv, parv->raiz, reg);
}

tnode *percorre_esq(tnode *arv) {
    tnode *aux = arv;
    if (aux->esq == NULL) {
        return arv;
    } else {
        while (aux->esq->esq != NULL)
            aux = aux->esq;
        return aux->esq;
    }
}

tnode *sucessor(tnode *arv) {
    if (arv->dir != NULL) {
        return percorre_esq(arv->dir);
    }

    tnode *pai = arv->pai;
    while (pai != NULL && arv == pai->dir) {
        arv = pai;
        pai = pai->pai;
    }
    return pai;
}

void avl_destroi(tnode *pnode) {
    if (pnode != NULL) {
        avl_destroi(pnode->esq);
        avl_destroi(pnode->dir);
        free(pnode);
    }
}
