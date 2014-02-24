#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simplesub.h"
#include "scoreText.h"

static float findaKey(char *text,int len, char* maxKey);
char *simplesubEncipher(char *key, char *in,char *out, int len);
static char *shuffleKey(char *in);

int main(int argc, char *argv[]){
    char ctext[] = "HPHYJTYOPGYJJMGVJTRMRMTZTRPJZHEPMJCITHEPJUPHYZJCITRJQVHPMPMJRGZZJYCGEQHEWIJPPJZHEPMJRHCMJZTICMTSJP";
    char ptext[] = "qqqqqqqqqq                                                                                        ";
    char *key = simpleSubCrack(ctext,2000);
    simplesubEncipher(key, ctext,ptext, strlen(ctext));
    printf("ptext: %s\n",ptext);
    return 0;
}

char *simpleSubCrack(char *text, int N){
    char key[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char *bestKey = malloc(sizeof(char)*27);
    int i,len = strlen(text);
    double score,maxscore=-99e99;
    for(i=0;i<N;i++){
        shuffleKey(key);
        score = findaKey(text,len,key);
        if(score > maxscore){
            maxscore = score;
            strcpy(bestKey,key);   
            //printf("maxscore: %f, %d\n",maxscore,i);
        }
        //if(i%100==0) printf("i=%d\n",i);
    }
    return bestKey;
}

static float findaKey(char *text,int len, char* maxKey){
    int i,j;
    int maxi,maxj;
    char temp, *deciphered = malloc(sizeof(char) * len);
    char testKey[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    double maxmaxscore=-99e99,maxscore,score;
    while(1){
        simplesubEncipher(maxKey,text,deciphered,len);
        maxscore = scoreTextQgram(deciphered,len);
        maxi = 0;
        maxj = 0;
        // determine which swap results in highest fitness
        for(i=0;i<26;i++){
            for(j=i;j<26;j++){
                strcpy(testKey,maxKey);
                temp = testKey[i];
                testKey[i]= testKey[j];
                testKey[j] = temp;
                simplesubEncipher(testKey,text,deciphered,len);
                score = scoreTextQgram(deciphered,len);
                if (score > maxscore){
                    maxscore = score;
                    maxi = i;
                    maxj = j;
                }
            }
        }
        if (maxi == maxj) break;    
        if (maxscore > maxmaxscore){
            maxmaxscore = maxscore;
            temp = maxKey[maxi];
            maxKey[maxi] = maxKey[maxj];
            maxKey[maxj] = temp;
            //printf("    maxscore: %f\n",maxscore);
        }
    }
    free(deciphered);
    return maxmaxscore;
}


char *simplesubEncipher(char *key, char *in,char *out, int len){
    int i;

    for (i = 0; i < len; i++){
        if(isalpha(in[i])) out[i] = key[in[i]-'A']; 
        else out[i] = in[i];
    }
    out[i]='\0';
    return out;
}

static char *shuffleKey(char *in){
    int i,j;
    char temp;
    for (j = 0; j < 26; j++){
        i = rand() % 26;
        temp = in[j];
        in[j] = in[i];
        in[i] = temp;
    }
    return in;
} 
