#include <stdlib.h>
#include <math.h>
#include "Item.h"
#include "BST.h"


typedef struct BSTnode* link;
struct BSTnode { Item item; link p; link l; link  r; int N; } ;

struct binarysearchtree { link head; int N; link z; };

link NEW(Item item, link p, link l, link r, int N) {
  link x = malloc(sizeof *x);
  x->item = item; x->p = p; x->l = l; x->r = r; x->N = N;
  return x;
}

BST BSTinit( ) {
  BST bst = malloc(sizeof *bst) ;
  bst->N = 0;
  bst->head = ( bst->z = NEW(ITEMsetvoid(), NULL, NULL, NULL, 0));
  return bst;
}

//int BSTcount(BST bst) {
//  return bst->N;
//}

//int BSTempty(BST bst) {
//  if ( BSTcount(bst) == 0)
//    return 1;
//  else
//    return 0;
//}

Item minR(link h, link z) {
  if (h == z)
    return ITEMsetvoid();
  if (h->l == z)
    return (h->item);
  return minR(h->l, z);
}

Item BSTmin(BST bst) {
  return minR(bst->head, bst->z);
}

Item maxR(link h, link z) {
  if (h == z)
    return ITEMsetvoid();
  if (h->r == z)
    return (h->item);
  return maxR(h->r, z);
}

Item BSTmax(BST bst) {
  return maxR(bst->head, bst->z);
}

Item searchR(link h, Key k, link z) {
  if (h == z)
    return ITEMsetvoid();
  if (KEYcompare(k, KEYget(h->item))==0)
    return h->item;
  if (KEYcompare(k, KEYget(h->item))==-1)
    return searchR(h->l, k, z);
  else
    return searchR(h->r, k, z);
  }

Item BSTsearch(BST bst, Key k) {
  return searchR(bst->head, k, bst->z);
}

link insertR(link h, Item x, link z) {
  if (h == z)
    return NEW(x, z, z, z, 1);
  if (ITEMless(x, h->item)) {
    h->l = insertR(h->l, x, z);
    h->l->p = h;
  }
  else {
    h->r = insertR(h->r, x, z);
    h->r->p = h;
  }
  (h->N)++;
  return h;
}

void BSTinsert_leafR(BST bst, Item x) {
  bst->head = insertR(bst->head, x, bst->z);
  bst->N++;
}

void BSTinsert_leafI(BST bst, Item x) {
  link p = bst->head, h = p;
  if (bst->head == bst->z) {
    bst->head = NEW(x, bst->z, bst->z, bst->z, 1);
    bst->N++;
    return;
  }
  while (h != bst->z) {
    p = h;
    h->N++;
    h = (ITEMless(x, h->item)) ? h->l : h->r;
  }
  h = NEW(x, p, bst->z, bst->z, 1);
  bst->N++;
  if (ITEMless(x, p->item))
    p->l = h;
  else
    p->r = h;
}

link rotR(link h) {
  link x = h->l;
  h->l = x->r;
  x->r->p = h;
  x->r = h;
  x->p = h->p;
  h->p = x;
  x->N = h->N;
  h->N = h->r->N + h->l->N +1;
  return x;
}

link rotL(link h) {
  link x = h->r;
  h->r = x->l;
  x->l->p = h;
  x->l = h;
  x->p = h->p;
  h->p = x;
  x->N = h->N;
  h->N = h->l->N + h->r->N +1;
  return x;
}

//  STinsert in root recursive version

link insertT(link h, Item x, link z) {
  if ( h == z)
    return NEW(x, z, z, z, 1);
  if (ITEMless(x, h->item)) {
    h->l = insertT(h->l, x, z);
    h = rotR(h);
    h->N++;
  }
  else {
    h->r = insertT(h->r, x, z);
    h = rotL(h);
    h->N++;
  }
  return h;
}

void BSTinsert_root(BST bst, Item x) {
  bst->head = insertT(bst->head, x, bst->z);
  bst->N++;
}

void sortinorderR(link h, link z) {
  if (h == z)
    return;
  sortinorderR(h->l, z);
  ITEMshow(h->item, stdout);
  sortinorderR(h->r, z);
}

