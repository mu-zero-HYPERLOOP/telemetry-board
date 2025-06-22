#pragma once

#include <cstddef>
#include <optional>
template <typename T, std::size_t SIZE> struct CyclicBuffer {

  CyclicBuffer() : m_buffer(), m_read(0), m_write(0) {}

  inline bool enqueue(const T &value) {
    const size_t w = m_write;
    size_t w_next = w + 1;
    if (w_next == SIZE) {
      w_next = 0U;
    }
    const size_t r = m_read;
    if (w_next == r) {
      return false;
    }
    m_buffer[w] = value;
    m_write = w_next;
    return true;
  }

  inline bool hasSpace() const {
    size_t wnext = m_write + 1;
    if (wnext == SIZE) {
      wnext = 0;
    }
    return m_read != wnext;
  }

  inline std::optional<T> dequeue() {
    size_t r = m_read;
    const size_t w = m_write;
    if (r == w) {
      return std::nullopt;
    }
    T ret = std::move(m_buffer[r++]);
    if (r == SIZE) {
      r = 0U;
    }
    m_read = r;
    return ret;
  }

  inline std::optional<T> peek() {
    size_t r = m_read;
    const size_t w = m_write;
    if (r == w) {
      return std::nullopt;
    }
    T ret = m_buffer[r];
    return ret;
  }

  void clear() {
    std::optional<T> v;
    while ((v = dequeue()).has_value()) { }
    m_read = 0;
    m_write = 0;
  }

private:
  T m_buffer[SIZE];
  std::size_t m_read;
  std::size_t m_write;
};
