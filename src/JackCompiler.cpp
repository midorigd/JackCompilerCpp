#include "JackCompiler.hpp"
#include "CompilationEngine.hpp"
#include "CompilerResources.hpp"

namespace Compiler {

namespace fs = std::filesystem;

const fs::path JackCompiler::DEBUG_FILE { "debug.txt" };

void JackCompiler::compile (const fs::path& sourceFile, bool debugMode) {
    TokenSet::init();

    if (sourceFile.extension() == ".jack") {
        files.push_back(sourceFile);
    } else {
        getJackFiles(sourceFile);
    }

    std::ofstream debugFile;
    if (debugMode) {
        debugFile = std::ofstream(DEBUG_FILE);
    }
    std::ofstream* const debugPtr { debugMode ? &debugFile : nullptr };

    for (fs::path infile : files) {
        fs::path outfile { infile };
        outfile.replace_extension(".vm");
        compileFile(infile, outfile, debugPtr);
    }
}

void JackCompiler::getJackFiles(const fs::path& dirname) {
    for (const fs::directory_entry& file : fs::directory_iterator(dirname)) {
        if (file.is_regular_file() && file.path().extension() == ".jack") {
            files.push_back(file);
        }
    }
}

void JackCompiler::compileFile(const fs::path& infile, const fs::path& outfile, std::ofstream* const debugFile) const {
    CompilationEngine compiler(infile, outfile, debugFile);
}

}
