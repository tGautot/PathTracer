#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <cstdlib>
#include "cuda.h" 


using std::make_shared;
using std::shared_ptr;
using std::sqrt;


const cu_double infinity = std::numeric_limits<cu_double>::infinity();
const cu_double PI = 3.1415926535897932385;
const cu_double EPSILON = 1e-8;

inline void swap(cu_double* a, cu_double* b){
    cu_double tmp = *a;
    *a = *b;
    *b = tmp;
}

inline cu_double degsToRads(cu_double deg){
    return deg * PI / CU_FLT_CST(180.0);
}

inline cu_double radsToDeg(cu_double rads){
    return rads * CU_FLT_CST(180.0) / PI;
}

inline cu_double randDouble(){
    return rand() / (RAND_MAX+CU_FLT_CST(1.0));
}

inline cu_double randDouble(cu_double min, cu_double max){
    return min + randDouble()*(max-min);
}

inline int rand_int(int min, int max){
    return min + rand()%(max-min);
}



#endif