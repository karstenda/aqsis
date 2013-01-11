/*
 * NonDiffusePointOctreeCache.cpp
 *
 *  Created on: Jan 10, 2013
 *      Author: Karsten Daemen
 */


#include "NonDiffusePointOctreeCache.h"

#include <cassert>
#include <cmath>
#include <cstring>

#include <Partio.h>

#include <aqsis/util/logging.h>

#include "NonDiffusePointOctree.h"

namespace Aqsis {


//------------------------------------------------------------------------------
NonDiffusePointOctree* NonDiffusePointOctreeCache::find(const std::string& fileName)
{
    MapType::const_iterator i = m_cache.find(fileName);
    if(i == m_cache.end())
    {
        // Try to open the file
        //
        // TODO: Path handling
        PointArray points;
        // Convert to octree
        boost::shared_ptr<NonDiffusePointOctree> tree;
        if(loadNonDiffusePointFile(points, fileName))
            tree.reset(new NonDiffusePointOctree(points));
        else
            Aqsis::log() << error << "Point cloud file \"" << fileName
                         << "\" not found\n";
        // Insert into map.  If we couldn't load the file, we insert
        // a null pointer to record the failure.
        m_cache.insert(MapType::value_type(fileName, tree));
        return tree.get();
    }
    return i->second.get();
}


void NonDiffusePointOctreeCache::clear()
{
    m_cache.clear();
}


}
