#include "CornSolver.hpp"

int main() {
    CornSolver solver{
        0, {1, 2, 3, 4, 5, 6}, {1, 2, 3, 4, 5, 6}, {1, 2, 3, 4, 5, 6},
        0, {1, 2, 3, 4, 5, 6}, {6, 2, 3, 4, 5, 1}, {1, 2, 3, 4, 5, 6}
    };
    solver.solve();

    return 0;
}
