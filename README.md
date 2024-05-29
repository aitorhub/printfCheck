[![GitHub license](https://img.shields.io/badge/C%2B%2B-17-blue)](https://en.cppreference.com/w/cpp/compiler_support#cpp17)

# printfCheck
by Aitor Folgoso <aitor.folgoso@gmail.com>

Header-only C++-17 file to check the printf() and printf-like trace functions at compile-time.
With the use of constexpr and internal macros the printfCheck can find print errors with cause undefined behaviour or crashes, being able to detect and fix them.

_Try it on the [Compiler Explorer](https://godbolt.org/z/8acPeq743)._

## [Features & Examples](example/)

* Too few arguments passed to printf()

  ```cpp
    #include <stdio.h>
    #include "printfCheck.h"
  
    int main()
    {
        printf("main starts\n");
        
        printf("TEST: arg %d arg %d \n", 5);
    }
  ```
  
  When compiling, the result will be:

  ```
    <source>:701:5: note: in expansion of macro 'printf'
    701 |     printf("TEST: arg %d arg %d \n", 5);
        |     ^~~~~~
  <source>:608:43: error: static assertion failed:  Too few arguments. fmt: "TEST: arg %d arg %d \n"
    608 |             static_assert(FmtFieldCounter <= ArgsSize,                              \
        |                           ~~~~~~~~~~~~~~~~^~~~~~~~~~~
  ```

## Compiler compatibility

* Clang/LLVM >= 5
* MSVC++ >= 14.11 / Visual Studio >= 2017
* Xcode >= 10
* GCC >= 9
* MinGW >= 9
* More...
* x86-64 gcc 9.3 -std=gnu++17

## Licensed under the [MIT License](LICENSE)

