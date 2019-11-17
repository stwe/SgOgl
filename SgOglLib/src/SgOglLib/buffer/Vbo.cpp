// This file is part of the SgOgl package.
// 
// Filename: Vbo.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include "Vbo.h"
#include "Log.h"
#include "Core.h"

//-------------------------------------------------
// Live and let die
//-------------------------------------------------

uint32_t sg::ogl::buffer::Vbo::GenerateVbo()
{
    uint32_t vboId{ 0 };
    glGenBuffers(1, &vboId);
    SG_OGL_CORE_ASSERT(vboId, "[Vbo::GenerateVbo()] Error while creating a new Vbo.")

    SG_OGL_CORE_LOG_DEBUG("[Vbo::GenerateVbo()] A new Vbo was created. Id: {}", vboId);

    return vboId;
}

uint32_t sg::ogl::buffer::Vbo::GenerateEbo()
{
    return GenerateVbo();
}

void sg::ogl::buffer::Vbo::DeleteVbo(const uint32_t t_vboId)
{
    SG_OGL_CORE_ASSERT(t_vboId, "[Vbo::DeleteVbo()] Invalid Vbo Id.")
    glDeleteBuffers(1, &t_vboId);
    SG_OGL_CORE_LOG_DEBUG("[Vbo::DeleteVbo()] Vbo was deleted. Id: {}", t_vboId);
}

void sg::ogl::buffer::Vbo::DeleteEbo(const uint32_t t_eboId)
{
    DeleteVbo(t_eboId);
}

//-------------------------------------------------
// Bind / Unbind
//-------------------------------------------------

void sg::ogl::buffer::Vbo::BindVbo(const uint32_t t_vboId, const uint32_t t_target)
{
    SG_OGL_CORE_ASSERT(t_vboId, "[Vbo::BindVbo()] Invalid Vbo Id.")
    glBindBuffer(t_target, t_vboId);
}

void sg::ogl::buffer::Vbo::BindEbo(const uint32_t t_eboId)
{
    BindVbo(t_eboId, GL_ELEMENT_ARRAY_BUFFER);
}

void sg::ogl::buffer::Vbo::UnbindVbo()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::ogl::buffer::Vbo::InitEmpty(const uint32_t t_vboId, const uint32_t t_floatCount, const uint32_t t_usage)
{
    BindVbo(t_vboId);

    glBufferData(GL_ARRAY_BUFFER, t_floatCount * sizeof(float), nullptr, t_usage);

    UnbindVbo();
}

//-------------------------------------------------
// Store data
//-------------------------------------------------

void sg::ogl::buffer::Vbo::StoreTransformationMatrices(const uint32_t t_vboId, const uint32_t t_floatCount, const std::vector<glm::mat4>& t_matrices)
{
    BindVbo(t_vboId);

    glBufferData(GL_ARRAY_BUFFER, t_floatCount * sizeof(float), t_matrices.data(), GL_STATIC_DRAW);

    UnbindVbo();
}

//-------------------------------------------------
// Attributes
//-------------------------------------------------

void sg::ogl::buffer::Vbo::AddInstancedAttribute(
    const uint32_t t_vboId,
    const uint32_t t_index,
    const int32_t t_dataSize,
    const int32_t t_instancedDataLength,
    const uint64_t t_offset
)
{
    BindVbo(t_vboId);

    glEnableVertexAttribArray(t_index);
    glVertexAttribPointer(t_index, t_dataSize, GL_FLOAT, GL_FALSE, t_instancedDataLength * sizeof(float), reinterpret_cast<uintptr_t*>(t_offset * sizeof(float)));
    glVertexAttribDivisor(t_index, 1);

    UnbindVbo();
}
