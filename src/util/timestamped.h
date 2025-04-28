#pragma once

#include "util/timestamp.h"
template <typename T> class Timestamped {
public:
  Timestamped() : m_value{}, m_time{}{
  }

  explicit Timestamped(T value, const Timestamp time)
      : m_value(value), m_time(time) {}

  static Timestamped<T> now(T value) {
    return Timestamped(value, Timestamp::now());
  }

  const T &value() const { return m_value; }

  T &value() { return m_value; }

  Timestamp timestamp() const { return m_time; }

private:
  T m_value;
  Timestamp m_time;
};
