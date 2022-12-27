.data

global_read_buffer_len: .word 256
global_read_buffer:
    .space 256  # Reserve 256 bytes of space in the data section
    .asciiz "This is a static string in MIPS that is 256 bytes long." 

.text

.globl fct_buffer_len
.globl fct_buffer_write
j main

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
    jr $ra
    
    
main:
    # Read a string from the user
    li $v0, 8  # Set up the syscall number for read_string (8) in register $v0
    la $a0, global_read_buffer  # Load the address of the string into register $a0
    li $a1, 256  # Set up the maximum number of characters to read in register $a1
    syscall  # Execute the syscall instruction

    # Count the number of characters in the string until the Nul character
    la $a0, global_read_buffer  # Load the address of the string into register $a0
    jal fct_buffer_len  # Call the fct_buffer_len function
    move $a1, $v0  # Save the return value (number of characters in the string) in register $a1

    # Write the string to the heap and add the return value (start address of the allocated memory) to the stack
    la $a0, global_read_buffer  # Load the address of the string into register $a0
    jal fct_buffer_write  # Call the fct_buffer_writ function
    sw $v0, 0($sp)  # Store the return value (start address of the allocated memory) at the top of the stack (offset 0 from the stack pointer)
