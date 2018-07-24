# CS 110 Spring 2018 hw3
# quick_sort.s

#==============================================================================
#                              Quick Sort MIPS
#                                   README
#==============================================================================
# In this file you will be implementing the def_quick_sort and def_partition 
# function, which form the most important part of this code.
# Input part and output part is given for you and do not modify it(or you may 
# fail some testcases).
# In line 48 we give you the choice to test your code using predefined array: array_1,
# you can decomment it and use array_1 to test your code.
# 
#                               IMPORTANT!!!
# Our asembler will support the following registers:
# 
# $zero, $at, $v0, $a0 - $a3, $t0 - $t3, $s0 - $s3, $sp, and $ra. 
# The name $0 can be used in lieu of $zero. Other register numbers 
# (eg. $1, $2, etc.) are not supported.
# 
# We will manually check whether you use registers properly after the ddl.
# So the grade on your autolab is not the final grade of your homework.
#==============================================================================

.data
    space:          .asciiz  " "            # a $space string.
    line:           .asciiz "\n"            # a newline string.
    colonsp:        .asciiz ": "            # a colon string with $space.
    .align 2
    array:          .space 1000             # array to be sorted
    array_1:        .space 1000   	        # test array for you 
    #array_1:       .word 678 567 456 765 876 987 543 654 684 374 # use this line if you want to uncomment line 48
    #array:         .word 678 567 456 765 876 987 543 654 684 374 # use this line if you want to uncomment line 48
    size:           .word  10             # actual count of the elements in the array
                                            # which will change later.                            
    question:               .asciiz "Input number of values to be sorted and traversaled(0 < N < 1000): "
    instruct:               .asciiz "Input each value: "
    inputstring:            .asciiz "Input list:  "
    sorted_array_string:    .asciiz "Sorted:      "
    post_order_traversal:   .asciiz "traversaled: "
    receive_values_loop_iter_string:    .asciiz "Input value#"
                                        

.text
.globl  main
main:
    #j receive_values_end            #if you want to read in instead of using default, delete this line.
    params_info:
        li    $v0, 4                # v0 = 4 means print_string syscall.
        la    $a0, question         # load params_info_string to argument register $a0.
        syscall                     # issue a system call.
    params:
        li    $v0, 5                # v0 = 5 means read_int syscall.
        syscall                     # get length of the array from key_board input
        la    $t0, size       
        sw    $v0, 0($t0)     
    receive_values_loop_info:
        li    $v0, 4                # prompt user to start feeding data into the array
        la    $a0, instruct   
        syscall             
        li    $v0, 4                # print new line
        la    $a0, line       
        syscall             

#   the input loop
    receive_values_loop_prep:
        la    $t0, array 
        la    $t3, array_1          # load array to $t3.
        lw    $t1, size             # load size to $t1.
        li    $t2, 0                # loop iter, starting from 0.
    receive_values_loop:
        bge   $t2, $t1, receive_values_end    # while ($t2 < $t1).
        li    $v0, 4                # prompt at every iteration during input
        la    $a0, receive_values_loop_iter_string 
        syscall             
        li    $v0, 1          
        addi  $a0, $t2, 1           # load (iter + 1) to argument register $a0.
        syscall             
        li    $v0, 4          
        la    $a0, colonsp        
        syscall             

        li    $v0, 5          
        syscall                     # USER INPUT
        sw    $v0, 0($t0)
        sw    $v0, 0($t3)           # store the user input in the array.
        addi  $t3, $t3, 4
        addi  $t0, $t0, 4           # increment array pointer by 4.
        addi  $t2, $t2, 1           # increment loop iter by 1.
        j receive_values_loop       # jump back to the beginning of the loop.

    receive_values_end:
        li    $v0, 4                # 4 = print_string syscall.
        la    $a0, inputstring        
        syscall
        la  $a0, array
        jal print                   # printing user input values

    # Set up the main mergesort call.
    # Arrays are	
    la $a0, array                   # a0 adrs of the array
    li $a1, 0                       # left val
    lw $a2, size                    # right val
    addi $a2, $a2, -1
    jal def_quick_sort  
    li    $v0, 4                # 4 = print_string syscall.
    la    $a0, sorted_array_string   
    syscall
    la  $a0, array
    jal print                   #print out the sorted list

    j exit


