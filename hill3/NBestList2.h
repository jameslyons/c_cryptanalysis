/* author: James Lyons 2/2014
*/

#ifndef NBESTLISTH
#define NBESTLISTH

#define MAXLEN 25
#define TRUE 1
#define FALSE 0

typedef struct nbest__{
    struct nbest__ *next;
    struct nbest__ *prev;
    float score;
    int key[9];
} NBestList;

NBestList *newListElem(int *key, float score);
NBestList *nbest_add(NBestList *base,int *key, float score);
void printList(NBestList *base);
void freeList(NBestList *base);

#endif
