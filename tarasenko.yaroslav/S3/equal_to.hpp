#ifndef EQUAL_TO_HPP
#define EQUAL_TO_HPP

namespace tarasenko
{
  template< class T >
  class EqualTo
  {
  public:
    constexpr auto operator()(const T& lhs, const T& rhs) const noexcept(noexcept(lhs == rhs))
      -> decltype(lhs == rhs)
    {
      return lhs == rhs;
    }
  };
}

#endif
