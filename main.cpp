#include "FileReader.h"
#include "UniqueWordsCounterAsync.h"

#include <iostream>

namespace {
    bool isInputValid(int argc, char** argv) {
        if (argc != 2) {
            std::cerr << "Invalid arguments" << std::endl;
            return false;
        }
        return true;
    }
    void printUsage() {
        std::cout << "Usage:" << std::endl;
        std::cout << "\tUniqueWordsCounter [filepath]" << std::endl;
        std::cout << "Example:" << std::endl;
        std::cout << "\t./UniqueWordsCounter ~/Effective_C++_55_Specific.txt" << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (!isInputValid(argc, argv)) {
        printUsage();
        return 1;
    }

    const auto filepath = argv[1];

    FileReader fileReader(filepath);
    if (!fileReader.OpenFile()) {
        std::cerr << "Failed to open file with filepath: " << filepath << std::endl;
        return 1;
    }

    const size_t fileSize = fileReader.GetFileSize();
    char* fileData = fileReader.MapFile(fileSize);
    if (!fileData) {
        std::cout << "Failed to map file" << std::endl;
        return 1;
    }

    std::cout << "Started calculation of unique words in file: " << filepath << std::endl;
    std::cout << "File size: " << fileSize << " bytes " << std::endl;

    UniqueWordsCounterAsync uniqueWordsCounterAsync(fileData, fileSize);
    auto count = uniqueWordsCounterAsync.CountUniqueWordsAsync();
    std::cout << "Unique words: " << count << std::endl;

    fileReader.CloseAndUnmapFile();
    return 0;
}