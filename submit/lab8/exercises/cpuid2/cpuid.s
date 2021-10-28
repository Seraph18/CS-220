#Currently get_cpuid(int *eax, int *ebx, int *ecx, int *edx).
#Modify to get_cpuid(int cpuid_op, int *eax, int *ebx, int *ecx, int *edx).	
	.text
	.globl get_cpuid
get_cpuid:
	pushq   %rcx
	pushq   %rdx
	movl 	%edi, (%eax)	#setup cpuid opcode
	cpuid
	#largest param in %eax
	#12-char manufacturer string in ebx, edx, ecx.
	movl	%eax, (%edi)	#store eax cpuid result
	popq	%rax
	movl	%ebx, (%rsi)	#store ebx cpuid result
	popq	%rax		#pop address for edxP
	movq    %r8, (%rax)    #store edx cpuid result
	popq    %rax		#pop address for ecxP
	movl	%edx, (%rax)	#store ecx cpuid result
	ret
	
