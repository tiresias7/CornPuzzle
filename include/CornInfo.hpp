#pragma once

#include <memory>
#include <ostream>

#include "CornOperation.hpp"

struct CornInfo {
public:
    size_t m_mediumRotation, m_lowerRotation;
    int m_emptySlotLayer, m_emptySlotIndex;
    int m_move;
    size_t m_mediumAdjustment, m_lowerAdjustment;
    bool m_isInitialMove;

    CornInfo(size_t mediumRotation, size_t lowerRotation, int emptySlotLayer, int emptySlotIndex, int move, size_t mediumAdjustment, size_t lowerAdjustment);
    // Always imagine moveing the empty slot
    CornInfo(size_t mediumRotation, size_t lowerRotation, int emptySlotLayer, int emptySlotIndex, int move);
    CornInfo(size_t mediumAdjustment, size_t lowerAdjustment);

    // bool notNull() const;
    bool isInitialMove() const;
    std::vector<CornOperation> getOperations() const;
    std::vector<CornOperation> getReverseOperations() const;

    void pprint(std::ostream &os) const;

private:
    enum class outputSetting {
        UpperLayerFixed,
        EmptySlotIndexFixed
    };
};

std::ostream &operator<<(std::ostream &os, const CornInfo &info);