[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api028.nas"]

		GLOBAL	_api_sharealloc

[SECTION .text]

_api_sharealloc:		; struct SHAR * api_sharealloc(char *name,int mode);
		PUSH	EBX
		MOV		EDX,28
		MOV		EBX,[ESP+8]			; buf
		MOV		ECX,[ESP+12]
		INT		0x40
		POP		EBX
		RET
