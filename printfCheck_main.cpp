
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <stdint.h>
#include <stdio.h>

/** *************************** **/
/** FILE: printfCheck_main.cpp  **/
/** *************************** **/

// ----------------------------------------------------------
// set/unset flags
// ----------------------------------------------------------
constexpr bool EnableWarningTooMuchArguments = true;
constexpr bool DisableFmtFieldValidity       = false;
#ifdef ENABLE_CHECK_FLOATING
constexpr bool EnableFloatingCheck           = true;
#else
constexpr bool EnableFloatingCheck           = false;
#endif

// ----------------------------------------------------------
// Enable your own printf!
// ----------------------------------------------------------

#if 1
#define printf(...)                     do{ PRINTF_CHECK(__VA_ARGS__); printf(__VA_ARGS__);                    }while(0)
#define fprintf(File, ...)              do{ PRINTF_CHECK(__VA_ARGS__); fprintf(File, __VA_ARGS__);             }while(0)
#define sprintf(BUFFER, ...)            do{ PRINTF_CHECK(__VA_ARGS__); sprintf(BUFFER, __VA_ARGS__);           }while(0)
#define snprintf(BUFFER, BUFSIZE, ...)  do{ PRINTF_CHECK(__VA_ARGS__); snprintf(BUFFER, BUFSIZE, __VA_ARGS__); }while(0)
#endif

/** *********************************************************************************
//  this is another example if the special traces used have more arguments. 
// The first argument of PRINTF_CHECK is the format text string, and then comes the arguments
*********************************************************************************** **/
#if 1
#define TRACEPRINT(index, level, ...)  do{ PRINTF_CHECK(__VA_ARGS__); printf(__VA_ARGS__);   }while(0)
#else
#define TRACEPRINT(index, level, ...)
#endif

// ----------------------------------------------------------
// error codes
// ----------------------------------------------------------
enum class FmtError
{
    NoError              =  0,
    ErrorString          = -1,
    ErrorNumber          = -2,
    ErrorCharArray       = -3,
    ErrorWidthVariable   = -4,
    WarningFieldValidity = -5,
    WarningFloatField    = -6,
};

constexpr std::string_view 
FormatFieldList = { "scdiuopfFeEgGaAxXn" };

constexpr std::string_view 
WidthSpecifierList = { "+-0123456789#.*hlzjtL" };

constexpr std::string_view 
FormatFloatingPointList = { "fFeEgGaA" };

// ----------------------------------------------------------
// defines and MACROs
// ----------------------------------------------------------

#define CONSTEVAL constexpr

#define CONCAT_IMPL(A, B) A##B
#define CONCAT(A, B) CONCAT_IMPL(A, B)

#define TO_STR(s) TO_STR_(s)
#define TO_STR_(s) #s

/** *************************************** **/
/**  DECLTYPE_OF_N(...) MACRO               **/
/** *************************************** **/
#define GET_DECLTYPE_OF_N(...)             APPLY_OF_N(decltype, __VA_ARGS__)

/** *************************************** **/
/**  APPLY_OF_N(...) MACRO                  **/
/** *************************************** **/
#define APPLY_OF_0(COMMAND, a)                  
#define APPLY_OF_1(COMMAND, a)             COMMAND(a)         
#define APPLY_OF_2(COMMAND, a, b)          COMMAND(a), APPLY_OF_1(COMMAND, b)
#define APPLY_OF_3(COMMAND, a,...)         COMMAND(a), APPLY_OF_2(COMMAND, __VA_ARGS__)
#define APPLY_OF_4(COMMAND, a,...)         COMMAND(a), APPLY_OF_3(COMMAND, __VA_ARGS__)
#define APPLY_OF_5(COMMAND, a,...)         COMMAND(a), APPLY_OF_4(COMMAND, __VA_ARGS__)
#define APPLY_OF_6(COMMAND, a,...)         COMMAND(a), APPLY_OF_5(COMMAND, __VA_ARGS__)
#define APPLY_OF_7(COMMAND, a,...)         COMMAND(a), APPLY_OF_6(COMMAND, __VA_ARGS__)
#define APPLY_OF_8(COMMAND, a,...)         COMMAND(a), APPLY_OF_7(COMMAND, __VA_ARGS__)
#define APPLY_OF_9(COMMAND, a,...)         COMMAND(a), APPLY_OF_8(COMMAND, __VA_ARGS__)
#define APPLY_OF_10(COMMAND, a,...)        COMMAND(a), APPLY_OF_9(COMMAND, __VA_ARGS__)
#define APPLY_OF_11(COMMAND, a,...)        COMMAND(a), APPLY_OF_10(COMMAND, __VA_ARGS__)
#define APPLY_OF_12(COMMAND, a,...)        COMMAND(a), APPLY_OF_11(COMMAND, __VA_ARGS__)
#define APPLY_OF_13(COMMAND, a,...)        COMMAND(a), APPLY_OF_12(COMMAND, __VA_ARGS__)
#define APPLY_OF_14(COMMAND, a,...)        COMMAND(a), APPLY_OF_13(COMMAND, __VA_ARGS__)
#define APPLY_OF_15(COMMAND, a,...)        COMMAND(a), APPLY_OF_14(COMMAND, __VA_ARGS__)
#define APPLY_OF_16(COMMAND, a,...)        COMMAND(a), APPLY_OF_15(COMMAND, __VA_ARGS__)

