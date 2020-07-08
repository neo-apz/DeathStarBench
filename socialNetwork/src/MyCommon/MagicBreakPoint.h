static inline __attribute__ ((always_inline))
    uint64_t  call_magic_2_64(uint64_t cmd_id, uint64_t arg1, uint64_t arg2){
#ifdef __aarch64__
        uint64_t  ret_value;
        __asm__ __volatile__ (
                "mov x0, %1\n"
                "mov x1, %2\n"
                "mov x2, %3\n"
                "orr x30,x30,x30\n" /* <------ This is where the magic happens */
                "mov %0, x0\n"      /* Flexus can write x0 for the return value */
                : "=r"(ret_value)     /* output registers*/
                : "r"(cmd_id), "r"(arg1), "r"(arg2)      /* input registers*/
                : "x0", "x1", "x2", "x30"   /* clobbered registers*/
                );
        return ret_value;
#else
        return 0;
#endif

    }

// static inline __attribute__ ((always_inline), (optimize("O0")))
static inline __attribute__ ((always_inline))
uint64_t call_magic_4_64(uint64_t cmd_id, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4){
#ifdef __aarch64__
    uint64_t  ret_value;
    __asm__ __volatile__ (
        "mov x0, %1\n"
        "mov x1, %2\n"
        "mov x2, %3\n"
        "mov x3, %4\n"
        "mov x4, %5\n"
        "orr x30,x30,x30\n" /* <------ This is where the magic happens */
        "mov %0, x0\n"      /* Flexus can write x0 for the return value */
        : "=r"(ret_value)     /* output registers*/
        : "r"(cmd_id), "r"(arg1), "r"(arg2), "r"(arg3), "r"(arg4)      /* input registers*/
        : "x0", "x1", "x2", "x3", "x4", "x30"   /* clobbered registers*/
    );
    return ret_value;
#else
    return 0;
#endif
}

#ifdef FLEXUS
#define BREAKPOINT() do { \
                        call_magic_2_64(999, 0, 0); \
                    } while (0)
#endif

#ifdef __aarch64__
#define SET_ITERATION_COUNT(COUNT) do { \
                                      call_magic_2_64(1200, COUNT, 0); \
                                   } while (0)
#endif