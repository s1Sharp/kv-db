/**
 * @file db_document.h
 * @author Maksim Tsarkov (s1sharp.it@gmail.com)
 * @brief file with document impl
 * @version 0.1
 * @date 2023-12-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <random>
#include <chrono>
#include <map>
#include <unordered_map>
#include <filesystem>


#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>

namespace doc {
class DocumentFinderPath;


class DocumentId {
public:
    DocumentId();

    DocumentId(const std::string& customId);
    DocumentId(const DocumentId& customId);

    std::string getId() const;

	bool operator<(const DocumentId& id) const;
	bool operator==(const DocumentId& id) const;

	operator std::string () const;

    friend std::ostream& operator<<(std::ostream& os, const DocumentId& id);

private:
    std::string generateRandomId();

public:
    template <typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/);

private:
    std::string _id;
};




class Document {
public:
    Document();

    DocumentFinderPath toFinderPath() const;

    DocumentId getId() const;
    std::string getStringId() const;

    const std::string& getContent() const;

    void setContent(const std::string& content);

    void addField(const std::string& key, const std::string& value);

    const std::string& getField(const std::string& key) const;

    friend std::ostream& operator<<(std::ostream& os, const Document& doc);

public:
    template <typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/);

private:
    DocumentId _id;
    std::string _content;
	// any metadata
    std::unordered_map<std::string, std::string> _fields;

private:
    static const std::string emptyField;
};


class DocumentFinderPath {
public:

    DocumentFinderPath()
        : _exists(false) {}

    DocumentFinderPath(const std::string& localName, const DocumentId& id)
        : _local_name(localName), _id(id), _exists(true) {}

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & _local_name;
        ar & _exists;
        ar & _id;
    }

    const std::string& getLocalName() const {
        return _local_name;
    }

    const DocumentId& getId() const {
        return _id;
    }

    bool operator<(const DocumentId& id) {
        return _id < id;
    }

    operator bool() {
        return _exists;
    }

    friend std::ostream& operator<<(std::ostream& os, const DocumentFinderPath& fp) {
        os << "Document ID: " << fp._id << "\n";
        os << "Local Path: " << fp._local_name << "\n";

        return os;
    }

    std::string _local_name;
    DocumentId _id;
    bool _exists;
};



class DocumentFinder {
private:
    std::filesystem::path rootPathAbs;
    std::filesystem::path rootPath;
    std::filesystem::path mapperPath;
    std::map<DocumentId, DocumentFinderPath> idToPathMap;

public:
    DocumentFinder(const std::filesystem::path& root, const std::filesystem::path& mapper)
        : rootPathAbs(std::filesystem::absolute(root))
        , rootPath(root)
        , mapperPath(mapper) {
        loadMapper();
    }

    ~DocumentFinder() {
        saveMapper();
    }

    DocumentFinderPath toFinderPath(const Document& doc) const
    {
        return DocumentFinderPath(
            (rootPath / "data" / doc.getStringId()).string(),
            doc.getId()
        );
    }

    DocumentFinderPath getDocumentPathById(const DocumentId& id) {
        auto e = idToPathMap.find(id);
        if (e == idToPathMap.end()) {
            return DocumentFinderPath();
        }
        const auto& d = e->first;
        return DocumentFinderPath(
            (rootPath / "data" / d.getId()).string(),
            d.getId()
        );
    }

    void addDocument(const Document& doc) {
        DocumentFinderPath finderPath = toFinderPath(doc);
        idToPathMap[finderPath.getId()] = finderPath;
    }

    // Получение пути к файлу данных по ID документа
    std::filesystem::path getDataFilePath(const DocumentId& id) const {
        auto it = idToPathMap.find(id);
        if (it != idToPathMap.end()) {
            return it->second.getLocalName();
        }
        throw std::runtime_error("Document not found");
    }

private:
    void loadMapper() {
        const std::filesystem::path filePath = rootPath / mapperPath;
        std::ifstream file(filePath, std::ios::binary);
        if (file) {
            boost::archive::text_iarchive ia(file);
            ia >> idToPathMap;
            file.close();
        } else {
            std::cerr << "Error opening file for reading: " << filePath << std::endl;
        }
    }

    void saveMapper() {
        const std::filesystem::path filePath = rootPath / mapperPath;
        // Создаем все несуществующие директории в пути
        std::filesystem::create_directories(filePath.parent_path());
        std::ofstream file(filePath, std::ios::binary | std::ios::trunc);
        if (file) {
            boost::archive::text_oarchive oa(file);
            oa << idToPathMap;
            file.close();
        } else {
            std::cerr << "Error opening file for writing: " << filePath << std::endl;
        }
    }
};



template <typename T>
class Writer {
public:
    Writer(const std::string& filename) : _filename(filename) {}

    bool write(const T& data) {
        std::ofstream file(_filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error: Unable to open file for writing: " << _filename << std::endl;
            return false;
        }

        try {
            boost::archive::binary_oarchive archive(file);
            archive << data;
        } catch (const std::exception& e) {
            std::cerr << "Error while writing to file: " << e.what() << std::endl;
            return false;
        }

        return true;
    }

private:
    std::string _filename;
};

template <typename T>
class Reader {
public:
    Reader(const std::string& filename) : _filename(filename) {}

    bool read(T& data) {
        std::ifstream file(_filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error: Unable to open file for reading: " << _filename << std::endl;
            return false;
        }

        try {
            boost::archive::binary_iarchive archive(file);
            archive >> data;
        } catch (const std::exception& e) {
            std::cerr << "Error while reading from file: " << e.what() << std::endl;
            return false;
        }

        return true;
    }

private:
    std::string _filename;
};




int init();


} // namespace doc