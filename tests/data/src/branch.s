addi x5, x0, 5
nop
nop
beq  x5, x2, skip   # not taken

addi x2, x0, 5

nop
nop

beq  x5, x2, finish   

skip:
addi x10, x5, 20
nop
nop
finish:
addi x10, x10, 50
#addi x11, x0, 12
