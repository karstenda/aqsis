surface bake_nondiffuse_points(color Cemit = 0;
                    string pointCloudName = "";
		    float microbufres = 10;
                    float maxsolidangle = 0.03)
{
    normal Nn = normalize(N);
    color indirect = 0;
    if(pointCloudName != "")
    {
        indirect = bake3d_brdf(	pointCloudName, 
			"", P, Nn,
			"maxsolidangle", maxsolidangle,
			"microbufres", microbufres,
			"phong", 20);
    }
    Oi = Os;
    Ci = indirect;
}
