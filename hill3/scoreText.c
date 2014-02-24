/* author: James Lyons
Aug 2012
this code is from http://www.practicalcryptography.com/cryptanalysis/breaking-machine-ciphers/cryptanalysis-enigma/
*/
#include "scoreText.h"
#include "qgr.h"
#include "bigr.h"

extern float qgram[];
extern float bigram[];

int t26[] = {0, 26, 52, 78, 104, 130, 156, 182, 208, 234, 260, 286, 312, 338, 364, 390, 416, 442, 468, 494, 520, 546, 572, 598, 624, 650};
int t676[] = {0, 676, 1352, 2028, 2704, 3380, 4056, 4732, 5408, 6084, 6760, 7436, 8112, 8788, 9464, 10140, 10816, 11492, 12168, 12844, 13520, 14196, 14872, 15548, 16224, 16900};
int t17576[]={0, 17576, 35152, 52728, 70304, 87880, 105456, 123032, 140608, 158184, 175760, 193336, 210912, 228488, 246064, 263640, 281216, 298792, 316368, 333944, 351520, 369096, 386672, 404248, 421824, 439400};

// assumes that text consists only of uppercase letters(no punctuation or spaces)
double scoreTextQgram(char *text,int len){
    int i;
    double score = 0;
    for (i=0;i<len-3;i++){
        score += qgram[t17576[(int)text[i]] + t676[(int)text[i+1]] + t26[(int)text[i+2]] + text[i+3]];
    }
    return score;
}

double scoreTextBigram(char *text,int len){
    int i;
    double score = 0;
    for (i=0;i<len-1;i++){
        score += bigram[t26[(int)text[i]] + text[i+1]];
    }
    return score;
}
