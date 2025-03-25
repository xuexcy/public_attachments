/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyunxue@gmail.com
# Date   : 2025/03/25 01:24:58
# Desc   :
########################################################################
*/
#include <iostream>
#include <utility>

template<typename ... Args>
bool is_all_true(Args... args)
{
  return (std::forward<Args>(args) && ...);
}

/* First instantiated from: insights.cpp:40 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
bool is_all_true<int, bool, __lambda_38_12>(int __args0, bool __args1, __lambda_38_12 __args2)
{
  return static_cast<bool>(std::forward<int>(__args0)) && (std::forward<bool>(__args1) && static_cast<const __lambda_38_12 &&>(std::forward<__lambda_38_12>(__args2)).operator __lambda_38_12::retType_38_12());
}
#endif


template<typename T>
bool is_true(T && value)
{
  if constexpr(std::is_invocable_v<T>) {
    return std::forward<T>(value)();
  } else /* constexpr */ {
    return static_cast<bool>(std::forward<T>(value));
  }

}

/* First instantiated from: insights.cpp:32 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
bool is_true<__lambda_38_12 &>(__lambda_38_12 & value)
{
  if constexpr(true) {
    return std::forward<__lambda_38_12 &>(value).operator()();
  } else /* constexpr */ {
  }

}
#endif


/* First instantiated from: insights.cpp:32 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
bool is_true<bool &>(bool & value)
{
  if constexpr(false) {
  } else /* constexpr */ {
    return static_cast<bool>(std::forward<bool &>(value));
  }

}
#endif


/* First instantiated from: insights.cpp:32 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
bool is_true<int &>(int & value)
{
  if constexpr(false) {
  } else /* constexpr */ {
    return static_cast<bool>(std::forward<int &>(value));
  }

}
#endif


template<typename ... Args>
bool is_all_true_with_call_function(Args &&... args)
{
  return (is_true(std::forward<Args>(args)) && ...);
}

/* First instantiated from: insights.cpp:41 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
bool is_all_true_with_call_function<int &, bool &, __lambda_38_12 &>(int & __args0, bool & __args1, __lambda_38_12 & __args2)
{
  return is_true(std::forward<int &>(__args0)) && (is_true(std::forward<bool &>(__args1)) && is_true(std::forward<__lambda_38_12 &>(__args2)));
}
#endif


int main()
{
  int a = 1;
  bool b = true;

  class __lambda_38_12
  {
    public:
    inline /*constexpr */ bool operator()() const
    {
      return true;
    }

    using retType_38_12 = bool (*)();
    inline constexpr operator retType_38_12 () const noexcept
    {
      return __invoke;
    }

    private:
    static inline /*constexpr */ bool __invoke()
    {
      return __lambda_38_12{}.operator()();
    }

    public:
    // inline /*constexpr */ __lambda_38_12(const __lambda_38_12 &) noexcept = default;

  };

  __lambda_38_12 c = __lambda_38_12{};
  std::cout.operator<<(is_all_true(a, b, __lambda_38_12(c))).operator<<(std::endl);
  std::cout.operator<<(is_all_true_with_call_function(a, b, c)).operator<<(std::endl);
  return 0;
}
