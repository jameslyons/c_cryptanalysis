#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "NBestList2.h"
#include "scoreText.h"

float monogram[] = {-2.45904, -4.13217, -3.4532, -3.25161, -2.11225, -3.82515, -3.86976, -3.00463, -2.61386, -6.1203, -4.8175, -3.16855, -3.67841, -2.63496, -2.59464, -3.87948, -6.8683, -2.75944, -2.69886, -2.41484, -3.61876, -4.54752, -4.00339, -6.25882, -4.06206, -6.77887};
void hill3_encipher(char *ctext, char *ptext, int *key, int L);
void invert_hill3_key(int *key, int *invkey);
int inverse_mod26(int d);
void hill3_crack(char *ctext, int *retkey , int L);

int main(int argc, char *argv[]){
    //char str[] = "DEFENDTHEEASTWALLOFTHECASTLEATDAWNAGAINSTINVADERSEE";
    char str[] = "THISPOSTWILLDISCUSSTHEAUTOCORRELATIONORYULEWALKERMETHODOFCOMPUTING";
    int i;
    int L = strlen(str);
    for (i=0; i<L; i++) str[i] -= 'A';
    char *ctext = malloc(sizeof(char)*(L+1));
    char *ptext = malloc(sizeof(char)*(L+1));
    
    int key[] = {11, 13, 3, 5, 17, 6, 1, 6, 19};
    int invkey[] = {0,0,0,0,0,0,0,0,0};
    
    hill3_encipher(ctext,str,key,L);
    invert_hill3_key(key,invkey);
    hill3_encipher(ptext,ctext,invkey,L);
    hill3_crack(ctext,invkey,L);
    
    for (i=0; i<L; i++) str[i] += 'A';
    for (i=0; i<L; i++) ctext[i] += 'A';
    for (i=0; i<L; i++) ptext[i] += 'A';
    printf("str: %s\n",str);
    printf("ctext: %s\n",ctext);    
    printf("ptext: %s\n",ptext);    
    return 0;
}

NBestList *base = NULL;
NBestList *base2 = NULL;

void hill3_crack(char *ctext, int *retkey , int L){
    int *newkey;
    char *temp = malloc(L+1);
    int i1,i2,i3,k;
    float score, bestscore;
    for(i1=0;i1<26;i1++){
      for(i2=0;i2<26;i2++){
        for(i3=0;i3<26;i3++){        
            if (i1%2==0 && i2%2==0 && i3%2==0) continue;
            if (i1%13==0 && i2%13==0 && i3%13==0) continue;            
            newkey = malloc(sizeof(int)*9);
            newkey[0] = i1;
            newkey[1] = i2;
            newkey[2] = i3;
            hill3_encipher(temp,ctext,newkey,L);
            score =0;
            for(k=0;k<L;k+=3){
                score += -monogram[(int)temp[k]];
            }
            base = nbest_add(base,newkey,score);
        }
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
            for(i3=0;i3<26;i3++){        
                if (i1%2==0 && i2%2==0 && i3%2==0) continue;
                if (i1%13==0 && i2%13==0 && i3%13==0) continue;          
                newkey[3] = i1;
                newkey[4] = i2;
                newkey[5] = i3;
                hill3_encipher(temp,ctext,newkey,L);
                score =0;
                for(k=0;k<L;k+=3){
                    score += -scoreTextBigram(&(temp[k]),2);
                }
                base2 = nbest_add(base2,newkey,score);
            }
          }
        }
    }
    /* we now have the 100 most likely middle rows, time to determine the final row */
    int bestkey[9] = {0,0,0,0,0,0,0,0,0};
    int det;
    bestscore = 99e99;
    for(elem = base2; elem != NULL; elem = elem->next){
        newkey = malloc(sizeof(int)*9);
        newkey[0] = elem->key[0];
        newkey[1] = elem->key[1];
        newkey[2] = elem->key[2];  
        newkey[3] = elem->key[3];
        newkey[4] = elem->key[4];
        newkey[5] = elem->key[5];                   
        for(i1=0; i1<26;i1++){
          for(i2=0; i2<26;i2++){
            for(i3=0;i3<26;i3++){    
                if (i1%2==0 && i2%2==0 && i3%2==0) continue;           
                newkey[6] = i1;
                newkey[7] = i2;
                newkey[8] = i3;
                det = (newkey[0]*newkey[4]*newkey[8] + newkey[1]*newkey[5]*newkey[6] + newkey[2]*newkey[3]*newkey[7]) - 
                      (newkey[2]*newkey[4]*newkey[6] + newkey[1]*newkey[3]*newkey[8] + newkey[0]*newkey[5]*newkey[7]);
                if (det%2==0 || det%13==0) continue;
                hill3_encipher(temp,ctext,newkey,L);
                score = -scoreTextQgram(temp,L);
                if (score < bestscore){
                    bestscore = score;
                    bestkey[0] = newkey[0];
                    bestkey[1] = newkey[1];
                    bestkey[2] = newkey[2];
                    bestkey[3] = newkey[3];
                    bestkey[4] = newkey[4];
                    bestkey[5] = newkey[5];
                    bestkey[6] = newkey[6];
                    bestkey[7] = newkey[7];                                                                                                                                            
                    bestkey[8] = newkey[8];                                  
                }
            }
          }
        }
        free(newkey);
    }    
    hill3_encipher(temp,ctext,bestkey,L);
    for(k=0;k<L;k++) temp[k] += 'A';
    printf("%s\n",temp);
}

