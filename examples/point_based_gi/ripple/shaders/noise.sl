displacement noise() {
  point Ptex = P + noise(P*2);
  P = P + N * 0.03 * float noise(15*Ptex);
  N = calculatenormal(P);  
}
