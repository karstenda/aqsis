surface indirect_diffuse(float Kd = 1;
			float Ka = 1;
			float Ki = 0.5;
			float Ke = 0.5;
			color Cemit = 0;
			float microbufres = 10;
			float maxsolidangle = 0.03;
			string diffusePointCloudName = "";
			string nonDiffusePointCloudName = "") {

	normal Nn = normalize(N);
	color indirect = 0;

	indirect = indirect(	P, Nn, I, 0,
				"diffuse_ptc", diffusePointCloudName,
				"nondiffuse_ptc", nonDiffusePointCloudName,
				"maxsolidangle", maxsolidangle,
				"microbufres", microbufres);

	Oi = Os;
	Ci = Oi*(Ke*Cemit + Cs*(Kd*diffuse(Nn) + Ka*ambient() + Ki*indirect));
}
