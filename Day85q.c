 Open-addressing hash map (string -> int) with tombstones, FNV-1a hashing, auto-resize.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
 
enum { EMPTY, USED, DELETED };
typedef struct { char *key; int value; unsigned char state; } Entry;
typedef struct { Entry *e; size_t cap, len, tomb; } Map;
 
static uint64_t fnv1a(const char *s) {
    uint64_t h = 1469598103934665603ULL;
    while (*s) { h ^= (unsigned char)*s++; h *= 1099511628211ULL; }
    return h;
}
 
static void map_init(Map *m, size_t cap) {          // cap must be a power of two
    m->e = calloc(cap, sizeof(Entry));
    m->cap = cap; m->len = m->tomb = 0;
}
 
static void map_free(Map *m) {
    for (size_t i = 0; i < m->cap; i++) if (m->e[i].state == USED) free(m->e[i].key);
    free(m->e);
}
 
static void rehash(Map *m, size_t newcap) {
    Entry *old = m->e; size_t oldcap = m->cap;
    m->e = calloc(newcap, sizeof(Entry)); m->cap = newcap; m->tomb = 0;
    for (size_t i = 0; i < oldcap; i++) {
        if (old[i].state != USED) continue;
        size_t j = fnv1a(old[i].key) & (newcap - 1);
        while (m->e[j].state == USED) j = (j + 1) & (newcap - 1);
        m->e[j] = old[i];                           // move key ownership
    }
    free(old);
}
 
static void map_put(Map *m, const char *key, int value) {
    if ((m->len + m->tomb + 1) * 10 > m->cap * 7) rehash(m, m->cap * 2);
    size_t i = fnv1a(key) & (m->cap - 1), first_del = (size_t)-1;
    while (m->e[i].state != EMPTY) {
        if (m->e[i].state == USED && strcmp(m->e[i].key, key) == 0) { m->e[i].value = value; return; }
        if (m->e[i].state == DELETED && first_del == (size_t)-1) first_del = i;
        i = (i + 1) & (m->cap - 1);
    }
    if (first_del != (size_t)-1) { i = first_del; m->tomb--; }
    m->e[i].key = strdup(key); m->e[i].value = value; m->e[i].state = USED; m->len++;
}
 
static int *map_get(Map *m, const char *key) {
    size_t i = fnv1a(key) & (m->cap - 1);
    while (m->e[i].state != EMPTY) {
        if (m->e[i].state == USED && strcmp(m->e[i].key, key) == 0) return &m->e[i].value;
        i = (i + 1) & (m->cap - 1);
    }
    return NULL;
}
 
static int map_del(Map *m, const char *key) {
    int *v = map_get(m, key);
    if (!v) return 0;
    Entry *en = (Entry *)((char *)v - offsetof(Entry, value));
    free(en->key); en->key = NULL; en->state = DELETED; m->len--; m->tomb++;
    return 1;
}
 
int main(void) {
    const char *words[] = {"the","quick","brown","fox","jumps","over","the","lazy","dog","the","fox"};
    Map m; map_init(&m, 4);
    for (size_t i = 0; i < sizeof words / sizeof *words; i++) {
        int *c = map_get(&m, words[i]);
        map_put(&m, words[i], c ? *c + 1 : 1);
    }
    printf("entries=%zu capacity=%zu\n", m.len, m.cap);
    for (size_t i = 0; i < m.cap; i++)
        if (m.e[i].state == USED) printf("  %-6s -> %d\n", m.e[i].key, m.e[i].value);
    map_del(&m, "the");
    printf("after delete: the=%s\n", map_get(&m, "the") ? "present" : "gone");
    map_free(&m);
    return 0;
}
 
