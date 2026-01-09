addi x5, x0, 5

beq  x5, x2, skip   # not taken

addi x2, x0, 5

beq  x5, x2, finish   

skip:
addi x10, x0, 20
finish:
addi x10, x0, 50
