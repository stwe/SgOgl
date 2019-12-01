// This file is part of the SgOgl package.
// 
// Filename: Material.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include "Material.h"

bool sg::ogl::resource::Material::HasAmbientMap() const
{
    return mapKa != 0;
}

bool sg::ogl::resource::Material::HasDiffuseMap() const
{
    return mapKd != 0;
}

bool sg::ogl::resource::Material::HasSpecularMap() const
{
    return mapKs != 0;
}

bool sg::ogl::resource::Material::HasBumpMap() const
{
    return mapBump != 0;
}

bool sg::ogl::resource::Material::HasNormalMap() const
{
    return mapKn != 0;
}
