#pragma once

struct SplatmapType
{
    sg::ogl::resource::TextureManager& textureManager;
    sg::ogl::resource::ShaderManager& shaderManager;
    std::string splatmapTextureName;
    std::string splatmapShaderName;
    uint32_t normalmapTextureId;
    int heightmapWidth;
};

class ComputeSplatmap : public sg::ogl::resource::ComputeShaderProgram<SplatmapType>
{
public:
    void SetUniformsAndDispatchCompute(const SplatmapType& t_type) override
    {
        // Get a new texture handle by name.
        const auto splatmapTextureId{ t_type.textureManager.GetTextureId(t_type.splatmapTextureName) };
        sg::ogl::resource::TextureManager::Bind(splatmapTextureId);
        sg::ogl::resource::TextureManager::UseBilinearFilter();

        // Specify the amount of storage we want to use for the new splatmap texture.
        const int32_t numMipmaps{ 1 };
        glTexStorage2D(GL_TEXTURE_2D, numMipmaps, GL_RGBA32F, t_type.heightmapWidth, t_type.heightmapWidth);

        // Get the shader program by name.
        auto& computeShaderProgram{ t_type.shaderManager.GetComputeShaderProgram(t_type.splatmapShaderName) };
        computeShaderProgram.Bind();

        // Render to splatmap texture.
        glActiveTexture(GL_TEXTURE0);
        sg::ogl::resource::TextureManager::BindForReading(t_type.normalmapTextureId, GL_TEXTURE0);
        computeShaderProgram.SetUniform("normalmap", 0);
        computeShaderProgram.SetUniform("heightmapWidth", t_type.heightmapWidth);

        glBindImageTexture(0, splatmapTextureId, 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);
        glDispatchCompute(t_type.heightmapWidth / 16, t_type.heightmapWidth / 16, 1);

        sg::ogl::resource::TextureManager::Bind(splatmapTextureId);
        sg::ogl::resource::TextureManager::UseBilinearFilter();
    }

protected:

private:

};