void BSTsortinorder(BST bst) {
  sortinorderR(bst->head, bst->z);
}

void sortpostorderR(link h, link z) {
  if (h == z)
    return;
  sortpostorderR(h->l, z);
  sortpostorderR(h->r, z);
  ITEMshow(h->item, stdout);
}

void BSTsortpostorder(BST bst) {
  sortpostorderR(bst->head, bst->z);
}

void sortpreorderR(link h, link z) {
  if (h == z)
    return;
  ITEMshow(h->item, stdout);
  sortpreorderR(h->l, z);
  sortpreorderR(h->r, z);
}

void BSTsortpreorder(BST bst){
  sortpreorderR(bst->head, bst->z);
}

Item selectR(link h, int k, link z) {
  int t;
  if (h == z)
    return ITEMsetvoid();
  t = (h->l == z) ? 0 : h->l->N;
  if (t > k)
    return selectR(h->l, k, z);
  if (t < k)
    return selectR(h->r, k-t-1, z);
  return h->item;
}

Item BSTselect(BST bst, int k) {
  return selectR(bst->head, k, bst->z);
}


link partR(link h, int k) {
  int t  = h->l->N;
  if ( t > k) {
    h->l = partR(h->l, k);
    h = rotR(h);
  }
  if ( t < k) {
    h->r = partR(h->r, k-t-1);
    h = rotL(h);
  }
  return h;
}

link joinLR(link a, link b, link z) {
  if (b == z)
    return a;
  b = partR(b, 0);
  b->l = a;
  a->p = b;
  b->N = a->N + b->r->N +1;
  return b;
}

link deleteR(link h, Key k, link z) {
  link y, p;
  if (h == z)
    return z;
  if (KEYcompare(k, KEYget(h->item))==-1)
    h->l = deleteR(h->l, k, z);
  if (KEYcompare(KEYget(h->item), k)==-1)
    h->r = deleteR(h->r, k, z);
  (h->N)--;
  if (KEYcompare(k, KEYget(h->item))==0) {
    y = h;
    p = h->p;
    h = joinLR(h->l, h->r, z);
    h->p = p;
    free(y);
  }
  return h;
}

void BSTdelete(BST bst, Key k) {
  bst->head = deleteR(bst->head, k, bst->z);
  bst->N--;
}

Item searchSucc(link h, Key k, link z) {
  link p;
  if (h == z)
    return ITEMsetvoid();
  if (KEYcompare(k, KEYget(h->item))==0) {
    if (h->r != z)
      return minR(h->r, z);
    else {
      p = h->p;
      while (p != z && h == p->r) {
        h = p;
        p = p->p;
      }
      return p->item;
    }
  }
  if (KEYcompare(k, KEYget(h->item))==-1)
    return searchSucc(h->l, k, z);
  else
    return searchSucc(h->r, k, z);
}

Item BSTsucc(BST bst, Key k) {
  return searchSucc(bst->head, k, bst->z);
}

Item searchPred(link h, Key k, link z) {
  link p;
  if (h == z)
    return ITEMsetvoid();
  if (KEYcompare(k, KEYget(h->item))==0) {
    if (h->l != z)
      return maxR(h->l, z);
    else {
      p = h->p;
      while (p != z && h == p->l) {
        h = p;
        p = p->p;
      }
      return p->item;
    }
  }
  if (KEYcompare(k, KEYget(h->item))==-1)
    return searchPred(h->l, k, z);
  else

    return searchPred(h->r, k, z);
}

Item BSTpred(BST bst, Key k) {
  return searchPred(bst->head, k, bst->z);
}


// funzioni richieste
link BSTreadPreorderR(link head, link z, link p, FILE *fp)
{
    Item i;
    Key k;
    int n, left, rigth;

    k = KEYscan(fp);
    if (fscanf(fp, "%d%d%d", &n, &left, &rigth) == EOF) // condizione di terminazione
        return NULL;

    i = newITEM(k, n);

    head = NEW(i, p, z, z, left + rigth);

    if (left)
    {
        head->l = BSTreadPreorderR(head->l, z, head, fp);
        head->N += head->l->N;
    }


    if (rigth)
    {
        head->r = BSTreadPreorderR(head->r, z, head, fp);
        head->N += head->r->N;
    }

    return head;
}

