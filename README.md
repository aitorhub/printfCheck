[![GitHub license](https://img.shields.io/badge/C%2B%2B-17-blue)](https://en.cppreference.com/w/cpp/compiler_support#cpp17)

# printfCheck
by Aitor Folgoso <aitor.folgoso@gmail.com>

Header-only C++-17 file to check the printf() and printf-like trace functions at compile-time.
With the use of constexpr and internal macros the printfCheck can find print errors with cause undefined behaviour or crashes, being able to detect and fix them.

_Try it on the [Compiler Explorer](https://godbolt.org/z/v9x98o51Y)._

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
  g++ -std=c++17 -I include/ main.cpp -o main
  ```

  When compiling, the result will be:
  ```
In file included from main.cpp:3:
main.cpp: In function ‘int main()’:
include/printfCheck.h:612:41: error: static assertion failed: It isn't a char* ! "(" "main.cpp" ":" "8" ")" fmt:"print name %s \n"
  612 |                 static_assert(errorCode != FmtError::ErrorString,                   \
      |                               ~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~
include/printfCheck.h:29:45: note: in expansion of macro ‘PRINTF_CHECK’
   29 | #define printf(...)                     do{ PRINTF_CHECK(__VA_ARGS__); printf(__VA_ARGS__);                    }while(0)
      |                                             ^~~~~~~~~~~~
main.cpp:8:3: note: in expansion of macro ‘printf’
    8 |   printf("print name %s \n", name);  // forget to write name.c_str()
      |   ^~~~~~
...
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
In file included from main.cpp:2:
main.cpp: In function ‘int main()’:
include/printfCheck.h:593:43: error: static assertion failed:  Too few arguments. fmt: "TEST: arg %d arg %d \n"
  593 |             static_assert(FmtFieldCounter <= ArgsSize,                              \
      |                           ~~~~~~~~~~~~~~~~^~~~~~~~~~~
include/printfCheck.h:29:45: note: in expansion of macro ‘PRINTF_CHECK’
   29 | #define printf(...)                     do{ PRINTF_CHECK(__VA_ARGS__); printf(__VA_ARGS__);                    }while(0)
      |                                             ^~~~~~~~~~~~
main.cpp:6:9: note: in expansion of macro ‘printf’
    6 |         printf("TEST: arg %d arg %d \n", 5);
      |         ^~~~~~
...
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
In file included from main.cpp:2:
main.cpp: In constructor ‘main()::WarningStruct_LineIs6_other0::WarningStruct_LineIs6_other0()’:
include/printfCheck.h:155:17: warning: ‘void main()::WarningStruct_LineIs6_other0::warnFunc(detail::false_type)’ is deprecated: Arguments mismatch fmt: "TEST WARN: arg %d \n" [-Wdeprecated-declarations]
  155 |         warnFunc( detail::converter<(condition)>() );             \
      |         ~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ```
The error line is found in the warning class: "WarningStruct_LineIs6_other0" means the warning is in the line 7

This case doesn't create an undefined behavior, for that reason is just a warning, not an error.

### Use trace function different from printf() 
You can obviously use the print-like field checker with other functions differents from printf.
By default, the printfCheck.h checks the functions printf, sprintf(), fprintf() and snprintf().

To add a new trace function to your checks, do the following:
* Let's say your trace function is called `MYPRINT` and you have more arguments than a normal printf()
* then, add the following macro, editing directly the printfCheck.h file, or adding it after the #include
* `#define MYPRINT(index, level, ...)  do{ PRINTF_CHECK(__VA_ARGS__); printf(__VA_ARGS__);   }while(0)`
* In the previous MACRO, you should ignore all arguments before the format string

  ```cpp
    #include <stdio.h>
    #include "printfCheck.h"
  
    #define MYPRINT(index, level, ...)  do{ PRINTF_CHECK(__VA_ARGS__); printf(__VA_ARGS__);   }while(0)
  
    int main()
    {
         int Index    = 100;
         int logLevel = 0xf;

         std::string name = "John";
         MYPRINT(Index, loglevel, "my name is %s \n", 10);
    }
  ```

  Compile output:
  ```
In file included from main.cpp:2:
main.cpp: In function ‘int main()’:
include/printfCheck.h:612:41: error: static assertion failed: It isn't a char* ! "(" "main.cpp" ":" "12" ")" fmt:"my name is %s \n"
  612 |                 static_assert(errorCode != FmtError::ErrorString,                   \
      |                               ~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~
main.cpp:4:45: note: in expansion of macro ‘PRINTF_CHECK’
    4 |     #define MYPRINT(index, level, ...)  do{ PRINTF_CHECK(__VA_ARGS__); printf(__VA_ARGS__);   }while(0)
      |                                             ^~~~~~~~~~~~
main.cpp:12:10: note: in expansion of macro ‘MYPRINT’
   12 |          MYPRINT(Index, loglevel, "my name is %s \n", 10);
      |          ^~~~~~~
      ...
  ```

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
In file included from main.cpp:2:
main.cpp: In function ‘int main()’:
include/printfCheck.h:616:41: error: static assertion failed: In '%.*s' the string arguments failed! "(" "main.cpp" ":" "6" ")" fmt: "Variable size string %.*s \n"
  616 |                 static_assert(errorCode != FmtError::ErrorCharArray,                \
      |                               ~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~
include/printfCheck.h:29:45: note: in expansion of macro ‘PRINTF_CHECK’
   29 | #define printf(...)                     do{ PRINTF_CHECK(__VA_ARGS__); printf(__VA_ARGS__);                    }while(0)
      |                                             ^~~~~~~~~~~~
main.cpp:6:9: note: in expansion of macro ‘printf’
    6 |         printf("Variable size string %.*s \n", "aaa", "array");
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
main.cpp: In constructor ‘main()::WarningStruct_LineIs7_other2::WarningStruct_LineIs7_other2()’:
include/printfCheck.h:155:17: warning: ‘void main()::WarningStruct_LineIs7_other2::warnFunc(detail::false_type)’ is deprecated: fmt fields aren't conformant "WARNING: %hhhd \n" [-Wdeprecated-declarations]
  155 |         warnFunc( detail::converter<(condition)>() );             \
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
g++ -std=c++17 -D ENABLE_CHECK_FLOATING -I include/ main.cpp -o main
```
Other way would be to set the `EnableFloatingCheck` boolean to true in the printfCheck.h header file.

Compile output:
```
In file included from main.cpp:3:
main.cpp: In constructor ‘main()::WarningStruct_LineIs7_other1::WarningStruct_LineIs7_other1()’:
include/printfCheck.h:155:17: warning: ‘void main()::WarningStruct_LineIs7_other1::warnFunc(detail::false_type)’ is deprecated: Float warning '%.*' "(" "main.cpp" ":" "7" ")" fmt: "Testing: %4.2f \n" [-Wdeprecated-declarations]
  155 |         warnFunc( detail::converter<(condition)>() );             \
```
This case is already caught by modern compiler and is disabled in the printCheck.h by default
