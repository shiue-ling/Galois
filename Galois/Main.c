#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Galois.h"

void shift_L_old(uint128_t R, uint128_t X, int n){
    double START, END;
    START = clock();
    int upbound = SIZE-1;

    for(;n>7;n-=8){
        for(int i=0; i<upbound; i++)
            (*R)[i] = ((*X)[i] << 8) | ((*X)[i+1] + 56);
        (*R)[upbound] = (*X)[upbound] << 8;
    }

    for(; n>0; n--){
        for(int i=0; i<upbound; i++)
            (*R)[i] = ((*X)[i] << 1) | ((*X)[i+1] + 63);
        (*R)[upbound] = (*X)[upbound] << 1; 
    } 
    END = clock();
    printf("shift_L_old time: %f\n", (END-START)/CLOCKS_PER_SEC);
}


int main(int argv, char *argc[]){

    SIZE = 2;

    uint128_t a = (uint128_t)(uint64_t[]){0x5877, 0x9636743};
    uint128_t b = (uint128_t)(uint64_t[]){0x0, 0x0};

    print("a", a);
    shift_L(b, a, 1);
    print("b", b);
    // shift_L_old(b, a, 100000000);
    // shift_R(b, a, 1); //Error
    // print("b", b);
    // shift_L(b, a, 8);
    // print("b", b);

    return 0;
}


// #include <stdio.h>
// #include <stdlib.h>
// #include "Galois.h"

// // typedef unsigned long long int GF;

// // GF* GFadd(GF *A, GF *B);

// // void printGF(char* name, GF X){
// //     char* str = calloc(128, sized(char))
// //     GF_toS
// // }

// int main(void){
//     int a=8;
    
//     printf("%d\n", sizeof(a));

//     // GF *A = {0x5, 0x5, 0x5, 0x5};
//     // GF *B = {0xA, 0xA, 0xA, 0xA};

//     // GF *C = GFadd(A, B);
//     // // GFadd(A, B);
//     // printf("%d %d %d %d \n", C[0], C[1], C[2], C[3]);
//     // // printf("%X\n", C);

//     // printf("Hello world");
//     return 0;
// }


// // GF* GFadd(GF *A, GF *B){
// //     GF *result;

// //     result[0] = A[0] ^ B[0];
// //     result[1] = A[1] ^ B[1];
// //     result[2] = A[2] ^ B[2];
// //     result[3] = A[3] ^ B[3];

// //     return result;
// // }
