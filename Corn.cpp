#include <iostream>
#include <vector>
#include <unordered_set>
#include <cassert>
#include <iostream>
#include <random>

class CornState {
private:
    // Each layer is an 18-bit integer, where each color is represented by 3 bits.
    // The "extra" layer will only use the 3 least significant bits.
    // IMPORTANT: ALWAYS normalize medium and lower levels to minimal rotation
    int m_extra, m_upper, m_medium, m_lower;

    // Function to rotate 3 bits
    static int rotate3Bits(int n, int bit_size = 18) {
        return ((n & 7) << (bit_size - 3)) | (n >> 3);
    }

    // Function to get the minimal rotation of a layer (Normalization)
    static int getMinRotation(int n) {
        int minRot = n;
        for (int i = 0; i < 6; i++) {
            n = ((n & 7) << 15) | (n >> 3);
            minRot = std::min(minRot, n);
        }
        return minRot;
    }

    // Function to convert colors to bit representation
    static int colorsToBit(const std::vector<int>& colors) {
        return colors[0] << 15 | colors[1] << 12 | colors[2] << 9 | 
            colors[3] << 6 | colors[4] << 3 | colors[5];
    }

    // Function to check if the colors are valid
    static void checkValidColors(int extra, const std::vector<int> &upper, const std::vector<int> &medium, const std::vector<int> &lower) {
        assert(upper.size() == 6);
        assert(medium.size() == 6);
        assert(lower.size() == 6);

        std::vector<int> colorCounts(7, 0);
        assert(extra >= 0 && extra < 7);
        colorCounts[extra]++;
        for (int i = 0; i < 6; i++) {
            assert(upper[i] >= 0 && upper[i] < 7);
            assert(medium[i] >= 0 && medium[i] < 7);
            assert(lower[i] >= 0 && lower[i] < 7);
            colorCounts[upper[i]]++;
            colorCounts[medium[i]]++;
            colorCounts[lower[i]]++;
        }

        // The color 0 should appear only once
        assert(colorCounts[0] == 1);
        // All other colors should appear exactly three times
        for (int i = 1; i < 7; i++) {
            assert(colorCounts[i] == 3);
        }
    }

public:
    // Constructor that accepts bit representations directly
    CornState(int extra, int upper, int medium, int lower) 
        : m_extra{extra}, m_upper{upper}, m_medium{medium}, m_lower{lower} {}

    CornState(int extra, const std::vector<int> &upper, const std::vector<int> &medium, const std::vector<int> &lower) {
        checkValidColors(extra, upper, medium, lower);
        m_extra = extra;
        m_upper = colorsToBit(upper);
        m_medium = getMinRotation(colorsToBit(medium));
        m_lower = getMinRotation(colorsToBit(lower));
    }

    int getExtra() const { return m_extra; }
    int getUpper() const { return m_upper; }
    int getMedium() const { return m_medium; }
    int getLower() const { return m_lower; }

    bool operator==(const CornState& other) const {
        return m_extra == other.m_extra && m_upper == other.m_upper && m_medium == other.m_medium && m_lower == other.m_lower;
    }

    void print(std::ostream &os) const {
        os << m_extra << " " << m_upper << " " << m_medium << " " << m_lower << "\n";
    }

    void pprint(std::ostream &os) const {
        os << m_extra << "\n";
        for (int i = 0; i < 6; i++) {
            os << ((m_upper >> ((5 - i) * 3)) & 7) << " ";
        }
        os << "\n";
        for (int i = 0; i < 6; i++) {
            os << ((m_medium >> ((5 - i) * 3)) & 7) << " ";
        }
        os << "\n";
        for (int i = 0; i < 6; i++) {
            os << ((m_lower >> ((5 - i) * 3)) & 7) << " ";
        }
        os << "\n";
    }

    struct Hash {
        size_t operator()(const CornState& state) const {
            return (state.m_extra << 27) | (state.m_upper << 9) | (state.m_medium << 18) | state.m_lower;
        }
    };
};

class CornSolver {
private:
    struct Info {
        size_t m_prevStateID;
        int m_depth;
        int m_mediumRotation, m_lowerRotation;
        int m_move;
        int m_mediumAdjustment, m_lowerAdjustment;
    };

    std::unordered_set<CornState, CornState::Hash> m_stateSet;
    std::vector<CornState> m_states;
    std::vector<Info> m_infos;

