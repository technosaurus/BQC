#include <stddef.h>
#include <stdint.h>

typedef uint8_t v64qi __attribute__ ((vector_size (64)));
// Create an explicit unaligned vector pointer type for s2
typedef v64qi v64qi_unaligned __attribute__((aligned(1)));

// Horizontal reduction helper (becomes VPTEST)
static inline int vector_any_non_zero_512(v64qi vec) {
    for (int i = 0; i < 64; ++i) {
        if (vec[i]) return 1;
    }
    return 0;
}

// Movemask helper (becomes VPMOVB2M on AVX-512)
static inline uint64_t get_movemask_portable_512(v64qi vec) {
    uint64_t mask = 0;
    for (int i = 0; i < 64; ++i) {
        mask |= ((uint64_t)(vec[i] >> 7) << i);
    }
    return mask;
}

void custom_base64_encode_512(const uint8_t *src, size_t src_len, char *dst) {
    // Define the full Base64 lookup alphabet as a static 64-byte vector literal
    const v64qi alphabet = {
        'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P',
        'Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f',
        'g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v',
        'w','x','y','z','0','1','2','3','4','5','6','7','8','9','+','/'
    };

    size_t i = 0;
    size_t j = 0;

    // Process blocks of 48 bytes -> expands cleanly into 64 characters
    while (i + 48 <= src_len) {
        v64qi indices;

        // Perform parallel bit-shuffling to unpack 3 raw bytes into four 6-bit chunks.
        // Both GCC and Clang unroll and auto-vectorize this loop using native SIMD shifts.
        for (int k = 0; k < 16; ++k) {
            uint32_t b0 = src[i + k * 3 + 0];
            uint32_t b1 = src[i + k * 3 + 1];
            uint32_t b2 = src[i + k * 3 + 2];

            // Re-pack into a 24-bit window
            uint32_t triple = (b0 << 16) | (b1 << 8) | b2;

            // Extract 6 bits per lane
            indices[k * 4 + 0] = (triple >> 18) & 0x3F;
            indices[k * 4 + 1] = (triple >> 12) & 0x3F;
            indices[k * 4 + 2] = (triple >> 6)  & 0x3F;
            indices[k * 4 + 3] =  triple        & 0x3F;
        }

        // Perform parallel character translation!
        // This single compiler builtin maps directly to a high-speed hardware 
        // register lookup like 'vpermb' on AVX-512 or 'tbl' on ARM Neon.
        v64qi encoded_chars = __builtin_shuffle(alphabet, indices);

        // Store 64 output characters to memory in a single clock cycle
        *(v64qi_unaligned *)&dst[j] = encoded_chars;

        i += 48;
        j += 64;
    }

    // Standard scalar cleanup block handles any remaining trailing bytes (with padding)
    const char *alphabet_str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    while (i < src_len) {
        uint32_t b0 = src[i++];
        uint32_t b1 = (i < src_len) ? src[i++] : 0;
        uint32_t b2 = (i < src_len) ? src[i++] : 0;

        uint32_t triple = (b0 << 16) | (b1 << 8) | b2;

        dst[j++] = alphabet_str[(triple >> 18) & 0x3F];
        dst[j++] = alphabet_str[(triple >> 12) & 0x3F];
        dst[j++] = (i > src_len + 1) ? '=' : alphabet_str[(triple >> 6) & 0x3F];
        dst[j++] = (i > src_len)     ? '=' : alphabet_str[triple & 0x3F];
    }
    dst[j] = '\0'; // Null-terminate destination string
}

int custom_memcmp_512(const void *s1, const void *s2, size_t n) {
    const char *p1 = (const char *)s1;
    const char *p2 = (const char *)s2;

    // 1. Process 64-byte chunks in parallel using unaligned vector blocks
    while (n >= 64) {
        v64qi v1 = *(const v64qi_unaligned *)p1;
        v64qi v2 = *(const v64qi_unaligned *)p2;

        // Compare all 64 bytes simultaneously (returns 0xFF on mismatch)
        uint64_t diff_mask = get_movemask_portable_512(v1 != v2);

        if (diff_mask != 0) {
            // Find the index of the very first byte that mismatched
            uint32_t offset = __builtin_ctzll(diff_mask);
            return (int)((unsigned char)p1[offset] - (unsigned char)p2[offset]);
        }

        p1 += 64;
        p2 += 64;
        n -= 64;
    }

    // 2. Process smaller power-of-two vector fragments for remaining bytes
    if (n >= 32) {
        typedef uint8_t v32qi __attribute__ ((vector_size (32)));
        typedef v32qi v32_unaligned __attribute__((aligned(1)));
        if (*(const v32_unaligned *)p1 != *(const v32_unaligned *)p2) {
            for (size_t i = 0; i < 32; ++i) {
                if (p1[i] != p2[i]) return (int)((unsigned char)p1[i] - (unsigned char)p2[i]);
            }
        }
        p1 += 32; p2 += 32; n -= 32;
    }
    if (n >= 16) {
        typedef uint8_t v16qi __attribute__ ((vector_size (16)));
        typedef v16qi v16_unaligned __attribute__((aligned(1)));
        if (*(const v16_unaligned *)p1 != *(const v16_unaligned *)p2) {
            for (size_t i = 0; i < 16; ++i) {
                if (p1[i] != p2[i]) return (int)((unsigned char)p1[i] - (unsigned char)p2[i]);
            }
        }
        p1 += 16; p2 += 16; n -= 16;
    }

    // 3. Scalar cleanup loop for the final 0-15 bytes
    while (n > 0) {
        if (*p1 != *p2) {
            return (int)((unsigned char)*p1 - (unsigned char)*p2);
        }
        p1++;
        p2++;
        n--;
    }

    return 0;
}

