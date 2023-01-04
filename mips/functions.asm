.data
global_read_buffer_len: .word 256
global_read_buffer:
    .space 256  # Reserve 256 bytes of space in the data section
    .asciiz "This is a static string in MIPS that is 256 bytes long." 

global_test: .asciiz "a1005"

.text

.globl fct_buffer_len
.globl fct_buffer_write
.globl fct_atoi
j main

print_string:
    # $a0 = address of string to print

    # Print all characters until the null terminator is encountered
    move $t0, $a0 # $t0 = address of string

print_string_loop:
    # Print the character at the current address
    lbu $t2, 0($t0) # $t2 = value of byte at address in $t0
    beq $t2, $zero, print_string_done # If character is null terminator, exit loop
    addi $t0, $t0, 4 # Increment address
    li $v0, 11 # System call for printing character
    syscall # Print character
    j print_string_loop # Loop until null terminator is encountered

print_string_done:
    # Return to caller
    jr $ra

# atoi - convert a string to an integer
#
# $a0 - address of the string to be converted
#
# returns: integer value of the string in $v0
fct_atoi:
  # Initialize the result to 0
  li $v0, 0

  # Initialize the index to 0
  li $t0, 0

  # Get the first character in the string
  lb $t1, 0($a0)

  # Loop until we reach the end of the string
  fct_atoi_loop:
    # Check if the character is a digit
    beq $t1, '0', fct_atoi_digit
    bgt $t1, '9', fct_atoi_not_digit
    subu $t1, $t1, '0'

    # Add the digit to the result
    sll $t2, $t0, 2
    addu $v0, $v0, $t2
    addu $v0, $v0, $t1

    # Increment the index
    addiu $t0, $t0, 1

    # Get the next character in the string
    lb $t1, 0($a0)
    j fct_atoi_loop

  # We have reached the end of the string, so return the result
  jr $ra

fct_atoi_digit:
  # The character is a digit, so add it to the result
  subu $t1, $t1, '0'
  addu $v0, $v0, $t1

  # Increment the index
  addiu $t0, $t0, 1

  # Get the next character in the string
  # lb $t1, $t0($a0)
  j fct_atoi_loop

fct_atoi_not_digit:
  # The character is not a digit, so return the result
  jr $ra

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
    
    jr $ra

# Function to count the number of characters in a buffer until the Nul ('\0') character
#
# Arguments:
#   $a0: Pointer to the start of the buffer
#
# Returns:
#   $v0: Number of characters in the buffer until the Nul character
fct_buffer_len:
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
    jr $ra # Return

# Function get an address from the stack at a specific offset and store it in a given register
#
# Arguments:
#   $a0: the offset of the address on the stack
#   $a1: the register to store the integer in
#
# Returns:
#   Nothing
fct_read_address_from_stack:
    add $t0, $sp, $a0   # Adds the offset to the stack pointer and saves it to $t0
    lw $a1, 0($t0)      # Load the address from the stack and store it in the given register
    jr $ra              # Return

# Convert a string to an integer
#
# Arguments:
#   $a0 - address of the string
#
# Returns:
#   $v0 - the integer representation of the string
fct_atoi:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    jal fct_save_temp_values

    move $a1, $a0
    jal fct_buffer_len
    beq $v0, $zero, _fct_atoi_error
    move $a0, $a1

    lb $t0, 0($a0)
    li $t6, 1

    li $t7, '+'
    bltu $t0, $t7, _fct_atoi_error
    li $t7, '9'
    bltu $t7, $t0, _fct_atoi_error
    li $t7, ','
    beq $t0, $t7, _fct_atoi_error
    li $t7, '.'
    beq $t0, $t7, _fct_atoi_error
    li $t7, '/'
    beq $t0, $t7, _fct_atoi_error

    li $t7, '+'
    beq $t0, $t7, _fct_atoi_set_plus
    li $t7, '-'
    beq $t0, $t7, _fct_atoi_set_minus

    _fct_atoi_init:
        li $t2, 10
        li $t1, 1
        move $t7, $v0
        move $v0, $zero
        beq $t7, 1, _fct_atoi_loop

    _fct_atoi_setup:
        mul $t1, $t1, 10
        addi $t6, $t6, 1
        blt $t6, $t7, _fct_atoi_setup

    _fct_atoi_loop:
        lb $t0, 0($a0)
        beq $t0, $zero, _fct_atoi_end_loop

        li $t7, '0'
        bltu $t0, $t7, _fct_atoi_error
        li $t7, '9'
        bltu $t7, $t0, _fct_atoi_error

        sub $t0, $t0, '0'
        mul $t0, $t0, $t1
        add $v0, $v0, $t0

        div $t1, $t2
        mflo $t1

        addi $a0, $a0, 1
        j _fct_atoi_loop

    _fct_atoi_set_plus:
        addi $t6, $t6, 1
        addi $a0, $a0, 1
        j _fct_atoi_init

    _fct_atoi_set_minus:
        li $v1, 1
        addi $t6, $t6, 1
        addi $a0, $a0, 1
        j _fct_atoi_init

    _fct_atoi_error:
        la $a0, _error_nan
        li $v0, 4
        syscall
        li $v0, 17
        syscall

    _fct_atoi_end_loop:
        bne $v1, 1, _fct_atoi_end

    _fct_atoi_apply_minus:
        mul $v0, $v0, -1

    _fct_atoi_end:
        jal fct_load_temp_values
        lw $ra, 0($sp)
        addi $sp, $sp, 4
        jr $ra

# Compares two strings
#
# Arguments:
#   $a0 - address of the first string
#   $a1 - address of the second string
#
# Returns:
#   $v0 - 1 if equal, else 0
fct_strcmp:

    # Write memory to stack
    addi $sp, $sp, -4
    sw $ra, 0($sp)

    move $t0, $a0 # copy a0 into t0 since function following is editing a0
    move $t1, $a1 # copy a1 into t1 since function following is editing a1

    jal fct_buffer_len # get a0 length into v0
    move $t2, $v0 # first string length into $t2

    move $a0, $a1 # setup call fct_buffer_len
    jal fct_buffer_len # get a0 length into v0
    move $t3, $v0 # second string length into $t3

    bne $t2, $t3, to_false # lengths not equal

    # moving strings back into registers
    move $a0, $t0
    move $a1, $t1

    loop :

    	# Load the current characters
        lb $t0, 0($a0)
        lb $t1, 0($a1)

        # Check if we've reached the end of the string
        beq $t0, $zero, to_true

        # Check if characters are equal
        bne $t0, $t1, to_false

        # Move to the next character in the string
        addi $a0, $a0, 1
        addi $a1, $a1, 1
    	j loop

    to_true:
    	li $v0, 1 # string equals
    	j end_loop

    to_false :
    	move $v0, $zero

    # End of loop memory from stack
    end_loop:
    # Load memory from stack
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    # Return the result
    jr $ra

main:

    
    la $a0, global_test
    li $v0, 4
    syscall

