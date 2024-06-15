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

int cmp(titem a, titem b) {
    return a.reg - b.reg;
}

void _avl_insere_node(tnode **parv, tnode *pai, titem item) {
    if (*parv == NULL) {
        *parv = (tnode *)malloc(sizeof(tnode));
        (*parv)->pai = pai;
        (*parv)->item = item;
        (*parv)->esq = NULL;
        (*parv)->dir = NULL;
        (*parv)->h = 0;
    } else if (cmp((*parv)->item, item) > 0) {
        _avl_insere_node(&(*parv)->esq, *parv, item);
    } else if (cmp((*parv)->item, item) < 0) {
        _avl_insere_node(&(*parv)->dir, *parv, item);
    } else {
        _avl_insere_node(&(*parv)->item.prox, *parv, item);
        return;
    }
    (*parv)->h = max(altura((*parv)->esq), altura((*parv)->dir)) + 1;
    _avl_rebalancear(parv);
}

void avl_insere(tnode **parv, titem item) {
    _avl_insere_node(parv, NULL, item);
}

void _rd(tnode **parv) {
    tnode *y = *parv;
    tnode *x = y->esq;
    tnode *A = x->esq;
    tnode *B = x->dir;
    tnode *C = y->dir;

    y->esq = B;
    x->dir = y;
    *parv = x;
    y->h = max(altura(B), altura(C)) + 1;
    x->h = max(altura(A), altura(y)) + 1;
}

void _re(tnode **parv) {
    tnode *x = *parv;
    tnode *y = x->dir;
    tnode *A = x->esq;
    tnode *B = y->esq;
    tnode *C = y->dir;

    x->dir = B;
    y->esq = x;
    *parv = y;
    x->h = max(altura(A), altura(B)) + 1;
    y->h = max(altura(x), altura(C)) + 1;
}

void _avl_rebalancear(tnode **parv) {
    int fb;
    int fbf;
    tnode *filho;
    fb = altura((*parv)->esq) - altura((*parv)->dir);

    if (fb == -2) {
        filho = (*parv)->dir;
        fbf = altura(filho->esq) - altura(filho->dir);
        if (fbf <= 0) { /* Caso 1  --> ->*/
            _re(parv);
        } else { /* Caso 2  --> <-*/
            _rd(&(*parv)->dir);
            _re(parv);
        }
    } else if (fb == 2) {
        filho = (*parv)->esq;
        fbf = altura(filho->esq) - altura(filho->dir);
        if (fbf >= 0) { /* Caso 3  <-- <-*/
            _rd(parv);
        } else { /* Caso 4  <-- ->*/
            _re(&(*parv)->esq);
            _rd(parv);
        }
    }
}

tnode *sucessor(tnode *arv) {
    tnode *aux = arv;
    if (aux->dir == NULL) {
        //  we need to go up until we find a parent that is a left child
        while (aux != NULL && aux->pai != NULL && aux->pai->esq != aux) {
            aux = aux->pai;
        }

        if (aux->pai == NULL)
            return aux;
        else
            return aux->pai;
    } else {
        while (aux->dir->esq != NULL)
            aux = aux->dir;
        return aux->dir;
    }
}

// void avl_remove(tnode **parv, titem reg)
// {
//     int cmp;
//     tnode *aux;
//     tnode **sucessor;
//     if (*parv != NULL)
//     {
//         cmp = (*parv)->item - reg;
//         if (cmp > 0)
//         { /* ir esquerda*/
//             avl_remove(&((*parv)->esq), reg);
//         }
//         else if (cmp < 0)
//         { /*ir direita*/
//             avl_remove(&((*parv)->dir), reg);
//         }
//         else
//         { /* ACHOU  */
//             if ((*parv)->esq == NULL && (*parv)->dir == NULL)
//             { /* no folha */
//                 free(*parv);
//                 *parv = NULL;
//             }
//             else if ((*parv)->esq == NULL || (*parv)->dir == NULL)
//             { /* tem um filho*/
//                 aux = *parv;
//                 if ((*parv)->esq == NULL)
//                 {
//                     *parv = (*parv)->dir;
//                 }
//                 else
//                 {
//                     *parv = (*parv)->esq;
//                 }
//                 free(aux);
//             }
//             else
//             { /* tem dois filhos */
//                 sucessor = percorre_esq(&(*parv)->dir);
//                 (*parv)->item = (*sucessor)->item;
//                 avl_remove(&(*parv)->dir, (*sucessor)->item);
//             }
//         }
//         if (*parv != NULL)
//         {
//             (*parv)->h = max(altura((*parv)->esq), altura((*parv)->dir))
//             + 1; _avl_rebalancear(parv);
//         }
//     }
// }

void avl_destroi(tnode *parv) {
    if (parv != NULL) {
        avl_destroi(parv->esq);
        avl_destroi(parv->dir);
        free(parv);
    }
}
