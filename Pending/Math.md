This top-level header design combines the entire system into a single file (libm_vm_core.h).
It integrates the Unaligned Target-Agnostic Types, the Dual-Compiler FMA/Reciprocal Matrix, the Cody-Waite Splitting Layouts, the Estrin & Goldschmidt Execution Tracks, and the Bitwise Parity Blend Engine. Everything expands inside a single, unified _Generic or function overloading mapping for your bytecode dispatcher loop.
------------------------------
## 🗂️ The Complete Master Header (libm_vm_core.h)

```c
#ifndef LIBM_VM_CORE_H#define LIBM_VM_CORE_H
#include <stdint.h>#include <string.h>
/* ============================================================================
   1. TARGET-AGNOSTIC VECTOR CONFIGURATIONS (STACK-SAFE ALIGNED(1))
   ============================================================================ */#define FOR_EACH_MATH_WIDTH(X) \
    X(scalar, float,  4,  1, 30, 0x80000000U, 0x7FFFFFFFU) \
    X(v128,   float,  16, 4,  30, 0x80000000U, 0x7FFFFFFFU) \
    X(v256,   float,  32, 8,  30, 0x80000000U, 0x7FFFFFFFU) \
    X(v512,   float,  64, 16, 30, 0x80000000U, 0x7FFFFFFFU) \
    X(scalard,double, 8,  1,  62, 0x8000000000000000ULL, 0x7FFFFFFFFFFFFFFFULL) \
    X(v128d,  double, 16, 2,  62, 0x8000000000000000ULL, 0x7FFFFFFFFFFFFFFFULL) \
    X(v256d,  double, 32, 4,  62, 0x8000000000000000ULL, 0x7FFFFFFFFFFFFFFFULL) \
    X(v512d,  double, 64, 8,  62, 0x8000000000000000ULL, 0x7FFFFFFFFFFFFFFFULL)
#define GENERATE_UNALIGNED_TYPES(NAME, V_TYPE, V_WIDTH, V_LANES, S_SHIFT, S_MASK, A_MASK) \
    typedef V_TYPE NAME##_t __attribute__((vector_size(V_WIDTH))) __attribute__((aligned(1))); \
    typedef uint64_t NAME##_u_base; /* Intermediary width safety step */ \
    typedef typeof((V_TYPE == float ? (uint32_t)0 : (uint64_t)0)) NAME##_lane_u; \
    typedef NAME##_lane_u NAME##_ut __attribute__((vector_size(V_WIDTH))) __attribute__((aligned(1)));

FOR_EACH_MATH_WIDTH(GENERATE_UNALIGNED_TYPES)#undef GENERATE_UNALIGNED_TYPES
/* ============================================================================
   2. DUAL-COMPILER COMPATIBILITY & OPTIMIZATION MATRIX
   ============================================================================ */#if defined(__clang__)
    #define VEC_FMA(A, B, C) __builtin_elementwise_fma((A), (B), (C))
    #define VEC_RCP(VEC)     __builtin_elementwise_rcp(VEC)
    #define VM_FP_FLAGS#else
    #define VEC_FMA(A, B, C) ((A) * (B) + (C))
    #define VEC_RCP(VEC) ({                                                    \
        enum { _l = sizeof(typeof(VEC)) / sizeof((VEC)) };                  \
        typeof(VEC) _ones = (typeof(VEC)){ [0 ... _l-1] = 1.0 };               \
        _ones / (VEC);                                                         \
    })
    #define VM_FP_FLAGS __attribute__((optimize("fp-contract=fast", "associative-math")))#endif
// Compiler-safe register broadcasts using compound initializers#define VM_BROADCAST(VEC_TYPE, LANES, VAL) ((VEC_TYPE){ [0 ... (LANES)-1] = (VAL) })
/* ============================================================================
   3. CACHE-LINE MEMORY LAYOUT DESCRIPTORS
   ============================================================================ */typedef struct { float c1, c2, c3, c4; } PolyCtxF4; // 16-byte stridetypedef struct { double c1, c2, c3, c4, c5, c6, c7, c8; } PolyCtxD8 __attribute__((aligned(64))); // 64-byte line
typedef struct { float n0, n1, n2, d1, d2, pad; } PolyCtxRationalF;typedef struct { double n0, n1, n2, d1, d2, d3, d4, pad; } PolyCtxRationalD __attribute__((aligned(64)));
/* ============================================================================
   4. INLINE PREPROCESSOR CODE-INJECTION MACROS
   ============================================================================ */
// A. 4th-Order Balanced Estrin Tree Core (Single Precision Fallbacks)#define INJECT_ESTRIN_F4(VEC_X, CTX, LANES, VEC_OUT)                           \do {                                                                           \
    typeof(VEC_X) _t   = (VEC_X) * (VEC_X);                                    \
    typeof(VEC_X) _t2  = _t * _t;                                              \
    typeof(VEC_X) _c1  = VM_BROADCAST(typeof(VEC_X), LANES, (CTX)->c1);       \
    typeof(VEC_X) _c2  = VM_BROADCAST(typeof(VEC_X), LANES, (CTX)->c2);       \
    typeof(VEC_X) _c3 = VM_BROADCAST(typeof(VEC_X), LANES, (CTX)->c3);        \
    typeof(VEC_X) _c4  = VM_BROADCAST(typeof(VEC_X), LANES, (CTX)->c4);       \
    typeof(VEC_X) _p01 = VEC_FMA(_t, _c2, _c1);                                \
    typeof(VEC_X) _p23 = VEC_FMA(_t, _c4, _c3);                                \
    (VEC_OUT)          = VEC_FMA(_t2, _p23, _p01);                            \
} while(0)
// B. 8th-Order Balanced Estrin Tree Core (Double Precision Target-Strides)#define INJECT_ESTRIN_D8(VEC_X, CTX, LANES, VEC_OUT)                           \do {                                                                           \
    typeof(VEC_X) _t   = (VEC_X) * (VEC_X);                                    \
    typeof(VEC_X) _t2  = _t * _t;                                              \
    typeof(VEC_X) _t4  = _t2 * _t2;                                            \
    typeof(VEC_X) _c1  = VM_BROADCAST(typeof(VEC_X), LANES, (CTX)->c1);       \
    typeof(VEC_X) _c2  = VM_BROADCAST(typeof(VEC_X), LANES, (CTX)->c2);       \
    typeof(VEC_X) _c3  = VM_BROADCAST(typeof(VEC_X), LANES, (CTX)->c3);       \
    typeof(VEC_X) _c4  = VM_BROADCAST(typeof(VEC_X), LANES, (CTX)->c4);       \
    typeof(VEC_X) _c5  = VM_BROADCAST(typeof(VEC_X), LANES, (CTX)->c5);       \
    typeof(VEC_X) _c6  = VM_BROADCAST(typeof(VEC_X), LANES, (CTX)->c6);       \
    typeof(VEC_X) _c7  = VM_BROADCAST(typeof(VEC_X), LANES, (CTX)->c7);       \
    typeof(VEC_X) _c8  = VM_BROADCAST(typeof(VEC_X), LANES, (CTX)->c8);       \
    typeof(VEC_X) _p01 = VEC_FMA(_t, _c2, _c1);                                \
    typeof(VEC_X) _p23 = VEC_FMA(_t, _c4, _c3);                                \
    typeof(VEC_X) _p45 = VEC_FMA(_t, _c6, _c5);                                \
    typeof(VEC_X) _p67 = VEC_FMA(_t, _c8, _c7);                                \
    typeof(VEC_X) _p03 = VEC_FMA(_t2, _p23, _p01);                             \
    typeof(VEC_X) _p47 = VEC_FMA(_t2, _p67, _p45);                             \
    (VEC_OUT)          = VEC_FMA(_t4, _p47, _p03);                            \
} while(0)
// C. Parallelized Division-Free Goldschmidt Rational Pipeline#define INJECT_GOLDSCHMIDT_RAT(VEC_X, CTX, LANES, VEC_OUT)                     \do {                                                                           \
    typeof(VEC_X) _t   = (VEC_X) * (VEC_X);                                    \
    typeof(VEC_X) _n0  = VM_BROADCAST(typeof(VEC_X), LANES, (CTX)->n0);       \
    typeof(VEC_X) _n1  = VM_BROADCAST(typeof(VEC_X), LANES, (CTX)->n1);       \
    typeof(VEC_X) _n2  = VM_BROADCAST(typeof(VEC_X), LANES, (CTX)->n2);       \
    typeof(VEC_X) _d1  = VM_BROADCAST(typeof(VEC_X), LANES, (CTX)->d1);       \
    typeof(VEC_X) _d2  = VM_BROADCAST(typeof(VEC_X), LANES, (CTX)->d2);       \
    typeof(VEC_X) _two = VM_BROADCAST(typeof(VEC_X), LANES, 2.0);              \
    typeof(VEC_X) _num = VEC_FMA(_t, _n2, _n1);                                \
    _num               = VEC_FMA(_t, _num, _n0) * (VEC_X);                     \
    typeof(VEC_X) _den = VEC_FMA(_t, _d2, _d1);                                \
    _den               = VEC_FMA(_t, _den, 1.0);                               \
    typeof(VEC_X) _y0  = VEC_RCP(_den);                                        \
    typeof(VEC_X) _N0  = _num * _y0;                                           \
    typeof(VEC_X) _D0  = _den * _y0;                                           \
    typeof(VEC_X) _F1  = _two - _D0;                                           \
    typeof(VEC_X) _N1  = _N0 * _F1;                                            \
    typeof(VEC_X) _D1  = _D0 * _F1;                                            \
    typeof(VEC_X) _F2  = _two - _D1;                                           \
    (VEC_OUT)          = _N1 * _F2;                                            \
} while(0)
// D. Branchless Bitwise Parity Correction Logic#define INJECT_PARITY_CORRECT(VEC_POLY, VEC_K, SHIFT, MASK, VEC_OUT)           \do {                                                                           \
    enum { _l = sizeof(typeof(VEC_POLY)) / sizeof((VEC_POLY)) };            \
    typedef typeof((VEC_POLY)) _v_f_t;                                         \
    typedef typeof( ((sizeof((VEC_POLY))/_l) == 4 ? (uint32_t)0 : (uint64_t)0) ) _l_u; \
    typedef _l_u _v_u_t __attribute__((vector_size(sizeof(_v_f_t)))) __attribute__((aligned(1))); \
    _v_u_t _k_u  = (_v_u_t)(VEC_K);                                            \
    _v_u_t _s_uq = _k_u & VM_BROADCAST(_v_u_t, _l, 2);                         \
    _v_u_t _s_mk = _s_uq << (SHIFT);                                           \
    _v_u_t _p_bt = (_v_u_t)(VEC_POLY);                                         \
    (VEC_OUT)    = (_v_f_t)(_p_bt ^ _s_mk);                                    \
} while(0)
/* ============================================================================
   5. EXPLICIT CODY-WAITE TRANSCENDENTAL REDUCTION CONSTANTS
   ============================================================================ */// Trigonometric Split for Pi/2 (Double Precision Level splits for full compliance)#define CW_P_HI  1.57079632673412561#define CW_P_MED 6.0771005413155702e-11#define CW_P_LO  2.0222662487959506e-21
// Exponential Split for Ln(2)#define CW_E_HI  0.6931471805599453#define CW_E_LO  2.3190468138462996e-17
#define INJECT_CODY_WAITE_TRIG(VEC_X, VEC_K, LANES, VEC_OUT_R)                 \do {                                                                           \
    typeof(VEC_X) _ch  = VM_BROADCAST(typeof(VEC_X), LANES, CW_P_HI);          \
    typeof(VEC_X) _cm  = VM_BROADCAST(typeof(VEC_X), LANES, CW_P_MED);         \
    typeof(VEC_X) _cl  = VM_BROADCAST(typeof(VEC_X), LANES, CW_P_LO);          \
    typeof(VEC_X) _r1  = VEC_FMA((VEC_K), -_ch, (VEC_X));                     \
    typeof(VEC_X) _r2  = VEC_FMA((VEC_K), -_cm, _r1);                          \
    (VEC_OUT_R)        = VEC_FMA((VEC_K), -_cl, _r2);                          \
} while(0)


/* ============================================================================
6. GLOBAL OPCODES REGISTER & CENTRAL DISPATCH INTERFACE
============================================================================ */
typedef enum {
#define GENERATE_OPCODES(NAME, V_TYPE, V_WIDTH, V_LANES, S_SHIFT, S_MASK, A_MASK) \
    OP_COS_KERNEL_##NAME, OP_COS_SAFE_##NAME, OP_ATAN_RATIONAL_##NAME, \
    FOR_EACH_MATH_WIDTH(GENERATE_OPCODES)
#undef GENERATE_OPCODES
} VMMathOpcode;
// Master VM Execution Entrypoint Function
VM_FP_FLAGS
static inline void VM_Math_Execute(VMMathOpcode op, void* poly_ctx, void* vec_in, void* vec_out) {
    switch (op) {
#define GENERATE_DISPATCH_CASES(NAME, V_TYPE, V_WIDTH, V_LANES, S_SHIFT, S_MASK, A_MASK) \
    case OP_COS_KERNEL_##NAME: { \
        NAME##_t x = (NAME##_t)vec_in; \
        NAME##t tail, res; \
        if (sizeof(V_TYPE) == 4) { \
            INJECT_ESTRIN_F4(x, (const PolyCtxF4*)poly_ctx, V_LANES, tail); \
            res = VEC_FMA(xx, tail, 1.0f); \
        } else { \
            INJECT_ESTRIN_D8(x, (const PolyCtxD8)poly_ctx, V_LANES, tail); \
            res = VEC_FMA(x*x, tail, 1.0; \
        } \
        (NAME##_t)vec_out = res; \
        break; \
    } \
    case OP_COS_SAFE##NAME: { \
        NAME##_t x = (NAME##_t)vec_in; \
        NAME##_t inv_p2 = VM_BROADCAST(NAME##_t, V_LANES, 0.63661977236758134); \
        NAME##_t k_f = (NAME##_t)__builtin_elementwise_round(x * inv_p2); \
        NAME##_t rx; \
        INJECT_CODY_WAITE_TRIG(x, k_f, V_LANES, rx); \
        NAME##_t tail, raw_poly; \
        if (sizeof(V_TYPE) == 4) { \
            INJECT_ESTRIN_F4(rx, (const PolyCtxF4*)poly_ctx, V_LANES, tail); \
            raw_poly = VEC_FMA(rxrx, tail, 1.0f); \
        } else { \
            INJECT_ESTRIN_D8(rx, (const PolyCtxD8)poly_ctx, V_LANES, tail); \
            raw_poly = VEC_FMA(rxrx, tail, 1.0); \ 
        } \
        NAME##t corrected; \
        INJECT_PARITY_CORRECT(raw_poly, k_f, S_SHIFT, S_MASK, corrected); \
        (NAME##_t)vec_out = corrected; \
        break; \
        } \
    case OP_ATAN_RATIONAL##NAME: { \
        NAME##_t x = (NAME##_t)vec_in; \
        NAME##_t res; \
        INJECT_GOLDSCHMIDT_RAT(x, (const PolyCtxRationalF)poly_ctx, V_LANES, res); \
        (NAME##_t)vec_out = res; \
        break; \
        } \
    FOR_EACH_MATH_WIDTH(GENERATE_DISPATCH_CASES)
#undef GENERATE_DISPATCH_CASES
    default:
    __builtin_unreachable();
    }
}
#endif // LIBM_VM_CORE_H


```