    static int rotate3kBits(int mask, int k, int bit_size = 18) {
        k *= 3;
        return ((mask & ((1 << k) - 1)) << (bit_size - k)) | (mask >> k);
    }

    static int getkthColor(int mask, int k, int bit_size = 18) {
        return (mask >> (bit_size - 3 - k * 3)) & 7;
    }

    static void setkthColor(int &mask, int k, int color, int bit_size = 18) {
        mask = (mask & ~((7 << (bit_size - 3 - k * 3)))) | (color << (bit_size - 3 - k * 3));
    }

    static void swapExtraAndUpper(int &extra, int &upper) {
        int temp = extra;
        extra = getkthColor(upper, 0);
        setkthColor(upper, 0, temp);
    }

    static void swapUpperAndMedium(int &upper, int &medium, int k) {
        int temp = getkthColor(upper, k);
        setkthColor(upper, k, getkthColor(medium, k));
        setkthColor(medium, k, temp);
    }

    static void swapMediumAndLower(int &medium, int &lower, int k) {
        int temp = getkthColor(medium, k);
        setkthColor(medium, k, getkthColor(lower, k));
        setkthColor(lower, k, temp);
    }

    void attemptInsert(std::vector<std::pair<CornState, Info>> &insertList, int extra, int upper, int medium, int lower, size_t prevStateID, int mediumRotation, int lowerRotation, int move) {
        int mediumAdjustment = 0;
        for (int mediumRotated = medium; mediumAdjustment < 6; mediumAdjustment++) {
            mediumRotated = ((mediumRotated & 7) << 15) | (mediumRotated >> 3);
            medium = std::min(medium, mediumRotated);
        }

        int lowerAdjustment = 0;
        for (int lowerRotated = lower; lowerAdjustment < 6; lowerAdjustment++) {
            lowerRotated = ((lowerRotated & 7) << 15) | (lowerRotated >> 3);
            lower = std::min(lower, lowerRotated);
        }

        // if (lower != 42798) {
        //     throw std::runtime_error("lower != 42798");
        // }
        
        // must ensure that medium and lower are in the minimum rotation
        CornState newState(extra, upper, medium, lower);

        if (m_stateSet.find(newState) == m_stateSet.end()) {
            // newState.pprint(std::cout);
            insertList.push_back({newState, {prevStateID, m_infos[prevStateID].m_depth + 1, mediumRotation, lowerRotation, move, mediumAdjustment, lowerAdjustment}});
            // m_stateSet.insert(newState);
            // m_states.push_back(newState);
            // m_infos.push_back({prevStateID, m_infos[prevStateID].m_depth + 1, mediumRotation, lowerRotation, move, mediumAdjustment, lowerAdjustment});
        }
    }

