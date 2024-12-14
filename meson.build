project('bezier', 'c', default_options: ['c_std=c17', 'warning_level=3'])

cc = meson.get_compiler('c')

sdl_dep = dependency('SDL3')
m_lib = cc.find_library('m')
m_dep = declare_dependency(dependencies: m_lib)


executable(
  'bezier',
  ['src/main.c'],
  dependencies: [sdl_dep, m_dep]
)
