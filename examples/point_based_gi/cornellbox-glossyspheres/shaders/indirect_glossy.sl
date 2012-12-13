surface indirect_glossy(float Kd = 1;
			float Ka = 1;
			float Ki = 1;
			float Ke = 1;
			color Cemit = 0;
			float microbufres = 10;
			float maxsolidangle = 0.03;
			float phong = -1;
			string diffusePointCloudName = "";
			string nonDiffusePointCloudName = "") {

	normal Nn = normalize(N);
	color indirect = 0;

	indirect = indirect(	P, Nn, I, 0,
				"diffuse_ptc", diffusePointCloudName,
				"nondiffuse_ptc", nonDiffusePointCloudName,
				"maxsolidangle", maxsolidangle,
				"microbufres", microbufres,
				"phong", phong);

	Oi = Os;
	Ci = Oi*(Ke*Cemit + Cs*(Kd*diffuse(Nn) + Ka*ambient() + Ki*indirect));
	Ci = Ki*indirect;
}
