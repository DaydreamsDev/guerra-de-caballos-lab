#include "console.h"
#include "core.h"

int main()
{
    GameResult result = run_game(11, "Learner");
    print_result(result);
    return 0;
}
