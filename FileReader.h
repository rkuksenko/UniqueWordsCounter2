#pragma once
#include <string>

class FileReader {
public:
    FileReader(const std::string& filepath);
    ~FileReader();

    FileReader() = delete;
    FileReader(const FileReader&) = delete;
    FileReader& operator=(const FileReader&) = delete;
    FileReader(FileReader&&) noexcept = delete;
    FileReader& operator=(FileReader&&) noexcept = delete;
    
    bool OpenFile();
    size_t GetFileSize();
    char* MapFile(size_t fileSize);
    void CloseAndUnmapFile();

private:
    const std::string _filepath;
    int _fd = -1;
    char* _fileData = nullptr;
    size_t _fileSize = 0;
};