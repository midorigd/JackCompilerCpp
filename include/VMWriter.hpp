#ifndef VMWRITER_H
#define VMWRITER_H

#include "CompilerResources.hpp"

#include <filesystem>
#include <fstream>
#include <string>

namespace Compiler {

namespace fs = std::filesystem;

class VMWriter {
public:
    VMWriter(const fs::path& outfilePath) : outfile(outfilePath) {}
    ~VMWriter();
    void writePush(const Segment& segment, const int index);
    void writePop(const Segment& segment, const int index);
    void writeArithmetic(const Command& command);
    void writeLabel(const std::string& label);
    void writeGoto(const std::string& label);
    void writeIf(const std::string& label);
    void writeCall(const std::string& name, const int nArgs);
    void writeFunction(const std::string& name, const int nVars);
    void writeReturn();

    void writeConstant(const int index);
    void writePushThisPtr();
    void writePopThisPtr();
    void writePushThatPtr();
    void writePopThatPtr();

private:
    std::ofstream outfile;
};

}

#endif
