#pragma once

class ParticleShaderProgram : public sg::ogl::resource::ShaderProgram
{
public:
    void UpdateUniforms(sg::ogl::scene::Entity& t_entity) override
    {

    }

    std::string GetFolderName() override
    {
        return "particle";
    }

protected:

private:

};
