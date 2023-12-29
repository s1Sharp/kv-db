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
#include "db_parser/db_parser.h"
#include "db_document/db_document.h"

#include "db_engine/db_parser.h"


int main(int argc, char **argv)
{

    CommandReader commandReader;
    commandReader.startReading();

    while (true) {
        if (commandReader.hasCommands()) {
            std::string commandString = commandReader.popCommand();
            CommandParser commandParser(commandString);

            if (commandParser.parse()) {
                std::cout << "Parsed Command: " << commandParser.getCommand() << std::endl;
                auto parameters = commandParser.getParameters();
                for (const auto& pair : parameters) {
                    std::cout << "  " << pair.first << ": " << pair.second << std::endl;
                }
            }
        }

        // Your database processing logic here...

        // For demonstration purposes, exit the loop after a certain condition.
        if (/* Your exit condition */) {
            commandReader.stopReading();
            break;
        }
    }


	doc::init();
	return 0;
}
