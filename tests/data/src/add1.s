  # x1 = 5
  addi  x1, x0, 5        # 0x00500093
                                                                                                                                                                                                 
  # x2 = x1 + x1         # depends on x1, waits on tag
  add   x2, x1, x1       # 0x001080b3  (wait: rd=x1? no, rd=x2)                                                                                                                                  
  # actually: add x2, x1, x1 -> 0x001080B3                                                                                                                                                       
                                                                                                                                                                                                 
  # x3 = x2 + 1          # depends on x2 -> chained RAW, sits in RS with rs1_tag set                                                                                                             
  addi  x3, x2, 1        # 0x00110193                                                                                                                                                            
                                                                                                                                                                                                 
  # x4 = 7 + 3           # independent -> should complete before x3 resolves                                                                                                                     
  addi  x4, x0, 7        # 0x00700213
  addi  x5, x0, 3        # 0x00300293                                                                                                                                                            
  add   x6, x4, x5       # 0x00520333
