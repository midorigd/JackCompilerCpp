#ifndef JACKCOMPILER_H
#define JACKCOMPILER_H

#include <filesystem>
#include <fstream>
#include <vector>

namespace Compiler {

namespace fs = std::filesystem;

class JackCompiler {
public:
    void compile(const fs::path& sourceFile, bool debugMode);

private:
    static const fs::path DEBUG_FILE;
    std::vector<fs::path> files;

    void getJackFiles(const fs::path& dirname);
    void compileFile(const fs::path& infile, const fs::path& outfile, std::ofstream* const debugFile) const;
};

}

#endif
