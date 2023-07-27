#pragma once

#include <vector>
#include <ostream>

struct CornOperation {
    enum class Type {
        RotateUpper,
        RotateMedium,
        RotateLower,
        Move
    };

    Type m_type;
    int m_count;

    void pprint(std::ostream &os) const;

    static std::vector<CornOperation> compressOperations(const std::vector<CornOperation> &opers);
};

std::ostream &operator<<(std::ostream &os, const CornOperation &oper);