#define APPLY_OF_N_PRIV(COMMAND, N, ...)   APPLY_OF_ ## N(COMMAND, __VA_ARGS__)
#define APPLY_OF_N_PRIV_1(COMMAND, N, ...) APPLY_OF_N_PRIV(COMMAND, N, __VA_ARGS__)

#define APPLY_OF_N(COMMAND, ...)           APPLY_OF_N_PRIV_1(COMMAND, GET_ARG_COUNT(__VA_ARGS__), __VA_ARGS__)

/** *************************************** **/
/**  GET_ARG_COUNT() MACRO                  **/
/**  __VA_OPT__(,) needed for 0             **/
/** *************************************** **/
// GET_ARG_COUNT(...) evaluates to the literal number of the passed-in arguments.
#define _GET_ARG_COUNT_PRIVATE(X,                      \
                               Val63,Val62,Val61,Val60,\
                               Val59,Val58,Val57,Val56,Val55,Val54,Val53,Val52,Val51,Val50,\
                               Val49,Val48,Val47,Val46,Val45,Val44,Val43,Val42,Val41,Val40,\
                               Val39,Val38,Val37,Val36,Val35,Val34,Val33,Val32,Val31,Val30,\
                               Val29,Val28,Val27,Val26,Val25,Val24,Val23,Val22,Val21,Val20,\
                               Val19,Val18,Val17,Val16,Val15,Val14,Val13,Val12,Val11,Val10,\
                               Val9,Val8,Val7,Val6,Val5,Val4,Val3,Val2,Val1,N,...) N

#define GET_ARG_COUNT(...) _GET_ARG_COUNT_PRIVATE(0, __VA_ARGS__ __VA_OPT__(,)63,62,61,60,\
                                59,58,57,56,55,54,53,52,51,50,\
                                49,48,47,46,45,44,43,42,41,40,\
                                39,38,37,36,35,34,33,32,31,30,\
                                29,28,27,26,25,24,23,22,21,20,\
                                19,18,17,16,15,14,13,12,11,10,\
                                9,8,7,6,5,4,3,2,1,0)

#define FILE_LINE_LIT()      TO_STR("(" __FILE__ ":" TO_STR(__LINE__) ")")

// ----------------------------------------------------------
// static_warning()
// ----------------------------------------------------------
namespace detail
{
    struct true_type  {};
    struct false_type {};
    template <int test> struct converter    : public true_type  {};
    template <>         struct converter<0> : public false_type {};
}

#define static_warning_impl(condition, WarningMessage, LINE_TXT)  \
struct CONCAT(WarningStruct, LINE_TXT)                            \
{                                                                 \
    void warnFunc(detail::true_type) {};                          \
    void warnFunc(detail::false_type)  __attribute__((deprecated(WarningMessage))) {};    \
                                                                  \
    /** Constructor **/                                           \
    CONCAT(WarningStruct,LINE_TXT)()                              \
    {                                                             \
        warnFunc( detail::converter<(condition)>() );             \
    }                                                             \
}

#define CONCAT_ARG_3(A1,A2,A3)      CONCAT(CONCAT(A1,A2), A3)
#define CONCAT_ARG_4(A1,A2,A3,A4)   CONCAT(CONCAT_ARG_3(A1,A2,A3),A4)

// "_LineIs" + __LINE__ + "_other" + __COUNTER__
#define UNIQUE_LINE_MACRO()         CONCAT_ARG_4(_LineIs,__LINE__, _other,__COUNTER__)

#define static_warning(condition, WarningMessage)   \
               static_warning_impl(condition, WarningMessage, UNIQUE_LINE_MACRO() )

// ----------------------------------------------------------
// functions
// ----------------------------------------------------------

