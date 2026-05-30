#include "../include/intsort.h"

static const int IO_BUF_SIZE = 1 << 16;
static char in_buf[IO_BUF_SIZE];
static int in_pos = 0, in_len = 0;
static char out_buf[IO_BUF_SIZE];
static int out_pos = 0;

static inline int get_char() {
    if (in_pos == in_len) {
        in_pos = 0;
        in_len = fread(in_buf, 1, IO_BUF_SIZE, stdin);
        if (in_len == 0) return EOF;
    }
    return (unsigned char)in_buf[in_pos++];
}
static inline void flush_out() {
    if (out_pos > 0) {
        fwrite(out_buf, 1, out_pos, stdout);
        out_pos = 0;
    }
}
static inline void put_char(char c) {
    if (out_pos == IO_BUF_SIZE) flush_out();
    out_buf[out_pos++] = c;
}
static inline int read_int() {
    int x = 0;
    int c = get_char();
    bool is_negative = false;
    while (c != EOF && (c < '0' || c > '9')) {
        if (c == '-') is_negative = true;
        c = get_char();
    }
    if (c == EOF) return 0;
    while (c >= '0' && c <= '9') {
        x = x * 10 + (c - '0');
        c = get_char();
    }
    return is_negative ? -x : x;
}
static inline void print_int(int x) {
    if (x == 0) {
        put_char('0');
        put_char('\n');
        return;
    }
    if (x < 0) {
        put_char('-');
        if (x == -2147483648) {
            const char* min_val = "2147483648\n";
            for (int i = 0; i < 11; ++i) put_char(min_val[i]);
            return;
        }
        x = -x;
    }
    char buf[12];
    int idx = 0;
    while (x) {
        buf[idx++] = (char)((x % 10) + '0');
        x /= 10;
    }
    while (idx--) {
        put_char(buf[idx]);
    }
    put_char('\n');
}

static void radix_sort(int* arr, unsigned int n, int* buffer) {
    if (n < 2) return;

    unsigned int* src = (unsigned int*)arr;
    unsigned int* dst = (unsigned int*)buffer;

    for (unsigned int i = 0; i < n; ++i) {
        src[i] ^= 0x80000000;
    }

    for (int shift = 0; shift < 32; shift += 8) {
        unsigned int counts[256] = {0};

        for (unsigned int i = 0; i < n; ++i) counts[(src[i] >> shift) & 0xFF]++;

        unsigned int prefix = 0;

        for (int i = 0; i < 256; ++i) {
            unsigned int tmp = counts[i];
            counts[i] = prefix;
            prefix += tmp;
        }
        for (unsigned int i = 0; i < n; ++i) dst[counts[(src[i] >> shift) & 0xFF]++] = src[i];

        unsigned int* temp = src;
        src = dst;
        dst = temp;
    }
    for (unsigned int i = 0; i < n; ++i) src[i] ^= 0x80000000;
}

void runIntSort() {
    unsigned int n = read_int();
    if (n == 0) {
        flush_out();
        return;
    }
    print_int(n);

    int* data = (int*)malloc(n * sizeof(int));
    int* workspace = (int*)malloc(n * sizeof(int));

    for (unsigned int i = 0; i < n; ++i) data[i] = read_int();

    radix_sort(data, n, workspace);

    for (unsigned int i = 0; i < n; ++i) print_int(data[i]);

    flush_out();
    free(data);
    free(workspace);
    return;
}
