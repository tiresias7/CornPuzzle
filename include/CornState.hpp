#pragma once

#include <vector>
#include <cassert>
#include <iostream>

#include "CornInfo.hpp"
// #include "CornOperation.hpp"

class CornState {
public:
    struct Hash {
        size_t operator()(const CornState& state) const {
            return (state.m_extra << 54) ^ (state.m_upper << 36) ^ (state.m_medium << 18) ^ state.m_lower;
        }
    };

    // Constructor that accepts bit representations directly
    CornState(size_t extra, size_t upper, size_t medium, size_t lower);

    CornState(size_t extra, const std::vector<size_t> &upper, const std::vector<size_t> &medium, const std::vector<size_t> &lower);

    size_t getExtra() const { return m_extra; }
    size_t getUpper() const { return m_upper; }
    size_t getMedium() const { return m_medium; }
    size_t getLower() const { return m_lower; }

    bool operator==(const CornState& other) const;

    void print(std::ostream &os = std::cout) const;
    void pprint(std::ostream &os = std::cout) const;

    CornState rotateAndMove(CornInfo &info) const;
    void getNewStates(std::vector<std::pair<CornState, CornInfo>>& newStates) const;

    CornState getPreviousState(const CornInfo &info) const;

private:
    // Each layer is an 18-bit integer, where each color is represented by 3 bits.
    // The "extra" layer will only use the 3 least significant bits.
    // IMPORTANT: ALWAYS normalize medium and lower levels to minimal rotation
    size_t m_extra, m_upper, m_medium, m_lower;

    // Function to rotate 3 bits
    static size_t rotate3Bits(size_t mask);
    // Function to rotate 3k bits
    static size_t rotate3kBits(size_t mask, size_t k);

    static size_t getkthColor(size_t mask, size_t k);
    static void setkthColor(size_t &mask, size_t k, size_t color);

    // Function to get the minimal rotation of a layer (Normalization)
    static size_t getMinRotation(size_t mask, size_t &rotated);

    // Function to convert colors to bit representation
    static size_t colorsToBit(const std::vector<size_t>& colors);
    // Function to check if the colors are valid
    bool checkValidColors();
    static void checkValidColors(size_t extra, const std::vector<size_t> &upper, const std::vector<size_t> &medium, const std::vector<size_t> &lower);

    std::pair<int, int> getEmptySlotLoc() const;

    void printLine(size_t mask, std::ostream &os = std::cout) const;
};

std::ostream &operator<<(std::ostream &os, const CornState &state);