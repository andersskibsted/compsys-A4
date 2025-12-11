# Et simpelt RISC-V program der adderer to tal

.data
    result: .word 0          # Plads til resultatet

.text
.globl main

main:
# Indlæs to tal i registre
              li    a0, 42          # Load immediate: a0 = 42
              li    a1, 58          # Load immediate: a1 = 58

# Addér tallene
              add   a2, a0, a1      # a2 = a0 + a1

# Gem resultatet i hukommelsen
              la    t0, result      # Load address of result
              sw    a2, 0(t0)       # Store word: gem a2 på adressen i t0
print_string:
              lbu   a4, a0, 0
              addi  a5, a0, 0
              beq   a4, zero, 10610
              addi  a5, a5, 1
              addi  a0, a4, 0
              addi  a7, zero, 2
              ecall

uns_to_string:
              addi  a4, a0, 0
              addi  a2, a0, 0
              addi  a7, zero, 9
              addi  a0, zero, 10
              beq   a1, zero, xx
              remu  a3, a1, a0
              addi

# Afslut programmet (syscall exit)
              li    a7, 10          # Syscall nummer 10 = exit
              ecall                 # Environment call
