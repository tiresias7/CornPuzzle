#include <array>

#include "CornState.hpp"

CornState::CornState(size_t extra, size_t upper, size_t medium, size_t lower) 
    : m_extra{extra}, m_upper{upper}, m_medium{medium}, m_lower{lower} {}

CornState::CornState(size_t extra, const std::vector<size_t> &upper, const std::vector<size_t> &medium, const std::vector<size_t> &lower) {
    checkValidColors(extra, upper, medium, lower);
    m_extra = extra;
    m_upper = colorsToBit(upper);
    getMinRotation(colorsToBit(medium), m_medium);
    getMinRotation(colorsToBit(lower), m_lower);
}

size_t CornState::getEmptySlotID() const {
    if (m_extra == 0) {
        return 0;
    }
    if (getkthColor(m_medium, 0) == 0) {
        return 12;
    }
    if (getkthColor(m_lower, 0) == 0) {
        return 18;
    }
    for (size_t col = 0; col < 6; ++col) {
        if (getkthColor(m_upper, col) == 0) {
            return col + 6;
        }
    }
    // for (size_t col = 0; col < 6; ++col) {
    //     if (getkthColor(m_upper, col) == 0) {
    //         return col + 6;
    //     }
    //     if (getkthColor(m_medium, col) == 0) {
    //         return col + 12;
    //     }
    //     if (getkthColor(m_lower, col) == 0) {
    //         return col + 18;
    //     }
    // }
    return 0;
}

bool CornState::operator==(const CornState& other) const {
    return m_extra == other.m_extra && m_upper == other.m_upper && m_medium == other.m_medium && m_lower == other.m_lower;
}

void CornState::print(std::ostream &os) const {
    os << m_extra << " " << m_upper << " " << m_medium << " " << m_lower << "\n";
}

void CornState::pprint(std::ostream &os) const {
    os << m_extra << "\n";
    printLine(m_upper, os);
    printLine(m_medium, os);
    printLine(m_lower, os);
}

// Modify the info in place
// The info should contain m_mediumRotation, m_lowerRotation, m_move
// The method should return the new state
// And update the info with m_mediumAdjustment, m_lowerAdjustment
CornState CornState::rotateAndMove(CornInfo &info, int emptySlotLayer, size_t emptySlotIndex) const {
    std::array<size_t, 4> newLayers{m_extra << 15, m_upper, rotate3kBits(m_medium, info.m_mediumRotation), rotate3kBits(m_lower, info.m_lowerRotation)};

    // Move the empty slot
    if (info.m_move > 0) {
        for (int layer = emptySlotLayer; layer < emptySlotLayer + info.m_move; ++layer) {
            setkthColor(newLayers[layer], emptySlotIndex, getkthColor(newLayers[layer + 1], emptySlotIndex));
        }
        setkthColor(newLayers[emptySlotLayer + info.m_move], emptySlotIndex, 0);
    } else if (info.m_move < 0) {
        for (int layer = emptySlotLayer; layer > emptySlotLayer + info.m_move; --layer) {
            setkthColor(newLayers[layer], emptySlotIndex, getkthColor(newLayers[layer - 1], emptySlotIndex));
        }
        setkthColor(newLayers[emptySlotLayer + info.m_move], emptySlotIndex, 0);
    }

    // Adjust the medium and lower layers
    info.m_mediumAdjustment = getMinRotation(newLayers[2], newLayers[2]);
    info.m_lowerAdjustment = getMinRotation(newLayers[3], newLayers[3]);

    return CornState(newLayers[0] >> 15, newLayers[1], newLayers[2], newLayers[3]);
}

