	.text
	.globl get_parity
#edi contains n	
get_parity:

	#@TODO: add code here to set eax to 1 iff edi has even parity
	
	testl %edi, %edi
	
	jpe even
	movl $0, %eax
	ret

even:	movl $1, %eax
	
	ret