    void rotateAndMove(size_t curStateID) {
        const CornState curState = m_states[curStateID];

        // Locate the position of the empty slot
        // The ID of the slots:
        // 18
        // 0 1 2 3 4 5
        // 6 7 8 9 10 11
        // 12 13 14 15 16 17
        int emptySlotID = -1;
        std::vector<std::pair<CornState, Info>> insertList;
        if (curState.getExtra() == 0) {
            // The empty slot is in the extra layer
            emptySlotID = 18;
            for (int mediumRotation = 0; mediumRotation < 6; mediumRotation++) {
                for (int lowerRotation = 0; lowerRotation < 6; lowerRotation++) {
                    if (mediumRotation == 0 && lowerRotation == 0) { continue; }
                    int newExtra = curState.getExtra();
                    int newUpper = curState.getUpper();
                    int newMedium = rotate3kBits(curState.getMedium(), mediumRotation);
                    int newLower = rotate3kBits(curState.getLower(), lowerRotation);

                    swapExtraAndUpper(newExtra, newUpper);
                    attemptInsert(insertList, newExtra, newUpper, newMedium, newLower, curStateID, mediumRotation, lowerRotation, 1);
                    swapUpperAndMedium(newUpper, newMedium, 0);
                    attemptInsert(insertList, newExtra, newUpper, newMedium, newLower, curStateID, mediumRotation, lowerRotation, 2);
                    // swapMediumAndLower(newMedium, newLower, 0);
                    // attemptInsert(insertList, newExtra, newUpper, newMedium, newLower, curStateID, mediumRotation, lowerRotation, 3);
                }
            }
        }
        else if (getkthColor(curState.getMedium(), 0) == 0) {
            // The empty slot is in the medium layer
            emptySlotID = 6;
            for (int mediumRotation = 0; mediumRotation < 6; mediumRotation++) {
                for (int lowerRotation = 0; lowerRotation < 6; lowerRotation++) {
                    if (mediumRotation == 0 && lowerRotation == 0) { continue; }
                    int newExtra = curState.getExtra();
                    int newUpper = curState.getUpper();
                    int newMedium = rotate3kBits(curState.getMedium(), mediumRotation);
                    int newLower = rotate3kBits(curState.getLower(), lowerRotation);

                    // swapMediumAndLower(newMedium, newLower, mediumRotation);
                    // attemptInsert(insertList, newExtra, newUpper, newMedium, newLower, curStateID, mediumRotation, lowerRotation, 1);
                    // swapMediumAndLower(newMedium, newLower, mediumRotation);
                    swapUpperAndMedium(newUpper, newMedium, mediumRotation);
                    attemptInsert(insertList, newExtra, newUpper, newMedium, newLower, curStateID, mediumRotation, lowerRotation, -1);
                    if (mediumRotation == 0) {
                        swapExtraAndUpper(newExtra, newUpper);
                        attemptInsert(insertList, newExtra, newUpper, newMedium, newLower, curStateID, mediumRotation, lowerRotation, -2);
                    }
                }
            }
        }
        else if (getkthColor(curState.getLower(), 0) == 0) {
            // The empty slot is in the lower layer
            emptySlotID = 12;
            for (int mediumRotation = 0; mediumRotation < 6; mediumRotation++) {
                for (int lowerRotation = 0; lowerRotation < 6; lowerRotation++) {
                    if (mediumRotation == 0 && lowerRotation == 0) { continue; }
                    int newExtra = curState.getExtra();
                    int newUpper = curState.getUpper();
                    int newMedium = rotate3kBits(curState.getMedium(), mediumRotation);
                    int newLower = rotate3kBits(curState.getLower(), lowerRotation);

                    swapMediumAndLower(newMedium, newLower, lowerRotation);
                    // attemptInsert(insertList, newExtra, newUpper, newMedium, newLower, curStateID, mediumRotation, lowerRotation, -1);
                    swapUpperAndMedium(newUpper, newMedium, lowerRotation);
                    attemptInsert(insertList, newExtra, newUpper, newMedium, newLower, curStateID, mediumRotation, lowerRotation, -2);
                    if (lowerRotation == 0) {
                        swapExtraAndUpper(newExtra, newUpper);
                        attemptInsert(insertList, newExtra, newUpper, newMedium, newLower, curStateID, mediumRotation, lowerRotation, -3);
                    }
                }
            }
        }
        else {
            // The empty slot is in the upper layer
            for (int k = 0; k < 6; k++) {
                if (getkthColor(curState.getUpper(), k) == 0) {
                    emptySlotID = k;
                    break;
                }
            }

            if (emptySlotID == -1) {
                curState.pprint(std::cout);
                throw std::runtime_error("Invalid state");
            }

            for (int mediumRotation = 0; mediumRotation < 6; mediumRotation++) {
                for (int lowerRotation = 0; lowerRotation < 6; lowerRotation++) {
                    if (mediumRotation == 0 && lowerRotation == 0) { continue; }
                    int newExtra = curState.getExtra();
                    int newUpper = curState.getUpper();
                    int newMedium = rotate3kBits(curState.getMedium(), mediumRotation);
                    int newLower = rotate3kBits(curState.getLower(), lowerRotation);

                    if (emptySlotID == 0) {
                        swapExtraAndUpper(newExtra, newUpper);
                        attemptInsert(insertList, newExtra, newUpper, newMedium, newLower, curStateID, mediumRotation, lowerRotation, -1);
                        swapExtraAndUpper(newExtra, newUpper);
                    }
                    swapUpperAndMedium(newUpper, newMedium, emptySlotID);
                    attemptInsert(insertList, newExtra, newUpper, newMedium, newLower, curStateID, mediumRotation, lowerRotation, 1);
                    // swapMediumAndLower(newMedium, newLower, emptySlotID);
                    // attemptInsert(insertList, newExtra, newUpper, newMedium, newLower, curStateID, mediumRotation, lowerRotation, 2);
                }
            }
        }

        // std::cout << "Insert degree: " << insertList.size() << std::endl;
        // if (insertList.size() > 50) {
            for (auto &pair: insertList) {
                m_stateSet.insert(pair.first);
                m_states.push_back(pair.first);
                m_infos.push_back(pair.second);
            }  
        // }
    }

public:
    CornSolver(const std::vector<CornState> &states) {
        for (const CornState &state : states) {
            m_stateSet.insert(state);
            m_states.push_back(state);
            m_infos.push_back({0, 0, 0, 0, 0, 0, 0});
        }
    }

