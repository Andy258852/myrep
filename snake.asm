model smALl
.stack 100h
.data
MLength dw 640
MHeight dw 480
CSIze equ 8
Cd equ CSIze + CSIze + 1
Cdw dw Cd
Color equ 3
TColor equ 15
PColor equ 4
PSIze equ 5 
Speed equ 3
Score dw 0
StartX dw Cd  + CSIze
StartY dw Cd + Cd + Cd + CSIze
ArrSIze equ 400
Arr dw Cd + CSIze, Cd + Cd + Cd + CSIze, ArrSIze + ArrSIze - 2 dup(0)
Ongoing dw 0
Last dw 0
WallCount equ 30
WallArr dw WallCount + WallCount dup(0)
WallCounter dw 0
fname db 'save.txt', 0
LastPointPos dw ?, ?
Two dw 2
Temp db ?
.code
Save PROC
	PUSH AX
	PUSH BX
	PUSH CX
	PUSH DX
	PUSH SI

	MOV AL, 0
	MOV AH, 3Ch
	MOV CX, 0
	MOV DX, offset fname
	INT 21h
	MOV SI, AX
	;;;;;;;;;
	MOV BX, SI
	MOV AH, 40h
	MOV DX, offset Temp
	MOV CX, 1
	INT 21h
	MOV AH, 40h
	MOV DX, offset Score
	MOV CX, 2
	INT 21h
	MOV AH, 40h
	MOV DX, offset LastPointPos
	MOV CX, 4
	INT 21h
	MOV AH, 40h
	MOV DX, offset WallArr
	MOV CX, WallCount + WallCount + WallCount + WallCount
	INT 21h
	MOV AH, 40h
	MOV DX, offset Ongoing
	MOV CX, 2
	INT 21h
	MOV AH, 40h
	MOV DX, offset Last
	MOV CX, 2
	INT 21h
	MOV AH, 40h
	MOV DX, offset Arr
	MOV CX, ArrSIze + ArrSIze + ArrSIze + ArrSIze
	INT 21h
	
	MOV AH, 3Eh
	MOV BX, SI
	INT 21h
	
	POP SI
	POP DX
	POP CX
	POP BX
	POP AX
