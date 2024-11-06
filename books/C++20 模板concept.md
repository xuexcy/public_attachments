- 类比:
	- concept：类型名
	- requires：函数名
	- 语法: 
		- 类型名 变量名 = 函数名(参数类型 参数名) { 函数实现 }
		- `int is_odd = [] (int a) { return a & 1; }`
```cpp
#include <concepts>
// 模板限制
template <class T>
concept is_lockable = requires(Lock&& lock) {
	lock.lock();
	lock.unlock();
	{ lock.try_lock() } -> std::convertible_to<bool>;
}

// 使用模板限制
template <class T>
requires is_lockable<T>;
void f1(T& t) {
	std::condition_variable_any cv;
	cv.wait(t);
}

template <class T>
void f1(T& t) requires is_lockable<T> {
	std::condition_variable_any cv;
	cv.wait(t);
}

```