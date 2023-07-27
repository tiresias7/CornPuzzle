#pragma once


#include "CornExplorer.hpp"

class CornSolver {
public:
    // enum class OutputSetting {
    //     UpperLayerFixed,
    //     EmptySlotIndexFixed
    // };
    CornSolver(size_t extra1, const std::vector<size_t> &upper1, const std::vector<size_t> &medium1, const std::vector<size_t> &lower1,
        size_t extra2, const std::vector<size_t> &upper2, const std::vector<size_t> &medium2, const std::vector<size_t> &lower2);

    // void setOutputSetting(OutputSetting setting);

    void solve();
private:
    // OutputSetting m_outputSetting;
    CornExplorer m_explorer1, m_explorer2;

    void backtrace(const CornState &state);
};