[![GitHub license](https://img.shields.io/badge/C%2B%2B-17-blue)](https://en.cppreference.com/w/cpp/compiler_support#cpp17)

# printfCheck
by Aitor Folgoso <aitor.folgoso@gmail.com>

Header-only C++-17 file to check the printf() and printf-like trace functions at compile-time.
With the use of constexpr and internal macros the printfCheck can find print errors with cause undefined behaviour or crashes, being able to detect and fix them.

_Try it on the [Compiler Explorer]([https://godbolt.org/z/8acPeq743](https://godbolt.org/z/v9x98o51Y))._

## Compiler compatibility
The code must be compiled with C++17 or higher, where `constexpr` keyword and `std::string_view` class are available 

## Use examples

### printf(): '%s' printf field with wrong argument

  ```cpp
#include <stdio.h>
#include <string>
#include "include/printfCheck.h"

int main()
{
    const std::string name = "John";
    printf("print name %s \n", name);  // forget to write name.c_str() 
}
  ```

  To compile it:
  ```
  g++ -std=gnu++17 main.cpp -o main
  ```

  When compiling, the result will be:
  ```
In file included from main.cpp:3:
main.cpp: In function ‘int main()’:
include/printfCheck.h:628:41: error: static assertion failed: It isn't a char* ! "(" "main.cpp" ":" "8" ")" fmt:"print name %s \n"
  628 |                 static_assert(errorCode != FmtError::ErrorString,                   \
      |                               ~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~
include/printfCheck.h:25:45: note: in expansion of macro ‘PRINTF_CHECK’
   25 | #define printf(...)                     do{ PRINTF_CHECK(__VA_ARGS__); printf(__VA_ARGS__);                    }while(0)
      |                                             ^~~~~~~~~~~~
main.cpp:8:5: note: in expansion of macro ‘printf’
    8 |     printf("print name %s \n", name);
      |     ^~~~~~
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
  
  Compile output:
  ```
In file included from main.cpp:3:
main.cpp: In function ‘int main()’:
include/printfCheck.h:609:43: error: static assertion failed:  Too few arguments. fmt: "TEST: arg %d arg %d \n"
  609 |             static_assert(FmtFieldCounter <= ArgsSize,                              \
      |                           ~~~~~~~~~~~~~~~~^~~~~~~~~~~
include/printfCheck.h:25:45: note: in expansion of macro ‘PRINTF_CHECK’
   25 | #define printf(...)                     do{ PRINTF_CHECK(__VA_ARGS__); printf(__VA_ARGS__);                    }while(0)
      |                                             ^~~~~~~~~~~~
main.cpp:7:5: note: in expansion of macro ‘printf’
    7 |     printf("TEST: arg %d arg %d \n", 5);
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

  Compile output:
  ```
    In file included from main.cpp:3:
    main.cpp: In constructor ‘main()::WarningStruct7_0::WarningStruct7_0()’:
    include/printfCheck.h:174:17: warning: ‘void main()::WarningStruct7_0::warnFunc(detail::false_type)’ is deprecated: Arguments mismatch fmt: "TEST WARN: arg %d \n" [-Wdeprecated-declarations]
      174 |         warnFunc( detail::converter<(cond)>() );                \
          |         ~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ```
The error line is found in the warning class: "WarningStruct7_0" means the warning is in the line 7

This case doesn't create an undefined behavior, for that reason is just a warning, not an error.

### printf() string with precision field but wrong arguments

  ```cpp
    #include <stdio.h>
    #include "printfCheck.h"
  
    int main()
    {
        printf("Variable size string %.*s \n", "aaa", "array");
    }
  ```
  
  Compile output:
  ```
  In file included from main.cpp:3:
  main.cpp: In function ‘int main()’:
  include/printfCheck.h:632:41: error: static assertion failed: In '%.*s' the string arguments failed! "(" "main.cpp" ":" "7" ")" fmt: "Variable size string %.*s \n"
    632 |                 static_assert(errorCode != FmtError::ErrorCharArray,                \
        |                               ~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~
  include/printfCheck.h:25:45: note: in expansion of macro ‘PRINTF_CHECK’
     25 | #define printf(...)                     do{ PRINTF_CHECK(__VA_ARGS__); printf(__VA_ARGS__);                    }while(0)
        |                                             ^~~~~~~~~~~~
  main.cpp:7:5: note: in expansion of macro ‘printf’
      7 |     printf("Variable size string %.*s \n", "aaa", "array");
  ```

### Incompatible precision field combinations given

  ```cpp
#include <stdio.h>
#include <string>
#include "include/printfCheck.h"

int main()
{
    printf("WARNING: %hhhd \n");
    // also produces warnings due to wrong precision fields
    // printf("WARN: %10.10.10d \n");
    // printf("%hld \n");
}
  ```
  
  Compile output:
  ```
  In file included from main.cpp:3:
  main.cpp: In constructor ‘main()::WarningStruct7_2::WarningStruct7_2()’:
  include/printfCheck.h:174:17: warning: ‘void main()::WarningStruct7_2::warnFunc(detail::false_type)’ is deprecated: fmt fields aren't conformant "WARNING: %hhhd \n" [-Wdeprecated-declarations]
    174 |         warnFunc( detail::converter<(cond)>() );                \
        |         ~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ```

### number field given, but string or other argument given

  ```cpp
#include <stdio.h>
#include <string>
#include "include/printfCheck.h"

int main()
{
    std::string dummyStr1;
    printf("this is a number %d \n", dummyStr1);
}
  ```
  
Compile output:
  ```
In file included from main.cpp:3:
main.cpp: In function ‘int main()’:
include/printfCheck.h:630:41: error: static assertion failed: It isn't a number! "(" "main.cpp" ":" "8" ")" fmt: "this is a number %d \n"
  630 |                 static_assert(errorCode != FmtError::ErrorNumber,                   \
      |                               ~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~
include/printfCheck.h:25:45: note: in expansion of macro ‘PRINTF_CHECK’
   25 | #define printf(...)                     do{ PRINTF_CHECK(__VA_ARGS__); printf(__VA_ARGS__);                    }while(0)
      |                                             ^~~~~~~~~~~~
main.cpp:8:5: note: in expansion of macro ‘printf’
    8 |     printf("this is a number %d \n", dummyStr1);
  ```

### Other error checks that are also caught

  ```cpp
#include <stdio.h>
#include <string>
#include "include/printfCheck.h"

int main()
{
    printf("Width precision with number: %*d \n", 5, "array but should be integer");
    printf("text counter: %n \n", 10);
}
  ```

### Check '%f' floating point field

  ```cpp
#include <stdio.h>
#include <string>
#include "include/printfCheck.h"

int main()
{
    printf("Testing: %4.2f \n", 5);
}
  ```

Compile it with the floating check option:
```
g++ -std=gnu++17 -D ENABLE_CHECK_FLOATING main.cpp -o main
```
Other way would be to set the `EnableFloatingCheck` boolean to true in the printfCheck.h header file.

Compile output:
```
In file included from main.cpp:3:
main.cpp: In constructor ‘main()::WarningStruct7_1::WarningStruct7_1()’:
include/printfCheck.h:179:17: warning: ‘void main()::WarningStruct7_1::warnFunc(detail::false_type)’ is deprecated: Float warning '%.*' "(" "main.cpp" ":" "7" ")" fmt: "Testing: %4.2f \n" [-Wdeprecated-declarations]
```
This case is already caught by modern compiler and is disabled in the printCheck.h by default

## Licensed under the [MIT License](LICENSE)

