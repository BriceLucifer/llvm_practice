	.text
	.def	@feat.00;
	.scl	3;
	.type	0;
	.endef
	.globl	@feat.00
.set @feat.00, 0
	.file	"test.ll"
	.def	mult;
	.scl	2;
	.type	32;
	.endef
	.globl	mult                            # -- Begin function mult
	.p2align	4, 0x90
mult:                                   # @mult
# %bb.0:
	movl	%ecx, %eax
	imull	%edx, %eax
	retq
                                        # -- End function