########################################################
####################your code here######################
########################################################

# In this part you will implemente quick sort and partition seperately.
# You should learn how to use stack and function call before implemente.
# WARNING: using registers properly or you will get 30% deduction after ddl.
#          50% meaningful comments is needed or you will get 50% deduction after ddl.

# I use the following Python code as my method to implemente my MIPS code.


#def quick_sort_helper(a_list, first, last): # This is "def_quick_sort" in this code
#    if first < last:
#        split_point = partition(a_list, first, last)
#        quick_sort_helper(a_list, first, split_point - 1)
#        quick_sort_helper(a_list, split_point + 1, last)

#def partition(a_list, first, last):
#    pivot_value = a_list[first]
#    left_mark = first + 1
#    right_mark = last
#    done = False

#    while not done:
#        while left_mark <= right_mark and \
#        a_list[left_mark] <= pivot_value:
#        left_mark = left_mark + 1
#        while a_list[right_mark] >= pivot_value and \
#        right_mark >= left_mark:
#        right_mark = right_mark - 1

    #    if right_mark < left_mark:
    #        done = True
    #    else:
    #        temp = a_list[left_mark]
    #        a_list[left_mark] = a_list[right_mark]
    #        a_list[right_mark] = temp

#    temp = a_list[first]
#    a_list[first] = a_list[right_mark]
#    a_list[right_mark] = temp
    
#    return right_mark



#quick_sort part
# a0 - address of array
# a1 - begin index
# a2 - end index
def_quick_sort:
    addi    $sp, $sp, -4            # spare 4 bits for address storage
    sw	    $ra, 0($sp)		        # store the address we come from

    bge     $a1, $a2, exit_qsort    # if first < last:   (conduct the following operations)
    jal     def_partition           #        split_point = partition(a_list, first, last)

    addi    $sp, $sp, -4             # (temperarily save the varible last ($a2))
    sw      $a2, 0($sp)              # store the address
    addi    $a2, $a3, -1             # $a2 = $a3 - 1
    jal     def_quick_sort           # quick_sort_helper(a_list, first, split_point - 1)

    lw      $a2, 0($sp)             # (get the varible last ($a2))            
    addi    $sp, $sp, 4             # store the 
    addi    $a1, $a3, 1             # $a1 = $a3 + 1
    jal     def_quick_sort          # quick_sort_helper(a_list, split_point + 1, last)

    exit_qsort:
        lw  $ra, 0($sp)             # get the address we come from
        addi $sp, $sp, 4            # move back the stack pointer 4 bits
        jr  $ra                     # (back to where we are from)

#def partition(a_list, first, last):
#    pivot_value = a_list[first]
#    left_mark = first + 1
#    right_mark = last
#    done = False

#    while not done:
#        while left_mark <= right_mark and a_list[left_mark] <= pivot_value:
#           left_mark = left_mark + 1
#        while a_list[right_mark] >= pivot_value and right_mark >= left_mark:
#           right_mark = right_mark - 1

    #    if right_mark < left_mark:
    #        done = True
    #    else:
    #        temp = a_list[left_mark]
    #        a_list[left_mark] = a_list[right_mark]
    #        a_list[right_mark] = temp

#    temp = a_list[first]
#    a_list[first] = a_list[right_mark]
#    a_list[right_mark] = temp
    
#    return right_mark

# partition subroutine
# a0 - address of array
# a1 - begin index
# a2 - end index

