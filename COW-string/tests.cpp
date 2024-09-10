#include <gtest/gtest.h>
#include "COW-string.h"

using namespace bicycle;

TEST(COWFarm, CreateDefault) {
  COW<char> StrChar;
  ASSERT_EQ(StrChar.size(), 0);

  COW<int> StrInt;
  ASSERT_EQ(StrInt.size(), 0);
}

TEST(COWFarm, CreateRange) {
  std::basic_string<char> Str{"Hello"};
  COW<char> CowStr(Str.begin(), Str.end());
  ASSERT_EQ(CowStr.size(), 5);
}

TEST(COWFarm, CreateGet) {
  std::basic_string<char> Str{"Hello"};
  COW<char> CowStr(Str.begin(), Str.end());
  
  ASSERT_EQ(CowStr.get(0), 'H');
  ASSERT_EQ(CowStr.get(4), 'o');
}

TEST(COWFarm, Copy) {
  std::basic_string<char> Str{"Hello"};
  COW<char> CowStr(Str.begin(), Str.end());
  COW<char> NewCow(CowStr);

  ASSERT_EQ(CowStr.get(0), 'H');
  ASSERT_EQ(CowStr.get(4), 'o');
  ASSERT_EQ(NewCow.get(0), 'H');
  ASSERT_EQ(NewCow.get(4), 'o');
}

TEST(COWFarm, Set) {
  std::basic_string<char> Str{"Hello"};
  COW<char> CowStr(Str.begin(), Str.end());

  ASSERT_EQ(CowStr.get(0), 'H');
  ASSERT_EQ(CowStr.get(4), 'o');

  CowStr.set('a', 1);
  ASSERT_EQ(CowStr.get(1), 'a');
}

TEST(COWFarm, CopySet) {
  std::basic_string<char> Str{"Hello"};
  COW<char> CowStr(Str.begin(), Str.end());
  COW<char> NewCow(CowStr);

  CowStr.set('a', 1);

  ASSERT_EQ(CowStr.get(0), 'H');
  ASSERT_EQ(CowStr.get(1), 'a');
  ASSERT_EQ(NewCow.get(0), 'H');
  ASSERT_EQ(NewCow.get(4), 'o');
}

TEST(COWFarm, Move) {
  std::basic_string<char> Str{"Hello"};
  COW<char> CowStr(Str.begin(), Str.end());
  COW<char> NewCow(std::move(CowStr));

  ASSERT_EQ(NewCow.get(0), 'H');
  ASSERT_EQ(NewCow.get(4), 'o');
}

TEST(COWFarm, Assign) {
  std::basic_string<char> Str{"Hello"};
  COW<char> CowStr(Str.begin(), Str.end());
  COW<char> NewCow;

  NewCow = CowStr;

  ASSERT_EQ(CowStr.get(0), 'H');
  ASSERT_EQ(CowStr.get(4), 'o');
  ASSERT_EQ(NewCow.get(0), 'H');
  ASSERT_EQ(NewCow.get(4), 'o');
}

TEST(COWFarm, AssignExisting) {
  std::basic_string<char> Str1{"Hello"};
  std::basic_string<char> Str2{"Boba"};
  COW<char> CowStr(Str1.begin(), Str1.end());
  COW<char> NewCow(Str2.begin(), Str2.end());

  NewCow = CowStr;

  ASSERT_EQ(CowStr.get(0), 'H');
  ASSERT_EQ(CowStr.get(4), 'o');
  ASSERT_EQ(NewCow.get(0), 'H');
  ASSERT_EQ(NewCow.get(4), 'o');
}

TEST(COWFarm, AssignMove) {
  std::basic_string<char> Str{"Hello"};
  COW<char> CowStr(Str.begin(), Str.end());
  COW<char> NewCow;

  NewCow = std::move(CowStr);

  ASSERT_EQ(NewCow.get(0), 'H');
  ASSERT_EQ(NewCow.get(4), 'o');
}

TEST(COWFarm, Find) {
  std::basic_string<char> Str{"Hello"};
  std::basic_string<char> StrA{"A"};
  COW<char> H(Str.begin(), Str.begin() + 1);
  COW<char> A(StrA.begin(), StrA.end());
  COW<char> CowStr(Str.begin(), Str.end());

  ASSERT_EQ(CowStr.find(H), 0);
  ASSERT_EQ(CowStr.find(A), CowStr.size());
}

TEST(COWFarm, Tokenizer) {
  std::string Str{"Hello_to__you___"};
  COW<char> CowStr(Str.begin(), Str.end());
  bicycle::COW<char>::Tokenizer Tokenizer(CowStr, '_');
  std::vector<std::string> Tokens;

  std::transform(Tokenizer.begin(), Tokenizer.end(),
                 std::back_inserter(Tokens),
                 [](auto &&Str) {
                   return bicycle::transformCOW<std::string>(Str);
                 });

  ASSERT_EQ(Tokens.size(), 3);
  ASSERT_TRUE(Tokens[0] == "Hello");
  ASSERT_TRUE(Tokens[1] == "to");
  ASSERT_TRUE(Tokens[2] == "you"); 
}
