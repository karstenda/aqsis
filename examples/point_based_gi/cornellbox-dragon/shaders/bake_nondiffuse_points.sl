surface bake_nondiffuse_points(color Cemit = 0;
				string diffusePointCloudName = "";
				string nonDiffusePointCloudName = "default_nondiffuse.bin";
				float microbufres = 10;
				float maxsolidangle = 0.03;
				float phong = 20)
{
    normal Nn = normalize(N);
    color indirect = 0;

    indirect = bake3d_brdf("", P, Nn, I,
			"diffuse_ptc", diffusePointCloudName,
			"nondiffuse_ptc", nonDiffusePointCloudName,
			"maxsolidangle", maxsolidangle,
			"microbufres", microbufres,
			"phong", phong);

    Oi = Os;
    Ci = indirect;
}
