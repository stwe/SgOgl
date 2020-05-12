include("conanbuildinfo.premake.lua")

workspace "SgOgl"
    conan_basic_setup()

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

    linkoptions { conan_exelinkflags }

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/src/SgOglLib",
        "%{prj.name}/vendor",
        "%{prj.name}/vendor/gli",
        "%{prj.name}/vendor/LuaBridge",
        "%{prj.name}/vendor/LuaBridge/detail"
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

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

project "Sandbox"
    location "Sandbox"
    architecture "x64"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("obj/" .. outputdir .. "/%{prj.name}")

    linkoptions { conan_exelinkflags }

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "SgOglLib/src",
        "SgOglLib/src/SgOglLib",
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
