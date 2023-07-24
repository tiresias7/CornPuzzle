#include "CornSolver.hpp"


CornSolver::CornSolver(const std::vector<CornState> &states) {
    if (states.size() == 0) {
        throw std::invalid_argument("states must not be empty");
    }
    for (const CornState &state : states) {
        attemptInsert(state, CornInfo{}, 0);
    }
}

size_t CornSolver::getCurDepth() const { return m_depths[m_depths.size() - 1]; }

std::span<const CornState> CornSolver::getDeepestStates() const {
    // m_depths is non-decreasing
    size_t maxDepth = getCurDepth();
    // use lower bound to find the first state with depth = maxDepth
    size_t startID = std::lower_bound(m_depths.begin(), m_depths.end(), maxDepth) - m_depths.begin();
    return std::span<const CornState>(m_states).subspan(startID);
}

void CornSolver::proceedDeepestStates() {
    size_t curDepth = getCurDepth();
    size_t startID = m_states.size() - getDeepestStates().size();
    size_t endID = m_states.size();
    // std::cout << "deepest states = " << endID - startID << std::endl;
    for (size_t i = startID; i < endID; i++) {
        std::vector<std::pair<CornState, CornInfo>> nextStates;
        m_states[i].getNewStates(nextStates);
        for (const auto &nextState : nextStates) {
            attemptInsert(std::move(nextState.first), std::move(nextState.second), curDepth + 1);
        }
    }
}

// CornSolver::findSameStates(std::span<const CornState> states) const {
//     for (const CornState &state : states) {
//         if (m_stateMap.find(state) != m_stateMap.end()) {
//             state.pprint(std::cout);
//             return true;
//         }
//     }
//     return false;
// }

std::optional<CornState> CornSolver::findSameState(std::span<const CornState> states) const {
    for (const CornState &state : states) {
        if (m_stateMap.find(state) != m_stateMap.end()) {
            return state;
        }
    }
    return std::nullopt;
}

bool CornSolver::attemptInsert(CornState state, CornInfo info, int depth) {
    if (m_stateMap.find(state) == m_stateMap.end()) {
        m_stateMap.insert({state, m_states.size()});
        m_states.push_back(state);
        m_infos.push_back(info);
        m_depths.push_back(depth);
        return true;
    }
    return false;
}