    const std::vector<CornState> &getStates() const { return m_states; }

    void solve() {
        int curStateID = 0;
        int curDepth = 0;
        while (curStateID < m_states.size()) {
            if (m_infos[curStateID].m_depth > curDepth) {
                curDepth = m_infos[curStateID].m_depth;
                if (curDepth == 9) break;
                std::cout << "curDepth = " << curDepth << std::endl;
                // m_states[curStateID].pprint(std::cout);
            }
            if (curStateID > 30000000) break;
            rotateAndMove(curStateID);
            curStateID++;
        }
        std::cout << "total states = " << m_states.size() << std::endl;
    }

    void findSameStates(const std::vector<CornState> &states) {
        for (const CornState &state : states) {
            if (m_stateSet.find(state) != m_stateSet.end()) {
                state.pprint(std::cout);
            }
        }
    }

    CornState getRandomState(std::mt19937 &gen) {
        std::uniform_int_distribution<> dis(0, m_states.size() - 1);
        int choice = dis(gen);
        std::cout << "choice = " << choice << std::endl;
        return m_states[choice];
    }

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
};

int main() {
    std::vector<CornState> states1{
        {0, {1, 2, 3, 4, 5, 6}, {1, 2, 3, 4, 5, 6}, {1, 2, 3, 4, 5, 6}},
        {1, {0, 2, 3, 4, 5, 6}, {1, 2, 3, 4, 5, 6}, {1, 2, 3, 4, 5, 6}},
        {1, {1, 2, 3, 4, 5, 6}, {0, 2, 3, 4, 5, 6}, {1, 2, 3, 4, 5, 6}},
        // {1, {1, 2, 3, 4, 5, 6}, {1, 2, 3, 4, 5, 6}, {0, 2, 3, 4, 5, 6}},
    };
    CornSolver solver1(states1);
    solver1.solve();

    // std::vector<CornState> states{
    //     // {6, {6, 2, 3, 4, 5, 0}, {1, 2, 3, 4, 5, 1}, {1, 2, 3, 4, 5, 6}},
    //     // {6, {6, 2, 3, 4, 5, 1}, {1, 2, 3, 4, 5, 0}, {1, 2, 3, 4, 5, 6}},
    //     // {6, {6, 2, 3, 4, 5, 1}, {1, 2, 3, 4, 5, 6}, {1, 2, 3, 4, 5, 0}},
    //     {0, {6, 2, 3, 4, 5, 1}, {6, 2, 3, 4, 5, 1}, {1, 2, 3, 4, 5, 6}},
    //     {6, {0, 2, 3, 4, 5, 1}, {6, 2, 3, 4, 5, 1}, {1, 2, 3, 4, 5, 6}},
    //     {6, {6, 2, 3, 4, 5, 1}, {0, 2, 3, 4, 5, 1}, {1, 2, 3, 4, 5, 6}},
    //     // {6, {6, 2, 3, 4, 5, 1}, {1, 2, 3, 4, 5, 1}, {0, 2, 3, 4, 5, 6}},
    // };

    std::vector<CornState> states2{
        {0, {1, 4, 4, 2, 1, 6}, {5, 5, 2, 6, 3, 3}, {1, 2, 3, 4, 5, 6}},
        {1, {0, 4, 4, 2, 1, 6}, {5, 5, 2, 6, 3, 3}, {1, 2, 3, 4, 5, 6}},
        {1, {5, 4, 4, 2, 1, 6}, {0, 5, 2, 6, 3, 3}, {1, 2, 3, 4, 5, 6}},
    };

    CornSolver solver2(states2);
    solver2.solve();
    std::cout << "findSameStates" << std::endl;
    solver1.findSameStates(solver2.getStates());

    // const unsigned int seed = 44; // Choose a seed value
    // std::mt19937 gen(seed); // Standard mersenne_twister_engine seeded with rd()

    // for (int choice = 0; choice < 100; ++choice) {
    //     std::cout << "choiceID = " << choice << std::endl;
    //     CornSolver solver3({solver1.getRandomState(gen)});
    //     CornSolver solver4({solver2.getRandomState(gen)});
        
    //     solver3.solve();
    //     solver4.solve();
    //     solver3.findSameStates(solver4.getStates());
    // }

    return 0;
}
