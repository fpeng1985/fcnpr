// Benchmark "C17.iscas" written by ABC on Fri Mar  3 18:01:06 2017

module \C17.iscas  ( 
    pi0, pi1, pi2, pi3, pi4,
    po0, po1  );
  input  pi0, pi1, pi2, pi3, pi4;
  output po0, po1;
  wire n9, n10, n11, n13;
  assign n9 = pi0 & pi2;
  assign n10 = pi2 & pi3;
  assign n11 = ~n10 & pi1;
  assign po0 = n11 | n9;
  assign n13 = ~n10 & pi4;
  assign po1 = n11 | n13;
endmodule
