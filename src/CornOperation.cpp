#include <map>

#include "CornOperation.hpp"

void CornOperation::pprint(std::ostream &os) const {
    if (m_type == Type::Move) {
        if (m_count > 0) {
            os << "Move " << m_count << " balls below the empty slot towards it.";
            return;
        }
        else if (m_count < 0){
            os << "Move " << -m_count << " balls above the empty slot towards it.";
            return;
        }
    }
    std::string_view layerName;
    switch (m_type) {
        case Type::RotateUpper:
            layerName = "upper";
            break;
        case Type::RotateMedium:
            layerName = "medium";
            break;
        case Type::RotateLower:
            layerName = "lower";
            break;
        default:
            layerName = "unknown";
            break;
    }
    if (m_count > 0) {
        os << "Rotate the " << layerName << " layer counterclockwise by " << m_count << ".";
        return;
    }
    else if (m_count < 0) {
        os << "Rotate the " << layerName << " layer clockwise by " << -m_count << ".";
        return;
    }
}

std::ostream &operator<<(std::ostream &os, const CornOperation &oper) {
    oper.pprint(os);
    return os;
}

std::vector<CornOperation> CornOperation::compressOperations(const std::vector<CornOperation> &opers) {
    std::vector<CornOperation> compressed;
    std::map<CornOperation::Type, int> rotationCounts = {
        {CornOperation::Type::RotateUpper, 0},
        {CornOperation::Type::RotateMedium, 0},
        {CornOperation::Type::RotateLower, 0}
    };

    auto addRotationIfNonZero = [&compressed](int& count, CornOperation::Type type) {
        count = (count % 6 + 6) % 6;
        if (count != 0) {
            compressed.push_back({type, (count + 2) % 6 - 2});
            count = 0;
        }
    };

    for (const auto &oper : opers) {
        switch (oper.m_type) {
            case CornOperation::Type::Move:
                for (auto &rotationCount : rotationCounts) {
                    addRotationIfNonZero(rotationCount.second, rotationCount.first);
                }
                // if (compressed.size() > 0 && compressed.back().m_type == CornOperation::Type::Move) {
                //     compressed.back().m_count += oper.m_count;
                // }
                // else {
                //     compressed.push_back(oper);
                // }
                compressed.push_back(oper);
                break;
                
            default:
                rotationCounts[oper.m_type] += oper.m_count;
                break;
        }
    }

    for (auto &rotationCount : rotationCounts) {
        addRotationIfNonZero(rotationCount.second, rotationCount.first);
    }

    return compressed;
}


