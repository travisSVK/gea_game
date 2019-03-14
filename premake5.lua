function win_sdk_version()
    local reg_arch = iif( os.is64bit(), "\\Wow6432Node\\", "\\" )
    local sdk_version = os.getWindowsRegistry( "HKLM:SOFTWARE" .. reg_arch .."Microsoft\\Microsoft SDKs\\Windows\\v10.0\\ProductVersion" )
    if sdk_version ~= nil then return sdk_version end
end

function include_glm()
    includedirs "external/GLM/include/"
end

function include_nlhomann()
    includedirs "external/nlhomann/include/"
end
    
function includeSDL()
    includedirs "external/SDL/Include"
end

function linkSDL()
    libdirs "external/SDL/Lib/Win64/"
    filter "kind:not StaticLib"
        links { "SDL2", "SDL2main" }
    filter {}
end

function includeMixer()
    includedirs "external/SDLmixer/include"
end

function linkMixer()
    libdirs "external/SDLmixer/lib/x64/"
    filter "kind:not StaticLib"
        links { "SDL2_mixer"}
    filter {}
end

function includeTTF()
    includedirs "external/SDLttf/include"
end

function linkTTF()
    libdirs "external/SDLttf/lib/x64/"
    filter "kind:not StaticLib"
        --links { "SDL2_ttf", "libfreetype-6", "zlib1" }
        links { "SDL2_ttf" }
    filter {}
end

function includeSDLimage()
    includedirs "external/SDLimage/include"
end

function linkSDLimage()
    libdirs "external/SDLimage/lib/x64/"
    filter "kind:not StaticLib"
        links { "SDL2_image" }
    filter {}
end

