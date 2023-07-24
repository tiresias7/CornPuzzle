#include "CornInfo.hpp"

CornInfo::CornInfo()
    : m_mediumRotation{0}, m_lowerRotation{0}, m_emptySlotLayer{0}, m_emptySlotIndex{0}, m_move{0}, m_mediumAdjustment{0}, m_lowerAdjustment{0} {}

CornInfo::CornInfo(size_t mediumRotation, size_t lowerRotation, int emptySlotLayer, int emptySlotIndex, int move, size_t mediumAdjustment, size_t lowerAdjustment)
    : m_mediumRotation{mediumRotation}, m_lowerRotation{lowerRotation}, m_emptySlotLayer{emptySlotLayer}, m_emptySlotIndex{emptySlotIndex}, m_move{move}, m_mediumAdjustment{mediumAdjustment}, m_lowerAdjustment{lowerAdjustment} {}

CornInfo::CornInfo(size_t mediumRotation, size_t lowerRotation, int emptySlotLayer, int emptySlotIndex, int move)
    : m_mediumRotation{mediumRotation}, m_lowerRotation{lowerRotation}, m_emptySlotLayer{emptySlotLayer}, m_emptySlotIndex{emptySlotIndex}, m_move{move}, m_mediumAdjustment{0}, m_lowerAdjustment{0} {}

bool CornInfo::notNull() const {
    return m_mediumRotation != 0 || m_lowerRotation != 0 || m_emptySlotLayer != 0 || m_emptySlotIndex != 0 || m_move != 0 || m_mediumAdjustment != 0 || m_lowerAdjustment != 0;
}

void CornInfo::pprint(std::ostream &os) const {
    os << "Medium rotation: " << m_mediumRotation << std::endl;
    os << "Lower rotation: " << m_lowerRotation << std::endl;
    os << "Empty slot layer: " << m_emptySlotLayer << std::endl;
    os << "Empty slot index: " << m_emptySlotIndex << std::endl;
    os << "Move: " << m_move << std::endl;
    os << "Medium adjustment: " << m_mediumAdjustment << std::endl;
    os << "Lower adjustment: " << m_lowerAdjustment << std::endl;
}

std::ostream &operator<<(std::ostream &os, const CornInfo &info) {
    info.pprint(os);
    return os;
}

// std::vector<CornOperation> CornInfo::getReverseOperation() const {
//     std::vector<CornOperation> result;
//     // First restore adjustment
//     result.push_back({CornOperation::Type::RotateMedium, 6 - (int)m_mediumAdjustment});
//     result.push_back({CornOperation::Type::RotateLower, 6 - (int)m_lowerAdjustment});
//     // Then restore move
//     result.push_back({CornOperation::Type::Move, -m_move});
//     // Then restore rotation
//     result.push_back({CornOperation::Type::RotateMedium, 6 - (int)m_mediumRotation});
//     result.push_back({CornOperation::Type::RotateLower, 6 - (int)m_lowerRotation});
// }

