[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api031.nas"]

		GLOBAL	_api_getin

[SECTION .text]

_api_getin:		; int api_getin(struct SHAR *shar);
		PUSH	EBX
		MOV		EDX,31
		MOV		EBX,[ESP+8]			; shar
		INT		0x40
		POP		EBX
		RET