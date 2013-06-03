surface bake_diffuse_points(color Cemit = 0;
                    string pointCloudName = "")
{
    normal Nn = normalize(N);
    if(pointCloudName != "")
    {
        Ci = bake3d_diffuse(pointCloudName, P, Nn, Cs, area(P));
    }
    Oi = Os;
}