void *custom_memcpy_512(void *dest, const void *src, size_t n) {
    char *d = (char *)dest;
    const char *s = (const char *)src;

    // 1. Process 64-byte chunks (Cacheline Pipeline)
    while (n >= 64) {
        // The compiler generates a single unaligned load and a single unaligned store
        *(v64qi_unaligned *)d = *(const v64qi_unaligned *)s;
        d += 64;
        s += 64;
        n -= 64;
    }

    // 2. Clear out remaining tail fragments using smaller native power-of-two vectors
    if (n >= 32) {
        typedef uint8_t v32qi __attribute__ ((vector_size (32)));
        typedef v32qi v32_unaligned __attribute__((aligned(1)));
        *(v32_unaligned *)d = *(const v32_unaligned *)s;
        d += 32; s += 32; n -= 32;
    }
    if (n >= 16) {
        typedef uint8_t v16qi __attribute__ ((vector_size (16)));
        typedef v16qi v16_unaligned __attribute__((aligned(1)));
        *(v16_unaligned *)d = *(const v16_unaligned *)s;
        d += 16; s += 16; n -= 16;
    }
    
    // 3. Absolute scalar fallback clean up for the final 0-15 trailing bytes
    while (n > 0) {
        *d++ = *s++;
        n--;
    }

    return dest;
}

size_t custom_strlen_512(const char *s) {
    // 1. Round pointer DOWN to the nearest 64-byte boundary
    uintptr_t uptr = (uintptr_t)s;
    const v64qi *vp = (const v64qi *)(uptr & ~(uintptr_t)63);
    const v64qi all0 = {0};

    // 2. Load the first (potentially misaligned) 64-byte block
    v64qi v = (*vp == all0);
    uint64_t mask = get_movemask_portable_512(v);

    // 3. Create a mask to ignore bytes before the string's start
    // If s is 3 bytes past the boundary, shift out the lower 3 bits
    uint32_t misalignment = uptr & 63;
    mask >>= misalignment;

    // 4. Fast path: If a null byte is found in the first block, return immediately
    if (mask != 0) {
        return __builtin_ctzll(mask);
    }

    // 5. Main Loop: Move to the next cache line and search normally
    // This loop is perfectly aligned and has zero interior scalar branches
    while (1) {
        vp++;
        v = (*vp == all0);
        if (vector_any_non_zero_512(v)) {
            break;
        }
    }

    // 6. Final calculation for later blocks
    mask = get_movemask_portable_512(v);
    return (const char *)vp - s + __builtin_ctzll(mask);
}

void *custom_memchr_512(const void *s, int c, size_t n) {
    if (n == 0) return NULL;

    uintptr_t uptr = (uintptr_t)s;
    const v64qi *vp = (const v64qi *)(uptr & ~(uintptr_t)63);
    
    // Broadcast the target character across all 64 bytes of a vector
    v64qi target;
    for (int i = 0; i < 64; ++i) target[i] = (uint8_t)c;

    uint32_t misalignment = uptr & 63;
    
    // 1. First block processing (Page-Safe)
    v64qi v = (*vp == target);
    uint64_t mask = get_movemask_portable_512(v);
    
    // Drop bytes that occur before the requested start pointer
    mask >>= misalignment;

    // Handle end-of-buffer bounds checking safely within the mask
    if (n < 64 - misalignment) {
        uint64_t length_mask = (1ULL << n) - 1;
        mask &= length_mask;
        return mask ? (void *)(uptr + __builtin_ctzll(mask)) : NULL;
    }

    if (mask != 0) {
        return (void *)(uptr + __builtin_ctzll(mask));
    }

    // Update remaining length tracking
    size_t processed = 64 - misalignment;
    n -= processed;

    // 2. Main Aligned Loop
    while (n >= 64) {
        vp++;
        v = (*vp == target);
        if (vector_any_non_zero_512(v)) {
            mask = get_movemask_portable_512(v);
            return (void *)((const char *)vp + __builtin_ctzll(mask));
        }
        n -= 64;
    }

    // 3. Final Tail block processing
    if (n > 0) {
        vp++;
        v = (*vp == target);
        mask = get_movemask_portable_512(v);
        uint64_t length_mask = (1ULL << n) - 1;
        mask &= length_mask;
        if (mask != 0) {
            return (void *)((const char *)vp + __builtin_ctzll(mask));
        }
    }

    return NULL;
}

