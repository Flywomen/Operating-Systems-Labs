[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api021.nas"]

		GLOBAL	_api_showAddr

[SECTION .text]

_api_showAddr:	    ; void api_showAddr(int addr);
		  PUSH	    EBX
		  MOV		EDX,21
		  MOV		EBX,[ESP+8]		; addr
		  INT		0x40
		  POP		EBX
		  RET
