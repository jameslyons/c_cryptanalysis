/* author: James Lyons 2/2014
*/
#include "NBestList2.h"
#include <stdlib.h>
#include <stdio.h>

NBestList *newListElem(int *key, float score){
    NBestList *ret = malloc(sizeof(NBestList));
    ret->score = score;
    ret->next = NULL;
    ret->prev = NULL;
    ret->key[0] = key[0];
    ret->key[1] = key[1];
    ret->key[2] = key[2];
    ret->key[3] = key[3];   
    ret->key[4] = key[4];   
    ret->key[5] = key[5];   
    ret->key[6] = key[6];   
    ret->key[7] = key[7];   
    ret->key[8] = key[8];   
    return ret;
}

NBestList *nbest_add(NBestList *base,int *key, float score){
    NBestList *elem = newListElem(key,score);
    NBestList *current;
    int i,added = FALSE;
    if(base==NULL){ 
        base = elem;
        return base;
    }
    current = base;
    i = 0;
    while(current != NULL){
        //printf("@ %d, %f\n",i,current->score);
        if((score < current->score) && (added==FALSE)){
            if(current == base){
                base->prev = elem;
                elem->next = base;
                base = elem;
            }else{
                elem->prev = current->prev;
                elem->prev->next = elem;
                elem->next = current; 
                current->prev = elem;            
            }
            added = TRUE;
        }
        if(i >= MAXLEN){
            if(!added) free(elem);
            else{ 
                current->prev->next = NULL;
                freeList(current);
            }
            break;
        }
        i++;
        if((current->next == NULL) && (added==FALSE)){
            current->next = elem;
            elem->prev = current;
            break;
        }
        current = current->next;
    }
    return base;
}

/*************************************************
free a NBestList list, given an element, it frees the current element
and everything that follows it.
*************************************************/
void freeList(NBestList *base){
    NBestList *current = base;
    if(base == NULL) return;
    while(current->next != NULL){
        current = current->next;
        free(current->prev);
    }
    free(current);
}


/*************************************************
prints a NBestList list, useful for debugging
*************************************************/
void printList(NBestList *base){
    NBestList *current = base;
    int i = 0;
    while(current != NULL){
        printf("> %d, %f\n",i++,current->score);
        current = current->next;
    }
}
