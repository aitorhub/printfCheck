[![GitHub license](https://img.shields.io/badge/C%2B%2B-17-blue)](https://en.cppreference.com/w/cpp/compiler_support#cpp17)

# printfCheck
by Aitor Folgoso <aitor.folgoso@gmail.com>

Header-only C++-17 file to check the printf() and printf-like trace functions at compile-time.
With the use of constexpr and internal macros the printfCheck can find print errors with cause undefined behaviour or crashes, being able to detect and fix them.

_Try it on the [Compiler Explorer](https://godbolt.org/z/8acPeq743)._

## [Features & Examples](example/)

### printf() string type not found

  ```cpp
    #include <stdio.h>
    #include "printfCheck.h"
  
    int main()
    {
        printf("print name %s \n", 5); 
    }
  ```
  
  When compiling, the result will be:

  ```
      <source>: In function 'int main()':
      <source>:627:41: error: static assertion failed: Is a std::string! "(" "<source>" ":" "699" ")" fmt:"print name %s \n"
        627 |                 static_assert(errorCode != FmtError::ErrorString,                   \
            |                               ~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~
      <source>:24:45: note: in expansion of macro 'PRINTF_CHECK'
         24 | #define printf(...)                     do{ PRINTF_CHECK(__VA_ARGS__); printf(__VA_ARGS__);                    }while(0)
            |                                             ^~~~~~~~~~~~
      <source>:699:5: note: in expansion of macro 'printf'
        699 |     printf("print name %s \n", 5);
  ```

### Too few arguments passed to printf()

  ```cpp
    #include <stdio.h>
    #include "printfCheck.h"
  
    int main()
    {        
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

### Warning: Too much arguments passed
  ```cpp
    #include <stdio.h>
    #include "printfCheck.h"
  
    int main()
    {        
         printf("TEST WARN: arg %d \n", 1, 2);
    }
  ```

  Output:
  ```
    <source>: In constructor 'main()::WarningStruct699_0::WarningStruct699_0()':
    <source>:173:47: warning: 'void main()::WarningStruct699_0::warnFunc(detail::false_type)' is deprecated: Arguments mismatch fmt: "TEST WARN: arg %d \n" [-Wdeprecated-declarations]
      173 |         warnFunc( detail::converter<(cond)>() );                \
          |                                               ^
    ...
    <source>:699:5: note: in expansion of macro 'printf'
      699 |     printf("TEST WARN: arg %d \n", 1, 2);
          |     ^~~~~~
  ```
  The error line is found in the warning class: "WarningStruct699_0" means the warning is in the line 699

## Compiler compatibility

* Clang/LLVM >= 5
* MSVC++ >= 14.11 / Visual Studio >= 2017
* Xcode >= 10
* GCC >= 9
* MinGW >= 9
* More...
* x86-64 gcc 9.3 -std=gnu++17

## Licensed under the [MIT License](LICENSE)

