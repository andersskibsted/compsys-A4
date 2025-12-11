.section .text
.global _start

_start:  AUIPC      gp, 0x1
                               ADDI       gp, gp, 776
                               LUI        a0, 0x1000
                               ADDI       sp, a0, 0
                               LUI        a0, 0x2000
                               JAL        ra, 1072c
                               LUI        a0, 0x1000
                               JAL        ra, 1051c
                               JAL        ra, 10554
