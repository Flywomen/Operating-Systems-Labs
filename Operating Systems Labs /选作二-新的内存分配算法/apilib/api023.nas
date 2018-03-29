[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api023.nas"]

		GLOBAL	_api_setWorstFit

[SECTION .text]

_api_setWorstFit:	    			; void api_setWorstFit(void addr);
		  	     MOV		EDX,23
		  		 INT		0x40
		  		 RET
