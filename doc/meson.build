doxygen = find_program('doxygen', required : false)
doc_option = get_option('NEED_DOC')

if doxygen.found() and doc_option

  cdata = configuration_data()
  cdata.set('VERSION', meson.project_version())

  if find_program('dot', required : false).found()
    cdata.set('HAVE_DOT', 'YES')
  else
    cdata.set('HAVE_DOT', 'NO')
  endif


  root = meson.project_source_root()
  src_dir = join_paths(root, 'src')
  test_dir = join_paths(root, 'test')

  cdata.set('PROJECT_NAME', meson.project_name())
  cdata.set('PROJECT_DOCDIR', meson.current_source_dir())
  cdata.set('PROJECT_BUILDDIR', meson.build_root())
  cdata.set('PROJECT_TESTDIR', test_dir)
  cdata.set('PROJECT_SRCDIR', src_dir)

  doxyfile = configure_file(input: 'Doxyfile.in',
                            output: 'Doxyfile',
                            configuration: cdata,
                            install: false)

  datadir = join_paths(get_option('datadir'), 'doc', meson.project_name())

  html_target = custom_target('doc',
    build_always_stale: not meson.is_subproject(),
    input: doxyfile,
    output: 'html',
    command: [doxygen, doxyfile],
    install: not meson.is_subproject(),
    install_dir: datadir
  )

endif
