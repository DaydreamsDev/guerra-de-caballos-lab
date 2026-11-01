#include "game_engine.h"

#include <chrono>
#include <cstdio>

int main()
{
    std::freopen("/dev/null", "w", stdout);
    auto startTime = std::chrono::steady_clock::now();
    for (int seedValue = 1; seedValue <= 10000; seedValue += 1)
    {
        GameEngine<8, 8> engine;
        engine.run(seedValue, "Learner");
    }
    auto endTime = std::chrono::steady_clock::now();
    auto elapsedMilliseconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::fprintf(stderr, "games=10000 milliseconds=%lld\n",
                 static_cast<long long>(elapsedMilliseconds.count()));
    return 0;
}
