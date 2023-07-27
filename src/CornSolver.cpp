#include "CornSolver.hpp"

CornSolver::CornSolver(size_t extra, const std::vector<size_t> &upper, const std::vector<size_t> &medium, const std::vector<size_t> &lower)
    : m_explorer1(extra, upper, medium, lower), m_explorer2(0, {1, 2, 3, 4, 5, 6}, {1, 2, 3, 4, 5, 6}, {1, 2, 3, 4, 5, 6}) {}

// void CornSolver::setOutputSetting(OutputSetting setting) { m_outputSetting = setting; }

void CornSolver::solve() {
    // std::cout << "solving..." << std::endl;
    int MAX_STEP = 10;
    for (int step = 0; step < MAX_STEP; ++step) {
        m_explorer1.proceedDeepestStates();
        std::optional<CornState> optstate = m_explorer1.findSameState(m_explorer2.getDeepestStates());
        if (optstate.has_value()) {
            // std::cout << "Found same state at step " << step << std::endl;
            CornState state = optstate.value();
            // std::cout << "State: " << std::endl << state << std::endl;

            // get the path to the state
            backtrace(state);
            return;
        }
        m_explorer2.proceedDeepestStates();
    }
    // std::cout << "No same state found" << std::endl;
}

void CornSolver::backtrace(const CornState &state) {
    std::vector<CornState> path1, path2;
    std::vector<CornInfo> infos1, infos2;
    CornState curState = state;
    while (true) {
        path1.push_back(curState);
        infos1.push_back(m_explorer1.getInfo(curState));
        if (infos1.back().isInitialMove()) {
            break;
        }
        curState = curState.getPreviousState(infos1.back());
    }

    curState = state;
    while (true) {
        path2.push_back(curState);
        infos2.push_back(m_explorer2.getInfo(curState));
        if (infos2.back().isInitialMove()) {
            break;
        }
        curState = curState.getPreviousState(infos2.back());
    }

    // std::cout << "Path1: " << std::endl;
    // for (int i = path1.size() - 1; i >= 0; --i) {
    //     std::cout << path1[i] << std::endl;
    // }
    // std::cout << "Path2: " << std::endl;
    // for (int i = path2.size() - 1; i >= 0; --i) {
    //     std::cout << path2[i] << std::endl;
    // }

    std::cout << "All Operations: " << std::endl;
    std::vector<CornOperation> operations;
    for (int i = infos1.size() - 1; i >= 0; --i) {
        // infos1[i].getOperations() gives a vector of operations, add all of them to operations
        std::vector<CornOperation> forwardOperations = infos1[i].getOperations();
        operations.insert(operations.end(), forwardOperations.begin(), forwardOperations.end());
    }
    for (int i = 0; i < infos2.size(); ++i) {
        // infos2[i].getOperations() gives a vector of operations, add all of them to operations
        std::vector<CornOperation> backwardOperations = infos2[i].getReverseOperations();
        operations.insert(operations.end(), backwardOperations.begin(), backwardOperations.end());
    }
    // compress the operations
    std::vector<CornOperation> compressedOperations = CornOperation::compressOperations(operations);
    for (const CornOperation &operation : compressedOperations) {
        std::cout << operation << std::endl;
    }
}