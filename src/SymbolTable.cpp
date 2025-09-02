#include "SymbolTable.hpp"
#include "CompilerResources.hpp"

namespace Compiler {

namespace fs = std::filesystem;

std::ostream& operator<<(std::ostream& os, const SymbolTable::Entry& entry) {
    os << entry.type << ' ' << entry.segment << ' ' << entry.index;
    return os;
}

SymbolTable::SymbolTable(std::ofstream* const debugFilePath) :
    debugFile(debugFilePath),
    counters {
        {Segment::THIS, 0},
        {Segment::STATIC, 0},
        {Segment::ARG, 0},
        {Segment::LOCAL, 0},
    } {}

bool SymbolTable::contains(const std::string& name) const {
    return data.find(name) != data.end();
}

void SymbolTable::reset() {
    data.clear();
    for (std::pair<const Segment, int>& pair : counters) {
        pair.second = 0;
    }
}

void SymbolTable::define(const std::string& name, const std::string& type, const Segment& segment) {
    Segment internalSegment = (segment == Segment::FIELD ? Segment::THIS : segment);
    SymbolTable::Entry entry(type, internalSegment, counters.at(internalSegment)++);
    data[name] = entry;
}

void SymbolTable::defineThisObject(const std::string& type) {
    define(+Keyword::THIS, type, Segment::ARG);
}

int SymbolTable::varCount(const Segment& segment) const {
    Segment internalSegment = (segment == Segment::FIELD ? Segment::THIS : segment);
    return counters.at(internalSegment);
}

const SymbolTable::Entry* SymbolTable::getEntry(const std::string& name) const {
    return &data.at(name);
}

std::string SymbolTable::typeOf(const std::string& name) const {
    return getEntry(name)->type;
}

Segment SymbolTable::segmentOf(const std::string& name) const {
    return getEntry(name)->segment;
}

int SymbolTable::indexOf(const std::string& name) const {
    return getEntry(name)->index;
}

void SymbolTable::dumpTable(const std::string& tag) {
    if (debugFile) {
        *debugFile << tag << "SymbolTable\n";
        for (const std::pair<const std::string, SymbolTable::Entry>& pair : data) {
            *debugFile << pair.first << ": " << pair.second << '\n';
        }
        *debugFile << "------\n";
    }
}

}