def_partition:
    sll    $t0, $a1, 2              # $t0 = 4 * first
    addu   $t0, $t0, $a0            # $t0 = $t0 + $a0, the address of array[first]
    lw     $s0, 0($t0)              # $s0 = array[first] (pivot_value)
    addi   $s1, $a1, 1              # $s1 = first + 1 (left_mark)
    add    $s2, $a2, $zero          # $s2 = last (right_mark)
    add    $s3, $zero, $zero        # $s3 = False (0)

    mark_move:
        bne       $s3, $zero, mark_move_end             # while not done:

        left_mark_move:
            bgt       $s1, $s2, right_mark_move         # while left_mark <= right_mark
            sll       $t0, $s1, 2                       # $t0 = $s1 * 4
            addu      $t0, $t0, $a0                     # $t0 = $t0 + $a0, the address of array[left_mark]
            lw        $t0, 0($t0)                       # $t0 = array[left_mark]
            bgt       $t0, $s0, right_mark_move         # and a_list[left_mark] <= pivot_value:
            addi      $s1, $s1, 1                       # left_mark = left_mark + 1
            j         left_mark_move                    # continue looping

        right_mark_move:
            sll       $t0, $s2, 2                       # $t0 = $s2 * 4
            addu      $t0, $t0, $a0                     # $t0 = $t0 + $a0, the address of array[right_mark]
            lw        $t0, 0($t0)                       # $t0 = array[right_mark]
            blt       $t0, $s0, exchange_value          # while a_list[right_mark] >= pivot_value
            blt       $s2, $s1, exchange_value          # and right_mark >= left_mark:
            addi      $s2, $s2, -1                      # right_mark = right_mark - 1
            j         right_mark_move                   # continue looping

        exchange_value:
            bge       $s2, $s1, exchange_value_else     # if right_mark < left_mark:
            addi      $s3, $zero, 1                     # done = True
            j         mark_move                         # go to the end of the program

        exchange_value_else:
            sll       $t0, $s1, 2                       # $t0 = $s1 * 4
            addu      $t0, $t0, $a0                     # $t0 = $t0 + $a0, the address of array[left_mark]
            sll       $t1, $s2, 2                       # $t1 = $s2 * 4
            addu      $t1, $t1, $a0                     # $t1 = $t1 + $a0, the address of array[right_mark]

            lw        $t2, 0($t0)                       # $t2 = array[left_mark]
            lw        $t3, 0($t1)                       # $t3 = array[right_mark]
            sw        $t3, 0($t0)                       # array[left_mark] = $t3
            sw        $t2, 0($t1)                       # array[right_mark] = $t2

            j         mark_move                         # continu  looping

    mark_move_end:
        sll       $t0, $a1, 2              # $t0 = 4 * first
        addu      $t0, $t0, $a0            # $t0 = $t0 + $a0, the address of array[first]
        sll       $t1, $s2, 2              # $t1 = $s2 * 4
        addu      $t1, $t1, $a0            # $t1 = $t1 + $a0, the address of array[right_mark]

        lw        $t2, 0($t0)              # $t2 = array[first]
        lw        $t3, 0($t1)              # $t3 = array[right_mark]
        sw        $t3, 0($t0)              # array[first] = $t3
        sw        $t2, 0($t1)              # array[right_mark] = $t2

        add       $a3, $s2, $zero          # return right_mark ($a3 = $s2)
        jr        $ra                      # return to where we are f







# prog$rams ends
# 
exit:
	li     $v0, 10                         # 10 is the end call.
    syscall                                # syscall

########################################################
####################End of your code####################
########################################################

###       Printing array with size

print:
    print_loop_prep:
        move $t0,$a0
        lw  $t1, size
        li  $t2, 0
    print_loop:
        bge $t2, $t1, print_end
        li  $v0, 1
        lw  $a0, 0($t0)
        syscall
        li  $v0, 4
        la  $a0, space
        syscall
        addi    $t0, $t0, 4
        addi    $t2, $t2, 1
        j   print_loop
    print_end:
        li  $v0, 4
        la  $a0, line
        syscall
        jr  $ra
