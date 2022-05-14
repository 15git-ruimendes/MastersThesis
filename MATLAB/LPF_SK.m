function LPF_SK (wo,Q,R1,R2)
  C1= (Q*R2+Q*R1)/(R1*R2*wo)
  C2 = 1/((Q*R2+Q*R1)*wo)
endfunction;

