;@author Pessy Israeli
;This file aint supposed to pass preprocessor!
.entry LENGTH 
.extern W
jmp
mcr m1
	MAIN: mov #78, r0 
	BB: .string "Bvu" 
endmcr mov
LOOP: jmp L1(#-1,r6)
prn #-5		
bne W(r4,#89)
 sub r1, r4
 
 mcr jmp
	bne L3
endmcr

L1: inc K
.entry LOOP
bne LOOP(K,W)
inc r6
END: stop
m1 
jsr K(r1,#-78)	
STR: .string "ttalaksghdfkajhds"
LENGTH: .data 6,-9, 15
K: .data 22
.extern L3
mcr m2 B

	.entry BB
	endmcr
Main: mov #5, BB
