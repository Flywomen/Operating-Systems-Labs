[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api030.nas"]

		GLOBAL	_api_change

[SECTION .text]

_api_change:		; void api_change(struct SHAR *shar, int position,char ch);
		PUSH	EBX
		MOV		EDX,30
		MOV		EBX,[ESP+8]			; shar
		MOV		ECX,[ESP+12]		;position
		MOV		ESI,[ESP+16]		;ch
		INT		0x40
		POP		EBX
		RET