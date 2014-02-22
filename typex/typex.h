/* author: James Lyons
*/
typedef int Rotor;

// a structure for storing enigma key information
typedef struct key__{
    Rotor rotors[5];
    int indicator[5];
} TypexKey;    

/*******************************************************************************/                     
char reflector(char in);
char rotor(Rotor rotor, char in, int offset);
Rotor inverse(Rotor r);
void increment_indicator_settings(int settings[3], Rotor r[3]);
char apply_steckers(char in, char steckers[13][2]);
char typex(TypexKey *key, char* ctext, char* ptext);
char typex_encipher(char ch, TypexKey *key);

void initTypexKey(TypexKey *key);
void printTypexKey(TypexKey *key);
/*******************************************************************************/

