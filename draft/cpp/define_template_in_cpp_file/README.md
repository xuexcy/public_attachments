# 方法 1: 在编译单元 test_temp 中主动实例化需要的类型，这样就可以在需要的编译单元中进行链接
- 方法 a: 在 `test_temp.cpp` 中定义一个临时函数，利用函数对需要的 `typename T` 进行实例化
```cpp
void temporary_function() {
  TestTemp<int> obj1;
  TestTemp<double> obj2;
}
```
- 方法 b: 主动声明需要实例化的 class
```cpp
template class TestTemp<int>;
template class TestTemp<double>;
```

# 方法 2: 在需要的编译单元中 `#include test_temp.cpp` 并进行编译
```cpp
// 这里不需要引入头文件，因为 test_temp.cpp 文件中已经引入了
#include "test_temp.cpp"
```

# 方法 3: 在头文件 `test_temp.h` 的最后一行 `#include "test_temp.cpp"`, 在需要的编译单元中 `#include "test_temp.h"`
`test_temp.h`
```cpp
// test_temp.h 最后一行
#include "test_temp.cpp"
```
`main.cc`
```cpp
#include "test_temp.h"
```

