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
    /**
     * Models the value of a SymbolTable entry with its data type and memory segment and index.
     */
    struct Entry {
        std::string type;
        Segment segment;
        int index;

        Entry(std::string t, Segment s, int i) : type(t), segment(s), index(i) {}
        template <typename... Args>
        Entry(Args&&... args) : type(std::forward<Args>(args)...), segment(std::forward<Args>(args)...), index(std::forward<Args>(args)...) {}

        friend std::ostream& operator<<(std::ostream& os, const Entry& entry);
    };

    /**
     * Creates a new SymbolTable module, with an optional debug file to write the data in the symbol table to.
     */
    SymbolTable(std::ofstream* const debugFilePath);

    /**
     * Returns whether or not the provided symbol name exists in the symbol table.
     */
    bool contains(const std::string& name) const;

    /**
     * Clears the symbol table.
     */
    void reset();

    /**
     * Adds a new entry to the symbol table with the provided name and properties,
     * using the next free index for the provided memory segment.
     */
    void define(const std::string& name, const std::string& type, const Segment& segment);

    /**
     * Adds a new entry to the symbol table named "this" with the provided type.
     * Used to support object-oriented methods by passing this as the first argument to a method call.
     */
    void defineThisObject(const std::string& type);

    /**
     * Returns the number of variables in the provided memory segment in the symbol table.
     */
    int varCount(const Segment& segment) const;

    /**
     * Returns a pointer to the entry of the provided symbol.
     */
    const SymbolTable::Entry* getEntry(const std::string& name) const;

    /**
     * Returns the data type of the provided symbol.
     */
    std::string typeOf(const std::string& name) const;

    /**
     * Returns the memory segment of the provided symbol.
     */
    Segment segmentOf(const std::string& name) const;

    /**
     * Returns the memory index of the provided symbol.
     */
    int indexOf(const std::string& name) const;

    /**
     * Writes the entire table to the debug file, if one was provided.
     */
    void dumpTable(const std::string& tag);

private:
    std::unordered_map<std::string, SymbolTable::Entry> data;
    std::unordered_map<Segment, int> counters;
    std::ofstream* const debugFile;
};

}

#endif
