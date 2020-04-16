#include "Galois.h"

void add(uint128_t R, uint128_t X, uint128_t Y){
    for(int i=0; i<SIZE; i++)
        (*R)[i] = (*X)[i] ^ (*Y)[i];
}

void shift_L(uint128_t R, uint128_t X, int n){
    
    double START, END;
    START = clock();
    uint8_t upbound = SIZE-1;

    for(;n>7;n-=8){
        for(int i=0; i<upbound; i++)
            (*R)[i] = ((*X)[i] << 8) | (*(uint8_t*)((uint32_t) &(*X)[i+1] +7));
        (*R)[upbound] = (*X)[upbound] << 8;
    }

    for(; n>0; n--){
        for(int i=0; i<upbound; i++)
            (*R)[i] = ((*X)[i] << 1) | (*(uint8_t*)((uint32_t) &(*X)[i+1] +7) >> 7);
        (*R)[upbound] = (*X)[upbound] << 1; 
    }

    END = clock();
    printf("shift_L time: %f\n", (END-START)/CLOCKS_PER_SEC);


    // for(; n>0; n--){
    //     for(int i=0; i<upbound; i++)
    //         (*R)[i] = ((*X)[i] << 1) | ((*X)[i+1] >> 63);
    //     (*R)[upbound] = (*X)[upbound] << 1; 
    // }    

}// 64/8=8

// (*X)[1] = 0x1122334455667788
// &(*X)[1] = 0x332211  //addr
// (uint32_t)&(*X)[1] + 1 = 0x332212  //addr (uint64_t*) + 1bit
// (uint8_t*)((uint32_t)&(*X)[1] + 7) = 0x0x332218  //addr
// *(uint8_t*)((uint32_t)&(*X)[1] + 7) = 0x0000000000000011

void shift_R(uint128_t R, uint128_t X, int n){
    uint8_t upbound = SIZE-1;

    for(;n>7;n-=8){
        for(int i=0; i<upbound; i++)
            (*R)[i] = ((*X)[i] << 8) | (*(uint8_t*)((uint32_t) &(*X)[i+1] +7)); // +7 (向右移56bits)
        (*R)[upbound] = (*X)[upbound] >> 8;
    }

    for(; n>0; n--){
        for(int i=0; i<upbound; i++)
            (*R)[i] = ((*X)[i] >> 1) | (*(uint8_t*)((uint32_t) &(*X)[i+1] +7) << 7);
        (*R)[upbound] = (*X)[upbound] >> 1; 
    }
}

void shift_L_Mod(uint128_t R, uint128_t X, int n){

}

void power2(uint128_t R){

}


char* uint128_t2string(uint128_t X){
    char* str = (char*)calloc(128, sizeof(char));
    sprintf(str, "0X");
    for(int i=0; i<SIZE; i++)
        sprintf(str, "%s%.8llX", str, (*X)[i]);
    return str;
}

void print(char* name, uint128_t X){
    printf("%s: %s \n", name, uint128_t2string(X));
}
