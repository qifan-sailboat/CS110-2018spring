# CS 61C Spring 2015 Project 1-2 
# string.s

#==============================================================================
#                              Project 1-2 Part 1
#                               String README
#==============================================================================
# In this file you will be implementing some utilities for manipulating strings.
# The functions you need to implement are:
#  - strlen()
#  - strncpy()
#  - copy_of_str()
# Test cases are in linker-tests/test_string.s
#==============================================================================

.data
newline:	.asciiz "\n"
tab:	.asciiz "\t"

.text
#------------------------------------------------------------------------------
# function strlen()
#------------------------------------------------------------------------------
# Arguments:
#  $a0 = string input
#
# Returns: the length of the string
#------------------------------------------------------------------------------
strlen:
	# In your code you are NOT allowed to use the following registers:
	# $t0, $t1, $t6, $t7, $t8, $t9, $s0, $s1, $s2, $s3, $s4, $s5.
	# So basically you have $t2 to $t5 as well as $s6 and $s7 to work with. 
	# Also, you are not allowed to use any $x where x is just a number.
	addi 		$sp, $sp, -4
	sw 			$s6, 0($sp)
	addi 		$sp, $sp, -4
	sw 			$s7, 0($sp)

	li 			$s6, 0
	# Initialize the function result $v0
	# move 		$s7, $a0
strlen_loop:

	lb  		$t3, 0($a0)
	# Get one character (1 byte) 
	addiu 		$s6, $s6, 1
	# Count one more since there is a character 
	addiu 		$a0, $a0, 1
	# move one position backwards to get the next character
	bne 		$t3, $zero, strlen_loop
	# If the character we get now is zero, it means that it has finished
	# Otherwise, it means there are still some characters left

	addi 		$v0, $s6, -1
	# The last character in fact does not exist
	# minus one back

	# YOUR CODE HERE
	lw 			$s7, 0($sp)
	addi 		$sp, $sp, 4
	lw 			$s6, 0($sp)
	addi 		$sp, $sp, 4
	jr $ra

#------------------------------------------------------------------------------
# function strncpy()
#------------------------------------------------------------------------------
# Arguments:
#  $a0 = pointer to destination array
#  $a1 = source string
#  $a2 = number of characters to copy
#
# Returns: the destination array
#------------------------------------------------------------------------------
strncpy:
	# In your code you are NOT allowed to use the following registers:
	# $t0, $t1, $t6, $t7, $t8, $t9, $s0, $s1, $s2, $s3, $s4, $s5.
	# So basically you have $t2 to $t5 as well as $s6 and $s7 to work with. 
	# Also, you are not allowed to use any $x where x is just a number.
	# YOUR CODE HERE
	addi 		$sp, $sp, -4
	sw 			$s6, 0($sp)
	# store the $s6 register
	addi 		$sp, $sp, -4
	sw 			$s7, 0($sp)
	# store the $s7 register

	move 		$s6, $a0
	# move $a0 into saved values
	move		$s7, $a1
	# move $a1 into saved values
	addi 		$t5, $zero, 0
	# $t5 is used to count the number of copying times

copy:
	beq			$t5, $a2, after
	# judge whether the copy time reaches what $a2 requires
	lb			$t2, 0($s7)
	# save one character to temporary register $t2
	sb 			$t2, 0($s6)
	# save the character to the target array
	addi 		$t5, $t5, 1
	# count one more time
	addi 		$s6, $s6, 1
	# move the pointer to the array one more step
	addi 		$s7, $s7, 1
	# move the source string one more step backwards
	j copy
	# looping

after:
	move 		$v0, $a0
	# return the pointer to destination array

	lw 			$s7, 0($sp)
	# store the $s7 register back
	addi 		$sp, $sp, 4
	# store the stack ptr back
	lw 			$s6, 0($sp)
	# store the $s6 register back
	addi 		$sp, $sp, 4
	# store the stack ptr back
	jr $ra
	# return to where we are

