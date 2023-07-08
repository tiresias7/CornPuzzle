#pragma once

#include <memory>

struct CornInfo {
    size_t m_mediumRotation, m_lowerRotation;
    int m_move;
    size_t m_mediumAdjustment, m_lowerAdjustment;

    CornInfo(size_t mediumRotation, size_t lowerRotation, int move, size_t mediumAdjustment, size_t lowerAdjustment)
        : m_mediumRotation{mediumRotation}, m_lowerRotation{lowerRotation}, m_move{move}, m_mediumAdjustment{mediumAdjustment}, m_lowerAdjustment{lowerAdjustment} {}
    
    // Always imagine moveing the empty slot
    CornInfo(size_t mediumRotation, size_t lowerRotation, int move)
        : m_mediumRotation{mediumRotation}, m_lowerRotation{lowerRotation}, m_move{move}, m_mediumAdjustment{0}, m_lowerAdjustment{0} {}
};