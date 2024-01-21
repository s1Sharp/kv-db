#include "db_command.h"

#pragma once
/**
 * @file db_parser.h
 * @author Maksim Tsarkov (s1sharp.it@gmail.com)
 * @brief file with db parsing logic
 * @version 0.1
 * @date 2023-12-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <ctime>
#include <chrono>
#include <iostream>
#include <format>

#include <grpcpp/grpcpp.h>
#include "db_service.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using db::Database;
using db::Query;
using db::Response;

namespace project {

	std::ostream& operator << (std::ostream &out, ICommand* com)
	{
		return out << (com->toString());
	}

    Query SetCommand::toQuery() const
    {
        Query q;
        q.set_operation(db::Operation::SET_OP);
        db::Token key_token;
        key_token.set_type("key");
        key_token.set_value(m_key);
        q.add_tokens()->CopyFrom(key_token);

        db::Token value_token;
        value_token.set_type("value");
        value_token.set_value(m_value);
        q.add_tokens()->CopyFrom(value_token);

        return q;
    }

    std::string SetCommand::toString() const {
        return std::format( "set command key='{}' value='{}'\n" , m_key, m_value);
    } 
};