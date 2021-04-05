#ifndef MODEL_H
#define MODEL_H

#include <map>
#include <set>
#include <random>
#include <functional>
#include "grid.h"

using seed_dist_t = std::uniform_int_distribution<size_t>;

class Model{
  public:
    struct Position{
      int row;
      int col;

      Position(int r=0, int c=0) : row(r), col(c) {}
      bool operator==(const Position& a) const { return (row == a.row && col == a.col); }
      bool operator<(const Position& a) const { return (row < a.row && col < a.col); }
    };

  private:
    Grid grid_;
    std::map<Position, int> eligible_fields_;
    std::set<Position> occupied_fields_;

    void update_tile_weight(const Position& pos);
    std::vector<Position> weighted_positions_list();
    Position rnd_eligible_field();

  public:
    Model(Grid& g);
    Grid grid() {return grid_; }
    int eligible_fields_count() {return eligible_fields_.size(); }
    void populate_field(const Position& pos);
    void populate_random_field();
    float calculate_hairiness();
    Position find_centre_mass();
};

#endif