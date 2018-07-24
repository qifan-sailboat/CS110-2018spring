# CS 61C Spring 2015 Project 1-2 
# linker_utils.s

#==============================================================================
#                             Project 1-2 Part 3b
#                             Linker Utils README
#==============================================================================
# Implement inst_needs_relocation() and relocate_inst().
#
# It may be helpful to understand how our linker works first. Our linker first
# opens each file, reads, and stores the symbol and relocation data (found in
# the .symbol and .relocation section of each object file). During this pass
# it also computes the size of each text segment so that the final addresses of
# each symbol is known. The function fill_data(), located at the bottom of this
# file, performs this operation on a single file. The linker calls fill_data()
# on each object file.
#
# At this step, the symbol table contains the absolute address of every symbol
# as well as the relative byte offsets of each item that needs relocation (why
# is one absolute and the other relative?). The linker then copies the contents
# of each .text segment into the same executable. However, certain instructions
# need relocation. The inst_needs_relocation() function checks whether an 
# instruction need relocation. If so, the relocate_inst() function will perform
# the relocation.
#==============================================================================

.include "symbol_list.s"
.include "parsetools.s"

.data
textLabel:	.asciiz ".text"
symLabel:	.asciiz ".symbol"
relocLabel: .asciiz ".relocation"

.text

#------------------------------------------------------------------------------
# function inst_needs_relocation()
#------------------------------------------------------------------------------
# Returns whether the given instruction needs relocation.
#
# Arguments:
#  $a0 = 32-bit MIPS instruction
#
# Returns: 1 if the instruction needs relocation, 0 otherwise.
#------------------------------------------------------------------------------
inst_needs_relocation:
	# In your code you are NOT allowed to use the following registers:
	# $t0, $t1, $t6, $t7, $t8, $t9, $s0, $s1, $s2, $s3, $s4, $s5.
	# So basically you have $t2 to $t5 as well as $s6 and $s7 to work with. 
	# Also, you are not allowed to use any $x where x is just a number.
	# YOUR CODE HERE

	addi 			$sp, $sp, -4
	sw 				$s6, 0($sp)
	# store the $s6 register
	addi 			$sp, $sp, -4
	sw 				$s7, 0($sp)
	# store the $s7 register

	addi 			$sp, $sp, -16
	sw 				$t2, 0($sp)
	# store the $t2 register
	sw 				$t3, 4($sp)
	# store the $t3 register
	sw 				$t4, 8($sp)
	# store the $t4 register
	sw 				$t5, 12($sp)
	# store the $t5 register

	move 			$s6, $a0
	# store the $a0
	srl 			$s6, $s6, 26
	# shift $s6 right for 26 bits
	addiu 			$t5, $zero, 2
	# set $t5 as 2
	beq 			$s6, $t5, need_relocation
	# if $s6 == 2 we need to relocate
	addiu 			$t5, $zero, 3
	# set $t5 as 3
	beq 			$s6, $t5, need_relocation
	# if $s6 == 3 we need to relocate
	addiu 			$v0, $zero, 0
	# set $v0 as 0

	lw 				$t2, 0($sp)
	# store the $t2 register back
	lw 				$t3, 4($sp)
	# store the $t3 register back
	lw 				$t4, 8($sp)
	# store the $t4 register back
	lw 				$t5, 12($sp)
	# store the $t5 register back
	addi 			$sp, $sp, 16
	# store the stack ptr back
	lw 				$s7, 0($sp)
	# store the $s7 register back
	addi 			$sp, $sp, 4
	# store the stack ptr back
	lw 				$s6, 0($sp)
	# store the $s6 register back
	addi 			$sp, $sp, 4
	# store the stack ptr back
	jr 				$ra
	# return to where we are

need_relocation:
	
	addiu 			$v0, $zero, 1
	# return 1

	lw 				$t2, 0($sp)
	# store the $t2 register back
	lw 				$t3, 4($sp)
	# store the $t3 register back
	lw 				$t4, 8($sp)
	# store the $t4 register back
	lw 				$t5, 12($sp)
	# store the $t5 register back
	addi 			$sp, $sp, 16
	# store the stack ptr back
	lw 				$s7, 0($sp)
	# store the $s7 register back
	addi 			$sp, $sp, 4
	# store the stack ptr back
	lw 				$s6, 0($sp)
	# store the $s6 register back
	addi 			$sp, $sp, 4
	# store the stack ptr back
	jr 				$ra
	# return to where we are
	
