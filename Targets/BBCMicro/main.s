
OS_WRCH = &FFEE
OS_WORD = &FFF1
OS_FILE = &FFDD
OS_BYTE = &FFF4
ORG     = &1900
	
.start:	
LDA #22
JSR OS_WRCH
LDA #7
JSR OS_WRCH

.loadFile:
; load data file in via OSFILE
LDA #$FF
LDX #LO(fileBlock)
LDY #HI(fileBlock)
JSR OS_FILE

.newRoom:
JSR describeRoom

.loop:
LDA #'?'
JSR OS_WRCH
JSR getInput
JSR parseInput
BCS newRoom
JMP loop
RTS

.fileName:
	EQUB "data",13

.fileBlock:
	EQUB LO(fileName)
	EQUB HI(fileName)
	EQUB 0,$7d,0,0 ; load address? why 4 bytes?
	EQUB 0,0,0,0
	EQUB 0,0,0,0
	EQUB 0,0,0,0

.describeRoom:
	RTS

.getInput:
	LDA #0
	LDX #LO(inputBlock)
	LDY #HI(inputBlock)
	JSR OS_WORD
	RTS
	
.inputBlock:
	EQUB LO(inputBuffer)
	EQUB HI(inputBuffer)
	EQUB 32
	EQUB 32   ; space
	EQUB 122  ; 'z'
	
.inputBuffer:
	SKIP 32
	
.parseInput:
	CLC
	RTS

.end:

SAVE "charm",start,end
PUTFILE "data",&7D00
