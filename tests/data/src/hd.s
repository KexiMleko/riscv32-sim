addi x1, x0, 42       # x1 = 42                                                                                                                                                                
sw   x1, 0(x0)        # mem[0] = 42                                                                                                                                                            
lw   x2, 0(x0)        # x2 <- mem[0]  (producer, in EX at cycle N)                                                                                                                             
add  x3, x2, x1       # consumer of x2 — load-use hazard, should stall 1 cycle                                                                                                                 
addi x4, x3, 1        # x4 = 85 if correct 
