lex_input = 'parser.ll'
lex_headers = ['scanner.hpp']
lex_sources = ['scanner.cpp']

yacc_input = 'grammar.yy'
yacc_headers = ['grammar.hpp']
yacc_sources = ['grammar.cpp']

# Генерация scanner.cpp из parser.ll
# Capture the output and return code of the flex command
flex_gen_result = run_command(find_program('flex'), 'parser.ll')

# Print the output and return code
message('Flex command output:', flex_gen_result.stdout())
message('Flex command return code:', flex_gen_result.returncode())

# Генерация grammar.cpp и grammar.hpp из grammar.yy
# Capture the output and return code of the bison command
bison_gen_result = run_command(find_program('bison'), 'grammar.yy')

# Print the output and return code
message('Bison command output:', bison_gen_result.stdout())
message('Bison command return code:', bison_gen_result.returncode())


flex_bison_dep_sources = lex_sources + yacc_sources
flex_bison_dep_headers = lex_headers + yacc_headers

parser_headers = ['db_parser.h'] + flex_bison_dep_headers
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
