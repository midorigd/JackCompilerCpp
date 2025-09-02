#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "CompilerResources.hpp"

#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>
#include <utility>

namespace Compiler {

namespace fs = std::filesystem;

class SymbolTable {
public:
    struct Entry {
        std::string type;
        Segment segment;
        int index;

        Entry(std::string t, Segment s, int i) : type(t), segment(s), index(i) {}
        template <typename... Args>
        Entry(Args&&... args) : type(std::forward<Args>(args)...), segment(std::forward<Args>(args)...), index(std::forward<Args>(args)...) {}

        friend std::ostream& operator<<(std::ostream& os, const Entry& entry);
    };

    SymbolTable(std::ofstream* const debugFilePath);
    bool contains(const std::string& name) const;
    void reset();
    void define(const std::string& name, const std::string& type, const Segment& segment);
    void defineThisObject(const std::string& type);
    int varCount(const Segment& segment) const;
    const SymbolTable::Entry* getEntry(const std::string& name) const;
    std::string typeOf(const std::string& name) const;
    Segment segmentOf(const std::string& name) const;
    int indexOf(const std::string& name) const;
    void dumpTable(const std::string& tag);

private:
    std::unordered_map<std::string, SymbolTable::Entry> data;
    std::unordered_map<Segment, int> counters;
    std::ofstream* const debugFile;
};

}

#endif
