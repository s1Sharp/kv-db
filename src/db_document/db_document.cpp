#include "db_document.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <random>
#include <chrono>
#include <unordered_map>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/string.hpp>

namespace doc {

DocumentId::DocumentId() : _id(generateRandomId()) {}

DocumentId::DocumentId(const std::string& customId) : _id(customId) {}
DocumentId::DocumentId(const DocumentId& customId) : _id(customId._id) {}

std::string DocumentId::getId() const {
    return _id;
}

bool DocumentId::operator<(const DocumentId& id) const {
    return _id < id._id;
}

bool DocumentId::operator==(const DocumentId& id) const {
    return _id == id._id;
}

DocumentId::operator std::string () const {
    return _id;
}

std::ostream& operator<<(std::ostream& os, const DocumentId& id) {
    return os << id._id;
}

std::string DocumentId::generateRandomId() {
    std::random_device rd;
    std::mt19937 gen(rd());

    // Генерация UUIDv4
    std::uniform_int_distribution<> dis(0, 15);

    auto randDigit = [&]() {
        const char digits[] = "0123456789abcdef";
        return digits[dis(gen)];
    };

    // Генерация 8-4-4-4-12 блоков UUID
    std::stringstream ss;
    for (int i = 0; i < 8; ++i)
        ss << randDigit();
    ss << '-';
    for (int i = 0; i < 4; ++i)
        ss << randDigit();
    ss << "-4";
    for (int i = 0; i < 3; ++i)
        ss << randDigit();
    ss << "-a";
    for (int i = 0; i < 3; ++i)
        ss << randDigit();
    ss << '-';
    for (int i = 0; i < 12; ++i)
        ss << randDigit();

    return ss.str();
}

template <typename Archive>
void DocumentId::serialize(Archive& ar, const unsigned int /*version*/) {
    ar & _id;
}



/// Document






Document::Document() : _id(), _content("") {}

DocumentId Document::getId() const {
    return _id;
}

std::string Document::getStringId() const {
    return _id;
}

const std::string& Document::getContent() const {
    return _content;
}

void Document::setContent(const std::string& content) {
    _content = content;
}

void Document::addField(const std::string& key, const std::string& value) {
    _fields[key] = value;
}

const std::string& Document::getField(const std::string& key) const {
    auto it = _fields.find(key);
    return (it != _fields.end()) ? it->second : emptyField;
}

std::ostream& operator<<(std::ostream& os, const Document& doc) {
    os << "Document ID: " << doc._id << "\n";
    os << "Content: " << doc._content << "\n";

    for (const auto& field : doc._fields) {
        os << field.first << ": " << field.second << "\n";
    }

    return os;
}

template <typename Archive>
void Document::serialize(Archive& ar, const unsigned int /*version*/) {
    ar & _id;
    ar & _content;
    ar & _fields;
}


const std::string Document::emptyField = "";


int init() {
    // Пример использования классов Document и DocumentId
    Document doc;

    // Добавление дополнительных полей
    doc.addField("author", "John Doe");
    doc.addField("created_at", "2022-01-01");
    doc.setContent("now to find a way to stop programing\n there is no way to stop");

    // Вывод информации о документе и его ID
    std::cout << "Document ID: " << doc.getId() << "\n";
    std::cout << "Content: " << doc.getContent() << "\n";
    std::cout << "Author: " << doc.getField("author") << "\n";
    std::cout << "Created At: " << doc.getField("created_at") << "\n";

	std::cout << doc;

    std::cout << "========= serialization =========" << std::endl;

    Writer<Document> writer("/tmp/document.bin");
    if (writer.write(doc)) {
        std::cout << "Document successfully written to file.\n";
    } else {
        std::cerr << "Failed to write document to file.\n";
        return 1;
    }

    // Чтение из файла
    Document readDoc;
    Reader<Document> reader("/tmp/document.bin");
    if (reader.read(readDoc)) {
        std::cout << "Document successfully read from file:\n" << readDoc;
    } else {
        std::cerr << "Failed to read document from file.\n";
        return 1;
    }


    DocumentFinder df("build/db" , "dbmeta.bin");
    df.addDocument(doc);
    std::cout << "Document successfully getDocumentPathById: " << doc.getId() << "doc path: " << df.getDocumentPathById(doc.getId()) << std::endl;

    return 0;
}

} // namespace doc