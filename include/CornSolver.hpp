#pragma once


#include "CornExplorer.hpp"

class CornSolver {
public:
    // enum class OutputSetting {
    //     UpperLayerFixed,
    //     EmptySlotIndexFixed
    // };
    CornSolver(size_t extra, const std::vector<size_t> &upper, const std::vector<size_t> &medium, const std::vector<size_t> &lower);

    // void setOutputSetting(OutputSetting setting);

    void solve();
private:
    // OutputSetting m_outputSetting;
    CornExplorer m_explorer1, m_explorer2;

    void backtrace(const CornState &state);
};