#include "entities/IEndCondition.hpp"
#include "entities/IMeasurement.hpp"
#include "entities/Simulation.hpp"
#include "entities/State.hpp"

#include <algorithm>
#include <functional>
#include <memory>

using namespace std::placeholders;

namespace entities {

void
Simulation::execute(State* state,
        std::vector<std::unique_ptr<IMeasurement> >& measurements, IRNG* rng) {
    std::vector<StateModificationDescriptor> state_component_modifications;

    // XXX Get initial state component modifications (all things created)

    while (true) {
        auto accumulated_rates = calculate_accumulated_rates(
                state, state_component_modifications);
        if (accumulated_rates.empty()) {
            state->total_event_rate = 0;
        } else {
            state->total_event_rate = accumulated_rates.back();
        }

        // Note that updating the state time here makes all measurements
        // actually measurements of a particular value in the limit as t ->
        // state->time from the left.  This is perfectly OK, but should be
        // explicitly documented.  This is beter than OK, it's more correct.
        // Values are of all measurements are generally discrete and
        // discontinuous.
        state->time += rng->exponential(state->total_event_rate);

        for (auto& m : measurements) {
            m->perform(state);
        }

        if (0 == state->total_event_rate ||
            std::any_of(end_conditions_.cbegin(), end_conditions_.cend(),
                std::bind(&IEndCondition::satisfied, _1, state))) {
            break;
        }

        state_component_modifications = perform_next_event(
                state, accumulated_rates, rng);
    }
}


std::vector<double>
Simulation::calculate_accumulated_rates(State const* state,
            std::vector<StateModificationDescriptor> const&
                modified_state_components) {
    std::vector<double> accumulated_rates;
    accumulated_rates.reserve(event_generators_.size());

    double accumulated_rate = 0;
    for (auto& i : event_generators_) {
         accumulated_rate += i->rate(state, modified_state_components);
         accumulated_rates.push_back(accumulated_rate);
    }

    return accumulated_rates;
}


std::vector<StateModificationDescriptor>
Simulation::perform_next_event(State* state,
        std::vector<double> const& accumulated_rates,
        IRNG* rng) const {
    double r = rng->uniform(0, state->total_event_rate);
    auto i = std::lower_bound(accumulated_rates.cbegin(),
            accumulated_rates.cend(), r);

    if (i == accumulated_rates.cend()) {
        return std::vector<StateModificationDescriptor>();
    } else {
        state->event_count++;
        return event_generators_[std::distance(
                accumulated_rates.cbegin(), i)]->perform_event(state, *i - r);
    }
}

} // namespace entities
