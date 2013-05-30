surface bake_nondiffuse_points(color Cemit = 0;
				string pointCloudName = "";
				float cubemapres = 10;
				float nbands = 5;
				float phong = 20;
				float nsamples = 3000;
				string approxhemi = "";)
{
	normal Nn = normalize(N);
	if(pointCloudName != "")
	{
        	Ci = bake3d_nondiffuse(pointCloudName, P, Nn, Cs, area(P),
			"cubemapres", cubemapres,
			"phong", phong, 
			"nbands", nbands, 
			"nsamples", nsamples, 
			"approxhemi", approxhemi,
			"dPdu", dPdu,
			"dPdv", dPdv);
	}
	Oi = Os;
}
