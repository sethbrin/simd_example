EMCC=$(EMSCRIPTEN)/emcc --memory-init-file 0 -O0
DISASM=$(LLVM_BIN)/llvm-dis

OBJECTS = \
_mm_load_ps.bc \
_mm_load_ps1.bc \
_mm_load_ss.bc \
_mm_load1_ps.bc \
_mm_loadh_pi.bc \
_mm_loadl_pi.bc \
_mm_loadr_ps.bc \
_mm_loadu_ps.bc \
_mm_set_ps.bc \
_mm_set_ps1.bc \
_mm_set1_ps.bc \
_mm_set_ss.bc \
_mm_setr_ps.bc \
_mm_setzero_ps.bc

TARGETS = \
_mm_load_ps.js \
_mm_load_ps.ll \
_mm_load_ps1.js \
_mm_load_ps1.ll \
_mm_load_ss.js \
_mm_load_ss.ll \
_mm_load1_ps.js \
_mm_load1_ps.ll \
_mm_loadh_pi.js \
_mm_loadh_pi.ll \
_mm_loadl_pi.js \
_mm_loadl_pi.ll \
_mm_loadr_ps.js \
_mm_loadr_ps.ll \
_mm_loadu_ps.js \
_mm_loadu_ps.ll \
_mm_set_ps.js \
_mm_set_ps.ll \
_mm_set_ps1.js \
_mm_set_ps1.ll \
_mm_set1_ps.js \
_mm_set1_ps.ll \
_mm_set_ss.js \
_mm_set_ss.ll \
_mm_setr_ps.js \
_mm_setr_ps.ll \
_mm_setzero_ps.js \
_mm_setzero_ps.ll

all: $(TARGETS)

%.bc: %.cc
	$(EMCC) $< -o $@

%.js: %.bc
	$(EMCC) $< -o $@

%.ll: %.bc
	$(DISASM) $< -o $@

clean:
	del $(TARGETS) $(OBJECTS)