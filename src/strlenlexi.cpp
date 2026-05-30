#include "../include/strlenlexi.h"

static const int OUT_BUF_SIZE = 1 << 20;
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

static void mkqs(char** a, int n, int depth, int max_length) {
    if (n <= 1 || depth >= max_length) return;

    if (n < 32) {
        for (int i = 1; i < n; i++) {
            char* tmp = a[i];
            int j = i - 1;
            while (j >= 0 && strcmp(a[j] + depth, tmp + depth) > 0) {
                a[j + 1] = a[j];
                j--;
            }
            a[j + 1] = tmp;
        }
        return;
    }

    int p1 = a[0][depth], p2 = a[n / 2][depth], p3 = a[n - 1][depth];
    int pivot = p1;
    if ((p2 <= p1 && p2 >= p3) || (p2 >= p1 && p2 <= p3))
        pivot = p2;
    else if ((p3 <= p1 && p3 >= p2) || (p3 >= p1 && p3 <= p2))
        pivot = p3;

    int i = 0, j = 0, k = n;
    while (j < k) {
        int c = a[j][depth];
        if (c < pivot) {
            char* tmp = a[i];
            a[i] = a[j];
            a[j] = tmp;
            i++;
            j++;
        } else if (c > pivot) {
            k--;
            char* tmp = a[j];
            a[j] = a[k];
            a[k] = tmp;
        } else {
            j++;
        }
    }

    mkqs(a, i, depth, max_length);
    mkqs(a + i, k - i, depth + 1, max_length);
    mkqs(a + k, n - k, depth, max_length);
}

void runStrLenLexiSort() {
    fseek(stdin, 0, SEEK_END);
    long fsize = ftell(stdin);
    rewind(stdin);

    char* pool;
    size_t pool_size = 0;

    if (fsize > 0) {
        pool = (char*)malloc(fsize + 2);
        pool_size = fread(pool, 1, fsize, stdin);
        pool[pool_size] = '\0';
    } else {
        size_t cap = 16 * 1024 * 1024;
        pool = (char*)malloc(cap);
        while (true) {
            size_t bytes = fread(pool + pool_size, 1, cap - pool_size, stdin);
            if (bytes == 0) break;
            pool_size += bytes;
            if (pool_size == cap) {
                cap = cap * 3 / 2;
                pool = (char*)realloc(pool, cap);
            }
        }
        pool[pool_size] = '\0';
    }

    char* p = pool;
    while (*p && *p <= ' ') p++;
    if (!*p) return;

    unsigned int n = 0;
    while (*p >= '0' && *p <= '9') {
        n = n * 10 + (*p - '0');
        p++;
    }

    print_int(n);
    if (n == 0) {
        flush_out();
        return;
    }

    char** arr = (char**)malloc(n * sizeof(char*));
    int* lengths = (int*)malloc(n * sizeof(int));

    unsigned int idx = 0;
    int max_len = 0;
    while (*p && idx < n) {
        while (*p && *p <= ' ') p++;
        if (!*p) break;

        char* start = p;
        arr[idx] = start;

        while (*p && *p > ' ') p++;

        int len = p - start;
        lengths[idx] = len;
        if (len > max_len) max_len = len;

        if (*p) {
            *p = '\0';
            p++;
        }
        idx++;
    }
    n = idx;

    int* len_counts = (int*)calloc(max_len + 1, sizeof(int));
    for (unsigned int i = 0; i < n; i++) {
        len_counts[lengths[i]]++;
    }

    int* len_offsets = (int*)malloc((max_len + 2) * sizeof(int));
    int current_offset = 0;
    for (int i = 0; i <= max_len; i++) {
        len_offsets[i] = current_offset;
        current_offset += len_counts[i];
    }

    char** sorted_by_len = (char**)malloc(n * sizeof(char*));
    for (unsigned int i = 0; i < n; i++) {
        sorted_by_len[len_offsets[lengths[i]]++] = arr[i];
    }
    for (int i = 1; i <= max_len; i++) {
        int count = len_counts[i];
        if (count > 1) {
            int start = len_offsets[i] - count;

            mkqs(sorted_by_len + start, count, 0, i);
        }
    }

    for (unsigned int i = 0; i < n; i++) {
        print_str(sorted_by_len[i]);
    }

    free(pool);
    free(arr);
    free(lengths);
    free(len_counts);
    free(len_offsets);
    free(sorted_by_len);
    flush_out();
    return;
}
