parser_headers = ['db_parser.h']
parser_sources = ['db_parser.cpp']

parser_lib = static_library('db_parser',
				include_directories : inc,
				install : true,
				sources: [parser_headers, parser_sources],
				dependencies: [
						boost_deps,
					]
				)

parser_dep = declare_dependency(include_directories : inc,
					   link_with : parser_lib)

install_headers(parser_headers,
		 subdir : 'db_parser')

