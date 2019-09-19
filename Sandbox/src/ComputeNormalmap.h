#pragma once

struct NormalmapType
{
    sg::ogl::resource::TextureManager& textureManager;
    std::string normalmapTextureName;
    uint32_t heightmapTextureId;
    int heightmapWidth;
    float normalStrength;
};

class ComputeNormalmap : public sg::ogl::resource::ComputeShaderProgram<NormalmapType>
{
public:
    void SetUniformsAndDispatchCompute(const NormalmapType& t_type) override
    {
        // Get a new texture handle by name.
        const auto normalmapTextureId{ t_type.textureManager.GetTextureId(t_type.normalmapTextureName) };
        sg::ogl::resource::TextureManager::Bind(normalmapTextureId);
        sg::ogl::resource::TextureManager::UseBilinearFilter();

        // Specify the amount of storage we want to use for the new normalmap texture.
        const int32_t numMipmaps{ 1 };
        glTexStorage2D(GL_TEXTURE_2D, numMipmaps, GL_RGBA32F, t_type.heightmapWidth, t_type.heightmapWidth);

        // Render to normalmap texture.
        glActiveTexture(GL_TEXTURE0);
        sg::ogl::resource::TextureManager::BindForReading(t_type.heightmapTextureId, GL_TEXTURE0);
        SetUniform("heightmap", 0);
        SetUniform("heightmapWidth", t_type.heightmapWidth);
        SetUniform("normalStrength", t_type.normalStrength);

        glBindImageTexture(0, normalmapTextureId, 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);
        glDispatchCompute(t_type.heightmapWidth / 16, t_type.heightmapWidth / 16, 1);

        sg::ogl::resource::TextureManager::Bind(normalmapTextureId);
        sg::ogl::resource::TextureManager::UseBilinearFilter();
    }

protected:

private:

};
