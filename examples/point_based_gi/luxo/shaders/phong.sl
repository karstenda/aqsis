surface phong(float exponent = 20) {

	normal Nn = normalize(N);
	Oi = Os;
	Ci = phong(N,-I,exponent);
}
