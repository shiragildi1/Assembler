MAIN: mov loop ,r7 
.data ,100 , 25 , +7, -12   
.extern LINE  
LOOP: jmp loop (r7,loop) 
inc r-3  
mov r2, r5
rts 
.entry STR 
inc r2
mov A, r1
rts 
prn #-500 
cmp #-32 , K,hkj 
K: bne LOOP(r4,r3) 
add #-78, #45 
.string "hello" 
jmp SHALOM(LINE,LOOP) 
sub r1, r4
bne #45 
L1: inc K 
aaa: bne LOOP(K,STR) 
inc r2
mov A, r1
rts 
sub r1, r4
bne END 
END: stop  
HELLO: not r5
SHALOM: .string"cbasfg54" 
red r3
jsr K
jmp LINE 
LENGTH: .data 6,-9,15 
sub #22, aaa 
lea #-56, #45 
klk: .data 256 
.extern SHALOM 
clr r4 
X: dec Num 
red r6 
jsr aaa(#75,X) 
A: inc X 
.entry LOOP 
bne SHALOM(r2,STR) 
inc STR 
dec SHALOM 
stop nhun
