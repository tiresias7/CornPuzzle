#include <iostream>
#include <vector>

#include "CornState.hpp"
#include "CornSolver.hpp"

void solve(const std::vector<CornState> &states1, const std::vector<CornState> &states2) {
    CornSolver solver1(states1);
    CornSolver solver2(states2);

    int MAX_STEP = 10;
    for (int step = 0; step < MAX_STEP; ++step) {
        solver1.proceedDeepestStates();
        std::optional<CornState> state = solver1.findSameState(solver2.getDeepestStates());
        if (state.has_value()) {
            std::cout << "Found same state at step " << step << std::endl;
            std::cout << "State: " << *state << std::endl;
            return;
        }
        solver2.proceedDeepestStates();
    }
    std::cout << "No same state found" << std::endl;
}

int main() {
    std::vector<CornState> states1{
        {0, {1, 2, 3, 4, 5, 6}, {1, 2, 3, 4, 5, 6}, {1, 2, 3, 4, 5, 6}},
    };
    std::vector<CornState> states2{
        {0, {1, 4, 4, 2, 1, 6}, {5, 5, 2, 6, 3, 3}, {1, 2, 3, 4, 5, 6}},
    };

    solve(states1, states2);

    return 0;
}
