#include "Basics.h"

int Basics::Abs(int val){
    if(val < 0){
           return -val;       
    }
    return val;
}

int Basics::Pow(int base, int exp){
    if(exp == 0){
         return 1;       
    }else{
         int ret = base;
         for(int i = 0; i<exp-1; i++){
            ret *= base;
         }
         return ret;
    }
}
