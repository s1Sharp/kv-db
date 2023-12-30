lex_input = 'db_commands.lex'
lex_source = ['lex.yy.cpp']

yacc_input = 'db_commands.yacc'
yacc_headers = ['y.tab.hpp']
yacc_sources = ['y.tab.cpp']

# Генерация lex.yy.c из db_commands.lex
flex_gen = custom_target(
  'flex_gen',
  input : lex_input,
  output : 'lex.yy.c',
  command : ['flex', '-o', '@OUTPUT@', '@INPUT@'],
  install : false,
  build_by_default : true,
)
# Capture the output and return code of the flex command
flex_gen_result = run_command(find_program('flex'), '--c++', '-o', 'lex.yy.cpp', 'db_commands.lex')

# Print the output and return code
message('Flex command output:', flex_gen_result.stdout())
message('Flex command return code:', flex_gen_result.returncode())

# Генерация y.tab.cpp и y.tab.hpp из db_commands.yacc
bison_gen = custom_target(
  'bison_gen',
  input : yacc_input,
  output : 'y.tab.cpp',
  command : ['bison', '-o', '@OUTPUT@', '-d', '@INPUT@', '-L', 'c++'],
  install : false,
  build_by_default : true,
)
# Capture the output and return code of the bison command
bison_gen_result = run_command(find_program('bison'), '-o', 'y.tab.cpp', '-d', 'db_commands.yacc', '-L', 'c++')

# Print the output and return code
message('Bison command output:', bison_gen_result.stdout())
message('Bison command return code:', bison_gen_result.returncode())


flex_bison_dep_sources = lex_source + yacc_sources

parser_headers = ['db_parser.h'] + yacc_headers
parser_sources = ['db_parser.cpp'] + flex_bison_dep_sources


# Bison and Flex dependencies
flex_dep = dependency('libfl', static: true, required: true)

parser_lib = static_library('db_parser',
				include_directories : inc,
				install : true,
				sources: [
					parser_headers,
					parser_sources,
					flex_bison_dep_sources,
				],
				dependencies: [
						boost_deps,
						flex_dep,
						# bison_dep,
					]
				)

parser_dep = declare_dependency(include_directories : inc,
					   link_with : parser_lib)

install_headers(parser_headers,
		 subdir : 'db_parser')
