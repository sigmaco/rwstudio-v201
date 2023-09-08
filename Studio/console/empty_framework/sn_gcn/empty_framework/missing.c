// Function / value missing from libraries etc. Required for correct linkage.
// All are C functions, NOT C plus plus.

#include <stdio.h>
#include <string.h>
#include <precomp.h>

// setjmp - should be in an library file (.a file).

#include <setjmp.h>
int __setjmp(jmp_buf env)
{
   return(setjmp(env));
}

// floating point values, should be in a library file (.a file).

unsigned long __float_nan[]    ={0x7FFFFFFF};
unsigned long __float_huge[]   ={0x7f800000};

// calloc_r - INTERNAL function to ProDG libraries, not defined !!!

void *calloc_r(size_t num, size_t size)
{
   void *pData = RwMalloc(num * size, rwMEMHINTDUR_GLOBAL);
   memset(pData, 0, num * size);

   return(pData);
}