RET
ENDP
Open:
	PUSH DI
	PUSH AX
	PUSH BX
	PUSH CX
	PUSH DX
	PUSH SI
	
	MOV AX, 3D00h
	MOV CX, 0
	MOV DX, offset fname
	INT 21h
	;;;;;;;;;
	JNC OSkip1
	POP SI
	POP DX
	POP CX
	POP BX
	POP AX
	POP DI
	JMP SkipLoad
	OSkip1:
	;;;;;;;;;
	MOV SI, AX
	MOV BX, SI
	MOV AX, 3F00h
	MOV DX, offset Temp
	MOV CX, 1
	INT 21h
	JNC OSkip2
	POP SI
	POP DX
	POP CX
	POP BX
	POP AX
	POP DI
	JMP SkipLoad
	OSkip2:
	MOV AX, 3F00h
	MOV DX, offset Score
	MOV CX, 2
	INT 21h
	JNC OSkip3
	POP SI
	POP DX
	POP CX
	POP BX
	POP AX
	POP DI
	JMP SkipLoad
	OSkip3:
	MOV AX, 3F00h
	MOV DX, offset LastPointPos
	MOV CX, 4
	INT 21h
	JNC OSkip4
	POP SI
	POP DX
	POP CX
	POP BX
	POP AX
	POP DI
	JMP SkipLoad
	OSkip4:
	MOV AX, 3F00h
	MOV DX, offset WallArr
	MOV CX, WallCount + WallCount + WallCount + WallCount
	INT 21h
	JNC OSkip5
	POP SI
	POP DX
	POP CX
	POP BX
	POP AX
	POP DI
	JMP SkipLoad
	OSkip5:
	MOV AX, 3F00h
	MOV DX, offset Ongoing
	MOV CX, 2
	INT 21h
	JNC OSkip6
	POP SI
	POP DX
	POP CX
	POP BX
	POP AX
	POP DI
	JMP SkipLoad
	OSkip6:
	MOV AX, 3F00h
	MOV DX, offset Last
	MOV CX, 2
	INT 21h
	JNC OSkip7
	POP SI
	POP DX
	POP CX
	POP BX
	POP AX
	POP DI
	JMP SkipLoad
	OSkip7:
	MOV AX, 3F00h
	MOV DX, offset Arr
	MOV CX, ArrSIze + ArrSIze + ArrSIze + ArrSIze
	INT 21h
	JNC OSkip8
	POP SI
	POP DX
	POP CX
	POP BX
	POP AX
	POP DI
	JMP SkipLoad
	OSkip8:
	
	MOV AH, 3Eh
	MOV BX, SI
	INT 21h
	
	CALL CLEARSCR
	
	MOV AL,Color
	CALL DrawField
	MOV AL, TColor
	CALL DrawScore
	
	MOV DI, offset LastPointPos
	MOV SI, [DI]
	ADD DI, 2
	MOV DX, [DI]
	MOV AL,PColor
	MOV BX,	PSIze	;радиус
	CALL DrawRhombus
	
	MOV WallCounter, 0
	WALLCYCLE:
	MOV DI, WallCounter
	MOV SI, WallArr[DI]
	MOV DX, WallArr[DI + 2]
	ADD WallCounter, 4
	MOV AL, Color
	MOV BX,	Cd	
	MOV CX, Cd
	CALL DrawHorLine
	CMP WallCounter, WallCount + WallCount + WallCount + WallCount
	JNE WALLCYCLE

	MOV BX,	CSIze
	MOV CX, Ongoing
	ADD CX, 4
	CMP CX, ArrSIze
	JNE  CheckSkip
	MOV CX, 0
	CheckSkip:
	MOV DI, Last
	MOV AL, Color
	DRAWSNAKEEL:
	MOV SI, Arr[DI]
	MOV DX, Arr[DI + 2]
	CALL DrawRhombus
	ADD DI, 4
	CMP DI, ArrSIze
	JNE  CheckSkip1
	MOV DI, 0
	CheckSkip1:
	CMP DI, CX
	JNE DRAWSNAKEEL
	
	POP SI
	POP DX
	POP CX
	POP BX
	POP AX
	
	MOV DI, Ongoing
	MOV SI, Arr[DI]
	MOV DX, Arr[DI + 2]
	
	MOV CH, Temp
	POP DI
	JMP GameCyCLe
CLEARSCR PROC
PUSH AX
PUSH BX
PUSH CX
PUSH DX
MOV AX, 0600h
MOV BH, 0
MOV CX, 0
MOV DX, 3050h
INT 10h
POP DX
POP CX
POP BX
POP AX
RET
ENDP
DrawHorLineWithCHecking PROC
PUSH AX
MOV AL, Color
PUSH DX
PUSH BX
cyCLe5:
	PUSH SI
	PUSH CX
	line3:
		PUSH CX
		MOV AH,0CH	;Функция вывода пиксела
		MOV BH,0	;Видеостраница
		MOV CX,SI	;Х-координата
		PUSH BX
		PUSH AX
		MOV AH, 0Dh
		inT 10h
		MOV bl, AL
		POP AX
		CMP bl, Color
		JNE drawSkip1
		MOV DI, 1
		drawSkip1:
		CMP bl, PColor
		JNE drawSkip2
		MOV DI, 2
		drawSkip2:
		POP BX
		inT 10h
		inC SI
		POP CX
		loop line3
	POP CX	
	POP SI
	inC DX
	DEC BL
	CMP BL, 0
	JNE cyCLe5
POP BX
POP DX
POP AX
RET
ENDP
DrawHorLine PROC
PUSH DX
PUSH BX
cyCLe1:
	PUSH SI
	PUSH CX
	line1:
		PUSH CX
		MOV AH,0CH	;Функция вывода пиксела
		MOV BH,0	;Видеостраница
		MOV CX,SI	;Х-координата
		inT 10h
		inC SI
		POP CX
		loop line1
	POP CX	
	POP SI
	inC DX
	DEC BL
	CMP BL, 0
	JNE cyCLe1
