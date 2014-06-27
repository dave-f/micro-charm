
; 16-bit add macro etc
MACRO ADDI16 addr, val
        CLC
	LDA addr
	ADC #LO(val)
	STA addr
	LDA addr+1
	ADC #HI(val)
	STA addr+1
ENDMACRO
	
	