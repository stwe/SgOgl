#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "SgOglException.h"

namespace sg::ogl::resource
{
    class SG_OGL_LOCAL ShaderUtil
    {
    public:
        static auto ReadShaderFile(const std::string& t_fileName)
        {
            std::string content;
            std::ifstream shaderFile;

            // Ensure ifstream objects can throw exceptions.
            shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

            try
            {
                // Open file.
                shaderFile.open(t_fileName);
                std::stringstream shaderStream;

                // Read file buffer content into stream.
                shaderStream << shaderFile.rdbuf();

                // Close file.
                shaderFile.close();

                // Convert stream into string.
                content = shaderStream.str();
            }
            catch (const std::ifstream::failure&)
            {
                throw SG_OGL_EXCEPTION("[ShaderUtil::ReadShaderFile()] Exception caught while loading of file " + t_fileName + ".");
            }

            return content;
        }

        /**
         * @brief Find all positions of the a SubString in given String.
         * @param t_positions The found positions.
         * @param t_data The given string.
         * @param t_toSearch The search string.
         */
        static void FindAllOccurances(std::vector<size_t>& t_positions, const std::string& t_data, const std::string& t_toSearch)
        {
            // Get the first occurrence.
            auto pos{ t_data.find(t_toSearch) };

            // Repeat till end is reached.
            while (pos != std::string::npos)
            {
                // Add position to the vector.
                t_positions.push_back(pos);

                // Get the next occurrence from the current position.
                pos = t_data.find(t_toSearch, pos + t_toSearch.size());
            }
        }

    protected:

    private:

    };
}
