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

bool sg::ogl::resource::Material::HasNormalMap() const
{
    return mapBump != 0;
}
