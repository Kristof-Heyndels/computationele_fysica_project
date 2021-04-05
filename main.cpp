#include <iostream>
#include "grid.cpp"
#include "model.cpp"

void init_model(Model& m) {
  int centre_row = m.grid().nr_rows()/2;
  int centre_col =  m.grid().nr_cols()/2;
  m.populate_field({centre_row, centre_col});
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

  Grid grid(n, n);
  Model model(grid);
  init_model(model);

  for (int i = 0; i != t; ++i){
    if (model.eligible_fields_count() == 0) {
      std::cout << "Grid full, exiting loop at t = " << i << "\n\n";
      break;
    }
     model.populate_random_field();
  }

  grid_print_cout(model.grid());
  std::cout << "H: " << model.calculate_hairiness() << "\n";
  Model::Position centre_of_mass = model.find_centre_mass();
  std::cout << "CoM: \n x = " 
            << centre_of_mass.row << "\n y = " 
            << centre_of_mass.col << "\n";

  return 0;
}