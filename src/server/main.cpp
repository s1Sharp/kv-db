/**
 * @file main.cpp
 * @author Maksim Tsarkov (s1sharp.it@gmail.com)
 * @brief also test file
 * @version 0.1
 * @date 2023-10-07
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <memory>
#include <string>
#include <iostream>
#include "db_config/db_config.h"
#include "db_engine/db_engine.h"

int main(int argc, char **argv)
{
    auto cfg = project::loadConfig(argc, argv);
    std::cout << "host " << cfg.getHost() << " port " << cfg.getPort() << " user " << cfg.getUser() << " pass " << cfg.getPass() << std::endl;

    RunServer();
	return 0;
}
