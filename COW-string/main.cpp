#include "COW-string.h"

int main() {
  std::string Str;
  COW<char> Obj(Str.begin(), Str.end());
  COW<char> NewCow(Obj);
}