[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api035.nas"]

		GLOBAL	_api_set_time_waiting

[SECTION .text]

_api_set_time_waiting:		; void api_changeout(struct SHAR *shar,int in);
		PUSH	EBX
		MOV		EDX,35
		MOV		EBX,[ESP+8]			; shar
		INT		0x40
		POP		EBX
		RET
