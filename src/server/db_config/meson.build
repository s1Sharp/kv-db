config_headers = ['db_config.h']
config_sources = ['db_config.cpp']

config_lib = static_library('db_config',
				include_directories : inc,
				install : true,
				sources: [config_headers, config_sources],
				dependencies: [
						boost_deps,
					]
				)

config_dep = declare_dependency(include_directories : inc,
					   link_with : config_lib)

install_headers(config_headers,
		 subdir : 'db_config')

