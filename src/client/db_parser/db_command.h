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

	class ICommand {
	public:
		friend std::ostream& operator << (std::ostream &out, ICommand* com);

		virtual Query toQuery() const = 0;
		virtual std::string toString() const = 0;
	};

	class SetCommand : public ICommand {
	public:
		virtual Query toQuery() const override;

		virtual std::string toString() const override;

		SetCommand(const std::string& key, const std::string& value)
			: m_key{key}
			, m_value{value}
			{ } ;

	private:
		const std::string m_key;
		const std::string m_value;
	};


}