void hill3_encipher(char *ctext, char *ptext, int *key, int L){
    int i;
    for(i=0; i<L; i+=3){
        ctext[i]   = (ptext[i]*key[0] + ptext[i+1]*key[1] + ptext[i+2]*key[2])%26;
        ctext[i+1] = (ptext[i]*key[3] + ptext[i+1]*key[4] + ptext[i+2]*key[5])%26;
        ctext[i+2] = (ptext[i]*key[6] + ptext[i+1]*key[7] + ptext[i+2]*key[8])%26;        
    } 
    ctext[L] = '\0';
}

void invert_hill3_key(int *key, int *invkey){
    int det = (key[0]*key[4]*key[8] + key[1]*key[5]*key[6] + key[2]*key[3]*key[7]) - 
              (key[2]*key[4]*key[6] + key[1]*key[3]*key[8] + key[0]*key[5]*key[7]);
    int idet = inverse_mod26(det);   
    if (idet == 0) printf("Warning: key is not invertible, choose a different key.\n");
    
    invkey[0] = (idet*((key[4]*key[8])-(key[5]*key[7])) + 17576)%26;
    invkey[1] = (idet*((key[2]*key[7])-(key[1]*key[8])) + 17576)%26;
    invkey[2] = (idet*((key[1]*key[5])-(key[2]*key[4])) + 17576)%26;
    invkey[3] = (idet*((key[5]*key[6])-(key[3]*key[8])) + 17576)%26;
    invkey[4] = (idet*((key[0]*key[8])-(key[2]*key[6])) + 17576)%26;
    invkey[5] = (idet*((key[2]*key[3])-(key[0]*key[5])) + 17576)%26;
    invkey[6] = (idet*((key[3]*key[7])-(key[4]*key[6])) + 17576)%26;
    invkey[7] = (idet*((key[1]*key[6])-(key[0]*key[7])) + 17576)%26;
    invkey[8] = (idet*((key[0]*key[4])-(key[1]*key[3])) + 17576)%26;
    printf("%d %d %d\n",invkey[0],invkey[1],invkey[2]);
    printf("%d %d %d\n",invkey[3],invkey[4],invkey[5]);
    printf("%d %d %d\n",invkey[6],invkey[7],invkey[8]);        
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
