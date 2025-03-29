add_rules(
	"mode.debug",
	"mode.release",
	"plugin.vsxmake.autoupdate"
)

set_project("catrescue")
set_version("0.0.0", { build = "%Y%m%d%H%M%S" })

set_languages("gnuxx20")
set_warnings("all")

add_includedirs("$(scriptdir)")
add_defines("GAME_NAME=\"Cat Rescue\"", "_CRT_SECURE_NO_WARNINGS")

if is_mode("debug") then
	add_defines("GAME_DEBUG")
else
	add_defines("GAME_RELEASE")
end

add_requires("assimp")
add_requires("flecs")
add_requires("glm")
add_requires("joltphysics")
add_requires("libsdl3", {debug = true, configs = {shared = true}})
add_requires("qoi")
add_requires("stb")

target("catrescue")
	add_headerfiles("game/**.h", "assets/shaders/**")
	add_files("game/**.cpp")

	after_build(function (target)
		if not os.exists(path.join(target:targetdir(), "assets")) then
			os.ln("$(scriptdir)/assets/out", path.join(target:targetdir(), "assets"))
		end
	end)

	add_packages("assimp", "flecs", "glm", "joltphysics", "libsdl3", "qoi", "stb")
target_end()

