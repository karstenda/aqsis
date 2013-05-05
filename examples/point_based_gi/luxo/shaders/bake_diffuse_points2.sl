surface bake_diffuse_points2(color Cemit = 0;
				string diffusePointCloudName = "";
				string nonDiffusePointCloudName = "";
				string diffusePointCloudName2 = "";
				float microbufres = 10;
				float maxsolidangle = 0.03;)
{
    normal Nn = normalize(N);
    if(diffusePointCloudName2 != "")
    {

	color indirect = Cs*indirect( diffusePointCloudName, nonDiffusePointCloudName, P, Nn, I,
			"maxsolidangle", maxsolidangle,
			"microbufres", microbufres);

	color col = Cs*diffuse(Nn)+indirect; 
        bake3d(diffusePointCloudName2, "", P, Nn,
               "_area", area(P),
               "_radiosity", col,
               "interpolate", 1);
	Ci = col;

    }
    Oi = Os;
}
