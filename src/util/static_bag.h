#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <memory>
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

  template <typename Tf> bool add(Tf &&value) {
    if (m_size == Capacity) {
      return false;
    }
    new (&m_values[m_size++].value) T(std::forward<Tf>(value));
    return true;
  }

  bool remove(std::uint32_t idx) {
    if (idx < m_size) {
      assert(idx < m_size);
      m_values[idx].value = std::move(m_values[--m_size].value);
      std::destroy_at(&m_values[m_size].value);
      return true;
    } else {
      return false;
    }
  }

  T &operator[](std::uint32_t i) {
    assert(i < m_size);
    return m_values[i].value;
  }

  std::uint32_t size() const { return m_size; }

  void clear() {
    for (size_t i = 0; i < m_size; ++i) {
      std::destroy_at(&m_values[i].value);
    }
    m_size = 0;
  }

private:
  std::uint32_t m_size;
  Nullable m_values[Capacity];
};
