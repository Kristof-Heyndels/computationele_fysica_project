#ifndef POSITION_H
#define POSITION_H

struct Position{
  int row;
  int col;

  bool operator< (const Position& p2);
  bool operator==(const Position& p) const { return row == p.row && col == p.col;}
  std::ostream& operator<< (std::ostream& os);

  //copied from example
  struct Hash_function {
    std::size_t operator() (const Position& p) const {
      std::size_t row_hash = std::hash<int>() (p.row);
      std::size_t col_hash = std::hash<int>() (p.col);
      return row_hash ^ col_hash;
    }
  };
};

#endif