/** *****************************
//  hasMoreThanOnceCharacter()
****************************** **/
CONSTEVAL
uint32_t 
hasMoreThanOnceCharacter(std::string_view fmtField, char c)
{
    if(fmtField.empty() == true) return 0;

    auto pos = fmtField.find(c);
    if(pos == std::string::npos)
        return 0;

    pos = fmtField.find(c, pos + 1);
    if(pos == std::string::npos)
        return 1;

    pos = fmtField.find(c, pos + 1);
    if(pos == std::string::npos)
        return 2;
    
    //  for checking the width specifiers, at most we have the case of
    // 'lld', where the 'l' is repeated twice. If there is more than twice,
    // is an error

    return 3;
}

/** *****************************
//  checkFmtFieldValidity()
//  '-', '+', '#', 0
//  'h', 'l', 'hh', 'll', 'z', 'j', 't', 'L'
****************************** **/
CONSTEVAL
bool 
checkFmtFieldValidity(std::string_view fmtField)
{
    if(fmtField.empty() == true) return true;

    uint32_t counter = 0;

    // More than one '.'
    counter = hasMoreThanOnceCharacter(fmtField, '.');
    if(counter > 1) return false;

    // More than one '*'
    counter = hasMoreThanOnceCharacter(fmtField, '*');
    if(counter > 1) return false;

    // More than one '+'
    counter = hasMoreThanOnceCharacter(fmtField, '+');
    if(counter > 1) return false;

    // More than one '-'
    counter = hasMoreThanOnceCharacter(fmtField, '-');
    if(counter > 1) return false;

    // More than twice 'h'
    counter = hasMoreThanOnceCharacter(fmtField, 'h');
    if(counter > 2) return false;

    // More than twice 'l'
    counter = hasMoreThanOnceCharacter(fmtField, 'l');
    if(counter > 2) return false;

    // 'h' and 'l' at the same time
    bool isHandL = (fmtField.find('l') != std::string::npos) && (fmtField.find('h') != std::string::npos);
    if(isHandL == true) return false;

    // More than one 'z'
    counter = hasMoreThanOnceCharacter(fmtField, 'z');
    if(counter > 1) return false;

    // More than one 'j'
    counter = hasMoreThanOnceCharacter(fmtField, 'j');
    if(counter > 1) return false;

    // More than one 't'
    counter = hasMoreThanOnceCharacter(fmtField, 't');
    if(counter > 1) return false;

    // More than one 'L'
    counter = hasMoreThanOnceCharacter(fmtField, 'L');
    if(counter > 1) return false;

    return true;
}

/** *****************************
//  getFieldIndicesWithoutCheck()
****************************** **/
CONSTEVAL 
auto 
getFieldIndicesWithoutCheck(std::string_view fmtSv, uint32_t StartIndex)
-> std::tuple<bool             /** atEnd?     **/,
              uint32_t         /** NextIndex  **/,
              std::string_view /** FmtField   **/ >
{
    uint32_t Index = StartIndex;

    if(StartIndex >= fmtSv.size()) return { true, Index, {} };

    Index = fmtSv.find('%', Index);
    if(Index == (uint32_t)std::string::npos){
        return { true, Index, {} };
    }
    StartIndex = Index;

    Index++;    // skip '%'
    // -------------------------------------
    if(Index < fmtSv.size() && fmtSv[Index] == '%')
    {
        // avoid '%%d'
        Index = fmtSv.find_first_not_of('%', Index);
        if(Index == (uint32_t)std::string::npos) return { true, Index, {} };

        bool isEven = (Index - StartIndex) % 2 == 0;

        // '%%d'
        if(isEven == true)
        {
            return { false, Index, {} };
        }

        // Adjust StartIndex, to remove extra '%'
        StartIndex = Index - 1;
    }
    // -------------------------------------

    // Index = fmtSv.find_first_not_of("+-0123456789#.*hlzjtL", Index);
    Index = fmtSv.find_first_not_of(WidthSpecifierList, Index);
    if(Index == (uint32_t)std::string::npos){
        return { true, Index, {} };
    }

    if (FormatFieldList.find(fmtSv[Index]) == std::string_view::npos)
    {
        // not found
        return { false, Index, {} };
    }

    auto fieldFmtSv = std::string_view(fmtSv.data() + StartIndex, Index - StartIndex + 1);

    #if 0
    bool isValid = checkFmtFieldValidity(fieldFmtSv);
    if(isValid == false) 
        return { false, Index, {} };  
    #endif

    return { false, Index, fieldFmtSv };
}

/** *****************************
//  getFieldIndices()
****************************** **/
CONSTEVAL 
auto 
getFieldIndices(std::string_view fmtSv, uint32_t StartIndex)
-> std::tuple<bool             /** atEnd?     **/,
              uint32_t         /** NextIndex  **/,
              std::string_view /** FmtField   **/ >
{
    auto fieldPack            = getFieldIndicesWithoutCheck(fmtSv, StartIndex);

    bool atEnd                = std::get<0>(fieldPack);
    uint32_t NextStartIndex   = std::get<1>(fieldPack);
    std::string_view FmtField = std::get<2>(fieldPack);

    bool isValid = checkFmtFieldValidity(FmtField);
    if(isValid == false) 
        return { atEnd, NextStartIndex, {} };
    else
        return { atEnd, NextStartIndex, FmtField };
}

