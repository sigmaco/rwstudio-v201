xvs.1.1

#include "FlagDefs.h"

; apply a sine wave to the vertex


; wave 1
mov  r6,   VSIN_REG_POS.zzzz
add  r6, r6,VSCONST_REG_TIME.xxxx
mul  r6, r6, VSCONST_REG_HFREQA

; calculate the sine of r6.x
mad  r5.x,   r6.x, VSCONST_REG_TSERIES_0.w,  VSCONST_REG_TSERIES_0.y	; bring argument into -pi, .., +pi range
expp r5.y,   r5.x
mad  r5.x,   r5.y, VSCONST_REG_TSERIES_0.z, -VSCONST_REG_TSERIES_0.x
dst  r7.xy,  r5.x, r5.x		                                          ; generate 1, (r5.x)^2, .. (r5.x)^6
mul  r7.z,   r7.y, r7.y                                              
mul  r7.w,   r7.y, r7.z                                              
mul  r5,     r7,   r5.x		                                          ; generate r5.x, (r5.x)^3, .., (r5.x)^7
dp4  r5.y,   r5,   VSCONST_REG_TSERIES_2		                        ; compute sin(r5.y)

; apply scaling to the sine wave
mul  r5.y,   r5.y, VSIN_REG_COLOR.w
mul  r11,    r5.y, VSCONST_REG_HAMPLITUDEA


; wave 2
mov  r6,   VSIN_REG_POS.yyyy;zzzz
add  r6, r6,VSCONST_REG_TIME.yyyy
mul  r6, r6, VSCONST_REG_HFREQB

; calculate the sine of r6.x
mad  r5.x,   r6.x, VSCONST_REG_TSERIES_0.w,  VSCONST_REG_TSERIES_0.y	; bring argument into -pi, .., +pi range
expp r5.y,   r5.x
mad  r5.x,   r5.y, VSCONST_REG_TSERIES_0.z, -VSCONST_REG_TSERIES_0.x
dst  r7.xy,  r5.x, r5.x		                                          ; generate 1, (r5.x)^2, .. (r5.x)^6
mul  r7.z,   r7.y, r7.y                                              
mul  r7.w,   r7.y, r7.z                                              
mul  r5,     r7,   r5.x		                                          ; generate r5.x, (r5.x)^3, .., (r5.x)^7
dp4  r5.y,   r5,   VSCONST_REG_TSERIES_2		                        ; compute sin(r5.y)

; apply scaling to the sine wave
mul  r5.y,   r5.y, VSIN_REG_COLOR.w
mul  r10,    r5.y, VSCONST_REG_HAMPLITUDEB

; wave 3
mov  r6,   VSIN_REG_POS.zzzz
add  r6, r6,VSCONST_REG_TIME.zzzz
mul  r6, r6, VSCONST_REG_VFREQA

; calculate the sine of r6.x
mad  r5.x,   r6.x, VSCONST_REG_TSERIES_0.w,  VSCONST_REG_TSERIES_0.y	; bring argument into -pi, .., +pi range
expp r5.y,   r5.x
mad  r5.x,   r5.y, VSCONST_REG_TSERIES_0.z, -VSCONST_REG_TSERIES_0.x
dst  r7.xy,  r5.x, r5.x		                                          ; generate 1, (r5.x)^2, .. (r5.x)^6
mul  r7.z,   r7.y, r7.y                                              
mul  r7.w,   r7.y, r7.z                                              
mul  r5,     r7,   r5.x		                                          ; generate r5.x, (r5.x)^3, .., (r5.x)^7
dp4  r5.y,   r5,   VSCONST_REG_TSERIES_2		                        ; compute sin(r5.y)


; apply scaling to the sine wave
mul  r5.y,   r5.y, VSIN_REG_COLOR.w
mul  r5.y,   r5.y, VSCONST_REG_VAMPLITUDEA

mov r6, VSIN_REG_POS
add r6.x, r6, r10.yyyy
add r6.x, r6, r11.yyyy
add r6.y, r6, r5.yyyy



; transform vertex
dp4  oPos.x, r6, VSCONST_REG_TRANSFORM_X
dp4  oPos.y, r6, VSCONST_REG_TRANSFORM_Y
dp4  oPos.z, r6, VSCONST_REG_TRANSFORM_Z
dp4  oPos.w, r6, VSCONST_REG_TRANSFORM_W

;
; light the vertex 
;

; calculate the vertex-light vector
sub  r4, r6, VSCONST_REG_LIGHT_POS

; normalize the vertex-light vector
dp3  r4.w,   r4, r4
rsq  r4.w,   r4.w
mul  r4,     r4, r4.w


;dot product of vertex-light and vertex normal
dp3  r4.w,   r4, VSIN_REG_NORMAL

;clamp in the range 0.0 to 1.0
mul  r4,     VSCONST_REG_ONE, r4.www
min  r4,     VSCONST_REG_ONE,  r4
max  r4,     VSCONST_REG_ZERO, r4

                     