workspace "GEAEngine"
    location "solution"
    language "C++"

    startproject "engine"
	
	architecture "x86_64"
    configurations { "debug", "release" }

	filter { "configurations:debug", "system:windows", "action:vs*"}
        symbols "On"
        targetdir "builds/debug"
        systemversion(win_sdk_version() .. ".0")
        objdir "builds/debug/obj/%{prj.name}/%{cfg.longname}"
	
	filter { "configurations:release", "system:windows", "action:vs*"}
        optimize "On"
        targetdir "builds/release"
        systemversion(win_sdk_version() .. ".0")
        objdir "builds/release/obj/%{prj.name}/%{cfg.longname}"

    filter { }

    -- Copy SDL library to build directory.
    filter { "system:windows" }
        -- SDL
        os.copyfile("external/SDL/Lib/Win64/SDL2.dll", "Builds/Debug/SDL2.dll")
        os.copyfile("external/SDL/Lib/Win64/SDL2.dll", "Builds/Release/SDL2.dll")
        -- SDL TTF
        os.copyfile("external/SDLttf/lib/x64/SDL2_ttf.dll", "Builds/Debug/SDL2_ttf.dll")
        os.copyfile("external/SDLttf/lib/x64/SDL2_ttf.dll", "Builds/Release/SDL2_ttf.dll")
        os.copyfile("external/SDLttf/lib/x64/libfreetype-6.dll", "Builds/Debug/libfreetype-6.dll")
        os.copyfile("external/SDLttf/lib/x64/libfreetype-6.dll", "Builds/Release/libfreetype-6.dll")
        os.copyfile("external/SDLttf/lib/x64/zlib1.dll", "Builds/Debug/zlib1.dll")
        os.copyfile("external/SDLttf/lib/x64/zlib1.dll", "Builds/Release/zlib1.dll")
        -- SDL image
        os.copyfile("external/SDLimage/lib/x64/libjpeg-9.dll", "Builds/Debug/libjpeg-9.dll")
        os.copyfile("external/SDLimage/lib/x64/libjpeg-9.dll", "Builds/Release/libjpeg-9.dll")
        os.copyfile("external/SDLimage/lib/x64/libpng16-16.dll", "Builds/Debug/libpng16-16.dll")
        os.copyfile("external/SDLimage/lib/x64/libpng16-16.dll", "Builds/Release/libpng16-16.dll")
        os.copyfile("external/SDLimage/lib/x64/libtiff-5.dll", "Builds/Debug/libtiff-5.dll")
        os.copyfile("external/SDLimage/lib/x64/libtiff-5.dll", "Builds/Release/libtiff-5.dll")
        os.copyfile("external/SDLimage/lib/x64/libwebp-7.dll", "Builds/Debug/libwebp-7.dll")
        os.copyfile("external/SDLimage/lib/x64/libwebp-7.dll", "Builds/Release/libwebp-7.dll")
        os.copyfile("external/SDLimage/lib/x64/SDL2_image.dll", "Builds/Debug/SDL2_image.dll")
        os.copyfile("external/SDLimage/lib/x64/SDL2_image.dll", "Builds/Release/SDL2_image.dll")
        -- SDL mixer
        os.copyfile("external/SDLmixer/lib/x64/SDL2_mixer.dll", "Builds/Debug/SDL2_mixer.dll")
        os.copyfile("external/SDLmixer/lib/x64/SDL2_mixer.dll", "Builds/Release/SDL2_mixer.dll")
        os.copyfile("external/SDLmixer/lib/x64/libFLAC-8.dll", "Builds/Debug/libFLAC-8.dll")
        os.copyfile("external/SDLmixer/lib/x64/libFLAC-8.dll", "Builds/Release/libFLAC-8.dll")
        os.copyfile("external/SDLmixer/lib/x64/libmodplug-1.dll", "Builds/Debug/libmodplug-1.dll")
        os.copyfile("external/SDLmixer/lib/x64/libmodplug-1.dll", "Builds/Release/libmodplug-1.dll")
        os.copyfile("external/SDLmixer/lib/x64/libmpg123-0.dll", "Builds/Debug/libmpg123-0.dll")
        os.copyfile("external/SDLmixer/lib/x64/libmpg123-0.dll", "Builds/Release/libmpg123-0.dll")
        os.copyfile("external/SDLmixer/lib/x64/libogg-0.dll", "Builds/Debug/libogg-0.dll")
        os.copyfile("external/SDLmixer/lib/x64/libogg-0.dll", "Builds/Release/libogg-0.dll")
        os.copyfile("external/SDLmixer/lib/x64/libopus-0.dll", "Builds/Debug/libopus-0.dll")
        os.copyfile("external/SDLmixer/lib/x64/libopus-0.dll", "Builds/Release/libopus-0.dll")
        os.copyfile("external/SDLmixer/lib/x64/libopusfile-0.dll", "Builds/Debug/libopusfile-0.dll")
        os.copyfile("external/SDLmixer/lib/x64/libopusfile-0.dll", "Builds/Release/libopusfile-0.dll")
        os.copyfile("external/SDLmixer/lib/x64/libvorbis-0.dll", "Builds/Debug/libvorbis-0.dll")
        os.copyfile("external/SDLmixer/lib/x64/libvorbis-0.dll", "Builds/Release/libvorbis-0.dll")
        os.copyfile("external/SDLmixer/lib/x64/libvorbisfile-3.dll", "Builds/Debug/libvorbisfile-3.dll")
        os.copyfile("external/SDLmixer/lib/x64/libvorbisfile-3.dll", "Builds/Release/libvorbisfile-3.dll")
        
    filter {}

project "engine"
    kind "SharedLib"
    location "source/engine"
    files { "source/engine/**.hpp", "source/engine/**.cpp" }

    defines "ENGINE_API_DLL_EXPORT"

    includeSDL()
    linkSDL()
    includeTTF()
    linkTTF()
    includeSDLimage()
    linkSDLimage()
    include_glm()
    include_nlhomann()
    includeMixer()
    linkMixer()

function use_engine()
    includedirs "source/engine"
    filter "kind:not StaticLib"
        links "engine"
    
    filter {}
    
    includeSDL()
    linkSDL()
    includeTTF()
    linkTTF()
    include_glm()
    includeMixer()
    linkMixer()
end

project "game"
    kind "ConsoleApp"
    location "source/game"
    files { "source/game/**.hpp", "source/game/**.cpp" }
    use_engine()
