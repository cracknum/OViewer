#pragma once

#include <string>
#include <unordered_set>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

namespace glsl {

class Preprocessor {
public:
    std::string preprocess(const std::string& sourcePath) {
        m_includedFiles.clear();
        return preprocessFile(sourcePath, 0);
    }

private:
    std::unordered_set<std::string> m_includedFiles;

    std::string preprocessFile(const std::string& path, int depth) {
        if (depth > 50) {
            throw std::runtime_error("GLSL include depth too large (possible cycle)");
        }

        auto canonical = std::filesystem::canonical(path).string();
        if (m_includedFiles.count(canonical)) {
            return ""; 
        }
        m_includedFiles.insert(canonical);

        std::ifstream file(canonical);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open GLSL file: " + canonical);
        }

        std::string line;
        std::ostringstream out;
        int lineNumber = 0;

        out << "#line 1 \"" << canonical << "\"\n";

        while (std::getline(file, line)) {
            lineNumber++;
            if (line.find("#pragma once") != std::string::npos) {
                continue;
            }

            if (line.find("#include \"") != std::string::npos) {
                size_t start = line.find('"');
                size_t end = line.rfind('"');
                if (start != std::string::npos && end > start) {
                    std::string includeName = line.substr(start + 1, end - start - 1);
                    std::string includePath = (std::filesystem::path(canonical).parent_path() / includeName).string();
                    try {
                        std::string includedContent = preprocessFile(includePath, depth + 1);
                        out << includedContent;
                    } catch (const std::exception& e) {
                        std::cerr << "Error including '" << includeName << "' from " << canonical
                                  << ":" << lineNumber << " — " << e.what() << "\n";
                        throw;
                    }
                    continue;
                }
            }
            out << line << "\n";
        }

        return out.str();
    }
};

} // namespace glsl