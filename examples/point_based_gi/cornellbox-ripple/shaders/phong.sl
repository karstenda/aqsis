surface phong(color Cemit = 0;
              float exponent = 50.0)
{
	color highlight = phong(N, I, exponent);

	Oi = Os;
	Ci = highlight;
	Ci = color(1,1,1);

}