#------------------------------------------------------------------------------
# function copy_of_str()
#------------------------------------------------------------------------------
# Creates a copy of a string. You will need to use sbrk (syscall 9) to allocate
# space for the string. strlen() and strncpy() will be helpful for this function.
# In MARS, to malloc memory use the sbrk syscall (syscall 9). See help for details.
#
# Arguments:
#   $a0 = string to copy
#
# Returns: pointer to the copy of the string
#------------------------------------------------------------------------------
copy_of_str:
	# In your code you are NOT allowed to use the following registers:
	# $t0, $t1, $t6, $t7, $t8, $t9, $s0, $s1, $s2, $s3, $s4, $s5.
	# So basically you have $t2 to $t5 as well as $s6 and $s7 to work with. 
	# Also, you are not allowed to use any $x where x is just a number.
	# YOUR CODE HERE

	addi 		$sp, $sp, -4
	sw 			$s6, 0($sp)
	# store the $s6 register
	addi 		$sp, $sp, -4
	sw 			$s7, 0($sp)
	# store the $s7 register

	move		$s6, $a0
	# store the string to copy
	addi 		$sp, $sp, -4
	sw 			$ra, 0($sp)
	# since we will use jal, $ra will be used
	# we save $ra temporarily in stack

	jal strlen
	# get the length of the string to be copied
	addi 		$a0, $v0, 1	
	# spare one more space to store the string

	addi 		$v0, $zero, 9
	# let $v0 be 9, which will spare space (size of $a0)
	syscall
	# allocate the space

	move		$a2, $a0
	# set $a2 as the number of the string
	move 		$a0, $v0
	# give the allocated space to $a0
	move 		$a1, $s6
	# give the string to be copied
	jal			strncpy
	# start to copy

	lw 			$ra, 0($sp)
	addi 		$sp, $sp, 4
	# get the original address to $ra
	# move the stack ptr back

	lw 			$s7, 0($sp)
	# store the $s7 register back
	addi 		$sp, $sp, 4
	# store the stack register back
	lw 			$s6, 0($sp)
	# store the $s6 register back
	addi 		$sp, $sp, 4
	# store the stack register back
	jr $ra
	# go back to where we are

###############################################################################
#                 DO NOT MODIFY ANYTHING BELOW THIS POINT                       
###############################################################################

#------------------------------------------------------------------------------
# function streq() - DO NOT MODIFY THIS FUNCTION
#------------------------------------------------------------------------------
# Arguments:
#  $a0 = string 1
#  $a1 = string 2
#
# Returns: 0 if string 1 and string 2 are equal, -1 if they are not equal
#------------------------------------------------------------------------------
streq:
	beq $a0, $0, streq_false	# Begin streq()
	beq $a1, $0, streq_false
streq_loop:
	lb $t0, 0($a0)
	lb $t1, 0($a1)
	addiu $a0, $a0, 1
	addiu $a1, $a1, 1
	bne $t0, $t1, streq_false
	beq $t0, $0, streq_true
	j streq_loop
streq_true:
	li $v0, 0
	jr $ra
streq_false:
	li $v0, -1
	jr $ra			# End streq()

#------------------------------------------------------------------------------
# function dec_to_str() - DO NOT MODIFY THIS FUNCTION
#------------------------------------------------------------------------------
# Convert a number to its unsigned decimal integer string representation, eg.
# 35 => "35", 1024 => "1024". 
#
# Arguments:
#  $a0 = int to write
#  $a1 = character buffer to write into
#
# Returns: the number of digits written
#------------------------------------------------------------------------------
dec_to_str:
	li $t0, 10			# Begin dec_to_str()
	li $v0, 0
dec_to_str_largest_divisor:
	div $a0, $t0
	mflo $t1		# Quotient
	beq $t1, $0, dec_to_str_next
	mul $t0, $t0, 10
	j dec_to_str_largest_divisor
dec_to_str_next:
	mfhi $t2		# Remainder
dec_to_str_write:
	div $t0, $t0, 10	# Largest divisible amount
	div $t2, $t0
	mflo $t3		# extract digit to write
	addiu $t3, $t3, 48	# convert num -> ASCII
	sb $t3, 0($a1)
	addiu $a1, $a1, 1
	addiu $v0, $v0, 1
	mfhi $t2		# setup for next round
	bne $t2, $0, dec_to_str_write
	jr $ra			# End dec_to_str()
