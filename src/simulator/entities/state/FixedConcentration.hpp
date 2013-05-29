#pragma once

#include "entities/IConcentration.hpp"

#include <inttypes.h>


namespace entities {
namespace state {


class FixedConcentration : public IConcentration {
public:
    FixedConcentration(double new_value) : _value(new_value) {}

    double value() const {return _value;};
    void add_monomer() {};
    void remove_monomer() {};
    uint64_t monomer_count() const { return 0; }

private:
    double _value;
};


} // namespace state
} // namespace entities
