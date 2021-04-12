#include "position.h"
#include <iostream>

bool operator< (const Position& p1, const Position& p2) {
  if (p1.row < p2.row) {return 1;}
  if (p1.row == p2.row && p1.col < p2.col) {return 1;}
  return 0;
}

std::ostream& operator<< (std::ostream& os, const Position& p) {
  os << "(" << p.row << "," << p.col << ")";
  return os;
}