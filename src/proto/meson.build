# Capture the output and return code of the compilation proto files
compile_proto_result = run_command(find_program('bash'), 'compile.sh')
# Print the output and return code
message('Proto command output:', compile_proto_result.stderr())
message('Proto command return code:', compile_proto_result.returncode())

# Добавьте зависимость от библиотеки protobuf
protobuf_dep = dependency('protobuf', method: 'pkg-config', required: true, static: true)
grpc_dep = dependency('grpc++', method: 'pkg-config', required: true, static: true)

db_proto_headers = ['db_service.pb.h', 'db_service.grpc.pb.h']
db_proto_sources = ['db_service.pb.cc', 'db_service.grpc.pb.cc']

db_proto_lib = static_library('db_db_proto',
				include_directories : '.',
				install : true,
				sources: [db_proto_headers, db_proto_sources],
				dependencies: [
						protobuf_dep,
						grpc_dep,
					]
				)

db_proto_dep = declare_dependency(include_directories : '.',
					   link_with : db_proto_lib)

install_headers(db_proto_headers,
		 subdir : 'db_db_proto')

