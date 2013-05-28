#pragma once

#include "entities/State.hpp"
#include <memory>

namespace entities {

struct StateModificationDescriptor {
    uint64_t component_id;

    enum component_type_t {CONCENTRATION, FILAMENT};
    component_type_t component_type;

    enum modification_t {CREATED, DELETED, MODIFIED};
    modification_t modification;
};


class IEventGenerator {
public:
    virtual ~IEventGenerator() {}

    virtual double rate(State const* state,
            std::vector<StateModificationDescriptor> const&
                modified_state_components) = 0;

    virtual std::vector<StateModificationDescriptor> perform_event(
            State* state, double const& random_number) const = 0;
};

} // namespace entities
