#include "ktime.h"

KTime ktime_now(void) {
    return (KTime){0, 0};
}

KTime ktime_from_ms(uint64_t ms) {
    return (KTime){
        .seconds = ms / 1000,
        .nanoseconds = (uint32_t)((ms % 1000) * 1000000ULL)
    };
}

uint64_t ktime_to_ms(KTime t) {
    return t.seconds * 1000ULL + t.nanoseconds / 1000000ULL;
}

double ktime_to_seconds(KTime t) {
    return (double)t.seconds + ((double)t.nanoseconds / 1000000000.0);
}

bool ktime_after(KTime a, KTime b) {
    if (a.seconds != b.seconds)
        return a.seconds > b.seconds;

    return a.nanoseconds > b.nanoseconds;
}

bool ktime_before(KTime a, KTime b) {
    if (a.seconds != b.seconds)
        return a.seconds < b.seconds;

    return a.nanoseconds < b.nanoseconds;
}

KTime ktime_diff(KTime a, KTime b) {
    KTime result = {0};

    /* garante diferença positiva */
    if (ktime_before(a, b)) {
        KTime temp = a;
        a = b;
        b = temp;
    }

    result.seconds = a.seconds - b.seconds;

    if (a.nanoseconds >= b.nanoseconds) {
        result.nanoseconds = a.nanoseconds - b.nanoseconds;
    } else {
        result.seconds--;
        result.nanoseconds =
            (1000000000ULL + a.nanoseconds) - b.nanoseconds;
    }

    return result;
}
