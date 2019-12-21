workspace "SgOgl"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release"
    }

    platforms
    {
        "StaticLib"
    }

    floatingpoint "Fast"
    flags "MultiProcessorCompile"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "SgOglLib"
    location "SgOglLib"
    architecture "x64"
    language "C++"
    cppdialect "C++17"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("obj/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/imgui/**.h",
        "%{prj.name}/vendor/imgui/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/src/SgOglLib", 
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/vendor/glew/include",
        "%{prj.name}/vendor/glfw/include",
        "%{prj.name}/vendor/tinyxml2/include",
        "%{prj.name}/vendor/glm",
        "%{prj.name}/vendor/assimp/include",
        "%{prj.name}/vendor/entt",
        "%{prj.name}/vendor/imgui",
        "%{prj.name}/vendor/gli",		
    }

    linkoptions
    {
        "/IGNORE:4099"
    }

    filter "system:windows"
        systemversion "latest"
        defines
        {
            "GLFW_INCLUDE_NONE"
        }

    filter "platforms:StaticLib"
        kind "StaticLib"

    filter "configurations:Debug"
        defines "SG_OGL_DEBUG_BUILD"
        runtime "Debug"
        symbols "on"
        libdirs
        {
            "%{prj.name}/vendor/glew/lib/static/win",
            "%{prj.name}/vendor/glfw/lib/static/win",
            "%{prj.name}/vendor/tinyxml2/lib/static/win/debug",
            "%{prj.name}/vendor/assimp/lib/static/win/debug"
        }
        links
        {
            "tinyxml2",
            "glew32s",
            "glfw3",
            "opengl32",
            "IrrXML",
            "zlibstaticd",
            "assimp-vc140-mt"
        }

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
        libdirs
        {
            "%{prj.name}/vendor/glew/lib/static/win",
            "%{prj.name}/vendor/glfw/lib/static/win",
            "%{prj.name}/vendor/tinyxml2/lib/static/win/release",
            "%{prj.name}/vendor/assimp/lib/static/win/release"
        }
        links
        {
            "tinyxml2",
            "glew32s",
            "glfw3",
            "opengl32",
            "IrrXML",
            "zlibstatic",
            "assimp-vc140-mt"
        }

project "Sandbox"
    location "Sandbox"
    architecture "x64"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("obj/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "SgOglLib/src",
        "SgOglLib/src/SgOglLib",
        "SgOglLib/vendor/spdlog/include",
        "SgOglLib/vendor/glew/include",
        "SgOglLib/vendor/glfw/include",
        "SgOglLib/vendor/tinyxml2/include",
        "SgOglLib/vendor/glm",
        "SgOglLib/vendor/assimp/include",
        "SgOglLib/vendor/entt",
        "SgOglLib/vendor/imgui"
    }

    links
    {
        "SgOglLib"
    }

    linkoptions
    {
        "/IGNORE:4099"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "SG_OGL_DEBUG_BUILD"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
