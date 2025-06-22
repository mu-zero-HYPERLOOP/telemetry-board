#pragma once

#include <type_traits>

namespace telemetry_board {

enum class LinkStateBits : unsigned int {
  LINK_DOWN = 0,
  LINK_UP = 1u << 1,
  INTERFACE_UP = 1u << 2,
  READY = LINK_UP | INTERFACE_UP,
  ERROR_INET_DOWN = 1u << 3,
  ERROR_BEGIN_ETHERNET = 1u << 4,
  ERROR_LINK_TIMEOUT = 1u << 5,
  ERROR = ERROR_INET_DOWN | ERROR_BEGIN_ETHERNET | ERROR_LINK_TIMEOUT,
};

//
// 2. Enable bitwise ops for LinkState (enum-only version)
//
constexpr LinkStateBits operator|(LinkStateBits lhs, LinkStateBits rhs) {
  using T = std::underlying_type_t<LinkStateBits>;
  return static_cast<LinkStateBits>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

constexpr LinkStateBits operator&(LinkStateBits lhs, LinkStateBits rhs) {
  using T = std::underlying_type_t<LinkStateBits>;
  return static_cast<LinkStateBits>(static_cast<T>(lhs) & static_cast<T>(rhs));
}

constexpr LinkStateBits operator~(LinkStateBits v) {
  using T = std::underlying_type_t<LinkStateBits>;
  return static_cast<LinkStateBits>(~static_cast<T>(v));
}

constexpr LinkStateBits &operator|=(LinkStateBits &lhs, LinkStateBits rhs) {
  lhs = lhs | rhs;
  return lhs;
}

constexpr LinkStateBits &operator&=(LinkStateBits &lhs, LinkStateBits rhs) {
  lhs = lhs & rhs;
  return lhs;
}

//
// 3. Define the wrapper: LinkStateFlags
//
struct LinkState {
  using mask_type = std::underlying_type_t<LinkStateBits>;

  constexpr LinkState() = default;
  constexpr LinkState(LinkStateBits bit) : mask_(static_cast<mask_type>(bit)) {}
  constexpr explicit LinkState(mask_type rawBits) : mask_(rawBits) {}

  // Conversions
  constexpr explicit operator bool() const { return mask_ != 0; }
  constexpr explicit operator mask_type() const { return mask_; }
  constexpr operator LinkStateBits() const {
    return static_cast<LinkStateBits>(mask_);
  }
  constexpr mask_type raw() const { return mask_; }

  // Tests
  [[nodiscard]] constexpr bool test(LinkStateBits bit) const {
    return (mask_ & static_cast<mask_type>(bit)) != 0;
  }

  [[nodiscard]] constexpr bool any() const { return mask_ != 0; }
  [[nodiscard]] constexpr bool none() const { return mask_ == 0; }

  // Mutating ops
  constexpr LinkState &operator|=(LinkStateBits bit) {
    mask_ |= static_cast<mask_type>(bit);
    return *this;
  }

  constexpr LinkState &operator|=(LinkState rhs) {
    mask_ |= rhs.mask_;
    return *this;
  }

  constexpr LinkState &operator&=(LinkStateBits bit) {
    mask_ &= static_cast<mask_type>(bit);
    return *this;
  }

  constexpr LinkState &operator&=(LinkState rhs) {
    mask_ &= rhs.mask_;
    return *this;
  }

private:
  mask_type mask_{0};
};

constexpr LinkState operator|(LinkState lhs, LinkStateBits rhs) {
  lhs |= rhs;
  return lhs;
}

constexpr LinkState operator|(LinkStateBits lhs, LinkState rhs) {
  rhs |= lhs;
  return rhs;
}

constexpr LinkState operator|(LinkState lhs, LinkState rhs) {
  lhs |= rhs;
  return lhs;
}

constexpr LinkState operator&(LinkState lhs, LinkStateBits rhs) {
  lhs &= rhs;
  return lhs;
}

constexpr LinkState operator&(LinkStateBits lhs, LinkState rhs) {
  rhs &= lhs;
  return rhs;
}

constexpr LinkState operator&(LinkState lhs, LinkState rhs) {
  lhs &= rhs;
  return lhs;
}

constexpr LinkState operator~(LinkState f) { return LinkState{~f.raw()}; }

} // namespace telemetry_board
