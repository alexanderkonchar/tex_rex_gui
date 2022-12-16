tex_rex_sources = [
  'main.cpp',
  'tex_rex-application.cpp',
  'tex_rex-window.cpp',
  'text_extractor.cpp',
]

tex_rex_deps = [
  dependency('gtk4'),
    dependency('libadwaita-1'),
    dependency('tesseract'),
    dependency('lept')
]

tex_rex_sources += gnome.compile_resources('tex_rex-resources',
  'tex_rex.gresource.xml',
  c_name: 'tex_rex'
)

executable('tex_rex', tex_rex_sources,
  dependencies: tex_rex_deps,
       install: true,
)
