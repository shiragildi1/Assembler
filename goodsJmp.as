;besad
jmp L1(#5,N)
Nv: jmp L1(r15,r0)
L1v: jmp L1(#-1,r6)
jmp L1(N,L1)         
			

r15:jmp N
				jmp L1
jmp r4
	  jmp                                 r1
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;besad
bne L1(#5,N)
Ndc: bne L1(r1,r4)
L1: bne L1(#-1,r6)
bne L1(N,L1)

bne N
				bne L1
bne r4
	  bne                                 r1
;;;;;;;;;;;;;;;;;;;;;;;;besad
jsr L1(#5,N)
N: jsr L1(r1,r4)
Lf1: jsr L1(#-1,r6)
jsr L1(N,L1)

jsr N
				jmp L1
jsr r4
	  jmp                                 r1
