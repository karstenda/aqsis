include_subproject(partio)

set(pointrender_srcs
    microbuf_proj_func.cpp
    MicroBuf.cpp
    OcclusionIntegrator.cpp
    RadiosityIntegrator.cpp
    diffuse/DiffusePointOctree.cpp
    diffuse/DiffusePointOctreeCache.cpp
    nondiffuse/spherical_harmon.cpp
    nondiffuse/SHProjection.cpp
    nondiffuse/NonDiffusePointOctree.cpp
    nondiffuse/NonDiffusePointOctreeCache.cpp
    nondiffuse/NonDiffusePoint.cpp
    nondiffuse/NonDiffusePoint2.cpp
    nondiffuse/brdf/PhongBrdf.cpp
)

make_absolute(pointrender_srcs ${pointrender_SOURCE_DIR})
list(APPEND pointrender_srcs ${partio_srcs})
list(APPEND pointrender_srcs ${pngpp_srcs})

set(pointrender_hdrs
    microbuf_proj_func.h
    MicroBuf.h
    OcclusionIntegrator.h
    RadiosityIntegrator.h
    diffuse/DiffusePointOctree.h
    diffuse/DiffusePointOctreeCache.h
    nondiffuse/spherical_harmon.h
    nondiffuse/SHProjection.h
    nondiffuse/NonDiffusePointOctree.hpp
    nondiffuse/NonDiffusePointOctreeCache.hpp
    nondiffuse/NonDiffusePoint.hpp
    nondiffuse/NonDiffusePoint2.hpp
    nondiffuse/brdf/Brdf.hpp
    nondiffuse/brdf/PhongBrdf.hpp
)

make_absolute(pointrender_hdrs ${pointrender_SOURCE_DIR})
source_group("Header Files" FILES ${pointrender_hdrs})

include_directories(${pointrender_SOURCE_DIR})

set(pointrender_libs ${partio_libs} ${math_libs})
