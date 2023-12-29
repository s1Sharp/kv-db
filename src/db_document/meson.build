document_headers = ['db_document.h']
document_sources = ['db_document.cpp']

document_lib = static_library('db_document',
				include_directories : inc,
				install : true,
				sources: [document_headers, document_sources],
				dependencies: [
						boost_deps,
					]
				)

document_dep = declare_dependency(include_directories : inc,
					   link_with : document_lib)

install_headers(document_headers,
		 subdir : 'db_document')

