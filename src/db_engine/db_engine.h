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
#include <unordered_map>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/string.hpp>

namespace doc {

class DocumentId {
public:
    DocumentId();

    DocumentId(const std::string& customId);
    DocumentId(const DocumentId& customId);

    const std::string& getId() const;

	operator const std::string& () const;

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

    const DocumentId& getId() const;

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


int init();


} // namespace doc