project('Base Conqueror', 'cpp',
        license         : [ 'MIT' ],
	version         : '1.1',
	default_options : ['warning_level=3', 'buildtype=release', 'cpp_std=c++14']
)

project_sources = []

subdir('src')
sdl2 = dependency('SDL2', version : '>= 2.0.1')
sdl2_mixer = dependency('SDL2_mixer', version : '>= 2.0.1')
sdl2_image = dependency('SDL2_image', version : '>= 2.0.1')
executable('BaseConqueror', project_sources, dependencies : [sdl2, sdl2_mixer, sdl2_image])