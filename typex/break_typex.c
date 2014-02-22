/* author: James Lyons
Aug 2012
use e.g. http://practicalcryptography.com/ciphers/mechanical-era/enigma/ to generate messages
this code is from http://www.practicalcryptography.com/cryptanalysis/breaking-machine-ciphers/cryptanalysis-enigma-part-2/
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "typex_perms.h"
#include "NBestList.h"
#include "scoreText.h"


TypexKey *break_typex(char* ctext);
float entropy_score(char *text);

// cipher text variable must be all capitals, with no spacing or punctuation, use e.g. http://practicalcryptography.com/ciphers/mechanical-era/enigma/
// to generate messages. This version can break messages with plugs, but both the original plaintext and corresponding ciphertext are required.
// This program determines the key used. 'otext' is the original plaintext, 'ctext' is the ciphertext
char ctext[] = "VDGTQAGBWMMIFJBKVZAPJUZCZQXN";
    
/******************************************************************
main - cracks the typex ciphertext stored in ctext, prints the result.
*******************************************************************/
int main(int argc, char *argv[]){
    char *ptext = malloc(sizeof(char)*(strlen(ctext)+1));
    /*char *ptext1 = malloc(sizeof(char)*(strlen(ctext)+1));
    TypexKey *ref = malloc(sizeof(TypexKey));
    initTypexKey(ref);
    //ref = break_typex(ctext);
    //printf("final key: \n");
    //printTypexKey(ref);
    typex(ref,ctext,ptext);     
    //initTypexKey(ref);   
    //typex(ref,ptext,ptext1);     
    printf("decryption: %s\n",ptext);
    free(ptext); 
    free(ref);*/
    TypexKey *ref;
    ref = break_typex(ctext);
    printf("final key: \n");
    printTypexKey(ref);
    typex(ref,ctext,ptext);     
    printf("decryption: %s\n",ptext);
    free(ptext); 
    free(ref);
}

// All possible permutations of 5 rotors, there are 60 total
extern char perms[][5];
extern int NPERMS;

NBestList *base = NULL;

/******************************************************************
Given a piece of ciphertext 'ctext', return the typex decryption key
******************************************************************/
TypexKey *break_typex(char* ctext){
    int i,j;
    char *ptext = malloc(sizeof(char)*(strlen(ctext)+1));
    TypexKey key;
    TypexKey *bestkey = malloc(sizeof(TypexKey));
    TypexKey store;
    char ind1,ind2,ind3=0,ind4=0,ind5=0;
    float bestscore,score;
    initTypexKey(bestkey);
    printf("searching for rotors: ");
    for(i=0;i<NPERMS;i++){
        for(ind1=0;ind1<26;ind1++){
          for(ind2=0;ind2<26;ind2++){
            for(ind3=0;ind3<26;ind3++){
              for(ind4=0;ind4<26;ind4++){
                for(ind5=0;ind5<26;ind5++){
                    key = *bestkey;
                    key.rotors[0] = perms[i][0];
                    key.rotors[1] = perms[i][1];
                    key.rotors[2] = perms[i][2];
                    key.rotors[3] = perms[i][3];
                    key.rotors[4] = perms[i][4];
                    key.indicator[0] = ind1; 
                    key.indicator[1] = ind2;        
                    key.indicator[2] = ind3;
                    key.indicator[3] = ind4;
                    key.indicator[4] = ind5;                 
                    store = key; typex(&store,ctext,ptext);
                    score = -scoreTextQgram(ptext,strlen(ptext));
                    base = nbest_add(base,&key,score);
                }
              }
            }
          }
        }
        printf("%d\n",i); fflush(stdout);
    }
    // we have the optimal indicators and rotors, search for the optimal ringsettings
    store = base->key;
    *bestkey = store;
    free(ptext);
    freeList(base);
    return bestkey;
}


