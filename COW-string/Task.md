# Task
Write a Copy On Write implementation of a string.
It shoukd have:
- big 5
- tokenization
- substring search

## Tokenization
Tokenizaer is a non-owning class that has `begin()` and `end()` methods for an iteration.

It should be a referense to a string that is being passed to in on construction.

## Substring search
Substring method should return a COW handler to a string without a copy.

COW handler should have a pointer to a buffer and a combination of a size and a position(or iterators).
This allows to return substrings without copy.