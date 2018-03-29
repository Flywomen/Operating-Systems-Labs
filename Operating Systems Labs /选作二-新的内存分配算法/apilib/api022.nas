[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api022.nas"]

		GLOBAL	_api_setBestFit

[SECTION .text]

_api_setBestFit:	    			; void api_setBestFit(void addr);
		  	     MOV		EDX,22
		  		 INT		0x40
		  		 RET
