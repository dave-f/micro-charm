OS_WRCH = &FFEE
OS_WORD = &FFF1
OS_FILE = &FFDD
OS_ASCI = &FFE3
OS_BYTE = &FFF4

INCLUDE "memory.s"
INCLUDE "Player.s"
INCLUDE "util.s"

MSG_PROMPT = 0
	
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
JSR fixupPtrs
        
.initPlayer:
LDA gameData+1
STA player+0                     ; current room
        
.newRoom:
JSR describeRoom

.loop:
LDA #MSG_PROMPT
JSR printText
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
	EQUB LO(gameData),HI(gameData),0,0 ; load address? why 4 bytes?
	EQUB 0,0,0,0
	EQUB 0,0,0,0
	EQUB 0,0,0,0

.describeRoom:
    LDA player+0 ; get room id
    JSR findRoom
    LDY #2
    LDA (t0),Y   ; get room description
	JSR printText
	RTS

    ; A=room
    ; return with roomLO,roomHI in t0,t1
.findRoom:
    {
    STA t2 ; room wanted
    LDA gameData+3
    STA t0
    LDA gameData+4
    STA t1
    LDY #0
 .loop:
    LDA (t0),Y
    CMP t2
    BEQ out
    ; Otherwise add on the room offset and keep looking
    CLC
    LDA t0
    ADC #8 ; just 8 for now
    STA t0
    LDA t1
    ADC #0
    STA t1
    JMP loop
.out:        
    RTS
    }

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
    {
    LDA inputBuffer
    CMP #'N'
    BEQ north
    CMP #'S'
    BEQ south
    CMP #'E'
    BEQ east
    CMP #'W'
    BEQ west
    ; Do nothing
.noChange:        
    CLC
	RTS
    ; Can we go north?
.north:
    LDA player+0
    JSR findRoom
    LDY #3 ; N
    LDA (t0),Y ; exit?
    BEQ noChange
    STA player+0
    SEC
    RTS
    ; Can we go south?
.south:
    LDA player+0
    JSR findRoom
    LDY #4 ; S
    LDA (t0),Y ; exit?
    BEQ noChange
    STA player+0
    SEC
    RTS
    ; Can we go east?
.east:
    LDA player+0
    JSR findRoom
    LDY #5 ; E
    LDA (t0),Y ; exit?
    BEQ noChange
    STA player+0
    SEC
    RTS
    ; Can we go west?
.west:
    LDA player+0
    JSR findRoom
    LDY #6 ; W
    LDA (t0),Y ; exit?
    BEQ noChange
    STA player+0
    SEC
    RTS
    }

.fixupPtrs:
	{
	LDX gameData+2 ; num strings + room offset
    INX
    STX t0
    LDX #3
        
.loop:
    CLC
    LDA gameData,X
    ADC #LO(gameData) ; hdr size=2 bytes at mo (version, start room)
    STA gameData,X
    LDA gameData+1,X
    ADC #HI(gameData)
    STA gameData+1,X
    INX
    INX ; needs to be done better
    DEC t0
	BNE loop
	RTS
	}

.gameData:
	EQUB 1 ; version
	EQUB 1 ; start room
    EQUB 1 ; num strings
    EQUW 0 ; offset to rooms
        
.textTable:
	FOR n,1,100
	  EQUW 0 ; 16 bit addresses
	NEXT
        
.end:

SAVE "charm",start,end
PUTFILE "data",&7D00