#------------------------------------------------------------------------------
# function relocate_inst()
#------------------------------------------------------------------------------
# Given an instruction that needs relocation, relocates the instruction based
# on the given symbol and relocation table.
#
# You should return error if 1) the addr is not in the relocation table or
# 2) the symbol name is not in the symbol table. You may assume otherwise the 
# relocation will happen successfully.
#
# Arguments:
#  $a0 = an instruction that needs relocating
#  $a1 = the byte offset of the instruction in the current file
#  $a2 = the symbol table
#  $a3 = the relocation table
#
# Returns: the relocated instruction, or -1 if error
#------------------------------------------------------------------------------
relocate_inst:
	# In your code you are NOT allowed to use the following registers:
	# $t0, $t1, $t6, $t7, $t8, $t9, $s0, $s1, $s2, $s3, $s4, $s5.
	# So basically you have $t2 to $t5 as well as $s6 and $s7 to work with. 
	# Also, you are not allowed to use any $x where x is just a number.
	# YOUR CODE HERE

	addi 			$sp, $sp, -28
	sw 				$s6, 0($sp)
	# store the $s6 register
	sw 				$s7, 4($sp)
	# store the $s7 register
	sw 				$t2, 8($sp)
	# store the $t2 register
	sw 				$t3, 12($sp)
	# store the $t3 register
	sw 				$t4, 16($sp)
	# store the $t4 register
	sw 				$t5, 20($sp)
	# store the $t5 register
	sw 				$ra, 24($sp)
	# store the $t6 register

	move 			$s6, $a0
	# save $a0 in $s6
	move 			$s7, $a1
	# save $a1 in $s7
	move 			$t3, $a2
	# save $a2 in $t3
	move 			$t4, $a3
	# save $a3 in $t4

	move 			$a1, $t4
	# save the $a3 to the first arg
	move 			$a0, $s7
	# save the $a` to the second arg
	jal 			symbol_for_addr
	# conduct the function symbol_for_addr

	beq 			$v0, $zero, err
	# return 0, it means err
	move 			$a1, $v0
	# save the return result to the second arg
	move 			$a0, $t3
	# save the return result to the first arg
	jal 			addr_for_symbol
	# get the addr

	addi 			$t2, $zero, -1
	# set $t2 -1
	beq 			$t2, $v0, err
	# return -1, it means there is no such addr
	srl 			$v0, $v0, 2
	# shift the result for 2 bits
	andi 			$v0, $v0, 0x03ffffff
	# get the last 26 bits
	andi 			$t5, $s6, 0xfc000000
	# get the first 6 bits
	or 				$v0, $t5, $v0
	# paste them together

	lw 				$s6, 0($sp)
	# store the $s6 register back
	lw 				$s7, 4($sp)
	# store the $s7 register back
	lw 				$t2, 8($sp)
	# store the $t2 register back
	lw 				$t3, 12($sp)
	# store the $t3 register back
	lw 				$t4, 16($sp)
	# store the $t4 register back
	lw 				$t5, 20($sp)
	# store the $t5 register back
	lw 				$ra, 24($sp)
	# store the address back
	addi 			$sp, $sp, 28
	# store the stack ptr back
	jr 				$ra
	# return to where we are

	err:
		addi 		$v0, $zero, -1	
		lw 			$s6, 0($sp)
		# store the $s6 register back
		lw 			$s7, 4($sp)
		# store the $s7 register back
		lw 			$t2, 8($sp)
		# store the $t2 register back
		lw 			$t3, 12($sp)
		# store the $t3 register back
		lw 			$t4, 16($sp)
		# store the $t4 register back
		lw 			$t5, 20($sp)
		# store the $t5 register back
		lw 			$ra, 24($sp)
		# store the address back
		addi 		$sp, $sp, 28
		# store the stack ptr back
		jr 			$ra
		# return to where we are

###############################################################################
#                 DO NOT MODIFY ANYTHING BELOW THIS POINT                       
###############################################################################	

#------------------------------------------------------------------------------
# function calc_text_size() - DO NOT MODIFY THIS FUNCTION
#------------------------------------------------------------------------------
# Returns number of bytes that the text section takes up. This function assumes
# that when called, the file pointer is currently at the beginning of the text
# section. It also assumes that there will be one instruction per line, and 
# that the .text section ends with a blank line.
#
# Arguments: 
#  $a0 = file pointer, must be pointing to 
#
# Returns: size of the text section in bytes
#------------------------------------------------------------------------------
calc_text_size:
	addiu $sp, $sp, -12
	sw $s0, 8($sp)
	sw $s1, 4($sp)
	sw $ra, 0($sp)

	li $s0, 0			# Initialize counter
	move $s1, $a0		# store file handle into $s1
calc_text_size_next:
	move $a0, $s1		# $a0 = file handle
	jal readline
	blt $v0, $0, calc_text_size_error
	lbu $t0, 0($v1)		# $t0 = first character in buffer
	beq $t0, $0, calc_text_size_done	# Reached a line without instructions
	addiu $s0, $s0, 4		# each instruction = 4 bytes
	bne $v0, $0, calc_text_size_next	# did not reached end of file
calc_text_size_done:
	move $v0, $s0
	j calc_text_size_exit
calc_text_size_error:
	li $v0, -1
calc_text_size_exit:
	lw $s0, 8($sp)
	lw $s1, 4($sp)
	lw $ra, 0($sp)
	addiu $sp, $sp, 12
	jr $ra

