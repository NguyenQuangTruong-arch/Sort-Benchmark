#include "../include/strlexi.h"

static const int OUT_BUF_SIZE = 1 << 18;
static char out_buf[OUT_BUF_SIZE];
static int out_pos = 0;

static inline void flush_out() {
    if (out_pos > 0) {
        fwrite(out_buf, 1, out_pos, stdout);
        out_pos = 0;
    }
}
static inline void put_char(char c) {
    if (out_pos == OUT_BUF_SIZE) flush_out();
    out_buf[out_pos++] = c;
}
static inline void print_str(const char* s) {
    while (*s) put_char(*s++);
    put_char('\n');
}
static inline void print_int(unsigned int x) {
    if (x == 0) {
        put_char('0');
        put_char('\n');
        return;
    }
    char buf[12];
    int idx = 0;
    while (x) {
        buf[idx++] = (char)((x % 10) + '0');
        x /= 10;
    }
    while (idx--) put_char(buf[idx]);
    put_char('\n');
}
static void msd_radix_sort(char** a, char** b, int n, int depth) {
    if (n <= 1) return;
    if (n < 32) {
        for (int i = 1; i < n; i++) {
            for (int j = i; j > 0; j--) {
                const char *s1 = a[j - 1] + depth, *s2 = a[j] + depth;
                while (*s1 && *s1 == *s2) {
                    s1++;
                    s2++;
                }
                if ((unsigned char)*s1 <= (unsigned char)*s2) break;
                char* tmp = a[j];
                a[j] = a[j - 1];
                a[j - 1] = tmp;
            }
        }
        return;
    }
    int count[258] = {0};
    for (int i = 0; i < n; i++) {
        count[(unsigned char)a[i][depth] + 1]++;
    }
    for (int i = 0; i < 257; i++) {
        count[i + 1] += count[i];
    }
    for (int i = 0; i < n; i++) {
        b[count[(unsigned char)a[i][depth]]++] = a[i];
    }
    for (int i = 0; i < n; i++) {
        a[i] = b[i];
    }
    for (int i = 1; i < 256; i++) {
        int start = (i == 0) ? 0 : count[i - 1];
        int end = count[i];
        if (end - start > 1) {
            msd_radix_sort(a + start, b + start, end - start, depth + 1);
        }
    }
}
void runStrLexiSort() {
    size_t capacity = 512 * 1024 * 1024;
    char* pool = (char*)malloc(capacity);
    size_t pool_size = fread(pool, 1, capacity, stdin);
    pool[pool_size] = '\0';
    char* p = pool;
    while (*p && *p <= ' ') p++;
    unsigned int n = 0;
    while (*p >= '0' && *p <= '9') n = n * 10 + (*p++ - '0');
    print_int(n);
    if (n == 0) {
        flush_out();
        return;
    }
    char** arr = (char**)malloc(n * sizeof(char*));
    char** tmp_buf = (char**)malloc(n * sizeof(char*));
    unsigned int idx = 0;
    while (*p && idx < n) {
        while (*p && *p <= ' ') p++;
        if (!*p) break;
        arr[idx++] = p;
        while (*p && *p > ' ') p++;
        if (*p) *p++ = '\0';
    }
    msd_radix_sort(arr, tmp_buf, n, 0);
    for (unsigned int i = 0; i < n; i++) {
        print_str(arr[i]);
    }
    flush_out();
    free(pool);
    free(arr);
    free(tmp_buf);
    return;
}
