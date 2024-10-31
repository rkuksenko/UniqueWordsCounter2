#pragma once
#include <stdlib.h>
#include <thread>
#include <unordered_map>

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
    static void countUniqueWordsForChunk(char* start, size_t chunkLength,
        size_t maxChunkOverlapIndex, std::unordered_map<size_t, int>& outWordToCount);

private:
    char* _fileData = nullptr;
    size_t _fileSize = 0;
    static std::mutex _mutex;
    std::unordered_map<size_t, int> _wordToCount;
};