POP BX
POP DX
RET
ENDP
DrawVerLine PROC
PUSH SI
PUSH BX
cyCLe2:
	PUSH CX
	PUSH DX
	line2:
		PUSH CX
		MOV AH,0CH	;Функция вывода пикселя
		MOV BH,0	;Видеостраница
		MOV CX,SI	;Х-координата
		inT 10h
		inC DX
		POP CX
		loop line2
	POP DX
	POP CX
	inC SI
	DEC BL
	CMP BL, 0
	JNE cyCLe2
POP BX
POP SI
RET
ENDP
DrawRhombus PROC
PUSH DX
PUSH DI
PUSH BX
PUSH SI
MOV DI, BX
ADD DI, BX
INC DI
SUB DX, BX
PUSH CX
MOV BX, 1
MOV CX, 1
CALL DrawHorLine
cyCLe3:
	ADD CX, 2
	inC DX
	DEC SI
	CALL DrawHorLine
	CMP CX, DI
	JNE cyCLe3
cyCLe4:
	SUB CX, 2
	inC DX
	inC SI
	CALL DrawHorLine
	CMP CX, 1
	JNE cyCLe4
POP CX
POP SI
POP BX
POP DI
POP DX
RET
ENDP
DrawRhombusWithCHecking PROC
PUSH DX
PUSH DI
PUSH BX
PUSH SI
MOV DI, BX
ADD DI, BX
inC DI
SUB DX, BX
PUSH CX
MOV BX, 1
MOV CX, 1
CALL DrawHorLine
cyCLe7:
	ADD CX, 2
	inC DX
	DEC SI
	PUSH DI
	XOR DI, DI
	CALL DrawHorLineWithCHecking
	CMP DI, 1
	JNE skipRh1
	MOV AX, 1
	skipRh1:
	CMP DI, 2
	JNE skipRh2
	MOV AX, 2
	skipRh2:
	POP DI
	CMP CX, DI
	JNE cyCLe7
cyCLe8:
	SUB CX, 2
	inC DX
	inC SI
	PUSH DI
	XOR DI, DI
	CALL DrawHorLineWithCHecking
	CMP DI, 1
	JNE skipRh3
	MOV AX, 1
	skipRh3:
	CMP DI, 2
	JNE skipRh4
	MOV AX, 2
	skipRh4:
	POP DI
	CMP CX, 1
	JNE cyCLe8
POP CX
POP SI
POP BX
POP DI
POP DX
RET
ENDP
DrawField PROC
MOV BX,Cd + Cd + Cd   ;Толщина линии
MOV DX,0	;Y-координата
MOV SI,0	;начальная Х-координата
MOV CX,MLength	;длина линии
CALL DrawHorLine

MOV BX,Cd   ;Толщина линии
MOV DX,0	;Y-координата
MOV SI,0	;начальная Х-координата
MOV CX, MHeight	;высота линии
CALL DrawVerLine

MOV BX,Cd   ;Толщина линии
MOV DX,MHeight	;Y-координата
SUB DX, BX
MOV SI,MLength	;начальная Х-координата
MOV CX,MLength	;длина линии
CALL DrawHorLine

MOV BX,Cd   ;Толщина линии
MOV DX,0	;Y-координата
MOV SI,MLength	;начальная Х-координата
SUB SI, BX
MOV CX, MHeight	;высота линии
CALL DrawVerLine
RET
ENDP
Random PROC
PUSH DX
in   AX,40h
XOR  DX,DX
DIV Two
DIV  DI
MOV AX, DX
POP DX
RET
ENDP
CreatePoinT PROC
PUSH AX
PUSH DX
PUSH BX
PUSH SI
PUSH DI
PUSH CX

Again:
XOR DX, DX
MOV AX, MLength
DIV Cdw
MOV DI, AX
DEC DI
CALL random
MOV SI, CSIze
tcyCLe1:
ADD SI, Cd
DEC AX
CMP AX, 0
JNE tcyCLe1

