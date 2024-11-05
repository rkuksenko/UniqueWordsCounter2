#include "UniqueWordsCounterAsync.h"

#include <future>
#include <iostream>
#include <unordered_set>
#include <vector>

using namespace std::chrono_literals;

UniqueWordsCounterAsync::UniqueWordsCounterAsync(char* fileData, size_t fileSize)
    : _fileData(fileData), _fileSize(fileSize) {
}

size_t UniqueWordsCounterAsync::CountUniqueWordsAsync() {
    if (!_fileData)
        return 0;

    size_t maxConcurrency = (_fileSize < 1024 * 1024) ? 1 : std::thread::hardware_concurrency();
    size_t chunkSize = _fileSize / maxConcurrency;
    std::cout << "Counting with " << maxConcurrency << " threads" << std::endl;

    std::vector<std::future<std::unordered_set<size_t>>> futures;
    futures.reserve(maxConcurrency);

    for (size_t i = 0; i < maxConcurrency; i++) {
        size_t offset = i * chunkSize;
        bool isLastChunk = (i == maxConcurrency - 1);
        size_t chunkLength = isLastChunk ? (_fileSize - offset) : chunkSize;
        size_t maxChunkOverlapIndex = isLastChunk ? chunkLength : chunkLength * 1.5;

        auto future = std::async(std::launch::async,
            countUniqueWordsForChunk, _fileData + offset, chunkLength, maxChunkOverlapIndex);
        futures.emplace_back(std::move(future));
    }

    std::unordered_set<size_t> wordToCount;
    for (auto& future : futures) {
        wordToCount.merge(std::move(future.get()));
    }
    return wordToCount.size();
}

std::unordered_set<size_t> UniqueWordsCounterAsync::countUniqueWordsForChunk(
    char* start, size_t chunkLength, size_t maxChunkOverlapIndex) {
    std::unordered_set<size_t> threadLocalSet;
    size_t hash = 5381;

    for (size_t i = 0; i < chunkLength; i++) {
        if (start[i] != ' ') {
            hash = (hash << 5) + hash + start[i];
        } else if (hash != 5381) {
            threadLocalSet.emplace(hash);
            hash = 5381;
        }
    }
    if (hash != 5381) {
        size_t i = chunkLength;
        while (i < maxChunkOverlapIndex && start[i] != ' ') {
            hash = (hash << 5) + hash + start[i];
            i++;
        }
        threadLocalSet.emplace(hash);
    }

    return threadLocalSet;
}