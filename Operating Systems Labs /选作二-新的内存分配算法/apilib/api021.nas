[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api021.nas"]

		GLOBAL	_api_setFirstFit

[SECTION .text]

_api_setFirstFit:	    			; void api_setFirstFit(void addr);
		  	     MOV		EDX,21
		  		 INT		0x40
		  		 RET
