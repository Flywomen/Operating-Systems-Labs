[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api029.nas"]

		GLOBAL	_api_get

[SECTION .text]

_api_get:		; char api_get(struct SHAR *shar, int position);
		PUSH	EBX
		MOV		EDX,29
		MOV		EBX,[ESP+8]			; shar
		MOV		ECX,[ESP+12]		;position
		INT		0x40
		POP		EBX
		RET