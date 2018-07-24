hex_to_str:
	# In your code you are NOT allowed to use the following registers:
	# $t0, $t1, $t6, $t7, $t8, $t9, $s0, $s1, $s2, $s3, $s4, $s5.
	# So basically you have $t2 to $t5 as well as $s6 and $s7 to work with. 
	# Also, you are not allowed to use any $x where x is just a number.
	# YOUR CODE HERE

	addi 		$sp, $sp, -4
	sw 			$s6, 0($sp)
	addi 		$sp, $sp, -4
	sw 			$s7, 0($sp)

	sb 				$zero, 9($a1)
	# the ninth element in one num is \0, which is 0 in ASCII
	addiu 			$t2, $zero, 10
	sb 				$t2, 8($a1)	
	# the ninth element in one num is \n, which is 10 in ASCII
	addiu 			$t3, $zero, 7
	# $t2 records the bit of the number, in total 8 bits

	hex_trans:
		blt 		$t3, $zero, hex_exit
		andi 		$t4, $a0, 15
		# $a0 & 15,get the last 4 bits
		srl 		$a0, $a0, 4
		# shift right for 4 bits, remove the last 4 numbers
		addi 		$s6, $zero, 10
		blt 		$t4, $s6, less_bit
		# If a 4 bit int less than or equal 9 (0111),
		# It means the number in fact has less than 3 bits.
		addiu 		$t4, $t4, 87
		addu 		$t5, $a1, $t3
		sb 			$t4, 0($t5)
		addiu 		$t3, $t3, -1
		j 			hex_trans

	less_bit:
		addiu 		$t4, $t4, 48
		# set the fourth, fifth and sixth bits as 1
		addu 		$t5, $a1, $t3
		sb 			$t4, 0($t5)
		addiu 		$t3, $t3, -1

	hex_exit:
		lw 			$s7, 0($sp)
		addi 		$sp, $sp, 4
		lw 			$s6, 0($sp)
		addi 		$sp, $sp, 4
		jr $ra