//  helper: isANumber()
template<typename T>
CONSTEVAL 
bool 
isANumber()
{
    using SimpleType = std::decay_t<T>;

    return std::is_arithmetic_v<SimpleType> ||
           std::is_enum_v<SimpleType>;

    #if 0
    return std::is_integral_v<SimpleType> || std::is_floating_point_v<SimpleTyple> || std::is_enum_v<SimpleType>
    #endif
}

template<typename T>
CONSTEVAL bool 
isPointerToNumber()
{
    using SimpleType = std::decay_t<T>;

    if (std::is_pointer_v<SimpleType> == false) 
        return false;

    // probably after this point we could return true

    // using IntegerType = std::remove_pointer_t<SimpleType>;
    using IntegerType = typename std::remove_pointer<SimpleType>::type;

    return isANumber<IntegerType>();
}

//  helper: isFieldANumber()
CONSTEVAL bool 
isFieldANumber(std::string_view sv)
{
    if(sv.empty() == true) return false;

    char c = sv.back();

    if((c != 's') && (FormatFieldList.find(c) != std::string_view::npos) )
        return true;

    return false;
}

//  helper: isFieldAFloatingNumber()
CONSTEVAL bool 
isFieldAFloatingNumber(std::string_view sv)
{
    if(sv.empty() == true) return false;

    char c = sv.back();

    if(FormatFloatingPointList.find(c) != std::string_view::npos)
        return true;

    return false;
}

//  helper: isCharArray()
template<typename T>
CONSTEVAL bool 
isCharArray()
{
    using SimplifiedType = std::decay_t<T>;

    return std::is_same_v<SimplifiedType, const char*> ||
           std::is_same_v<SimplifiedType, char*>;
}

/** ***************************************************************** **/
/**       COMPILE-TIME printf template functions                      **/
/** ***************************************************************** **/

template<uint32_t Start, uint32_t End, class Func>
constexpr 
FmtError 
constexpr_for_valididy (Func func)
{
    if constexpr (Start < End)
    {
        constexpr auto fieldPack            = func (Start);

        constexpr bool     atEnd            = std::get<0>(fieldPack);
        constexpr uint32_t NextStartIndex   = std::get<1>(fieldPack);
        constexpr std::string_view FmtField = std::get<2>(fieldPack);

        if (atEnd == true || FmtField.empty() == true) {
            return FmtError::NoError;
        }

        bool isValid = checkFmtFieldValidity(FmtField);
        if(isValid == false) 
        {
            return FmtError::WarningFieldValidity;
        }

        auto ret = constexpr_for_valididy<NextStartIndex, End>( func );
        if(ret != FmtError::NoError) return ret;
    }

    return FmtError::NoError;
}

template<uint32_t Start, uint32_t End, class F>
constexpr 
uint32_t 
constexpr_for_arg_counter (F func)
{
    uint32_t Counter = 0;
    if constexpr (Start < End)
    {
        constexpr auto fieldPack = func (Start);

        constexpr bool atEnd                = std::get<0>(fieldPack);
        constexpr uint32_t NextStartIndex   = std::get<1>(fieldPack);
        constexpr std::string_view FmtField = std::get<2>(fieldPack);
        
        if (atEnd == true || FmtField.empty() == true) {
            return 0;
        }

        using namespace std::literals;

        // "%.*s", "%*s", and also "%*d"
        bool IsWidthPrecisionAdded = FmtField.find('*') != std::string_view::npos;
        // constexpr auto WidthFormat = FmtField.substr(0, FmtField.size() - 1);
        // bool IsWidthPrecisionAdded = (WidthFormat == "%.*"sv) || (WidthFormat == "%*"sv);

        Counter += (IsWidthPrecisionAdded == true)? 2 : 1;

        Counter += constexpr_for_arg_counter<NextStartIndex, End>( func );
    }

    return Counter;
}

template<uint32_t Start, uint32_t End, 
         uint32_t SelectedIndex,
         typename TupleWithTypes, class F>
