#include "db_engine.h"

// db_server.cpp

#include <iostream>
#include <grpcpp/grpcpp.h>
#include "db_service.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using db::Database;
using db::Query;
using db::Response;

class DatabaseServiceImpl final : public Database::Service {
public:
    Status ExecuteQuery(ServerContext* context, const Query* request, Response* response) override {
        // Реализуйте обработку запроса к базе данных
        std::cout << "Received query: " << request->operation() << " query: " << request->tokens().at(0).value() <<  std::endl;

        // Пример: Всегда возвращаем успешный ответ
        response->set_success(true);
        response->set_message("Query executed successfully.");

        return Status::OK;
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    DatabaseServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}