XOR DX, DX
MOV AX, MHeight
DIV Cdw
MOV DI, AX
DEC DI
CALL random
MOV DX, CSIze
tcyCLe2:
ADD DX, Cd
DEC AX
CMP AX, 0
JNE tcyCLe2

MOV AH, 0Dh
MOV CX, SI
MOV BH, 0
inT 10h
CMP AL, Color
JE Again

MOV AL,PColor
MOV BX,	PSIze	;радиус
CALL DrawRhombus
MOV DI, offset LastPointPos
MOV WORD PTR [DI], SI
ADD DI, 2
MOV WORD PTR [DI], DX

POP CX
POP DI
POP SI
POP BX
POP DX
POP AX
RET
ENDP
DrawWall PROC
PUSH AX
PUSH DX
PUSH BX
PUSH SI
PUSH DI
PUSH CX

Again2:
XOR DX, DX
MOV AX, MLength
DIV Cdw
MOV DI, AX
SUB DI, 1
CALL random
MOV SI, CSIze
tcyCLe3:
ADD SI, Cd
DEC AX
CMP AX, 0
JNE tcyCLe3

XOR DX, DX
MOV AX, MHeight
DIV Cdw
MOV DI, AX
SUB DI, 1
CALL random
MOV DX, CSIze
tcyCLe4:
ADD DX, Cd
DEC AX
CMP AX, 0
JNE tcyCLe4

MOV AH, 0Dh
MOV CX, SI
MOV BH, 0
inT 10h
CMP AL, Color
JE Again2

ADD DX, Cd
ADD CX, Cd
INT 10h
CMP AL, Color
JNE WSkip1
JMP Again2
WSkip1:

SUB DX, Cd + Cd
INT 10h
CMP AL, Color
JNE WSkip2
JMP Again2
WSkip2:

SUB CX, Cd + Cd
INT 10h
CMP AL, Color
JNE WSkip3
JMP Again2
WSkip3:

ADD DX, Cd + Cd
INT 10h
CMP AL, Color
JNE WSkip4
JMP Again2
WSkip4:

SUB DX, Cd
SUB SI, CSIze
SUB DX, CSIze

MOV DI, WallCounter
MOV WallArr[DI], SI
MOV WallArr[DI + 2], DX
ADD WallCounter, 4

MOV AL, Color
MOV BX,	Cd	
MOV CX, Cd
CALL DrawHorLine

POP CX
POP DI
POP SI
POP BX
POP DX
POP AX
RET
ENDP
DrawSymbol PROC
	PUSH BX
	PUSH AX
	MOV AH, 0eh
	MOV BL, TColor
	inT 10h
	POP AX
	POP BX
RET
ENDP
SetCurCol PROC
	PUSH AX
	PUSH BX
	PUSH DX
	MOV AH, 02h
	MOV DH, 1
	MOV BH, 0
	inT 10h
	POP DX
	POP BX
	POP AX
RET
ENDP
DrawDIgit PROC
	XOR DX,DX
	MOV CX, 10
	DIV CX
	ADD DX, 48
	PUSH AX
	MOV AL, DL
	CALL DrawSymbol
	POP AX
RET
ENDP
DrawScore PROC
	PUSH AX
	PUSH BX
	PUSH DX
	PUSH CX
	
	MOV AX, Score
	MOV DL, 40
	CALL SetCurCol
	CALL DrawDIgit
	
	MOV DL, 39
	CALL SetCurCol
	CALL DrawDIgit
	
	MOV DL, 38
	CALL SetCurCol
	CALL DrawDIgit
	
	MOV DL, 37
	CALL SetCurCol
	CALL DrawDIgit
	
	POP CX
	POP DX
	POP BX
	POP AX
RET
ENDP
EsCLabel:
	MOV AH, 0h
	MOV AL, 3h
	inT 10h

	MOV AH, 4CH
	inT 21h
