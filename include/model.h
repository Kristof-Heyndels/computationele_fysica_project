#ifndef MODEL_H
#define MODEL_H

#include <unordered_map>
#include <set>
#include <random>
#include <functional>
#include <cmath>
#include "../src/grid.cpp"
#include "../src/position.cpp"

using seed_dist_t = std::uniform_int_distribution<size_t>;

// Governs the used neighbours model. Manages a Grid to display to the user and 
// keeps track of all the eligible fields (fields that have neighbours) and
// all the fields that are already populated.
// Populated fields are represented with with the number 9.
// Eligible fields are represented by their weight (their amount of neighbours).

class Model{    
  private:
    // Buffers for calculated properties
    float g_hair_ {-1};
    Position g_com_ {-1,-1};
    float g_inner_ {-1};
    float g_outer_ {-1};
    float g_density_ {-1};

    Grid grid_;
    std::unordered_map<Position, int, Position::Hash_function> eligible_fields_;
    std::set<Position> occupied_fields_;
    
    void update_field_weight(const Position& pos);
    std::vector<Position> weighted_positions_list();
    Position rnd_eligible_field();
    float pow_2(const float& x) { return x*x; }
    // calculates the distance between the centre of two fields
    float distance(const Position& p1, const Position& p2) { return std::sqrt(pow_2(p1.row - p2.row) + pow_2(p1.col - p2.col)); }
    void invalidate_buffers();

  public:
    Model(const int& dim): grid_(dim,dim) {};
    Grid grid() { return grid_; }
    int eligible_fields_count() { return eligible_fields_.size(); }
    void print_grid(){std::cout << grid_ << "\n";}
    void populate_field(const Position& pos);
    void populate_random_field();
    float hairiness();
    Position centre_mass();
    std::tuple<float, float> inner_outer_radius(const Position& com);
    float inner_radius(const Position& com) { return std::get<0>(inner_outer_radius(centre_mass())); }   
    float outer_radius(const Position& com) { return std::get<1>(inner_outer_radius(centre_mass())); } 
    float density();
};

#endif  