#include <iostream>
#include "grid.cpp"

void init_grid(Grid& g) {
  int centre_row = g.nr_rows()/2;
  int centre_col = g.nr_cols()/2;
  g.populate_field(centre_row, centre_col);
}

void grid_print_cout(Grid g){
  std::cout << g << "\n";
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "please enter grid size arguement and number of time steps" << "\n";
    return 2;
  }

  int n = std::stoi(argv[1]);
  int t = std::stoi(argv[2]);

  Grid grid(n,n);
  init_grid(grid);

  for (int i = 0; i != t; ++i){
    if (grid.eligible_fields_count() == 0) 
    {
      std::cout << "Grid full, exiting loop at t = " << i << "\n\n";
      break;
    }
     grid.populate_random_field();
  }

  grid_print_cout(grid);
  std::cout << "H: " << grid.calculate_hairiness() << "\n";
  Grid::Position centre_of_mass = grid.find_centre_mass();
  std::cout << "CoM: \n row=" 
            << centre_of_mass.row << "\n col=" 
            << centre_of_mass.col << "\n";

  return 0;
}