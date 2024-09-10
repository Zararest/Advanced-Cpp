#include "COW-string.h"
#include <iostream>

using namespace bicycle;

int main() {
  std::string Str{"Hello_to__you___"};
  COW<char> CowStr(Str.begin(), Str.end());
  bicycle::COW<char>::Tokenizer Tokenizer(CowStr, '_');
  std::vector<std::string> Tokens;

  std::transform(Tokenizer.begin(), Tokenizer.end(),
                 std::back_inserter(Tokens),
                 [](auto &&Str) {
                   return bicycle::transformCOW<std::string>(Str);
                 });

  std::cout << "|" << Tokenizer.begin()->get(0) << "|\n";

  for (auto &StrToken : Tokens)
    std::cout << StrToken << std::endl;
}