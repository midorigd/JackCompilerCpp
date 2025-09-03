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
    /**
     * Creates a new VMWriter module to write VM commands to the provided file.
     */
    VMWriter(const fs::path& outfilePath) : outfile(outfilePath) {}

    /**
     * Writes a VM push command with the provided memory segment and index to output.
     */
    void writePush(const Segment& segment, const int index);

    /**
     * Writes a VM pop command with the provided memory segment and index to output.
     */
    void writePop(const Segment& segment, const int index);

    /**
     * Writes a VM arithmetic command with the provided operation to output.
     */
    void writeArithmetic(const Command& command);

    /**
     * Writes a VM label command with the provided label to output.
     */
    void writeLabel(const std::string& label);

    /**
     * Writes a VM goto command with the provided label to output.
     */
    void writeGoto(const std::string& label);

    /**
     * Writes a VM if-goto command with the provided label to output.
     */
    void writeIf(const std::string& label);

    /**
     * Writes a VM function call command with the provided name and number of arguments to output.
     */
    void writeCall(const std::string& name, const int nArgs);

    /**
     * Writes a VM function definition command with the provided name and number of local variables to output.
     */
    void writeFunction(const std::string& name, const int nVars);

    /**
     * Writes a VM function return comamnd to output.
     */
    void writeReturn();
    
    /**
     * Writes a VM push command with the provided integer constant to output.
     */
    void writeConstant(const int index);

    /**
     * Writes the VM command "push pointer 0" to output.
     */
    void writePushThisPtr();

    /**
     * Writes the VM command "pop pointer 0" to output.
     */
    void writePopThisPtr();

    /**
     * Writes the VM command "push pointer 1" to output.
     */
    void writePushThatPtr();

    /**
     * Writes the VM command "pop pointer 1" to output.
     */
    void writePopThatPtr();

private:
    std::ofstream outfile;
};

}

#endif
