OS_WRCH = &FFEE
OS_WORD = &FFF1
OS_FILE = &FFDD
OS_ASCI = &FFE3
OS_BYTE = &FFF4

INCLUDE "memory.s"
INCLUDE "Player.s"
INCLUDE "util.s"
	
ORG &1900
	
.start:	
LDA #22
JSR OS_WRCH
LDA #7
JSR OS_WRCH
LDA #200
LDX #3
JSR OS_BYTE

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
        ; load player's room id
	LDA #1
	JSR printText
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

	; Print text with ID in X
	; Y maybe contains flags?
.printText:
	{
	; If A=X, print this, else move to offse
	ASL A
	TAX
	LDA textTable+1,X
	STA t1
	LDA textTable,X
	STA t0
	LDY #0
.loop:
	LDA (t0),Y
	JSR OS_ASCI ; preserves a,x,y
	INY
	CMP #13
	BNE loop
	RTS
	}

.parseInput:
	CLC
	RTS

.gameData:
	EQUB 1 ; version
	EQUB 1 ; start room

	; Text table follows
.textTable:
	EQUW strOne
	EQUW strTwo
	EQUB 0

.strOne:
	EQUS "You are in a cellar",13
.strTwo:
	EQUS "OK",13

.roomData:
    EQUB 0 ; room id
    EQUB 0 ; offset to next room
    EQUB 0 ; text ID
    EQUB 0 ;
    EQUB 0,0,0,0 ; n/s/e/w exits


.end:

SAVE "charm",start,end
PUTFILE "data",&7D00