#------------------------------------------------------------------------------
# function add_to_symbol_list() - DO NOT MODIFY THIS FUNCTION
#------------------------------------------------------------------------------
# Adds entries from the .symbol or .relocation section into the SymbolList. Each 
# line must be of the format "<number>\t<string>\n", and the section ends with
# a blank line. The file pointer must be at the begining of the section (so the
# line immediately after .symbol or .relocation). Returns the status code in $v0
# and the new SymbolList in $v1.
# 
# Arguments:
#  $a0 = file pointer
#  $a1 = the SymbolList to add to (may or may not be empty)
#  $a2 = base address offset
#
# Returns: 	$v0 = 0 if no errors, -1 if error
#	$v1 = the new SymbolList
#------------------------------------------------------------------------------
add_to_symbol_list:
	addiu $sp, $sp, -20
	sw $s0, 16($sp)
	sw $s1, 12($sp)
	sw $s2, 8($sp)
	sw $s3, 4($sp)
	sw $ra, 0($sp)

	move $s0, $a0		# store file handle into $s0
	move $s1, $a1		# store symbol list into $s1
	move $s2, $a2		# store symbol offset into $s2
atsl_next:
	move $a0, $s0		# $a0 = file handle
	jal readline
	blt $v0, $0, atsl_error
	
	lbu $t0, 0($v1)		# $t0 = first character in buffer
	beq $t0, $0, atsl_done		# Reached a line with only '\n'
	move $s3, $v0		# store whether EOF was reached into $s3
	
	move $a0, $v1
	jal tokenize
	move $a0, $s1		# $a0 = symbol list
	addu $a1, $s2, $v0		# $a1 = symbol offset (bytes)
	move $a2, $v1 		# $a2 = symbol name (string)
	jal add_to_list
	move $s1, $v0
	beq $s3, $0, atsl_done
	j atsl_next
atsl_done:
	li $v0, 0
	move $v1, $s1
	j atsl_end
atsl_error:
	li $v0, -1
atsl_end:
	lw $s0, 16($sp)
	lw $s1, 12($sp)
	lw $s2, 8($sp)
	sw $s3, 4($sp)
	lw $ra, 0($sp)
	addiu $sp, $sp, 20
	jr $ra

#------------------------------------------------------------------------------
# function fill_data() - DO NOT MODIFY THIS FUNCTION
#------------------------------------------------------------------------------
# Builds the symbol/relocation data for a single file. Calls calc_text_size()
# and add_to_symbol_list().
# 
# Arguments:
#  $a0 = file handle
#  $a1 = symbol table
#  $a2 = ptr to an reloc_data struct
#  $a3 = base offset
#
# The format of reloc_data (if it were declared in C) is:
#  struct reloc_data {
#    int text_size;
#    SymbolList* list;
#  }
#
# Returns: 	$v0 = 0 if no error, -1 if error
#	$v1 = the new symbol table ($a2 should be updated)
#------------------------------------------------------------------------------
fill_data:
	addiu $sp, $sp, -28
	sw $s0, 24($sp)
	sw $s1, 20($sp)
	sw $s2, 16($sp)
	sw $s3, 12($sp)
	sw $s4, 8($sp)
	sw $s5, 4($sp)
	sw $ra, 0($sp)

	move $s0, $a0		# store file handle into $s0
	move $s1, $a1		# store symbol table into $s1
	move $s2, $a2		# store relocdata ptr into $s2
	move $s3, $a3		# store offset into $s3
fill_data_next:
	move $a0, $s0		# $a0 = file handle
	jal readline
	blt $v0, $0, fill_data_error
	beq $v0, $0, fill_data_done
	move $s4, $v1		# store read string into $s4
	
	move $a0, $s4		# Test if reached .text section
	la $a1, textLabel
	jal streq
	beq $v0, $0, fill_data_text_size
	
	move $a0, $s4		# Test if reached .symbol section
	la $a1, symLabel
	jal streq
	beq $v0, $0, fill_data_symbol
	
	move $a0, $s4		# Test if reached .relocation section
	la $a1, relocLabel
	jal streq
	beq $v0, $0, fill_data_reloc
	
	j fill_data_next
fill_data_text_size:
	move $a0, $s0
	jal calc_text_size
	sw $v0, 0($s2)
	j fill_data_next
fill_data_symbol:
	move $a0, $s0
	move $a1, $s1
	move $a2, $s3
	jal add_to_symbol_list
	bne $v0, $0, fill_data_error
	move $s5, $v1
	j fill_data_next
fill_data_reloc:
	move $a0, $s0
	lw $a1, 4($s2)
	li $a2, 0
	jal add_to_symbol_list
	bne $v0, $0, fill_data_error
	sw $v1, 4($s2)
	j fill_data_next
fill_data_error:
	li $v0, -1
	j fill_data_end
fill_data_done:
	li $v0, 0
fill_data_end:	
	move $v1, $s5
	lw $s0, 24($sp)
	lw $s1, 20($sp)
	lw $s2, 16($sp)
	lw $s3, 12($sp)
	lw $s4, 8($sp)
	lw $s5, 4($sp)
	lw $ra, 0($sp)
	addiu $sp, $sp, 28
	jr $ra
