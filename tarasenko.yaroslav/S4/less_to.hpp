#ifndef LESS_TO_HPP
#define LESS_TO_HPP

namespace tarasenko
{
  template< class T >
  class LessTo
  {
  public:
    constexpr auto operator()(const T& lhs, const T& rhs) const noexcept(noexcept(lhs < rhs))
      -> decltype(lhs < rhs)
    {
      return lhs < rhs;
    }
  };
}

#endif
