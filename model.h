#ifndef MODEL_H
#define MODEL_H

#include <unordered_map>
#include <set>
#include <random>
#include <functional>
#include <cmath>
#include "grid.cpp"
#include "position.cpp"

using seed_dist_t = std::uniform_int_distribution<size_t>;

class Model{    
  private:
    Grid grid_;
    // TODO would like to make these unordered_map<Position, int> and set<Position>
    std::unordered_map<int, int> eligible_fields_;
    std::set<Position> occupied_fields_;

    // double index to single index
    int ditosi(const Position& pos);
    // single index to double index
    Position sitodi(const int& i);
    
    void update_tile_weight(const Position& pos);
    std::vector<Position> weighted_positions_list();
    Position rnd_eligible_field();
    float pow_2(float x) {return x*x;}
    // calculates the distance between the centre of two fields
    float distance(const Position& p1, const Position& p2) {return std::sqrt(pow_2(p1.row - p2.row) + pow_2(p1.col - p2.col));}

  public:
    Model(int dim = 3) : grid_(dim,dim) {}
    Grid grid() {return grid_;}
    int eligible_fields_count() {return eligible_fields_.size();}
    void populate_field(const Position& pos);
    void populate_random_field();
    void grid_print_cout( ){std::cout << grid_ << "\n";}
    float hairiness();
    Position centre_mass();
    float inner_radius(const Position& com);
    float outer_radius(const Position& com);
    float density();
};

#endif  