surface phong(color Cemit = 0;
              float exponent = 50.0)
{
	Oi = Os;
	Ci = phong(N, -I, exponent);
}
