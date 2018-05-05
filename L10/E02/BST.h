#ifndef BST_H_INCLUDED
#define BST_H_INCLUDED

typedef struct  binarysearchtree *BST;

BST	    BSTinit() ;
int		BSTcount(BST) ;
int     BSTempty(BST) ;
Utente*    BSTmin(BST) ;
Utente*    BSTmax(BST) ;
void 	BSTinsert_leafI(BST,Utente*) ;
void 	BSTinsert_leafR(BST,Utente*) ;
void 	BSTinsert_root(BST,Utente*) ;
Utente*	BSTsearch(BST,Key) ;
void	BSTdelete(BST,Key) ;
Utente* 	BSTselect(BST,int) ;
void	BSTsortinorder(BST) ;
void	BSTsortpreorder(BST) ;
void	BSTsortpostorder(BST) ;
Utente*	BSTsucc(BST,Key) ;
Utente*	BSTpred(BST,Key) ;

#endif // BST_H_INCLUDED
