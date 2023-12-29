#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <thread>
#include <queue>
#include <mutex>
#include <regex>
#include <condition_variable>


class CommandParser {
public:
    CommandParser(const std::string& command) : commandString(command) {}

    std::string getCommand() const {
        return command;
    }

    std::map<std::string, std::string> getParameters() const {
        return parameters;
    }

    bool parse() {
        std::istringstream iss(commandString);
        iss >> command;

        if (command.empty()) {
            std::cerr << "Error: Command is empty." << std::endl;
            return false;
        }

        std::string token;
        while (iss >> token) {
            size_t pos = token.find('=');
            if (pos != std::string::npos) {
                std::string key = token.substr(0, pos);
                std::string value = token.substr(pos + 1);
                parameters[key] = value;
            }
        }

        return true;
    }

private:
    std::string commandString;
    std::string command;
    std::map<std::string, std::string> parameters;
};


class CommandReader {
public:
    CommandReader() : stop(false) {}

    void startReading() {
        std::thread readerThread(&CommandReader::readCommands, this);
        readerThread.detach();
    }

    void stopReading() {
        stop = true;
        cv.notify_one();
    }

    bool hasCommands() const {
        return !commands.empty();
    }

    std::string popCommand() {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock, [this] { return !commands.empty() || stop; });

        if (!commands.empty()) {
            std::string command = commands.front();
            commands.pop();
            return command;
        }

        return "";
    }

private:
    std::queue<std::string> commands;
    std::mutex mutex;
    std::condition_variable cv;
    bool stop;

    void readCommands() {
        while (!stop) {
            std::string command;
            std::getline(std::cin, command);

            if (!command.empty()) {
                std::unique_lock<std::mutex> lock(mutex);
                commands.push(command);
                cv.notify_one();
            }
        }
    }
};




class Command {
public:
    virtual bool validate() = 0;
    virtual void execute() = 0;
};

class CreateCommand : public Command {
public:
    CreateCommand(const std::string& command)
        : commandLine(command) {}

    bool validate() override {
        static const std::regex pattern(R"(^create (\w+) content '(.*)'(?: fields (.*))?;$)");
        std::smatch matches;
        if (std::regex_match(commandLine, matches, pattern)) {
            type = matches[1].str();
            content = matches[2].str();
            if (matches.size() == 4) {
                parseFields(matches[3].str());
            }
            return true;
        }
        return false;
    }

    void execute() override {
        if (!validate()) {
            std::cerr << "Validation error for CREATE command." << std::endl;
            return;
        }

        // Здесь вызывайте соответствующую логику базы данных для операции CREATE
        // ...

        std::cout << "Executing CREATE command." << std::endl;
        // Дополнительная логика для команды CREATE
        // ...
    }

private:
    std::string commandLine;
    std::string type;
    std::string content;
    std::map<std::string, std::string> fields;

    void parseFields(const std::string& fieldsStr) {
        // Парсинг и добавление полей в map
        static const std::regex fieldPattern(R"((\w+)=(\w+))");
        std::smatch fieldMatches;
        auto it = fieldsStr.cbegin();
        while (std::regex_search(it, fieldsStr.cend(), fieldMatches, fieldPattern)) {
            fields[fieldMatches[1].str()] = fieldMatches[2].str();
            it = fieldMatches.suffix().first;
        }
    }
};

class UpdateCommand : public Command {
public:
    UpdateCommand(const std::map<std::string, std::string>& parameters)
        : parameters(parameters) {}

    bool validate() const override {
        // Валидация команды UPDATE
        static const std::regex pattern(R"(^update doc content 'as std::string here, optionaly' fields .+ where \(_id=="\d+"\)$)");
        return std::regex_match(parameters.at("fields"), pattern);
    }

    void execute() override {
        if (!validate()) {
            std::cerr << "Validation error for UPDATE command." << std::endl;
            return;
        }

        // Здесь вызывайте соответствующую логику базы данных для операции UPDATE
        // ...

        std::cout << "Executing UPDATE command." << std::endl;
        // Дополнительная логика для команды UPDATE
        // ...
    }

private:
    std::map<std::string, std::string> parameters;
};

class DeleteCommand : public Command {
public:
    DeleteCommand(const std::map<std::string, std::string>& parameters)
        : parameters(parameters) {}

    bool validate() const override {
        // Валидация команды DELETE
        static const std::regex pattern(R"(^delete doc where \(_id=="\d+"\)$)");
        return true;  // В данном случае DELETE всегда валиден
    }

    void execute() override {
        if (!validate()) {
            std::cerr << "Validation error for DELETE command." << std::endl;
            return;
        }

        // Здесь вызывайте соответствующую логику базы данных для операции DELETE
        // ...

        std::cout << "Executing DELETE command." << std::endl;
        // Дополнительная логика для команды DELETE
        // ...
    }

private:
    std::map<std::string, std::string> parameters;
};


class SelectCommand : public Command {
public:
    SelectCommand(const std::map<std::string, std::string>& parameters)
        : parameters(parameters) {}

    bool validate() const override {
        // Валидация команды SELECT
        static const std::regex pattern(R"(^select doc where \(_id=="\d+"\)$)");
        return true;  // В данном случае SELECT всегда валиден
    }

    void execute() override {
        if (!validate()) {
            std::cerr << "Validation error for SELECT command." << std::endl;
            return;
        }

        // Здесь вызывайте соответствующую логику базы данных для операции SELECT
        // ...

        std::cout << "Executing SELECT command." << std::endl;
        // Дополнительная логика для команды SELECT
        // ...
    }

private:
    std::map<std::string, std::string> parameters;
};