void CornState::getNewStates(std::vector<std::pair<CornState, CornInfo>> &newStates) const {
    // Locate the empty slot
    size_t emptySlotID = getEmptySlotID();
    int emptySlotLayer = emptySlotID / 6;
    size_t emptySlotIndex = emptySlotID % 6;

    // Rotate the medium and lower layers
    for (size_t mediumRotation = 0; mediumRotation < 6; mediumRotation++) {
        // for (size_t lowerRotation = 0; lowerRotation < 6; lowerRotation++) {
        //     for (int targetLayer = emptySlotIndex == 0 ? 0 : 1; targetLayer < 4; targetLayer++) {
        //         if (targetLayer == emptySlotLayer) {
        //             continue;
        //         }
        //         CornInfo info{mediumRotation, lowerRotation, targetLayer - emptySlotLayer};
        //         CornState newState = rotateAndMove(info, emptySlotID);
        //         newStates.push_back(std::make_pair(std::move(newState), std::move(info)));
        //     }
        // }

        size_t lowerRotation = 0;
        emptySlotIndex = emptySlotID % 6;
        if (emptySlotLayer == 2) {
            emptySlotIndex = (emptySlotIndex + mediumRotation) % 6;
        }
        else if (emptySlotLayer == 3) {
            emptySlotIndex = (emptySlotIndex + lowerRotation) % 6;
        }

        for (int targetLayer = emptySlotIndex == 0 ? 0 : 1; targetLayer < 3; targetLayer++) {
            if (targetLayer == emptySlotLayer) {
                continue;
            }
            CornInfo info{mediumRotation, lowerRotation, targetLayer - emptySlotLayer};
            CornState newState = rotateAndMove(info, emptySlotLayer, emptySlotIndex);
            if (!newState.checkValidColors()) {
                // output error message
                std::cout << "Invalid colors: " << std::endl;
                std::cout << "mediumRotation: " << mediumRotation << std::endl;
                auto mediumRotated = rotate3kBits(m_medium, mediumRotation);
                std::cout << "mediumRotated: ";
                printLine(mediumRotated, std::cout);
                std::cout << "lowerRotation: " << lowerRotation << std::endl;
                std::cout << "lowerRotated: " << rotate3kBits(m_lower, lowerRotation) << std::endl;
                std::cout << "targetLayer: " << targetLayer << std::endl;
                std::cout << "emptySlotID: " << emptySlotID << std::endl;
                std::cout << "emptySlotLayer: " << emptySlotLayer << std::endl;
                std::cout << "emptySlotIndex: " << emptySlotIndex << std::endl;

                pprint(std::cout);
                newState.pprint(std::cout);
                throw std::runtime_error("Invalid colors");
            }
            newStates.push_back(std::make_pair(std::move(newState), std::move(info)));
        }
    }
}

size_t CornState::rotate3Bits(size_t mask) {
    return ((mask & 7) << 15) | (mask >> 3);
}

size_t CornState::rotate3kBits(size_t mask, size_t k) {
    k *= 3;
    return ((mask & ((1 << k) - 1)) << (18 - k)) | (mask >> k);
}

size_t CornState::getkthColor(size_t mask, size_t k) {
    return (mask >> (15 - k * 3)) & 7;
}

void CornState::setkthColor(size_t &mask, size_t k, size_t color) {
    mask = (mask & ~((7 << (15 - k * 3)))) | (color << (15 - k * 3));
}

size_t CornState::getMinRotation(size_t mask, size_t &rotated) {
    size_t rotation = 0;
    rotated = mask;
    for (size_t rot = 0; rot < 6; rot++) {
        mask = ((mask & 7) << 15) | (mask >> 3);
        if (mask < rotated) {
            rotation = rot;
            rotated = mask;
        }
    }
    return rotation;
}

size_t CornState::colorsToBit(const std::vector<size_t>& colors) {
    return colors[0] << 15 | colors[1] << 12 | colors[2] << 9 | 
        colors[3] << 6 | colors[4] << 3 | colors[5];
}

void CornState::checkValidColors(size_t extra, const std::vector<size_t> &upper, const std::vector<size_t> &medium, const std::vector<size_t> &lower) {
    if (upper.size() != 6 || medium.size() != 6 || lower.size() != 6) {
        throw std::invalid_argument("Invalid number of colors");
    }

    std::vector<int> colorCounts(7, 0);
    if (extra < 0 || extra >= 7) {
        throw std::invalid_argument("Invalid color representation");
    }
    colorCounts[extra]++;
    for (int i = 0; i < 6; i++) {
        if (upper[i] < 0 || upper[i] >= 7 || medium[i] < 0 || medium[i] >= 7 || lower[i] < 0 || lower[i] >= 7) {
            throw std::invalid_argument("Invalid color representation");
        }
        colorCounts[upper[i]]++;
        colorCounts[medium[i]]++;
        colorCounts[lower[i]]++;
    }

    // The color 0 should appear only once
    if (colorCounts[0] != 1) {
        throw std::invalid_argument("Invalid color count");
    }
    // All other colors should appear exactly three times
    for (int i = 1; i < 7; i++) {
        if (colorCounts[i] != 3) {
            throw std::invalid_argument("Invalid color count");
        }
    }
}

bool CornState::checkValidColors() {
    try {
        checkValidColors(
            m_extra,
            {getkthColor(m_upper, 0), getkthColor(m_upper, 1), getkthColor(m_upper, 2), getkthColor(m_upper, 3), getkthColor(m_upper, 4), getkthColor(m_upper, 5)},
            {getkthColor(m_medium, 0), getkthColor(m_medium, 1), getkthColor(m_medium, 2), getkthColor(m_medium, 3), getkthColor(m_medium, 4), getkthColor(m_medium, 5)},
            {getkthColor(m_lower, 0), getkthColor(m_lower, 1), getkthColor(m_lower, 2), getkthColor(m_lower, 3), getkthColor(m_lower, 4), getkthColor(m_lower, 5)});
        return true;
    } catch (std::invalid_argument &e) {
        return false;
    }
}

void CornState::printLine(size_t mask, std::ostream &os) const {
    for (int shift = 15; shift >= 0; shift -= 3) {
        os << ((mask >> shift) & 7) << " ";
    }
    os << "\n";
}

