#include "VMWriter.hpp"
#include "CompilerResources.hpp"

namespace Compiler {

namespace fs = std::filesystem;

void VMWriter::writePush(const Segment& segment, const int index) {
    outfile << "\tpush " << segment << ' ' << index << '\n';
}

void VMWriter::writePop(const Segment& segment, const int index) {
    outfile << "\tpop " << segment << ' ' << index << '\n';
}

void VMWriter::writeArithmetic(const Command& command) {
    outfile << '\t' << command << '\n';
}

void VMWriter::writeLabel(const std::string& label) {
    outfile << "label " << label << '\n';
}

void VMWriter::writeGoto(const std::string& label) {
    outfile << "\tgoto " << label << '\n';
}

void VMWriter::writeIf(const std::string& label) {
    outfile << "\tif-goto " << label << '\n';
}

void VMWriter::writeCall(const std::string& name, const int nArgs) {
    outfile << "\tcall " << name << ' ' << nArgs << '\n';
}

void VMWriter::writeFunction(const std::string& name, const int nVars) {
    outfile << "function " << name << ' ' << nVars << '\n';
}

void VMWriter::writeReturn() {
    outfile << "\treturn\n";
}

void VMWriter::writeConstant(const int index) {
    writePush(Segment::CONST, index);
}

void VMWriter::writePushThisPtr() {
    writePush(Segment::POINTER, 0);
}

void VMWriter::writePopThisPtr() {
    writePop(Segment::POINTER, 0);
}

void VMWriter::writePushThatPtr() {
    writePush(Segment::POINTER, 1);
}

void VMWriter::writePopThatPtr() {
    writePop(Segment::POINTER, 1);
}

}
