// 不同.o檔 link起來 (macro)
#ifndef GALOIS_H 
#define GALOIS_H
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

typedef uint64_t (*uint128_t)[2]; //pointer array
// typedef uint64_t *uint128_t[2] //array pointer


/* pointer array

    X = &(*X) = &(*X)[0]            *X=(*X)[0]
    ---------                        --------
    |   *   |    --------------->    |  64  |    (*X)[0]
    ---------                        -------- 
                                     |  64  |    (*X)[1]  
                                     -------- 
*/

/* operation function*/
int SIZE; 
void add(uint128_t, uint128_t, uint128_t);
void shift_L(uint128_t, uint128_t, int);
void shift_R(uint128_t, uint128_t, int);
void shift_L_Mod(uint128_t, uint128_t, int);
void power2(uint128_t);
char* uint128_t2string(uint128_t);
void print(char*, uint128_t);

#endif

