engine_headers = ['db_engine.h']
engine_sources = ['db_engine.cpp']

engine_lib = static_library('db_engine',
				include_directories : inc,
				install : true,
				sources: [engine_headers, engine_sources],
				dependencies: [
						boost_deps,
					]
				)

engine_dep = declare_dependency(include_directories : inc,
					   link_with : engine_lib)

install_headers(engine_headers,
		 subdir : 'db_engine')

