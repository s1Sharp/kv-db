#include <iostream>
#include <boost/program_options.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include "db_config.h"


namespace po = boost::program_options;
namespace pt = boost::property_tree;

namespace project {


    AppConfig::AppConfig() : host_("127.0.0.1"), port_(8080), user_("admin"), pass_("admin") {}

    // Геттеры
    const std::string& AppConfig::getHost() const { return host_; }
    int AppConfig::getPort() const { return port_; }
    const std::string& AppConfig::getUser() const { return user_; }
    const std::string& AppConfig::getPass() const { return pass_; }

    // Методы загрузки и сохранения конфигурации
    void AppConfig::loadConfig(const std::string& filePath) {
        boost::property_tree::ini_parser::read_ini(filePath, pt_);
        loadValues();
    }

    void AppConfig::storeConfig(const std::string& filePath) {
        saveValues();
        boost::property_tree::ini_parser::write_ini(filePath, pt_);
    }

    void AppConfig::setConfig(const po::variables_map& vm) {
        // Fill the property tree with values from the command line
        host_ = vm["host"].as<std::string>();
        port_ = vm["port"].as<int>();
        user_ = vm["user"].as<std::string>();
        pass_ = vm["pass"].as<std::string>();
    }

    // Загрузка значений из property tree в поля класса
    void AppConfig::loadValues() {
        host_ = pt_.get<std::string>("host", "127.0.0.1");
        port_ = pt_.get<int>("port", 8080);
        user_ = pt_.get<std::string>("user", "admin");
        pass_ = pt_.get<std::string>("pass", "admin");
    }

    // Сохранение значений полей класса в property tree
    void AppConfig::saveValues() {
        pt_.put("host", host_);
        pt_.put("port", port_);
        pt_.put("user", user_);
        pt_.put("pass", pass_);
    }




    AppConfig loadConfig(int argc, char** argv) {
        // Создаем экземпляр класса AppConfig
        AppConfig config;
        try {
            po::options_description desc("Options");
            desc.add_options()
                ("host", po::value<std::string>()->default_value("127.0.0.1"), "Server host")
                ("port", po::value<int>()->default_value(8080), "Server port")
                ("user", po::value<std::string>()->default_value("admin"), "Username")
                ("pass", po::value<std::string>()->default_value("admin"), "Password");

            po::variables_map vm;
            po::store(po::parse_command_line(argc, argv, desc), vm);
            po::notify(vm);

            config.setConfig(vm);
            // // Write the configuration to an INI file
            // pt::write_ini("config.ini", config);
        } catch (const std::exception& ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
            return config;
        }

        return config;
    }

}