int custom_strcmp_optimized(const char *s1, const char *s2) {
    uintptr_t uptr1 = (uintptr_t)s1;
    const v64qi all0 = {0};

    // 1. Align s1 to a 64-byte boundary. 
    // We step s2 forward by the exact same distance to keep them perfectly in sync.
    uint32_t misalign1 = uptr1 & 63;
    const v64qi *vp1 = (const v64qi *)(uptr1 & ~(uintptr_t)63);
    const char *p2 = s2 - misalign1;

    // 2. Process the very first cache line (Page-Safe for s1)
    v64qi v1 = *vp1;
    // We must use a strictly unaligned read for s2 here because it isn't aligned yet
    v64qi v2 = *(const v64qi_unaligned *)p2; 

    uint64_t null_mask = get_movemask_portable_512(v1 == all0);
    uint64_t diff_mask = get_movemask_portable_512(v1 != v2);
    uint64_t match_mask = null_mask | diff_mask;

    // Shift out the bytes before the actual string start
    match_mask >>= misalign1;

    if (match_mask != 0) {
        uint32_t offset = __builtin_ctzll(match_mask);
        return (int)((unsigned char)s1[offset] - (unsigned char)s2[offset]);
    }

    // 3. Main Loop
    // vp1 is perfectly aligned to 64-byte boundaries.
    // p2 is completely unaligned, but its bytes match lane-for-lane with vp1!
    while (1) {
        vp1++;
        p2 += 64;

        v1 = *vp1;
        v2 = *(const v64qi_unaligned *)p2; // Forced unaligned vector load

        uint64_t n_mask = get_movemask_portable_512(v1 == all0);
        uint64_t d_mask = get_movemask_portable_512(v1 != v2);
        
        uint64_t critical_mask = n_mask | d_mask;
        if (critical_mask != 0) {
            uint32_t offset = __builtin_ctzll(critical_mask);
            const char *p1 = (const char *)vp1;
            return (int)((unsigned char)p1[offset] - (unsigned char)p2[offset]);
        }
    }
}

// Simple scalar helper to verify if the full needle matches at this specific offset
static inline int structural_verify(const char *h, const char *n) {
    while (*n && *h == *n) {
        h++;
        n++;
    }
    return *n == '\0';
}

const char *custom_strstr_512(const char *haystack, const char *needle) {
    // Edge case constraints
    if (!needle[0]) return haystack;
    if (!haystack[0]) return NULL;

    // Broadcast the first char of the needle across all 64 bytes of a vector
    v64qi token;
    for (int i = 0; i < 64; ++i) token[i] = (uint8_t)needle[0];

    uintptr_t uptr = (uintptr_t)haystack;
    const v64qi *vp = (const v64qi *)(uptr & ~(uintptr_t)63);
    const v64qi all0 = {0};

    uint32_t misalignment = uptr & 63;

    // 1. Process the first cache line safely (handling pointer misalignment)
    v64qi v_chars = *vp;
    uint64_t match_mask = get_movemask_portable_512(v_chars == token);
    uint64_t null_mask  = get_movemask_portable_512(v_chars == all0);

    // Drop lanes that occur prior to our true haystack start pointer
    match_mask >>= misalignment;
    null_mask  >>= misalignment;

    // Evaluate matching character targets found in the first block
    while (match_mask != 0) {
        uint32_t offset = __builtin_ctzll(match_mask);
        // If a null terminator occurs before this candidate match index, the string ended early
        if ((null_mask & ((1ULL << offset) - 1)) != 0) return NULL;

        if (structural_verify(haystack + offset, needle)) {
            return haystack + offset;
        }
        match_mask &= match_mask - 1; // Clear lowest set bit
    }
    
    // If a null byte was seen anywhere in the first block, we are completely finished
    if (null_mask != 0) return NULL;

    // Advance haystack tracking pointer to synchronize with the next loop boundary
    const char *curr_haystack = haystack + (64 - misalignment);
    vp++;

    // 2. Main Aligned Cacheline Loop
    while (1) {
        v_chars = *vp;
        match_mask = get_movemask_portable_512(v_chars == token);
        null_mask  = get_movemask_portable_512(v_chars == all0);

        // Process any potential token matches across this 64-byte block
        while (match_mask != 0) {
            uint32_t offset = __builtin_ctzll(match_mask);
            
            // Check if a null terminator appeared before this specific token index
            if ((null_mask & ((1ULL << offset) - 1)) != 0) return NULL;

            if (structural_verify(curr_haystack + offset, needle)) {
                return curr_haystack + offset;
            }
            match_mask &= match_mask - 1; 
        }

        // If a null terminator exists anywhere in this block, the search terminates
        if (null_mask != 0) return NULL;

        vp++;
        curr_haystack += 64;
    }
}
