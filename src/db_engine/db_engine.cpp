#include <iostream>
#include <fstream>
#include <vector>

#include "db_document.h"

using namespace doc;


class DataBaseCluster {
private:
    std::unordered_map<DocumentId, DocumentFinder> m;
};


class ICRUDoperations {
    using crudRet = std::string;
    enum WhatUpdated {
        UPDATE_CONTENT = 1 << 0,
        UPDATE_FIELDS = 1 << 1,
    };
    virtual crudRet createDoc(Document& docRef) = 0;
    virtual crudRet deleteDoc(DocumentId docId) = 0;
    virtual crudRet updateDoc(const Document& updatedDocRef, WhatUpdated dwu) = 0;
    virtual crudRet find(Document& docRef, DocumentId docId, bool& found) = 0;
};

class DatabaseEngine {
private:
    std::string filename;
    std::vector<Document> documents;

public:
    DatabaseEngine(const std::string& filename) : filename(filename) {
        loadFromFile();
    }

    ~DatabaseEngine() {
        saveToFile();
    }

    // Create
    void createDocument(const Document& doc) {
        documents.push_back(doc);
    }

    // Read
    Document readDocument(DocumentId id) const {
        for (const auto& doc : documents) {
            if (doc.getId() == id) {
                return doc;
            }
        }
        throw std::runtime_error("Document not found");
    }

    // Update
    void updateDocument(const Document& newDoc) {
        for (auto& doc : documents) {
            if (doc.getId() == newDoc.getId()) {
                doc = newDoc;
                return;
            }
        }
        throw std::runtime_error("Document not found");
    }

    // Delete
    void deleteDocument(DocumentId id) {
        auto it = std::remove_if(documents.begin(), documents.end(),
            [id](const Document& doc) { return doc.getId() == id; });

        if (it != documents.end()) {
            documents.erase(it, documents.end());
        } else {
            throw std::runtime_error("Document not found");
        }
    }

    // Display all documents
    void displayAllDocuments() const {
        for (const auto& doc : documents) {
            std::cout << "ID: " << doc.getId() << "\nData: " << doc.getContent() << "\nMetadata: " << doc.getField("meta") << "\n\n";
        }
    }

private:
    void loadFromFile() {
        std::ifstream file(filename, std::ios::binary);
        if (file) {
            while (file) {
                Document doc;
                file.read(reinterpret_cast<char*>(&doc), sizeof(Document));
                if (file) {
                    documents.push_back(doc);
                }
            }
            file.close();
        }
    }

    void saveToFile() {
        std::ofstream file(filename, std::ios::binary);
        if (file) {
            for (const auto& doc : documents) {
                file.write(reinterpret_cast<const char*>(&doc), sizeof(Document));
            }
            file.close();
        }
    }
};
