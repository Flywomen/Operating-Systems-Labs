[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api032.nas"]

		GLOBAL	_api_getout

[SECTION .text]

_api_getout:		; int api_getout(struct SHAR *shar);
		PUSH	EBX
		MOV		EDX,32
		MOV		EBX,[ESP+8]			; shar
		INT		0x40
		POP		EBX
		RET