constexpr 
FmtError 
constexpr_for_check_Field (F func)
{
    if constexpr (Start < End)
    {
        constexpr auto fieldPack = func (Start);

        constexpr bool atEnd                = std::get<0>(fieldPack);
        constexpr uint32_t NextStartIndex   = std::get<1>(fieldPack);
        constexpr std::string_view FmtField = std::get<2>(fieldPack);

        if (atEnd == true || FmtField.empty() == true) {
            return FmtError::NoError;  // not an error, has just finished
        }
        using namespace std::literals;

        constexpr uint32_t TupleSize = std::tuple_size_v<TupleWithTypes>;
        constexpr bool     IsWidthPrecisionAdded = FmtField.find('*') != std::string_view::npos;
        constexpr uint32_t NextSelectedIndex     = (IsWidthPrecisionAdded == true)? SelectedIndex + 2 : SelectedIndex + 1;

        if constexpr ( SelectedIndex < TupleSize )
        {
            using TupleType = std::tuple_element_t<SelectedIndex, TupleWithTypes>;

            if(IsWidthPrecisionAdded == true)
            {
                if constexpr (SelectedIndex + 1 < TupleSize)
                {
                    using TupleSecondType = std::tuple_element_t<SelectedIndex + 1, TupleWithTypes>;

                    if (FmtField.back() == 's')
                    {       
                        if( !(isANumber<TupleType>() == true &&
                              isCharArray<TupleSecondType>() == true) )
                        {
                            return FmtError::ErrorCharArray;
                        }
                    }
                    else
                    // "%*d"
                    {
                        if( !(isANumber<TupleType>() == true &&
                              isANumber<TupleSecondType>() == true) )
                        {
                            return FmtError::ErrorWidthVariable;
                        }
                    }
                }
            }
            else
            {
                /** *************** **/
                /** Type Comparison **/
                /** *************** **/
                if (FmtField == "%s"sv) 
                {
                    if (isCharArray<TupleType>() == false)       return FmtError::ErrorString;
                }
                else if (FmtField == "%n"sv)
                {
                    if (isPointerToNumber<TupleType>() == false) return FmtError::ErrorNumber;
                }
                else if (isFieldANumber(FmtField) == true)
                {
                    if (isANumber<TupleType>() == false)         return FmtError::ErrorNumber;
                
                    if (EnableFloatingCheck == true && 
                        isFieldAFloatingNumber(FmtField) == true && 
                        std::is_floating_point_v< std::decay_t<TupleType> > == false)
                    {
                                                                 return FmtError::WarningFloatField;
                    }
                }
            }
        }

        auto ret = constexpr_for_check_Field<NextStartIndex, End, NextSelectedIndex, TupleWithTypes>( func );
        if(ret != FmtError::NoError) return ret;
    }

    return FmtError::NoError;
}

