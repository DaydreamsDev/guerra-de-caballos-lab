#include "game_engine.h"
#include "recording_transcript.h"

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace
{
constexpr int threadCount = 8;
constexpr int gamesPerThread = 2000;
constexpr int totalGameCount = threadCount * gamesPerThread;

struct GameResult
{
    int seed;
    int moveCount;
    std::string winner;
};

GameResult run_one_game(int seed)
{
    RecordingTranscript transcript;
    GameEngine<8, 8> engine(transcript);
    engine.run(seed, "Learner");
    return GameResult{seed, transcript.finalMoveCount, transcript.winnerName};
}

std::uint64_t result_checksum(const std::vector<GameResult> &results)
{
    std::uint64_t checksum = 1469598103934665603ULL;
    for (const GameResult &result : results)
    {
        checksum ^= static_cast<std::uint64_t>(result.seed);
        checksum *= 1099511628211ULL;
        checksum ^= static_cast<std::uint64_t>(result.moveCount);
        checksum *= 1099511628211ULL;
        for (char character : result.winner)
        {
            checksum ^= static_cast<unsigned char>(character);
            checksum *= 1099511628211ULL;
        }
    }
    return checksum;
}

void sort_by_seed(std::vector<GameResult> &results)
{
    std::sort(results.begin(), results.end(),
              [](const GameResult &left, const GameResult &right)
              {
                  return left.seed < right.seed;
              });
}

void run_sequential()
{
    std::vector<GameResult> results;
    results.reserve(totalGameCount);
    for (int seed = 1; seed <= totalGameCount; seed += 1)
    {
        results.push_back(run_one_game(seed));
    }
    std::printf("mode=sequential games=%d written=%zu checksum=%llu\n", totalGameCount,
                results.size(), static_cast<unsigned long long>(result_checksum(results)));
}

void run_race()
{
    std::vector<GameResult> results(totalGameCount);
    int nextIndex = 0;
    std::vector<std::thread> threads;
    threads.reserve(threadCount);
    for (int threadIndex = 0; threadIndex < threadCount; threadIndex += 1)
    {
        threads.emplace_back(
            [&results, &nextIndex, threadIndex]
            {
                for (int gameIndex = 0; gameIndex < gamesPerThread; gameIndex += 1)
                {
                    int seed = threadIndex * gamesPerThread + gameIndex + 1;
                    GameResult result = run_one_game(seed);
                    int resultIndex = nextIndex;
                    nextIndex += 1;
                    results[resultIndex] = result;
                }
            });
    }
    for (std::thread &thread : threads)
    {
        thread.join();
    }

    std::vector<GameResult> writtenResults;
    for (const GameResult &result : results)
    {
        if (result.seed != 0)
        {
            writtenResults.push_back(result);
        }
    }
    std::printf("mode=race games=%d next_index=%d written=%zu checksum=%llu\n",
                totalGameCount, nextIndex, writtenResults.size(),
                static_cast<unsigned long long>(result_checksum(writtenResults)));
}

void run_mutex()
{
    std::vector<GameResult> results;
    std::mutex resultsMutex;
    std::vector<std::thread> threads;
    threads.reserve(threadCount);
    for (int threadIndex = 0; threadIndex < threadCount; threadIndex += 1)
    {
        threads.emplace_back(
            [&results, &resultsMutex, threadIndex]
            {
                for (int gameIndex = 0; gameIndex < gamesPerThread; gameIndex += 1)
                {
                    int seed = threadIndex * gamesPerThread + gameIndex + 1;
                    GameResult result = run_one_game(seed);
                    std::lock_guard<std::mutex> lock(resultsMutex);
                    results.push_back(result);
                }
            });
    }
    for (std::thread &thread : threads)
    {
        thread.join();
    }
    sort_by_seed(results);
    std::printf("mode=mutex games=%d written=%zu checksum=%llu\n", totalGameCount,
                results.size(), static_cast<unsigned long long>(result_checksum(results)));
}

void run_merge()
{
    std::vector<std::vector<GameResult>> perThreadResults(threadCount);
    std::vector<std::thread> threads;
    threads.reserve(threadCount);
    for (int threadIndex = 0; threadIndex < threadCount; threadIndex += 1)
    {
        threads.emplace_back(
            [&perThreadResults, threadIndex]
            {
                for (int gameIndex = 0; gameIndex < gamesPerThread; gameIndex += 1)
                {
                    int seed = threadIndex * gamesPerThread + gameIndex + 1;
                    perThreadResults[threadIndex].push_back(run_one_game(seed));
                }
            });
    }
    for (std::thread &thread : threads)
    {
        thread.join();
    }

    std::vector<GameResult> results;
    results.reserve(totalGameCount);
    for (const std::vector<GameResult> &threadResults : perThreadResults)
    {
        results.insert(results.end(), threadResults.begin(), threadResults.end());
    }
    sort_by_seed(results);
    std::printf("mode=merge games=%d written=%zu checksum=%llu\n", totalGameCount,
                results.size(), static_cast<unsigned long long>(result_checksum(results)));
}
}

int main(int argumentCount, char **argumentValues)
{
    if (argumentCount != 3 || std::strcmp(argumentValues[1], "--mode") != 0)
    {
        std::fprintf(stderr, "usage: %s --mode sequential|race|mutex|merge\n",
                     argumentValues[0]);
        return 2;
    }

    const char *mode = argumentValues[2];
    if (std::strcmp(mode, "sequential") == 0)
    {
        run_sequential();
        return 0;
    }
    if (std::strcmp(mode, "race") == 0)
    {
        run_race();
        return 0;
    }
    if (std::strcmp(mode, "mutex") == 0)
    {
        run_mutex();
        return 0;
    }
    if (std::strcmp(mode, "merge") == 0)
    {
        run_merge();
        return 0;
    }

    std::fprintf(stderr, "unknown mode: %s\n", mode);
    return 2;
}
