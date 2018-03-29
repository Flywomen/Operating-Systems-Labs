[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api034.nas"]

		GLOBAL	_api_changeout

[SECTION .text]

_api_changeout:		; void api_changeout(struct SHAR *shar,int in);
		PUSH	EBX
		MOV		EDX,34
		MOV		EBX,[ESP+8]			; shar
		MOV		ECX,[ESP+12]		;out
		INT		0x40
		POP		EBX
		RET