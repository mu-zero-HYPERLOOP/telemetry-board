#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <utility>

template <typename T, std::size_t Capacity> class StaticBag {
private:
  union Nullable {
    std::byte _;
    T value;
    Nullable() : _() {}
    ~Nullable() {}
  };
  static_assert(sizeof(Nullable) == sizeof(T));

public:
  StaticBag() : m_size(0) {};
  ~StaticBag() {
    for (std::uint32_t i = 0; i < m_size; ++i) {
      m_values[i].value.~T();
    }
  }

  bool add(T value) {
    if (m_size == Capacity) {
      return false;
    }
    m_values[m_size++].value = std::move(value);
    return true;
  }

  void remove(std::uint32_t idx) {
    assert(idx < m_size);
    m_values[idx].value = std::move(m_values[--m_size].value);
  }

  T &operator[](std::uint32_t i) {
    assert(i < m_size);
    return m_values[i].value;
  }

  std::uint32_t size() const { return m_size; }

private:
  std::uint32_t m_size;
  Nullable m_values[Capacity];
};
