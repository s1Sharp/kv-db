/**
 * @file db_config.h
 * @author Maksim Tsarkov (s1sharp.it@gmail.com)
 * @brief file with load config impl
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <boost/program_options.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>


namespace po = boost::program_options;
namespace pt = boost::property_tree;

namespace project {

    class AppConfig {
    public:
        AppConfig();

        // Геттеры
        const std::string& getHost() const;
        int getPort() const;
        const std::string& getUser() const;
        const std::string& getPass() const;

        // Методы загрузки и сохранения конфигурации
        void loadConfig(const std::string& filePath);

        void storeConfig(const std::string& filePath);

        void setConfig(const po::variables_map& vm);

    private:
        std::string host_;
        int port_;
        std::string user_;
        std::string pass_;
        boost::property_tree::ptree pt_;

        // Загрузка значений из property tree в поля класса
        void loadValues();

        // Сохранение значений полей класса в property tree
        void saveValues();
    };

    AppConfig loadConfig(int argc, char** argv);

}