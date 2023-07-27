#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <span>
#include <optional>

#include "CornInfo.hpp"
#include "CornState.hpp"

class CornExplorer {

public:
    CornExplorer(size_t extra, const std::vector<size_t> &upper, const std::vector<size_t> &medium, const std::vector<size_t> &lower);

    size_t getCurDepth() const;
    std::span<const CornState> getDeepestStates() const;
    void proceedDeepestStates();
    // bool findSameStates(std::span<const CornState> states) const;
    std::optional<CornState> findSameState(std::span<const CornState> states) const;

    CornInfo getInfo(const CornState &state) const;

private:
    bool attemptInsert(CornState state, CornInfo info, int depth);

    std::unordered_map<CornState, size_t, CornState::Hash> m_stateMap;
    std::vector<CornState> m_states;
    std::vector<CornInfo> m_infos;
    std::vector<size_t> m_depths;
};