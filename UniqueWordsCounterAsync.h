#pragma once
#include <stdlib.h>
#include <unordered_set>

class UniqueWordsCounterAsync {
public:
    UniqueWordsCounterAsync(char* fileData, size_t fileSize);
    UniqueWordsCounterAsync() = delete;
    UniqueWordsCounterAsync(const UniqueWordsCounterAsync&) = delete;
    UniqueWordsCounterAsync& operator=(const UniqueWordsCounterAsync&) = delete;
    UniqueWordsCounterAsync(UniqueWordsCounterAsync&&) noexcept = delete;
    UniqueWordsCounterAsync& operator=(UniqueWordsCounterAsync&&) noexcept = delete;

    size_t CountUniqueWordsAsync();

private:
    static std::unordered_set<size_t> countUniqueWordsForChunk(
        char* start, size_t chunkLength, size_t maxChunkOverlapIndex);

private:
    char* _fileData = nullptr;
    size_t _fileSize = 0;
};