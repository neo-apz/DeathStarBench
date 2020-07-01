static inline __attribute__ ((always_inline))
    uint64_t  qflex_magic_break(uint64_t son_function, uint64_t arg1, uint64_t arg2){
#ifdef __aarch64__
        uint64_t  ret_value;
        __asm__ __volatile__ (
                "mov x0, %1\n"
                "mov x1, %2\n"
                "mov x2, %3\n"
                "orr x30,x30,x30\n" /* <------ This is where the magic happens */
                "mov %0, x0\n"      /* Flexus can write x0 for the return value */
                : "=r"(ret_value)     /* output registers*/
                : "r"(son_function), "r"(arg1), "r"(arg2)      /* input registers*/
                : "x0", "x1", "x2", "x30"   /* clobbered registers*/
                );
        return ret_value;
#else
        return 0;
#endif

    }
