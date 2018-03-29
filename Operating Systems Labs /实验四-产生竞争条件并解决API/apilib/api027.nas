[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api027.nas"]

		GLOBAL	_api_jingzheng

[SECTION .text]

_api_jingzheng:		; int api_jingzheng(void);
		MOV		EDX,27
		INT		0x40
		RET
