surface beatysurf(float Ks = 1;
		float Kd = 1;
		float Ki = 1;
		color Cemit = 0;
		color Cspec = 1;
		float microbufres = 20;
		float maxsolidangle = 0.03;
		float phong = -1;
		string diffusePointCloudName = "";
		string nonDiffusePointCloudName = "";
		float hideDirect = 0) {

	normal Nn = normalize(N);

	color diff_indirect = Ki*indirect( diffusePointCloudName, "", P, Nn, I,
				"maxsolidangle", maxsolidangle,
				"microbufres", microbufres,
				"phong", phong);


	color nondiff_indirect = Ki*indirect( "", nonDiffusePointCloudName, P, Nn, I,
				"maxsolidangle", maxsolidangle,
				"microbufres", microbufres,
				"phong", phong);
	
	color indirect = Cs*(diff_indirect + nondiff_indirect);

	Oi = Os;
	color direct = 0;
	if (hideDirect != 1) {
		if (phong > 0) {
			direct = Cspec*Ks*phong(Nn,-I,phong)+Cs*Kd*diffuse(Nn);
		} else {
			direct = Cs*Kd*diffuse(Nn);
		}
	}

	Ci=Oi*(direct+indirect);
}