### 🔬 Post-Integration Compilation Mechanics

When your interpreter links against `libm_vm_core.h`, the entire mathematical stack becomes an invariant state engine:

1. **Preprocessed Generation:** The header injects independent macro pipelines for every single register width permutation (`scalar` through `v512d`). The compiler parses a centralized `switch` layout containing 24 statically generated code blocks.
2. **Explicit Dependency Chains:** Under compiling targets like `GCC` or `Clang` with flags `-O3 -mavx512f`, the compiler expands the internal macros (`INJECT_ESTRIN_D8` or `INJECT_GOLDSCHMIDT_RAT`) directly inside your interpretation routine blocks. 
3. **Register-Level Execution:** The resulting runtime instructions map to **zero data branches**, **zero memory reallocation cycles**, and **zero external symbol tracking lookups**. The machine code streams values sequentially from your bytecode registers directly into active CPU hardware execution slots, yielding a lean, performant execution path.


### Comparison table of parameter-passing registers for major processor architectures and their respective application binary interfaces (ABIs).


Architecture & ABI  Max RetInt   Register Args (Integer)    RetFloat Floating-Point / Vector Argument Registers
```
x86-64 / System V    6  RAX:RDX  RDI, RSI, RDX, RCX, R8, R9  XMM0:1  XMM0 to XMM7 (First 8)
x86-64 / Windows     4  RAX/RCX  RCX, RDX, R8, R9            XMM0    XMM0, XMM1, XMM2, XMM3
AArch64 / ARM64      8  X0(-X7)  X0-X7 (W0-W7 for 32-bit)    VO(-V7) V0 to V7 / D0 to D7 / S0 to S7
ARM (32-bit)         4  R0:R1    R0, R1, R2, R3              S0/D0   S0 to S15 or D0 to D7 (VFP/NEON base)
RISC-V (32/64-bit)   8  a0:a1    a0-a7 (x10-x11 up to x17)   fa0:1   fa0-fa7
MIPS O32             4  v0:v1    a0-a3 ($4-$7)               $f0:2   $f12, $f14 (or split across GPRs)
MIPS N32 / N64       8  $2:$3    a0-a7 ($4-$11)              $f0:2   $f12-$f19
PowerPC (32-bit)     8  GPR3:4   GPR3-GPR10                 FPR1/VR2 FPR1-FPR13
x86-32 cdecl/stdcall 0  EAX:EDX  Stack                      ST0/XMM0 Stack
x86-32 __fastcall    2  EAX:EDX  ECX, EDX                   ST0/XMM0 Stack
```




