#include "FileReader.h"

#include <string>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

FileReader::FileReader(const std::string& filepath)
    : _filepath(filepath) {
}

FileReader::~FileReader() {
    CloseAndUnmapFile();
}

bool FileReader::OpenFile() {
    _fd = open(_filepath.c_str(), O_RDONLY);
    return _fd != -1;
}

size_t FileReader::GetFileSize() {
    struct stat sb;
    if (fstat(_fd, &sb) == -1) {
        close(_fd);
        return 0;
    }
    _fileSize = sb.st_size;
    return sb.st_size;
}

char* FileReader::MapFile(size_t fileSize) {
    char* data = static_cast<char*>(mmap(nullptr, fileSize, PROT_READ, MAP_PRIVATE, _fd, 0));
    if (data == MAP_FAILED) {
        close(_fd);
        return nullptr;
    }
    _fileData = data;
    return data;
}

void FileReader::CloseAndUnmapFile() {
    if (_fileData != nullptr) {
        munmap(_fileData, _fileSize);
    }
    if (_fd != -1) {
        close(_fd);
        _fd = -1;
    }
}