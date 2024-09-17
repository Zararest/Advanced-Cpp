#pragma once

#include <limits>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>
#include <ranges>
#include <cassert>

namespace bicycle {

template <typename T>
class COW final { 

  class COWBeaf final {
    const std::basic_string<T> Buf;
    size_t RefCount = 1;

  public:
    using iterator = typename std::basic_string<T>::const_iterator;

    template <typename It>
    COWBeaf(It Beg, It End) : Buf(Beg, End) {}

    size_t getRefCount() const noexcept {
      return RefCount;
    }

    void incRef() {
      if (RefCount == std::numeric_limits<decltype(RefCount)>::max())
        throw std::overflow_error("Too many referencies");
      ++RefCount;
    }

    void removeRef() {
      if (RefCount == 0u)
        throw std::underflow_error("There is no refenrecies");
      --RefCount;
    }

    const auto begin() const {
      return Buf.begin();
    }

    const auto end() const {
      return Buf.end();
    }
  }; 

  COWBeaf *BufPtr = nullptr;
  typename COWBeaf::iterator StrBeg;
  typename COWBeaf::iterator StrEnd;

  static void tryDelete(COWBeaf *BufPtr) noexcept {
    if (!BufPtr || BufPtr->getRefCount() != 0u)
      return;
    
    delete BufPtr;
  }

public:
  using iterator = typename COWBeaf::iterator;

  COW() = default;
  
  template <typename It>
  COW(It Beg, It End) : BufPtr{new COWBeaf(Beg, End)}, 
                        StrBeg{BufPtr->begin()},
                        StrEnd{BufPtr->end()} {};

  COW(const COW &OldCow) : BufPtr{OldCow.BufPtr},
                           StrBeg{OldCow.StrBeg},
                           StrEnd{OldCow.StrEnd} {
    BufPtr->incRef();
  }

  COW(COW &&OldCow) noexcept : StrBeg{OldCow.StrBeg},
                               StrEnd{OldCow.StrEnd} {
    std::swap(BufPtr, OldCow.BufPtr);
  }

  COW &operator =(const COW &OldCow) {
    if (BufPtr)
      BufPtr->removeRef();
    tryDelete(BufPtr);

    BufPtr = OldCow.BufPtr;
    StrBeg = OldCow.StrBeg;
    StrEnd = OldCow.StrEnd;
    BufPtr->incRef();
    return *this;
  }

  COW &operator =(COW &&OldCow) noexcept {
    std::swap(BufPtr, OldCow.BufPtr);
    StrBeg = OldCow.StrBeg;
    StrEnd = OldCow.StrEnd;
    return *this;
  }

  ~COW() {
    if (!BufPtr)
      return;
    BufPtr->removeRef();
    tryDelete(BufPtr);
  }

  T get(size_t Pos) const noexcept {
    assert(Pos <= std::distance(StrBeg, StrEnd));
    return *std::next(StrBeg, Pos);
  }

  void set(T Char, size_t Pos) {
    auto NewStr = std::basic_string<T>(StrBeg, StrEnd);
    *std::next(NewStr.begin(), Pos) = Char;
    auto *NewBuf = new COWBeaf(NewStr.begin(), NewStr.end());

    BufPtr->removeRef();
    tryDelete(BufPtr);

    BufPtr = NewBuf;
    StrBeg = BufPtr->begin();
    StrEnd = BufPtr->end();
  }

  iterator begin() const { return StrBeg; }
  iterator end() const { return StrEnd; }

  size_t size() const {
    return BufPtr ? std::distance(StrBeg, StrEnd) : 0u;
  }

  size_t find(const COW &CowToFind) {
    auto SubStrBeg =
      std::search(StrBeg, StrEnd,
                  CowToFind.StrBeg, CowToFind.StrEnd);
    return std::distance(StrBeg, SubStrBeg);
  }

  class Tokenizer final {
    std::vector<COW> Tokens;

  public:
    using iterator = typename std::vector<COW>::iterator;

    Tokenizer(const COW<T> &Cow, T Separator) {
      auto SubBeg = Cow.begin();
      auto End = Cow.end();

      while (SubBeg != End) {
        auto SubStrEnd = std::find(SubBeg, End, Separator);
        if (SubBeg != SubStrEnd)
          Tokens.emplace_back(SubBeg, SubStrEnd);
        
        SubBeg = std::find_if(SubStrEnd, End, 
          [Separator](auto &&Char) {
            return Char != Separator;
          });
      }
    }
    
    Tokenizer(const Tokenizer &) = delete;
    Tokenizer(Tokenizer &&) = default;
    Tokenizer &operator=(const Tokenizer &) = delete;
    Tokenizer &operator=(Tokenizer &&) = default;

    iterator begin() { return Tokens.begin(); }
    iterator end() { return Tokens.end(); }
    size_t size() const noexcept { return Tokens.size(); }
  };
};

template <typename To, typename From>
To transformCOW(const COW<From> &Str);

template <>
std::string transformCOW(const COW<char> &Str) {
  auto Res = std::string{};
  for (auto Pos : std::ranges::iota_view{0u, Str.size()})
    Res += Str.get(Pos);
  return Res;
}

} // namespace bicycle