LLVM IR has the potential to allow this to be incorporated into cross architecture blobs with just a few changes.


Title: [RFC][Feature Request] Introduce select_arch Block Instruction to Defer Target-Specific Code Selection to Backend Lowering (llc)


Problem Statement

Currently, LLVM IR is fundamentally target-dependent. Frontends (such as Clang or Rustc) must evaluate target-specific macro blocks (#ifdef __x86_64__), structure alignments, and hardware intrinsics during the initial lexing/parsing phases. This couples the resulting LLVM Bitcode (.bc) or IR (.ll) to a specific architecture layout string.
While this model enables heavy middle-end optimizations, it prevents true target-agnostic IR distribution. Efforts to solve this for hot loops—such as GNU ifuncs or function multiversioning—operate at the function symbol or dynamic linker level. They require building multi-architecture "fat binaries," which suffer from severe binary bloat due to the massive duplication of "cold" code paths, headers, and metadata across isolated slices.
There is currently no mechanism in LLVM IR to write target-agnostic, portable modules that can cleanly embed optimized, micro-architectural "escape hatches" (e.g., AVX-512 vs. ARM SVE implementations of a vector strlen) without blinding the middle-end optimizer or breaking the IR type-system invariants.

Proposed Solution: The select_arch Structural Instruction

We propose introducing a new control-flow or block-level instruction wrapper: select_arch.
This instruction allows multiple, architecture-specific basic-block paths to coexist alongside a mandatory, target-agnostic default fallback path. Crucially, the resolution of this block is entirely deferred past the middle-end (opt) to the late-stage backend lowering/legalization phases (llc or libLTO).

Conceptual Syntax Example (strlen Vector Scan)

```
llvm
; The middle-end treats this instruction as a well-behaved operation 
; returning an i64 index based on the unified functional interface.
%zero_index = select_arch {
  
  x86_64_avx512: [
    ; --- TARGET SPECIFIC PATH A ---
    %vec = call <64 x i8> @llvm.x86.avx512.mask.loadu.q(...) 
    %mask = call i64 @llvm.x86.avx512.ptestm.b.512(...)
    %res = call i64 @llvm.ctz.i64(i64 %mask, i1 true) 
    ret i64 %res
  ],
  
  aarch64_sve: [
    ; --- TARGET SPECIFIC PATH B ---
    %pred = call <vscale x 16 x i1> @llvm.aarch64.sve.brkb(...)
    %res_arm = call i64 @llvm.aarch64.sve.cntp(...)
    ret i64 %res_arm
  ],
  
  default: [
    ; --- PORTABLE DEFAULT FALLBACK ---
    %vec_def = load <64 x i8>, <64 x i8>* %ptr, align 64
    %cmp = icmp eq <64 x i8> %vec_def, zeroinitializer
    %scalar_idx = call i64 @fallback_vector_scan(<64 x i1> %cmp)
    ret i64 %scalar_idx
  ]
} [attributes: noopt]
```

How It Works Under the Hood

1. The Semantic Equivalence Assumption (Middle-End opt):
To preserve compiler efficiency, the middle-end optimizer runs passes under the strict assumption that all architecture-specific paths are semantically equivalent to the default block. The optimizer can safely run Dead Code Elimination (DCE), Loop Invariant Code Motion (LICM), and inlining on the code surrounding the select_arch block by evaluating the behavior of the pure, target-agnostic default IR.
2. Interface and Type Invariance:
To prevent type explosion, every block option within the select_arch instruction must adhere to the exact same input signatures and return types. The optimizer treats the choice block as an opaque value generator.
3. The noopt Safeguard Attribute:
An optional noopt or preserve_interface attribute can be applied to the instruction block. This tells the middle-end optimizer to treat the selection boundary as a strict optimization barrier, ensuring aggressive loop transformations or variable merging do not invalidate the structural assumptions of the underlying assembly blocks.
4. Late-Stage Resolution (Backend Lowering in llc):
When llvm-link unifies modules (e.g., combining an application with static libraries), the select_arch block is preserved wholesale. Only when the final target architecture is defined during SelectionDAG/GlobalISel legalization does llc inspect the options:
	• It matches the compilation target (e.g., -march=skylake-avx512).
	• It drops the alternative architecture blocks and the default block entirely from memory.
	• It lowers the target-specific intrinsics and inline assembly constraints directly into native machine code.
	• If the architecture doesn't match any specified hardware flag, it gracefully lowers the pre-optimized default block.

Core Benefits

• Elimination of Fat Binary Bloat: Deploys a single, "Thin-Fat" architecture-neutral bitcode file. The size penalty is constrained strictly to the performance-critical hot loops rather than replicating 100% of an application's cold code paths.
• Unified System Libraries: Enables core system frameworks (like llvm-libc or libc++) to distribute highly portable, single-bitcode modules that automatically resolve to peak native micro-architectural speeds on-device at install time.
• Preserves Toolchain Separation: Bypasses the need for a separate "bitcode linker." The resolution mechanics fit seamlessly into the existing libLTO / backend legalization pipeline, requiring changes only to instruction handling rather than toolchain choreography.