/** *************************************** **/
/**             PRINTF_CHECK                **/
/** *************************************** **/
#define  PRINTF_CHECK(fmt_literal, ...)                do{                          \
            constexpr std::string_view fmt = fmt_literal;                           \
                                                                                    \
            constexpr int ArgsSize = GET_ARG_COUNT(__VA_ARGS__);                    \
                                                                                    \
            /** ************************************************ **/                \
            /** A.1) Check Fmt & Args Size                       **/                \
            /** ************************************************ **/                \
            constexpr                                                               \
            int FmtFieldCounter =                                                   \
                constexpr_for_arg_counter<0, fmt.size()>(                           \
                        [&fmt](uint32_t Index)                                      \
                        {                                                           \
                            return getFieldIndices(fmt, Index);                     \
                        }                                                           \
                    );                                                              \
                                                                                    \
            static_assert(FmtFieldCounter <= ArgsSize,                              \
                    " Too few arguments. fmt: " #fmt_literal);                      \
                                                                                    \
            /** Compile-time Args **/                                               \
            using TupleArgsType = std::tuple<GET_DECLTYPE_OF_N(__VA_ARGS__)>;       \
                                                                                    \
            /** ************************************************ **/                \
            /** A.2) Check Argument mismatch                     **/                \
            /** ************************************************ **/                \
            constexpr                                                               \
            auto errorCode =                                                        \
            constexpr_for_check_Field<0, fmt.size(), 0, TupleArgsType>(             \
                    [&fmt](uint32_t Index)                                          \
                    {                                                               \
                        return getFieldIndices(fmt, Index);                         \
                    }                                                               \
                );                                                                  \
            if(errorCode != FmtError::NoError)                                      \
            {                                                                       \
                static_assert(errorCode != FmtError::ErrorString,                   \
                     "It isn't a char* ! " FILE_LINE_LIT() " fmt:" #fmt_literal);   \
                static_assert(errorCode != FmtError::ErrorNumber,                   \
                       "It isn't a number! " FILE_LINE_LIT() " fmt: " #fmt_literal);\
                static_assert(errorCode != FmtError::ErrorCharArray,                \
                       "In '%.*s' the string arguments failed! " FILE_LINE_LIT() " fmt: " #fmt_literal); \
                static_assert(errorCode != FmtError::ErrorWidthVariable,            \
                       "In '%.*' the width arguments failed! " FILE_LINE_LIT() " fmt: " #fmt_literal);   \
            }                                                                       \
                                                                                    \
            /** ************************************************ **/                \
            /** A.3) Check Warnings (Optional)                   **/                \
            /** ************************************************ **/                \
            if constexpr(EnableWarningTooMuchArguments == true)                     \
            {                                                                       \
                static_warning(!(ArgsSize > FmtFieldCounter),                       \
                        "Arguments mismatch fmt: " #fmt_literal);                   \
            } /** !EnableWarningTooMuchArguments **/                                \
                                                                                    \
            /** ************************************************ **/                \
            /** A.4) Check Float Warning (Optional)              **/                \
            /** ************************************************ **/                \
            if constexpr(EnableFloatingCheck == true)                               \
            {                                                                       \
                static_warning(errorCode == FmtError::NoError ||                    \
                              errorCode != FmtError::WarningFloatField,             \
                    "Float warning '%.*' " FILE_LINE_LIT() " fmt: " #fmt_literal);  \
            } /** !EnableFloatingCheck **/                                          \
                                                                                    \
            /** ************************************************ **/                \
            /** A.5) Check Fmt Field Validity (Optional)         **/                \
            /** ************************************************ **/                \
            if constexpr(DisableFmtFieldValidity == false)                          \
            {                                                                       \
                constexpr                                                           \
                auto warningCode =                                                  \
                constexpr_for_valididy<0, fmt.size()>(                              \
                            [&fmt](uint32_t Index)                                  \
                            {                                                       \
                                return getFieldIndicesWithoutCheck(fmt, Index);     \
                            }                                                       \
                        );                                                          \
                                                                                    \
                static_warning(warningCode != FmtError::WarningFieldValidity,       \
                               "fmt fields aren't conformant " #fmt_literal);       \
                                                                                    \
            } /** !DisableFmtFieldValidity **/                                      \
                                                                                    \
            }while(0)

/** *************************************** **/
/**   TESTs                                 **/
/** *************************************** **/
#define FMT_DEBUG_ALL                      1
#define FMT_DEBUG_ERROR_ARGS_MISMATCH      0
#define FMT_DEBUG_WARN_ARGS_MISMATCH       0
#define FMT_DEBUG_ERROR_STRING             0
#define FMT_DEBUG_ERROR_NUMBER             0
#define FMT_DEBUG_ERROR_VARIABLE_STRING    0
#define FMT_DEBUG_VALIDITY_CHECK           0
#define FMT_DEBUG_ERROR_VARIABLE_WIDTH     0
#define FMT_DEBUG_ERROR_FIELD_N            0
#define FMT_DEBUG_WARNING_FLOAT_FIELD      0

#define LOG_DEBUG 0xFF

int main()
{
    printf("main starts\n");

    printf("Starts... \n");

    #if FMT_DEBUG_ERROR_ARGS_MISMATCH == 1
    TRACEPRINT(1, LOG_DEBUG, "TEST: arg %d arg %d \n", 5);
    #endif

    #if FMT_DEBUG_WARN_ARGS_MISMATCH == 1
    TRACEPRINT(1, LOG_DEBUG, "TEST WARN: arg %d \n", 1, 2);
    #endif

    #if FMT_DEBUG_ERROR_STRING == 1
    TRACEPRINT(1, LOG_DEBUG, "string error %s \n", 5); // Error String
    #endif

    #if FMT_DEBUG_ERROR_NUMBER == 1
    std::string dummyStr1;
    TRACEPRINT(1, LOG_DEBUG, "string error %d \n", dummyStr1);      // Error Number
    #endif

    #if FMT_DEBUG_ERROR_VARIABLE_STRING == 1
    TRACEPRINT(1, LOG_DEBUG, "1.) %.*s \n", "aaa", "array");
    #endif

    #if FMT_DEBUG_ERROR_VARIABLE_WIDTH == 1
    TRACEPRINT(1, LOG_DEBUG, "Width trick:   %*d \n", 5, "array but should be integer");
    TRACEPRINT(1, LOG_DEBUG, "Width trick 2: %*d \n", "aaa", 10);
    #endif

    #if FMT_DEBUG_VALIDITY_CHECK == 1
    TRACEPRINT(1, LOG_DEBUG, "WARN: %hhhd \n");
    TRACEPRINT(1, LOG_DEBUG, "WARN: %10.10.10d \n");
    TRACEPRINT(1, LOG_DEBUG, "%hld \n");
    #endif

    #if FMT_DEBUG_ERROR_FIELD_N == 1
    TRACEPRINT(1, LOG_DEBUG, "text counter: %n \n", 10);
    // char* dest;
    // TRACEPRINT(1, LOG_DEBUG, "text counter: %n \n", dest);
    #endif

    #if FMT_DEBUG_WARNING_FLOAT_FIELD == 1
    TRACEPRINT(1, LOG_DEBUG, "TEST float: %4.2f \n", 5);
    #endif

    #if FMT_DEBUG_ALL == 1  // ALL IF
    // -------------------
    // from https://en.cppreference.com/w/c/io/fprintf
    // -------------------
    const char* s = "This is a long Hello";
    TRACEPRINT(1, LOG_DEBUG, "Strings:\n");
    TRACEPRINT(1, LOG_DEBUG, " padding:\n");
    TRACEPRINT(1, LOG_DEBUG, "\t[%10s]\n", s);
    TRACEPRINT(1, LOG_DEBUG, "\t[%-10s]\n", s);
    TRACEPRINT(1, LOG_DEBUG, "\t[%*s]\n", 10, s);
    TRACEPRINT(1, LOG_DEBUG, " truncating:\n");
    TRACEPRINT(1, LOG_DEBUG, "\t%.4s\n", s);
    TRACEPRINT(1, LOG_DEBUG, "\t%.*s\n", 3, s);
    TRACEPRINT(1, LOG_DEBUG, "Characters:\t%c %%\n", 'A');
    TRACEPRINT(1, LOG_DEBUG, "Integers:\n");
    TRACEPRINT(1, LOG_DEBUG, "\tDecimal:\t%i %d %.6i %i %.0i %+i %i\n", 1, 2,   3, 0,   0,  4,-4);
    TRACEPRINT(1, LOG_DEBUG, "\tHexadecimal:\t%x %x %X %#x\n", 5, 10, 10, 6);
    TRACEPRINT(1, LOG_DEBUG, "\tOctal:\t\t%o %#o %#o\n", 10, 10, 4);
    TRACEPRINT(1, LOG_DEBUG, "Floating point:\n");
    TRACEPRINT(1, LOG_DEBUG, "\tRounding:\t %f %.0f %.32f\n", 1.5, 1.5, 1.3);
    TRACEPRINT(1, LOG_DEBUG, "\tPadding:\t%05.2f %.2f %5.2f\n", 1.5, 1.5, 1.5);
    TRACEPRINT(1, LOG_DEBUG, "\tScientific:\t%E %e\n", 1.5, 1.5);
    TRACEPRINT(1, LOG_DEBUG, "\tHexadecimal:\t%a %A\n", 1.5, 1.5);
    TRACEPRINT(1, LOG_DEBUG, "\tSpecial values:\t0/0=%g 1/0=%g\n", 0.0/0.0, 1.0/0.0);

    // -------------------
    // from https://cplusplus.com/reference/cstdio/printf/
    // -------------------
    printf ("Characters: %c %c \n", 'a', 65);
    printf ("Decimals: %d %ld\n", 1977, 650000L);
    printf ("Preceding with blanks: %10d \n", 1977);
    printf ("Preceding with zeros: %010d \n", 1977);
    printf ("Some different radices: %d %x %o %#x %#o \n", 100, 100, 100, 100, 100);
    printf ("floats: %4.2f %+.0e %E \n", 3.1416, 3.1416, 3.1416);
    
    // -------------------
    // '%%d' extra '%' TESTS
    // -------------------
    TRACEPRINT(1, LOG_DEBUG, "Esto no es %%d \n");
    TRACEPRINT(1, LOG_DEBUG, "Esto no es %%%d \n", 777);
    TRACEPRINT(1, LOG_DEBUG, "Esto si %%%s \n", "this");
    TRACEPRINT(1, LOG_DEBUG, "Esto si %%%.*s \n", 2, "this");
    // TRACEPRINT(1, LOG_DEBUG, "Esto no es %%d \n", 5);    // just warning

    // -------------------
    // '%ld' length specifiers CHECKS
    // -------------------
    TRACEPRINT(1, LOG_DEBUG, "%hd %ld %hhd %lld %zd %jd %td %Ld \n", 1, 2, 3, 4, 5, 6, 7, 8);

    // -------------------
    // '%.*s' '%*d' VALID TESTS
    // -------------------
    TRACEPRINT(1, LOG_DEBUG, "CT: complex test %.*s \n", 5, "array");
    char (fixedCharPointer)[5] = "1234";
    TRACEPRINT(1, LOG_DEBUG, "CT: complex test %.*s \n", 5, fixedCharPointer);
    TRACEPRINT(1, LOG_DEBUG, "CT: %.*s %d \n", 3, "array", 111);
    TRACEPRINT(1, LOG_DEBUG, "CT: %.*s %d %d \n", 3, "array", 111, 222);
    TRACEPRINT(1, LOG_DEBUG, "CT: %.*s %d %d %s \n", 3, "array", 111, 222, "third");
    TRACEPRINT(1, LOG_DEBUG, "CT: %.*s %d %d %.*s \n", 3, "array", 111, 222, 3, "array");
    TRACEPRINT(1, LOG_DEBUG, "CT: %.*s %d %d %.*s %d \n", 3, "array", 111, 222, 3, "array", 666);

    printf ("Width trick: %*d \n", 5, 10);
    printf ("Width trick: %.*d \n", 5, 10);
    TRACEPRINT(1, LOG_DEBUG, "CT: %.*d %d %d %.*d %d \n", 5, 100, 1000, 1001, 5, 100, 1003);

    // -------------------
    // '%n' VALID TESTS
    // -------------------
    int charsWritten; 
    printf("geeks for %n geeks  \n", &charsWritten);

    // -------------------
    // VALID TESTS
    // -------------------
    TRACEPRINT(1, LOG_DEBUG, "empty \n");
    TRACEPRINT(1, LOG_DEBUG, "%d \n", 1);
    TRACEPRINT(1, LOG_DEBUG, "%u \n", 2);
    TRACEPRINT(1, LOG_DEBUG, "%i \n", 3);
    TRACEPRINT(1, LOG_DEBUG, "%x \n", 4);
    TRACEPRINT(1, LOG_DEBUG, "%X \n", 5);
    TRACEPRINT(1, LOG_DEBUG, "%f \n", 6.0f);
    TRACEPRINT(1, LOG_DEBUG, "%4.2f \n", 7.5f);
    TRACEPRINT(1, LOG_DEBUG, "%ld \n", 8);
    TRACEPRINT(1, LOG_DEBUG, "%hd \n", 9);
    TRACEPRINT(1, LOG_DEBUG, "%s \n", "array");
    TRACEPRINT(1, LOG_DEBUG, "variable width %.*s \n", 5, "array");

    TRACEPRINT(1, LOG_DEBUG, "%d %d \n",       1, 2);
    TRACEPRINT(1, LOG_DEBUG, "%d %d %d \n",    1, 2, 3);
    TRACEPRINT(1, LOG_DEBUG, "%d %d %d %d \n", 1, 2, 3, 4);

    TRACEPRINT(1, LOG_DEBUG, "%d %u %x %f %4.2f %ld %hd %hhd \n", 1, 2, 3, 4.0f, 5.0f, 6, 7, 8);

    TRACEPRINT(1, LOG_DEBUG, "%d %u %x %f %4.2f %ld %hd %hhd %s %.*s \n", 1, 2, 3, 4.0f, 5.0f, 6, 7, 8, "array", 2, "var_array");

    FILE* file = nullptr;
    if(file) fprintf(file, "%d %u %x \n", 1, 2, 3);
    char buffer[50];
    sprintf(buffer, "%d %u %x \n", 1, 2, 3);
    snprintf(buffer, sizeof(buffer), "%d %u %x \n", 1, 2, 3);

    // -------------------
    // ERROR TEST ...
    // -------------------
    std::string dummyStr;
    // TRACEPRINT(1, LOG_DEBUG, "TEST: arg %d arg %d \n", 5);
    // TRACEPRINT(1, LOG_DEBUG, "ta %s \n", 5);             // Error String
    // TRACEPRINT(1, LOG_DEBUG, "ta %d \n", dummyStr);      // Error Number
    // TRACEPRINT(1, LOG_DEBUG, "ta %s \n", dummyStr);      // Error String
    // TRACEPRINT(1, LOG_DEBUG, "ta gehiau \n", dummyStr);  // Warning, but still ok
    
    // -------------------
    // Complex Error TESTs
    // -------------------
    TRACEPRINT(1, LOG_DEBUG, "complex test %.*s \n", 3, "array");
    // TRACEPRINT(1, LOG_DEBUG, "1.) %.*s \n", "array");
    // TRACEPRINT(1, LOG_DEBUG, "complex test %.*s \n", 5, dummyStr);
    // TRACEPRINT(1, LOG_DEBUG, "complex test %.*s \n", "not a number", "array");

    static_assert(GET_ARG_COUNT()      == 0, "failed for 0 arguments");
    static_assert(GET_ARG_COUNT(1)     == 1, "failed for 1 argument");
    static_assert(GET_ARG_COUNT(1,2)   == 2, "failed for 2 argument");
    static_assert(GET_ARG_COUNT(1,2,3) == 3, "failed for 3 argument");

    #endif // FMT_DEBUG_ALL

    return 0;
}