#ifndef KTIME_DEC
#define KTIME_DEC

#include "ktypes.h"
#include <stdint.h>
typedef struct {
    uint64_t seconds;
    uint32_t nanoseconds;
} KTime;

KTime ktime_now(void);
KTime ktime_from_ms(uint64_t ms);
uint64_t ktime_to_ms(KTime t);
double ktime_to_seconds(KTime t);
bool ktime_after(KTime a, KTime b);
bool ktime_before(KTime a, KTime b);
KTime ktime_diff(KTime a, KTime b);

#endif