BST BSTreadPreorder(FILE *in)
{
    BST tree = BSTinit();

    tree->head = BSTreadPreorderR(tree->head, tree->z, NULL, in);
    tree->N = tree->head->N + 1;

    return tree;
}

void BSTwritePreorderR(link h, link z, FILE *fp)
{
    if (h == z)
        return;

    ITEMshow(h->item, fp);
    BSTwritePreorderR(h->l, z, fp);
    BSTwritePreorderR(h->r, z, fp);
}

void BSTwritePreorder(FILE *out, BST tree)
{
    BSTwritePreorderR(tree->head, tree->z, out);
}

int BSTcountR(link head, link z, int l1, int l2, int curL)
{
    int count;

    if (head->l != z && head->r != z    // se ha due figli
        && curL >= l1 && curL <= l2)    // e sono nel range
        count = 1;
    else
        count = 0;

    if (head->l != z)   // condizione terminazione
        count += BSTcountR(head->l, z, l1, l2, curL + 1);

    if (head->r != z)   // condizione terminazione
        count += BSTcountR(head->r, z, l1, l2, curL + 1);

    // se sono tra i due livelli tengo conto di count
    return count;
}

int BSTcount(BST tree, int l1, int l2)
{
    return BSTcountR(tree->head, tree->z, l1, l2, 0);
}

int searchKey(link head, link z, Key k, int step)
{
    Key currentKey;

    if (head == z)
        return -1;

    currentKey = KEYget(head->item);

    // scendo a sx
    if (KEYcompare(k, currentKey) < 0)
    {
        return searchKey(head->l, z, k, step + 1);
    }
    // scendo a dx
    else if (KEYcompare(k, currentKey) > 0)
    {
        return searchKey(head->r, z, k, step + 1);
    }
    // trovato!
    else
        return step;
}

link longestFather(link head, link z, Key k1, Key k2)
{
    Key currentKey;

    if (head == z)
        return NULL;

    currentKey = KEYget(head->item);

    if (KEYcompare(k1, currentKey) == 0 || KEYcompare(k2, currentKey) == 0  // l'antenato è una delle due chiavi
        || KEYcompare(k1, currentKey) * KEYcompare(k2, currentKey) < 0)     // l'antenato è un altro nodo
            return head;


    // se sono entrambe a sinistra del nodo corrente
    if (KEYcompare(k1, currentKey) < 0 || KEYcompare(k1, currentKey) < 0)
        return longestFather(head->l, z, k1, k2);
    // se sono entrambe a destra del nodo corrente
    else
        return longestFather(head->r, z, k1, k2);
}

int BSTdistance(BST tree, Key k1, Key k2)
{
    link father;

    // trovo prima l'antenato comune
    father = longestFather(tree->head, tree->z, k1, k2);


    // a partire dall'antenato comune
    // trovo i sotto-percorsi dx e sx e li sommo
    int n1 = searchKey(father, tree->z, k1, 0);
    int n2 = searchKey(father, tree->z, k2, 0);

    // controlli
    if (n1 == -1)
        printf("%s NON presente nell'albero\n", k1);
    if (n2 == -1)
        printf("%s NON presente nell'albero\n", k2);

    return n1 + n2;
}

link BSTmirrorR(link head, link z)
{
    link tmp;

    // condizione di terminazione
    if (head == z)
        return z;

    tmp = head->l;
    head->l = head->r;
    head->r = tmp;

    head->l = BSTmirrorR(head->l, z);
    head->r = BSTmirrorR(head->r, z);

    return head;
}

void BSTmirror(BST tree)
{
    tree->head = BSTmirrorR(tree->head, tree->z);
}

void freeR(link h, link z) {
  if (h == z)
    return;
  freeR(h->l, z);
  freeR(h->r, z);
  ITEMfree(h->item);
  free(h);
}

void BSTfree(BST tree)
{
    freeR(tree->head, tree->z);
    free(tree);
}
