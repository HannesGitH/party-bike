#if !defined(MATH_HPP)
#define MATH_HPP

#define max(a,b) ((a) > (b) ? (a) : (b))

uint32_t ggT(uint32_t a, uint32_t b);
uint32_t kgV(uint32_t a, uint32_t b);














// #include <stddef.h>
// #include <sys/cdefs.h>
// #include <sys/types.h>


// #define is_same_type(a, b)     __builtin_types_compatible_p(typeof(a), typeof(b))
// #define is_array(arr)          (!is_same_type((arr), &(arr)[0]))
// #define must_be(e, ...)        (                                        \
//         0 * (int)sizeof(                                                \
//                 struct {                                                \
//                         _Static_assert((e)  __VA_OPT__(,)  __VA_ARGS__);\
//                         char ISO_C_forbids_a_struct_with_no_members__;  \
//                 }                                                       \
//         )                                                               \
// )
// #define must_be_array(arr)      must_be(is_array(arr), "Not a `[]` !")

// #define ARRAY_SIZE(arr)         (__arraycount((arr)) + must_be_array(arr))
// #define ARRAY_SSIZE(arr)        ((ptrdiff_t)ARRAY_SIZE(arr))
// #define ARRAY_BYTES(arr)        (sizeof(arr) + must_be_array(arr))
// #define ARRAY_SBYTES(arr)       ((ssize_t)ARRAY_BYTES(arr))

#endif // MATH_HPP