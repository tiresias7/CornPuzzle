#include <iostream>
#include <vector>

#include "CornState.hpp"
#include "CornSolver.hpp"

int main() {
    // CornState state{0, {1, 2, 3, 4, 5, 6}, {1, 2, 3, 4, 5, 6}, {1, 2, 3, 4, 5, 6}};
    // // test getNewStates
    // std::vector<std::pair<CornState, CornInfo>> nextStates;
    // state.getNewStates(nextStates);
    // for (const auto &nextState : nextStates) {
    //     nextState.first.pprint(std::cout);
    // }
    // std::cout << "nextStates.size() = " << nextStates.size() << std::endl;
    
    std::vector<CornState> states1{
        {0, {1, 2, 3, 4, 5, 6}, {1, 2, 3, 4, 5, 6}, {1, 2, 3, 4, 5, 6}},
        {1, {0, 2, 3, 4, 5, 6}, {1, 2, 3, 4, 5, 6}, {1, 2, 3, 4, 5, 6}},
        {1, {1, 2, 3, 4, 5, 6}, {0, 2, 3, 4, 5, 6}, {1, 2, 3, 4, 5, 6}},
        // {1, {1, 2, 3, 4, 5, 6}, {1, 2, 3, 4, 5, 6}, {0, 2, 3, 4, 5, 6}},
    };
    CornSolver solver1(states1);
    solver1.solve();

    // std::vector<CornState> states{
    //     // {6, {6, 2, 3, 4, 5, 0}, {1, 2, 3, 4, 5, 1}, {1, 2, 3, 4, 5, 6}},
    //     // {6, {6, 2, 3, 4, 5, 1}, {1, 2, 3, 4, 5, 0}, {1, 2, 3, 4, 5, 6}},
    //     // {6, {6, 2, 3, 4, 5, 1}, {1, 2, 3, 4, 5, 6}, {1, 2, 3, 4, 5, 0}},
    //     {0, {6, 2, 3, 4, 5, 1}, {6, 2, 3, 4, 5, 1}, {1, 2, 3, 4, 5, 6}},
    //     {6, {0, 2, 3, 4, 5, 1}, {6, 2, 3, 4, 5, 1}, {1, 2, 3, 4, 5, 6}},
    //     {6, {6, 2, 3, 4, 5, 1}, {0, 2, 3, 4, 5, 1}, {1, 2, 3, 4, 5, 6}},
    //     // {6, {6, 2, 3, 4, 5, 1}, {1, 2, 3, 4, 5, 1}, {0, 2, 3, 4, 5, 6}},
    // };

    std::vector<CornState> states2{
        {0, {1, 4, 4, 2, 1, 6}, {5, 5, 2, 6, 3, 3}, {1, 2, 3, 4, 5, 6}},
        {1, {0, 4, 4, 2, 1, 6}, {5, 5, 2, 6, 3, 3}, {1, 2, 3, 4, 5, 6}},
        {1, {5, 4, 4, 2, 1, 6}, {0, 5, 2, 6, 3, 3}, {1, 2, 3, 4, 5, 6}},
        // {0, {1, 2, 3, 4, 5, 6}, {6, 2, 3, 4, 5, 1}, {1, 2, 3, 4, 5, 6}},
        // {1, {0, 2, 3, 4, 5, 6}, {6, 2, 3, 4, 5, 1}, {1, 2, 3, 4, 5, 6}},
        // {1, {6, 2, 3, 4, 5, 6}, {0, 2, 3, 4, 5, 1}, {1, 2, 3, 4, 5, 6}},
    };

    CornSolver solver2(states2);
    solver2.solve();
    std::cout << "findSameStates" << std::endl;
    solver1.findSameStates(solver2.getStates());

    // const unsigned int seed = 44; // Choose a seed value
    // std::mt19937 gen(seed); // Standard mersenne_twister_engine seeded with rd()

    // for (int choice = 0; choice < 100; ++choice) {
    //     std::cout << "choiceID = " << choice << std::endl;
    //     CornSolver solver3({solver1.getRandomState(gen)});
    //     CornSolver solver4({solver2.getRandomState(gen)});
        
    //     solver3.solve();
    //     solver4.solve();
    //     solver3.findSameStates(solver4.getStates());
    // }

    return 0;
}
