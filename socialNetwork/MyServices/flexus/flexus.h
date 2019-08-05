//
// Created by Arash on 08.11.18.
//

#ifndef BENCHMARKING_FLEXUS_H
#define BENCHMARKING_FLEXUS_H

// #include "magic_iface.h"
#include "magic_inst.h"

//  Modes; VOL, SWL, LAD
#define xxx

// #define BREAKPOINT()    call_magic_sim_break(0, 0, 0)
#define BREAKPOINT()    call_magic_2_64(999, 0, 0)

#define MAGIC(x, y, z, w)  do { \
                            call_magic_4_64(42, x, y, z, w); \
                        } while(0)

// #define FENCE()         __asm__ __volatile__ ("membar #MemIssue\n\t")
// #define WAIT_FENCE()    __asm__ __volatile__ ("membar #StoreStore\n\t")
#define FENCE()         __asm__ __volatile__ ("dmb SY\n\t")
#define WAIT_FENCE()    __asm__ __volatile__ ("dmb ST\n\t")

#define TRANSFORMER(numFields, schemaFields, target)   do { \
                        FENCE(); \
                        MAGIC(numFields, schemaFields, target, 0); \
                        WAIT_FENCE(); \
                        } while(0)

#define TRANSFORMER_DESER(numFields, schemaFields, target, binBuffer)   do { \
                        FENCE(); \
                        MAGIC(numFields, schemaFields, target, binBuffer); \
                        WAIT_FENCE(); \
                        } while(0)

#define NOTIFY(tag) do { \
                        MAGIC(tag, 0, 0, 0); \
                        } while(0)
                        
#endif //BENCHMARKING_FLEXUS_H
