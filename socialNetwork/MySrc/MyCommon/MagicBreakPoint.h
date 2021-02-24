#ifndef MagicBreakPoint_H
#define MagicBreakPoint_H

#include "son-common/libsonuma/magic_iface.h"

#define MAGIC_GETREQ_BEGIN         			1101
#define MAGIC_GETREQ_END	         			1100

#define MAGIC_PROCESS_BEGIN        		 	1201
#define MAGIC_PROCESS_END          		 	1200

#define MAGIC_SKIP_BEGIN           		 	998
#define MAGIC_SKIP_END             		 	998

#define MAGIC_BREAKPOINT           		 	999

#define MAGIC_HEADER_BEGIN         		 	1301
#define MAGIC_DISPATCH_BEGIN       		 	1401
#define MAGIC_READ_BEGIN           		 	1501
#define MAGIC_WRITE_BEGIN          		 	1601
#define MAGIC_SERVICE_BEGIN        		 	1701

#define MAGIC_HEADER_END           		  1300
#define MAGIC_DISPATCH_END         		  1400
#define MAGIC_READ_END             		  1500
#define MAGIC_WRITE_END            		  1600
#define MAGIC_SERVICE_END          		  1700

#define MAGIC_RESP_BEGIN        				1801
#define MAGIC_RESP_END	       		 		 	1800

#define MAGIC_INPROCESS_BEGIN        		1901
#define MAGIC_INPROCESS_END	       		  1900


#define MAGIC_NESTED_BEGIN        			2001
#define MAGIC_NESTED_END	       		  	2000

#define MAGIC_NHEADER_BEGIN        			2101
#define MAGIC_NHEADER_END	       		  	2100

#define MAGIC_NDISPATCH_BEGIN        		2301
#define MAGIC_NDISPATCH_END	       		  2300

#define MAGIC_NTRANSPORT_BEGIN        	2201
#define MAGIC_NTRANSPORT_END	       		2200

#define MAGIC_EXCEPTION									0x1ABCDEF0

// static inline __attribute__ ((always_inline))
//     uint64_t  call_magic_2_64(uint64_t cmd_id, uint64_t arg1, uint64_t arg2){
// #ifdef __aarch64__
//         uint64_t  ret_value;
//         __asm__ __volatile__ (
//                 "mov x0, %1\n"
//                 "mov x1, %2\n"
//                 "mov x2, %3\n"
//                 "orr x30,x30,x30\n" /* <------ This is where the magic happens */
//                 "mov %0, x0\n"      /* Flexus can write x0 for the return value */
//                 : "=r"(ret_value)     /* output registers*/
//                 : "r"(cmd_id), "r"(arg1), "r"(arg2)      /* input registers*/
//                 : "x0", "x1", "x2", "x30"   /* clobbered registers*/
//                 );
//         return ret_value;
// #else
//         return 0;
// #endif

//     }

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

#define NOTIFY_EXCEPTION(EXIT_CODE)  do { \
                        call_magic_2_64(42, MAGIC_EXCEPTION, EXIT_CODE); \
                    } while (0)     

#define BREAKPOINT() do { \
                        call_magic_2_64(MAGIC_BREAKPOINT, 0, 0); \
                    } while (0)

#define SKIP_BEGIN() do { \
                        call_magic_2_64(MAGIC_SKIP_BEGIN, 1, 0); \
                    } while (0)

#define SKIP_END() do { \
                        call_magic_2_64(MAGIC_SKIP_END, 0, 0); \
                    } while (0)

#define GETREQ_BEGIN() do { \
                        call_magic_2_64(42, MAGIC_GETREQ_BEGIN, 0); \
                    } while (0)

#define HEADER_BEGIN() do { \
                        call_magic_2_64(42, MAGIC_HEADER_BEGIN, 0); \
                    } while (0)

#define DISPATCH_BEGIN() do { \
                        call_magic_2_64(42, MAGIC_DISPATCH_BEGIN, 0); \
                    } while (0)

#define READ_BEGIN() do { \
                        call_magic_2_64(42, MAGIC_READ_BEGIN, 0); \
                    } while (0)

#define WRITE_BEGIN() do { \
                        call_magic_2_64(42, MAGIC_WRITE_BEGIN, 0); \
                    } while (0)                                                            

#define SERVICE_BEGIN() do { \
                        call_magic_2_64(42, MAGIC_SERVICE_BEGIN, 0); \
                    } while (0)   

#define RESP_BEGIN() do { \
                        call_magic_2_64(42, MAGIC_RESP_BEGIN, 0); \
                    } while (0)


#define GETREQ_END() do { \
                        call_magic_2_64(42, MAGIC_GETREQ_END, 0); \
                    } while (0)

#define HEADER_END() do { \
                        call_magic_2_64(42, MAGIC_HEADER_END, 0); \
                    } while (0)

#define DISPATCH_END() do { \
                        call_magic_2_64(42, MAGIC_DISPATCH_END, 0); \
                    } while (0)

#define READ_END() do { \
                        call_magic_2_64(42, MAGIC_READ_END, 0); \
                    } while (0)

#define WRITE_END() do { \
                        call_magic_2_64(42, MAGIC_WRITE_END, 0); \
                    } while (0)          

#define SERVICE_END() do { \
                        call_magic_2_64(42, MAGIC_SERVICE_END, 0); \
                    } while (0)

#define RESP_END() do { \
                        call_magic_2_64(42, MAGIC_RESP_END, 0); \
                    } while (0)										   


#define IN_PROCESS_BEGIN() do { \
                        call_magic_2_64(42, MAGIC_INPROCESS_BEGIN, 0); \
                    } while (0)										   

#define IN_PROCESS_END() do { \
                        call_magic_2_64(42, MAGIC_INPROCESS_END, 0); \
                    } while (0)										   


#define NESTED_BEGIN() do { \
                        call_magic_2_64(42, MAGIC_NESTED_BEGIN, 0); \
                    } while (0)										   
#define NESTED_END() do { \
                        call_magic_2_64(42, MAGIC_NESTED_END, 0); \
                    } while (0)

#define NESTED_DISPATCH_BEGIN() do { \
                        call_magic_2_64(42, MAGIC_NDISPATCH_BEGIN, 0); \
                    } while (0)										   
#define NESTED_DISPATCH_END() do { \
                        call_magic_2_64(42, MAGIC_NDISPATCH_END, 0); \
                    } while (0)																				

#define NESTED_HEADER_BEGIN() do { \
                        call_magic_2_64(42, MAGIC_NHEADER_BEGIN, 0); \
                    } while (0)										   
#define NESTED_HEADER_END() do { \
                        call_magic_2_64(42, MAGIC_NHEADER_END, 0); \
                    } while (0)																				

#define NESTED_TRANSPORT_BEGIN() do { \
                        call_magic_2_64(42, MAGIC_NTRANSPORT_BEGIN, 0); \
                    } while (0)										   
#define NESTED_TRANSPORT_END() do { \
                        call_magic_2_64(42, MAGIC_NTRANSPORT_END, 0); \
                    } while (0)																				

#define PROCESS_END(PROCESS_COUNT) do { \
                        call_magic_2_64(42, MAGIC_PROCESS_END, PROCESS_COUNT); \
                    } while (0)

#define PROCESS_BEGIN(PROCESS_COUNT) do { \
                        call_magic_2_64(42, MAGIC_PROCESS_BEGIN, PROCESS_COUNT); \
                    } while (0)                    


#endif // MagicBreakPoint_H