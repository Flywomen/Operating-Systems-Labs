[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api028.nas"]

		GLOBAL	_api_printFreeMem

[SECTION .text]

_api_printFreeMem:					; int api_printFreeMem(void);
				  PUSH 		EBX
				  MOV		EBX,[CS:0x0020]
				  MOV		EDX,28
				  INT		0x40
				  POP		EBX
				  RET
