CFLAGS = -msse -mavx #-mavx2

TARGETS = \
_mm_load_ps.out \
_mm_load_ps1.out \
_mm_load_ss.out \
_mm_load1_ps.out \
_mm_loadh_pi.out \
_mm_loadl_pi.out \
_mm_loadr_ps.out \
_mm_loadu_ps.out \
_mm_set_ps.out \
_mm_set_ps1.out \
_mm_set1_ps.out \
_mm_set_ss.out \
_mm_setr_ps.out \
_mm_setzero_ps.out \
test_sse_avx_add_float.out \
test_sse_avx_add_double.out \
test_mmx_sse_avx_add_int.out \
test_likely.out \
_mm_slli_si128.out \

all: $(TARGETS)

%.out: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(TARGETS)
