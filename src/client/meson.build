cxx = meson.get_compiler('cpp')

fmt_deps = dependency('fmt', method: 'pkg-config')
boost_deps = dependency('boost', method: 'pkg-config', 
    modules: [
        'serialization',
        'system',
        'filesystem',
        'thread',
        'program_options',
    ]
)


qt_version = 'qt6'
qt = import(qt_version)
qt_deps = dependency(qt_version, version: '>=6.0', modules : ['Core', 'Gui', 'Widgets'], required : true)


inc = include_directories(['.', 'db_document', 'db_parser', 'db_engine', 'db_config', '../proto'])


cpp_header_files = []
cpp_files = [
    files('main.cpp'),
]

subdir('db_parser')
subdir('db_document')
subdir('db_engine')
subdir('db_config')



cpp_deps = [
    config_dep,
    engine_dep,
    document_dep,
    parser_dep,
    fmt_deps,
    boost_deps,
    db_proto_dep,
]

client_db_exe = executable(
    'client_db',
    sources      : cpp_files,
    dependencies : cpp_deps,
    install : true, install_dir : join_paths(meson.build_root(), 'bin'),
)
