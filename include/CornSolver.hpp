#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <random>

#include "CornInfo.hpp"
#include "CornState.hpp"

class CornSolver {
private:
    bool attemptInsert(CornState state, CornInfo info, int depth) {
        if (m_stateMap.find(state) == m_stateMap.end()) {
            m_stateMap.insert({state, m_states.size()});
            m_states.push_back(state);
            m_infos.push_back(info);
            m_depths.push_back(depth);
            return true;
        }
        return false;
    }

public:
    CornSolver(const std::vector<CornState> &states) {
        for (const CornState &state : states) {
            attemptInsert(state, CornInfo{0, 0, 0}, 0);
        }
    }

    const std::vector<CornState> &getStates() const { return m_states; }

    void solve() {
        int curStateID = 0;
        int curDepth = 0;
        while (curStateID < m_states.size()) {
            if (m_depths[curStateID] > curDepth) {
                curDepth = m_depths[curStateID];
                if (curDepth == 7) break;
                std::cout << "curDepth = " << curDepth << std::endl;
            }
            std::vector<std::pair<CornState, CornInfo>> nextStates;
            m_states[curStateID].getNewStates(nextStates);
            for (const auto &nextState : nextStates) {
                if (attemptInsert(std::move(nextState.first), std::move(nextState.second), curDepth + 1)) {
                    // nextState.first.pprint(std::cout);
                    // nextState.second.pprint(std::cout);
                }
            }
            
            curStateID++;
        }
        std::cout << "total states = " << m_states.size() << std::endl;
    }

    void findSameStates(const std::vector<CornState> &states) {
        for (const CornState &state : states) {
            if (m_stateMap.find(state) != m_stateMap.end()) {
                state.pprint(std::cout);
                return;
            }
        }
    }

    // CornState getRandomState(std::mt19937 &gen) {
    //     std::uniform_int_distribution<> dis(0, m_states.size() - 1);
    //     int choice = dis(gen);
    //     std::cout << "choice = " << choice << std::endl;
    //     return m_states[choice];
    // }

    // void test1() {
    //     CornState toy1{0, {1, 2, 3, 4, 5, 6}, {1, 2, 3, 4, 5, 6}, {1, 2, 3, 4, 5, 6}};
    //     std::cout << "getkthColor(toy1.getUpper(), 0) = " << getkthColor(toy1.getUpper(), 0) << std::endl;
    //     std::cout << "getkthColor(toy1.getUpper(), 1) = " << getkthColor(toy1.getUpper(), 1) << std::endl;
    //     std::cout << "getkthColor(toy1.getUpper(), 2) = " << getkthColor(toy1.getUpper(), 2) << std::endl;
    //     std::cout << "getkthColor(toy1.getUpper(), 3) = " << getkthColor(toy1.getUpper(), 3) << std::endl;
    //     std::cout << "getkthColor(toy1.getUpper(), 4) = " << getkthColor(toy1.getUpper(), 4) << std::endl;
    //     std::cout << "getkthColor(toy1.getUpper(), 5) = " << getkthColor(toy1.getUpper(), 5) << std::endl;
    //     setkthColor(toy1.getUpper(), 0, 7);
    //     std::cout << "getkthColor(toy1.getUpper(), 0) = " << getkthColor(toy1.getUpper(), 0) << std::endl;
    //     swapExtraAndUpper(toy1.getExtra(), toy1.getUpper());
    //     toy1.getLower() = rotate3kBits(toy1.getLower(), 1);
    //     swapMediumAndLower(toy1.getMedium(), toy1.getLower(), 1);
    //     toy1.pprint(std::cout);
    // }
private:
    std::unordered_map<CornState, size_t, CornState::Hash> m_stateMap;
    std::vector<CornState> m_states;
    std::vector<CornInfo> m_infos;
    std::vector<size_t> m_depths;
};