start:
	MOV AX, @data
	MOV DS, AX
	MOV ES, AX
	
	MOV AH,0h	
	MOV AL,12h	
	inT 10h
	
	MOV AL,Color
	CALL DrawField

	MOV DX, StartY	;начальная Y-координата
	MOV SI, StartX	;начальная Х-координата
	MOV BX,	CSIze	;радиус
	CALL DrawRhombus
	
	MOV CX, WallCount
	WallCreateAgain:
		CALL DrawWall
	LOOP WallCreateAgain

	CALL CreatePoinT
	
	CALL DrawScore
	
	MOV CH, 3 ;начальное направление

	GameCyCLe:
	PUSH SI
	PUSH DX
	PUSH SI
	
	MOV CL, 1
	MOV DL, 0ffh
	PUSH DX
	
	MOV  SI, Speed
	MOV  AH, 0
	PUSH CX
	inT  1AH
	POP CX

	MOV  BX, DX
	ADD  BX, SI
	
	delay_loop:	
		POP DX
		PUSH AX
		MOV AH, 6
		inT 21h
		
		JE skipinput
		;;;;;;;;;;;
		CMP AL, 27
		JNE EsCLabelSkip
		JMP EsCLabel
		EsCLabelSkip:
		;;;;;;;;;;;
		CMP AL, 32
		JNE SkipSave
		MOV Temp, CH
		call save
		SkipSave:
		;;;;;;;;;;;
		CMP AL, 9
		JNE SkipLoad
		JMP Open
		SkipLoad:
		;;;;;;;;;;;
		CMP CL, 0
		JE skipWASD
		CMP AL, 119
		JNE skipUp 
		CMP CH, 2
		JE skipinput
		XOR CH, CH
		XOR CL, CL
		JMP skipinput
		skipUp:
		;;;;;;;;;;;
		CMP AL, 97
		JNE skipL
		CMP CH, 3
		JE skipinput
		MOV CH, 1
		XOR CL, CL
		JMP skipinput
		skipL:
		;;;;;;;;;;;
		CMP AL, 115
		JNE skipD
		CMP CH, 0
		JE skipinput
		MOV CH, 2
		XOR CL, CL
		JMP skipinput
		skipD:
		;;;;;;;;;;;
		CMP AL, 100
		JNE skipR 
		CMP CH, 1
		JE skipinput
		MOV CH, 3
		XOR CL, CL
		JMP skipinput
		skipR:
		;;;;;;;;;;;
		skipWASD:
		;;;;;;;;;;;
		skipinput:
		
		POP AX
		PUSH DX
		PUSH CX
		inT 1AH
		POP CX
		CMP DX, BX
		JNE delay_loop
	POP DX
	POP SI 
	POP DX
	
	MOV BX,	CSIze	;радиус	
	
	CMP CH, 0
	JNE skipUp2
	SUB DX, Cd
	skipUp2:
	CMP CH, 1
	JNE skipL2
	SUB SI, Cd
	skipL2:
	CMP CH, 2
	JNE skipD2
	ADD DX, Cd
	skipD2:
	CMP CH, 3
	JNE skipR2
	ADD SI, Cd
	skipR2:
	
	MOV AL, Color
	
	CALL DrawRhombusWithCHecking
	CMP AX, 1
	JNE skipGameOver
	JMP EsCLabel
	skipGameOver:
	
	ADD Ongoing, 4
	CMP Ongoing, ArrSIze
	JNE SkipNewRound1
	MOV Ongoing, 0
	SkipNewRound1:
	PUSH DI
	MOV DI, Ongoing
	MOV Arr[DI], SI
	MOV Arr[DI + 2], DX
	POP DI
	
	CMP AX, 2
	JE GrowUp
	MOV AL, 0
	PUSH SI
	PUSH DX
	PUSH DI
	MOV DI, Last
	MOV SI, Arr[DI]
	MOV DX, Arr[DI + 2]
	POP DI
	CALL DrawRhombus
	ADD Last, 4
	CMP Last, ArrSIze
	JNE SkipNewRound2
	MOV Last, 0
	SkipNewRound2:
	POP DX
	POP SI
	JMP SkipPoinT
	GrowUp:
	PUSH AX
	MOV AX, score
	inC AX
	MOV score, AX
	POP AX
	CALL DrawScore
	CALL CreatePoinT
	SkipPoinT:
	JMP GameCyCLe
	
end start