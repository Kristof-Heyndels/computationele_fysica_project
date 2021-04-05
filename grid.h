#ifndef GRID_H
#define GRID_H

#include <valarray>
#include <vector>
#include <unordered_map>
#include <set>
#include <random>
#include <functional>
#include <iostream>

using seed_dist_t = std::uniform_int_distribution<size_t>;

class Grid {
  public: 
    struct Position{
      int row;
      int col;
    };

    struct Carthesian_Position{
      float x;
      float y;
    };

  private:
    int nr_rows_;
    int nr_cols_;
    std::valarray<int> matrix_;
    std::unordered_map<int, int> eligible_fields_;
    std::set<int> occupied_fields_;
    
    Position sitodi(const int& index);
    int ditosi(const int& row, const int& col);
    void update_tile_weight(const int& pos);
    std::vector<int> weighted_positions_list();
    Carthesian_Position transform_position_to_carthesian(const Position& pos);
    

  public:    
    Grid(const int nr_rows, const int nr_cols);
    int nr_rows() const { return nr_rows_; }
    int nr_cols() const { return nr_cols_; }
    int& operator()(const int& row, const int& col) { return matrix_[ditosi(row, col)]; }
    int eligible_fields_count() {return eligible_fields_.size(); }
    int rnd_eligible_field();
    void populate_field(const int& row, const int& col);
    void populate_random_field();
    float calculate_hairiness();
    Carthesian_Position find_centre_mass();
};
#endif