[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api033.nas"]

		GLOBAL	_api_changein

[SECTION .text]

_api_changein:		; void api_changein(struct SHAR *shar,int in);
		PUSH	EBX
		MOV		EDX,33
		MOV		EBX,[ESP+8]			; shar
		MOV		ECX,[ESP+12]		;in
		INT		0x40
		POP		EBX
		RET