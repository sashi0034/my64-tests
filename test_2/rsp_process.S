#include <rsp.inc>
.text

.section .data
.align 8

# Data memory
var1: .word 0x10001000
var2: .word 0x11111111
var3: .word 0x1234ABCD

# Register names
.name $t0, $8
.name $t1, $9
.name $t2, $10

# Entry point
.section .text
.align 8

_start:

	# Load words
	lw t0, var1
	lw t1, var2
	lw t2, var3

	# Calculate
	add t0, t0, t1
	add t0, t0, t2

	# Sore words
	sw t0, var1
	sw $0, var2
	sw $0, var3

	# Finish task
	break
