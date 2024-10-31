#include "UniqueWordsCounterAsync.h"

#include <future>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <vector>

std::mutex UniqueWordsCounterAsync::_mutex;

UniqueWordsCounterAsync::UniqueWordsCounterAsync(char* fileData, size_t fileSize)
    : _fileData(fileData), _fileSize(fileSize) {
}

size_t UniqueWordsCounterAsync::CountUniqueWordsAsync() {
    if (!_fileData)
        return 0;

    size_t maxConcurrency = (_fileSize < 1024 * 1024) ? 1 : std::thread::hardware_concurrency();
    size_t chunkSize = _fileSize / maxConcurrency;
    std::vector<std::thread> threads;
    threads.reserve(maxConcurrency);

    std::cout << "Counting with " << maxConcurrency << " threads" << std::endl;

    for (size_t i = 0; i < maxConcurrency; i++) {
        size_t offset = i * chunkSize;
        bool isLastChunk = (i == maxConcurrency - 1);
        size_t chunkLength = isLastChunk ? (_fileSize - offset) : chunkSize;
        size_t maxChunkOverlapIndex = isLastChunk ? chunkLength : chunkLength * 1.5;

        threads.emplace_back(countUniqueWordsForChunk, _fileData + offset, chunkLength,
            maxChunkOverlapIndex, std::ref(_wordToCount));
    }

    std::for_each(threads.begin(), threads.end(), [](std::thread &t){ t.join();});

    return _wordToCount.size();
}

void UniqueWordsCounterAsync::countUniqueWordsForChunk(char* start, size_t chunkLength,
    size_t maxChunkOverlapIndex, std::unordered_map<size_t, int>& outWordToCount) {
    std::unordered_map<size_t, int> localWordToCount;
    std::string word;

    for (size_t i = 0; i < chunkLength; i++) {
        if (!std::isspace(start[i])) {
            word.push_back(start[i]);
        } else if (!word.empty()){
            localWordToCount[std::hash<std::string>{}(word)]++;
            word.clear();
        }
    }

    if (!word.empty()) {
        size_t i = chunkLength;
        while (i < maxChunkOverlapIndex && !std::isspace(start[i])) {
            word.push_back(start[i]);
            i++;
        }
        localWordToCount[std::hash<std::string>{}(word)]++;
    }

    std::lock_guard<std::mutex> lock(_mutex);
    for (const auto& [word, count] : localWordToCount) {
        outWordToCount[word] += count;
    }
}