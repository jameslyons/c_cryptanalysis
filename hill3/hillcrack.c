#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "NBestList2.h"
#include "scoreText.h"

float monogram[] = {-2.45904, -4.13217, -3.4532, -3.25161, -2.11225, -3.82515, -3.86976, -3.00463, -2.61386, -6.1203, -4.8175, -3.16855, -3.67841, -2.63496, -2.59464, -3.87948, -6.8683, -2.75944, -2.69886, -2.41484, -3.61876, -4.54752, -4.00339, -6.25882, -4.06206, -6.77887};
void hill3_encipher(char *ctext, char *ptext, int *key);
void invert_hill3_key(int *key, int *invkey);
int inverse_mod26(int d);
void hill3_crack(char *ctext, int *retkey );

int main(int argc, char *argv[]){
    char str[] = "DEFENDTHEEASTWALLOFTHECASTLEATDAWNAGAINSTINVADERS";
    char *ctext = malloc(sizeof(char)*(strlen(str)+1));
    char *ptext = malloc(sizeof(char)*(strlen(str)+1));
    
    int key[] = {5, 17, 4, 15, 14,1,11,3,24};
    int invkey[] = {0,0,0,0,0,0,0,0,0};
    
    hill3_encipher(ctext,str,key);
    invert_hill3_key(key,invkey);
    hill3_encipher(ptext,ctext,invkey);
    hill3_crack(ctext,invkey);
    
    printf("%s\n",str);
    printf("%s\n",ctext);    
    printf("%s\n",ptext);    
    
}

NBestList *base = NULL;
NBestList *base2 = NULL;

void hill3_crack(char *ctext, int *retkey ){
    int *newkey;
    char *temp = malloc(strlen(ctext)+1);
    int i1,i2,i3,k;
    float score, bestscore;
    for(i1=0;i1<26;i1++){
      for(i2=0;i2<26;i2++){
        for(i3=0;i3<23;i3++){        
            if (i1%2==0 && i2%2==0 && i3%2==0) continue;
            if (i1%13==0 && i2%13==0 && i3%13==0) continue;            
            newkey = malloc(sizeof(int)*9);
            newkey[0] = i1;
            newkey[1] = i2;
            newkey[2] = i3;
            hill3_encipher(temp,ctext,newkey);
            score =0;
            for(k=0;k<strlen(ctext);k+=3){
                score += -monogram[temp[k]-'A'];
            }
            base = nbest_add(base,newkey,score);
        }
    }
    /* we now have the 100 most likely top rows, time to determine the middle row */
    NBestList *elem;
    for(elem = base; elem != NULL; elem = elem->next){
        newkey = malloc(sizeof(int)*9);
        newkey[0] = elem->key[0];
        newkey[1] = elem->key[1];
        newkey[2] = elem->key[2];                
        for(i1=0; i1<26;i1++){
          for(i2=0; i2<26;i2++){
            for(i3=0;i3<23;i3++){        
                if (i1%2==0 && i2%2==0 && i3%2==0) continue;
                if (i1%13==0 && i2%13==0 && i3%13==0) continue;               
                newkey[3] = i1;
                newkey[4] = i2;
                newkey[5] = i3;
                hill3_encipher(temp,ctext,newkey);
                score = -scoreTextQgram(temp,strlen(temp));
                base2 = nbest_add(base2,newkey,score);
            }
        }
    }
    /* we now have the 100 most likely middle rows, time to determine the final row */
    int bestkey[9];
    bestscore = 99e99;
    for(elem = base; elem != NULL; elem = elem->next){
        newkey = malloc(sizeof(int)*9);
        newkey[0] = elem->key[0];
        newkey[1] = elem->key[1];
        newkey[2] = elem->key[2];                
        for(i1=0; i1<26;i1++){
          for(i2=0; i2<26;i2++){
            for(i3=0;i3<23;i3++){        
                if (i1%2==0 && i2%2==0 && i3%2==0) continue;
                if (i1%13==0 && i2%13==0 && i3%13==0) continue;               
                newkey[3] = i1;
                newkey[4] = i2;
                newkey[5] = i3;
                hill3_encipher(temp,ctext,newkey);
                score = -scoreTextQgram(temp,strlen(temp));
                base2 = nbest_add(base2,newkey,score);
            }
        }
    }    
    hill3_encipher(temp,ctext,bestkey);
    printf("%s\n",temp);
}

void hill2_encipher(char *ctext, char *ptext, int *key){
    int i;
    int L = strlen(ptext);
    if(L%2 == 1){ 
        printf("Length of plaintext is not even, dropping last character.\n");
        L-=1;
    }
    for(i=0; i<L; i+=2){
        ctext[i]   = ((ptext[i]-'A')*key[0] + (ptext[i+1]-'A')*key[1])%26 + 'A';
        ctext[i+1] = ((ptext[i]-'A')*key[2] + (ptext[i+1]-'A')*key[3])%26 + 'A';
    } 
    ctext[L] = '\0';
}

void invert_hill2_key(int *key, int *invkey){
    int idet = inverse_mod26(key[0]*key[3] - key[1]*key[2]);   
    if (idet == 0) printf("Warning: key {%d %d; %d %d} is not invertible, choose a different key.\n",key[0],key[1],key[2],key[3]);
    invkey[0] = (idet*key[3])%26;
    invkey[1] = (idet*(26-key[1]))%26;
    invkey[2] = (idet*(26-key[2]))%26;
    invkey[3] = (idet*key[0])%26;          
}

int inverse_mod26(int d){
    d = (d+26)%26;
    switch ( d ) {
      case 1: return 1; 
      case 3: return 9; 
      case 5: return 21; 
      case 7: return 15; 
      case 9: return 3; 
      case 11: return 19;
      case 15: return 7; 
      case 17: return 23; 
      case 19: return 11; 
      case 21: return 5; 
      case 23: return 17; 
      case 25: return 25; 
      default: return 0; 
    }
}


