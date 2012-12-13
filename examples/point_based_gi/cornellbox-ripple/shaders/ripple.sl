displacement ripple(float Km = 0.04, 
                          numwaves = 8,
                          a = 0.3, 
                          b = 0.25)
{ 
	float sdist = s - a;
	float tdist = t - b;
	float dist = sqrt(sdist * sdist + tdist * tdist);
	float hump = sin(dist  * 2 * PI * numwaves); 
	normal n = normalize(N);
    
	P = P - n * hump * Km;
	N = calculatenormal(P);
}
