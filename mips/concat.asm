# Example : a = "test"
# b = "1" ${a}
.data

label: .asciiz "test"

.text
j main


# Function to save values from temporary registers to stack
#
# Arguments:
#   None
#
# Returns:
#   Nothing
fct_save_temp_values:
    # $t0-7 + $s0-7 => 64
    addi $sp, $sp, -64
    sw $t0, 0($sp)
    sw $t1, 4($sp)
    sw $t2, 8($sp)
    sw $t3, 12($sp)
    sw $t4, 16($sp)
    sw $t5, 20($sp)
    sw $t6, 24($sp)
    sw $t7, 28($sp)
    sw $s0, 32($sp)
    sw $s1, 36($sp)
    sw $s2, 40($sp)
    sw $s3, 44($sp)
    sw $s4, 48($sp)
    sw $s5, 52($sp)
    sw $s6, 56($sp)
    sw $s7, 60($sp)
    jr $ra

# Function to load values from stack to temporary registers
#
# Arguments:
#   None
#
# Returns:
#   Nothing
fct_load_temp_values:
    lw $t0, 0($sp)
    lw $t1, 4($sp)
    lw $t2, 8($sp)
    lw $t3, 12($sp)
    lw $t4, 16($sp)
    lw $t5, 20($sp)
    lw $t6, 24($sp)
    lw $t7, 28($sp)
    lw $s0, 32($sp)
    lw $s1, 36($sp)
    lw $s2, 40($sp)
    lw $s3, 44($sp)
    lw $s4, 48($sp)
    lw $s5, 52($sp)
    lw $s6, 56($sp)
    lw $s7, 60($sp)
    addi $sp, $sp, 64
    jr $ra

# Function to count the number of characters in a buffer until the Nul ('\0') character
#
# Arguments:
#   $a0: Pointer to the start of the buffer
#
# Returns:
#   $v0: Number of characters in the buffer until the Nul character
_fct_buffer_len:

    # Write memory to stack
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    jal fct_save_temp_values

    addi $t0, $zero, 0  # Initialize a counter to 0
    addi $t1, $zero, 1  # Initialize a loop condition to 1 (true)
    fct_buffer_len_loop:
        lb $t2, 0($a0)  # Load the current element of the buffer into register $t2
        beq $t2, $zero, fct_buffer_len_exit  # If the current element is the Nul character, go to the exit label
        addi $t0, $t0, 1  # Increment the counter
        addi $a0, $a0, 1  # Increment the address of the current element in the buffer
        b fct_buffer_len_loop  # Go back to the start of the loop
    fct_buffer_len_exit:
        move $v0, $t0  # Return the counter

        # Load memory from stack
        jal fct_load_temp_values
        lw $ra, 0($sp)
        addi $sp, $sp, 4

        jr $ra


# Function to write a buffer to the heap
#
# Arguments:
#   $a0: Pointer to the start of the buffer
#   $a1: Number of elements in the buffer
#
# Returns:
#   $v0: Pointer to the start of the allocated memory on the heap
fct_buffer_write:

    # Write memory to stack
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    jal fct_save_temp_values

    move $t0, $a0 # buffer_start_ptr to $t0
    # Allocate memory on the heap for the buffer
    li $v0, 9  # sbrk to $v0
    li $a0, 4  # Set up the first argument (number of bytes to allocate) in register $a0
    mul $a0, $a1, $a0  # Multiply the number of elements by the size of each element to get the total number of bytes to allocate
    syscall  # Execute the syscall instruction
    move $t1, $v0  # Save the return value (start address of the allocated memory) in register $t1
    move $t4, $v0 # Save the return value into the register $t4
    # Store the elements of the buffer in the allocated memory
    addi $t2, $zero, 0  # Initialize a loop counter to 0
    fct_write_buffer_loop:
        lw $t3, 0($t0)  # Load the current element of the buffer into register $t3
        sw $t3, 0($t1)  # Store the element at the current address in the allocated memory
        addi $t1, $t1, 4  # Increment the current address by the size of an element
        addi $t2, $t2, 1  # Increment the loop counter
        addi $t0, $t0, 4  # Increment the address of the current element in the buffer
        bne $t2, $a1, fct_write_buffer_loop  # If the loop counter is not equal to the number of elements, go back to the start of the loop

    # Return the start address of the allocated memory
    move $v0, $t4 # Get start ptr in register $t4

    # Load memory from stack
    jal fct_load_temp_values
    lw $ra, 0($sp)
    addi $sp, $sp, 4

    jr $ra # Return

main:

	# DEFINE OF 'a' HERE
    la $t0, label
    # Add one argument to function
    move $a0, $t0
    jal _fct_buffer_len

    move $t1, $v0
    addi $t1, $t1, 1 # add terminator

    move $a0, $t0
    move $a1, $t1
    jal fct_buffer_write

    addi $sp, $sp, -4
    sw $v0, 0($sp) # write




    #



    li $v0, 10
    syscall
