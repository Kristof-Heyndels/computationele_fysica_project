#include <iostream>
#include "model.cpp"

void init_model(Model& m) {
  int centre_row = m.grid().nr_rows()/2;
  int centre_col =  m.grid().nr_cols()/2;
  m.populate_field({centre_row, centre_col});
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Needs following arguements: grid size (int) and number of time steps (int)" << "\n";
    return 1;
  }

  int n = std::stoi(argv[1]);
  int t = std::stoi(argv[2]);

  Model model(n);
  init_model(model);

  for (int i = 0; i != t; ++i){
    if (model.eligible_fields_count() == 0) {
      std::cout << "Grid full, exiting loop at t = " << i << "\n\n";
      break;
    }
     model.populate_random_field();
  }

  model.print_grid();
  std::cout << "H: " << model.hairiness() << "\n";
  Position centre_of_mass = model.centre_mass();
  std::cout << "CoM: \n" 
            << " row = " << centre_of_mass.row << "\n"
            << " col = " << centre_of_mass.col << "\n";
  std::cout << "Inner Radius: " << model.inner_radius(centre_of_mass) << "\n";
  std::cout << "Outer Radius: " << model.outer_radius(centre_of_mass) << "\n";
  std::cout << "Density: " << model.density() << "\n";

  return 0;
}