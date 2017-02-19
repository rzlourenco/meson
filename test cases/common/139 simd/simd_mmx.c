#include<simdconfig.h>
#include<simdfuncs.h>

#include<stdint.h>

#ifdef _MSC_VER
#include<intrin.h>
int mmx_available() {
  return 1;
}

/* Contrary to MSDN documentation, MMX intrinsics
 * just plain don't work.
 */
void increment_mmx(float arr[4]) {
  arr[0]++;
  arr[1]++;
  arr[2]++;
  arr[3]++;
}

#else
#include<mmintrin.h>
#include<cpuid.h>
int mmx_available() {
    return __builtin_cpu_supports("mmx");
}

void increment_mmx(float arr[4]) {
    /* Super ugly but we know that values in arr are always small
     * enough to fit in int16;
     */
    __m64 packed = _mm_set_pi16(arr[3], arr[2], arr[1], arr[0]);
    __m64 incr = _mm_set1_pi16(1);
    __m64 result = _mm_add_pi16(packed, incr);
    int64_t unpacker = _m_to_int64(result);
    _mm_empty();
    for(int i=0; i<4; i++) {
      arr[i] = (float)(unpacker & ((1<<16)-1));
        unpacker >>= 16;
    }
}

#endif
