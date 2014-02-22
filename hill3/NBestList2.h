/* author: James Lyons
Aug 2012
this code is from http://www.practicalcryptography.com/cryptanalysis/breaking-machine-ciphers/cryptanalysis-enigma-part-2/
*/

#ifndef NBESTLISTH
#define NBESTLISTH

#define MAXLEN 100
#define TRUE 1
#define FALSE 0

typedef struct nbest__{
    struct nbest__ *next;
    struct nbest__ *prev;
    float score;
    int key[4];
} NBestList;

NBestList *newListElem(int *key, float score);
NBestList *nbest_add(NBestList *base,int *key, float score);
void printList(NBestList *base);
void freeList(NBestList *base);

#endif
