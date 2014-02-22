/* author: James Lyons
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "typex.h"

// Rotors
static char *key[32] ={ // forward
                       "QWECYJIBFKMLTVZPOHUDGNRSXA",
                       "AJDKSIRUXBLHWTMCQGZNPYFVOE",
                       "BDFHJLCPRTXVZNYEIWGAKMUSQO",
                       "ESOVPZJAYQUIRHXLNFTGKDCMWB",
                       "VZBRGITYUPSDNHLXAWMJQOFECK",
                       "FVPJIAOYEDRZXWGCTKUQSBNMHL",
                       "KZGLIUCJEHADXRYWVTNSFQPMOB",
                       "ZLVGOIFTYWUEPMABNCXRQSDKHJ",
                       // reverse
                       "QAXSRNGDUHOPZVTLMKFBIJYCEW",
                       "AEOVFYPNZGQCMTWHLBXURISKDJ",
                       "BOQSUMKAGWIEYNZVXTRPCLJHFD",
                       "EBWMCDKGTFNLXHRIUQYAJZPVOS",
                       "VKCEFOQJMWAXLHNDSPUYTIGRBZ",
                       "FLHMNBSQUKTCGWXZRDEYOAIJPV",
                       "KBOMPQFSNTVWYRXDAHEJCUILGZ",
                       "ZJHKDSQRXCNBAMPEUWYTFIOGVL",                       
                       // forward inverse
                       "ZHDTCIURGFJLKVQPAWXMSNBYEO",
                       "AJPCZWRLFBDKOTYUQGENHXMIVS",
                       "TAGBPCSDQEUFVNZHYIXJWLRKOM",
                       "HZWVARTNLGUPXQCEJMBSKDYOIF",
                       "QCYLXWENFTZOSMVJUDKGIARPHB",
                       "FVPJIAOYEDRZXWGCTKUQSBNMHL",
                       "KZGLIUCJEHADXSYWVNTRFQPMOB",
                       "OPRWLGDYFZXBNQEMUTVHKCJSIA",
                       // reverse inverse
                       "BTXHYSGJUVRPQFKLAEDOINZCWM",
                       "ARLYBEJPVZXQMHCGKUWNTDOSFI",
                       "HAUZLYIXKWGVFNBTCSDREPJQMO",
                       "TBEFAJHNPUGLDKYWROZIQXCMSV",
                       "KYCPDEWNVHBMIOFRGXQUSAJLTZ",
                       "VFLRSAMCWXJBDEUYHQGKIZNOTP",
                       "QBUPSGYRWTAXDICEFNHJVKLOMZ",
                       "MLJEPUXCVBDZNKWOGHFTQYRISA"};

char refl[27] = "YRUHQSLDPXNGOKMIEBFZCWVJAT";
     
// notches indicate where the rotors increment. 
char notch[10] = "ACEINQTVY";

/*********************************************************
typex - encipher (or decipher) a string 'from', put the result in 'to'
- note that the key will be modified by this function (indicator settings are incremented)
**********************************************************/
char typex(TypexKey *key, char* from, char* to){
    int i;
    for(i=0;i<strlen(from);i++){
        to[i] = typex_encipher(from[i],key);
    }
    to[i] = '\0'; //null terminate
}

/*********************************************************
typex_encipher - encipher (or decipher) a single character
- note that the settings array will be modified by this function
**********************************************************/
char typex_encipher(char ch, TypexKey *key){
    ch = toupper(ch);
    // increment the settings before the current letter is enciphered
    increment_indicator_settings(key->indicator, key->rotors);

    // encipher the current character going through rotors right to left
    ch=rotor(key->rotors[4],ch,key->indicator[4]);  
    ch=rotor(key->rotors[3],ch,key->indicator[3]);  
    ch=rotor(key->rotors[2],ch,key->indicator[2]);  
    ch=rotor(key->rotors[1],ch,key->indicator[1]);
    ch=rotor(key->rotors[0],ch,key->indicator[0]);  
    // ch gets to the reflector...
    ch=reflector(ch); 
    // now ch goes back through the rotors from left to right (inverse substitution)
    ch=rotor(inverse(key->rotors[0]),ch,key->indicator[0]);  
    ch=rotor(inverse(key->rotors[1]),ch,key->indicator[1]);   
    ch=rotor(inverse(key->rotors[2]),ch,key->indicator[2]);   
    ch=rotor(inverse(key->rotors[3]),ch,key->indicator[3]);   
    ch=rotor(inverse(key->rotors[4]),ch,key->indicator[4]);   

    return ch;
} 

/***********************************************************
 Given a rotor, return the inverse 
 ***********************************************************/
Rotor inverse(Rotor r){
    if(r<16) return r + 16;
    else return r -16;
}

/*********************************************************
ROTOR - from ENIGMA I - http://en.wikipedia.org/wiki/Enigma_rotor_details
- Rotor chooses which rotor to use
- assumes char in is an uppercase char A-Z
**********************************************************/
char rotor(Rotor rotor, char in, int offset){
     return (key[rotor][(in-'A'+26+offset)%26] -offset+26-'A') %26+'A';
}

/*********************************************************
REFLECTOR - from ENIGMA I - http://en.wikipedia.org/wiki/Enigma_rotor_details
- assumes char in is an uppercase char A-Z
**********************************************************/
char reflector(char in){
     return refl[in-'A'];
}

/****************************************************************************
advances the rotors depending on which rotors are in which position
- uses notch array, which records when each rotor increments the one to the left
*****************************************************************************/
void increment_indicator_settings(int settings[5], Rotor r[5]){
    int i;
    for(i = 0; i < 9; i++){
        if(settings[1]+'A' == notch[i]){
            settings[0] = (settings[0]+1)%26;
            settings[1] = (settings[1]+1)%26;
        }    
        if(settings[2]+'A' == notch[i]){
            settings[1] = (settings[1]+1)%26;
        }
        settings[2] = (settings[2]+1)%26;                     
    }
}


/****************************************************************************
print a given key, useful for debugging
*****************************************************************************/
void printTypexKey(TypexKey *key){
    int i;
    printf("indicator=%c%c%c%c%c, ",key->indicator[0]+'A',key->indicator[1]+'A',key->indicator[2]+'A',key->indicator[3]+'A',key->indicator[4]+'A');
    printf("rotors=%d%d%d%d%d, ",key->rotors[0]+1,key->rotors[1]+1,key->rotors[2]+1,key->rotors[3]+1,key->rotors[4]+1);
    printf("\n");
}

/****************************************************************************
Initialise a key, with default information
*****************************************************************************/
void initTypexKey(TypexKey *key){
    key->indicator[0] = 0;
    key->indicator[1] = 0;
    key->indicator[2] = 0;
    key->indicator[3] = 0;
    key->indicator[4] = 0;        
    key->rotors[0] = 0;
    key->rotors[1] = 1;
    key->rotors[2] = 2;
    key->rotors[3] = 3;
    key->rotors[4] = 4;        
}
