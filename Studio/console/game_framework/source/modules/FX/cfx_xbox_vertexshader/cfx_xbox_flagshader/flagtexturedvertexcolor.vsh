; The shader will be compiled using a custom build process from the Visual Studio project.
; xsasm $(InputPath) /h

#include "flagshaderbase.vsh"

add r4.xyz, r4, VSCONST_REG_MAT_AMB 
mul r4,     VSCONST_REG_MAT_COLOR, r4
mul oD0,    r4, v3
mov oT0,    v2    

