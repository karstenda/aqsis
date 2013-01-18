surface bake_nondiffuse_points(color Cemit = 0;
				string pointCloudName = "";
				float microbufres = 10;
				float maxsolidangle = 0.03;
				float phong = 20)
{
	normal Nn = normalize(N);
	if(pointCloudName != "")
	{
        	Ci = bake3d_nondiffuse(pointCloudName, P, Nn, Cs, area(P),
			"maxsolidangle", maxsolidangle,
			"microbufres", microbufres,
			"phong", phong);
	}
	Oi = Os;
}
