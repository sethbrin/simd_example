
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
_mm_setzero_ps.out

all: $(TARGETS)

%.out: %.cc
	$(CC) $< -o $@

clean:
	rm $(TARGETS)
