#include "db_engine.h"

// db_client.cpp

#include <thread>
#include <iostream>
#include <grpcpp/grpcpp.h>
#include "db_service.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using db::Database;
using db::Query;
using db::Response;

class DatabaseClient {
public:
    DatabaseClient(std::shared_ptr<Channel> channel) : stub_(Database::NewStub(channel)) {}

    void ExecuteQuery(const std::string& sql_query) {
        Query request;
        request.set_operation(db::Operation::SET_OP);
        db::Token token;
        token.set_type("query");
        token.set_value(sql_query);
        request.add_tokens()->CopyFrom(token);

        ClientContext context;
        Response response;

        Status status = stub_->ExecuteQuery(&context, request, &response);

        if (status.ok()) {
            std::cout << "Response: " << response.message() << std::endl;
        } else {
            std::cerr << "RPC failed: " << status.error_message() << std::endl;
        }
    }

private:
    std::unique_ptr<Database::Stub> stub_;
};

int send_message(std::string message) {
    // Подключаемся к серверу gRPC
    std::string server_address("localhost:50051");
    DatabaseClient client(grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials()));

    // Пример запроса к базе данных
    client.ExecuteQuery(message);
    std::this_thread::sleep_for(std::chrono::seconds(20));
    return 0;
}
