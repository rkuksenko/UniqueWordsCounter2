#include "UniqueWordsCounterAsync.h"

#include <future>
#include <iostream>
#include <unordered_set>
#include <vector>

constexpr size_t computeHash(size_t hash, char c) {
    return (hash * 33) ^ static_cast<size_t>(c);
}

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
        size_t maxChunkOverlapIndex = isLastChunk ? chunkLength : chunkLength + (chunkSize / 2);

        futures.emplace_back(std::async(
            std::launch::async,
            countUniqueWordsForChunk, _fileData + offset, chunkLength, maxChunkOverlapIndex
        ));
    }

    std::unordered_set<size_t> wordToCount;
    wordToCount.reserve(10'000'000);
    for (auto& future : futures) {
        wordToCount.merge(std::move(future.get()));
    }
    return wordToCount.size();
}

std::unordered_set<size_t> UniqueWordsCounterAsync::countUniqueWordsForChunk(
    char* start, size_t chunkLength, size_t maxChunkOverlapIndex) {
    std::unordered_set<size_t> threadLocalSet;
    threadLocalSet.reserve(1'000'000);

    constexpr size_t initialHash = 5381;
    size_t hash = initialHash;

    char* end = start + chunkLength;
    char* overlapEnd = start + maxChunkOverlapIndex;

    while (start < end) {
        if (*start != ' ') {
            hash = computeHash(hash, *start);
        } else if (hash != initialHash) {
            threadLocalSet.emplace(hash);
            hash = initialHash;
        }
        ++start;
    }

    if (hash != initialHash) {
        while (start < overlapEnd && *start != ' ') {
            hash = computeHash(hash, *start);
            ++start;
        }
        threadLocalSet.emplace(hash);
    }

    return threadLocalSet;
}