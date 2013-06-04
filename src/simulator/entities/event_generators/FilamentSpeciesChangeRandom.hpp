#pragma once

#include "entities/IEventGenerator.hpp"
#include "entities/IFilament.hpp"
#include "entities/State.hpp"
#include "entities/StateModifications.hpp"
#include "entities/common.hpp"

#include <vector>


namespace entities {
namespace event_generators {


class FilamentSpeciesChangeRandom : public IEventGenerator {
public:
    FilamentSpeciesChangeRandom(species_t const& old_species,
            species_t const& new_species, double const& rate)
        : old_species_(old_species), new_species_(new_species),
        rate_(rate), cached_count_(0) {}

    double rate(State const* state, StateModifications const& modifications);

    StateModifications perform_event(State* state,
            double const& random_number) const;

private:
    void update_cache(State const* state,
            StateModifications const& modifications);
private:
    species_t const old_species_;
    species_t const new_species_;
    double const rate_;

    uint64_t cached_count_;
    std::vector<uint64_t> cached_counts_;
};


} // namespace event_generators
} // namespace entities
