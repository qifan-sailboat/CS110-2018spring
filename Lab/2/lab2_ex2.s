# Compute first twelve Fibonacci numbers and put in array, then print
      .data
fibs: .word   0 : 8        # "array" of 12 words to contain fib values
size: .word  12             # size of "array" 
      .text
      li $v0, 5
      syscall
      move $s0, $v0
      
      li $v0, 5
      syscall
      move $s1, $v0
                  
      sw $s0, 0($t0)
      sw $s1, 4($t0)
      add $t2, $t1, $t0
      add $t3, $t2, $t1
      add $t4, $t3, $t2
      add $t5, $t4, $t3
      add $t6, $t5, $t4
      add $t7, $t6, $t5
      
      lw $a0, $t7
      li $a0, 1
      syscall