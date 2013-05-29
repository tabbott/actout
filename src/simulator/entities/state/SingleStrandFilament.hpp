#pragma once

#include "entities/IFilament.hpp"
#include "entities/SpeciesMap.hpp"

#include <boost/iterator/iterator_facade.hpp>
#include <inttypes.h>
#include <list>
#include <vector>


namespace entities {
namespace state {

class SingleStrandFilament : public IFilament{
private:
    struct Segment {
        uint64_t number;
        SpeciesMap::species_t species;

        Segment(uint64_t const& number_,
                SpeciesMap::species_t const& species_)
            : number(number_), species(species_) {}
    };

    uint64_t length_;
    std::list<Segment> segments_;
    std::vector<uint64_t> species_counts_;
    std::vector<std::vector<uint64_t> > boundary_counts_;

    class SingleStrandFilamentIterator : public boost::iterator_facade<
                                            SingleStrandFilamentIterator,
                                            SpeciesMap::species_t const,
                                            boost::bidirectional_traversal_tag> {
    public:
        SingleStrandFilamentIterator(
                std::list<SingleStrandFilament::Segment>::const_iterator segment)
            : segment_(segment), index_(0) {}

    private:
        std::list<SingleStrandFilament::Segment>::const_iterator segment_;
        uint64_t index_;

        friend class boost::iterator_core_access;

        void increment() {
            ++index_;
            if (index_ < segment_->number) {
                return;
            } else {
                index_ = 0;
                ++segment_;
            }
        }

        SpeciesMap::species_t const& dereference() const {
            return segment_->species;
        }
        bool equal(SingleStrandFilamentIterator const& other) const {
            return segment_ == other.segment_ && index_ == other.index_;
        }
    };
    void fracture_segment(std::list<Segment>::iterator& segment,
            uint64_t const& index,
            SpeciesMap::species_t const& new_species);

    void fracture_unitary_segment(std::list<Segment>::iterator& segment,
            SpeciesMap::species_t const& new_species);

public:
    void initialize_counts(uint64_t const& num_species);
    SingleStrandFilament(uint64_t const& num_species,
        std::vector<SpeciesMap::species_t>::iterator begin,
        std::vector<SpeciesMap::species_t>::iterator end);

    SingleStrandFilament(uint64_t const& num_species,
            uint64_t const& number,
            SpeciesMap::species_t const& species);

    uint64_t species_count(
            SpeciesMap::species_t const& species) const;
    uint64_t boundary_count(SpeciesMap::species_t const& pointed_species,
            SpeciesMap::species_t const& barbed_species) const;
    uint64_t length() const;

    SpeciesMap::species_t const peek_barbed() const;
    SpeciesMap::species_t const peek_pointed() const;


    void append_barbed(SpeciesMap::species_t const& species);
    void append_pointed(SpeciesMap::species_t const& species);

    SpeciesMap::species_t const pop_barbed();
    SpeciesMap::species_t const pop_pointed();

    void update_subunit(uint64_t instance_number,
            SpeciesMap::species_t const& old_species,
            SpeciesMap::species_t const& new_species);
    void update_boundary(uint64_t instance_number,
            SpeciesMap::species_t const& old_pointed_species,
            SpeciesMap::species_t const& old_barbed_species,
            SpeciesMap::species_t const& new_barbed_species);

    SingleStrandFilamentIterator begin() const;
    SingleStrandFilamentIterator end() const;
};


} // namespace state
} // namespace entities
