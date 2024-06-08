#ifndef CAPSTONE_CSTOOL_CSTOOL_H_
#define CAPSTONE_CSTOOL_CSTOOL_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <capstone/capstone.h>
#include "../cs_priv.h"
#include "../utils.h"

typedef struct {
	char *ptr;
	size_t index;
	size_t size;
} Stringbuf;

typedef struct {
	bool is_stdout;
	Stringbuf ss;
} Stream;

static inline void stream_init(Stream *s)
{
	assert(s);
	memset(s, 0, sizeof(Stream));
}

static inline void stream_init_stream(Stream *s)
{
	stream_init(s);
	s->ss.ptr = malloc(512);
	s->ss.size = 512;
}

static inline void stream_init_stdout(Stream *s)
{
	stream_init(s);
	s->is_stdout = true;
}

static inline char *stringbuf_back(Stringbuf *s)
{
	assert(s);
	return s->ptr + s->index;
}

static inline bool stringbuf_grow(Stringbuf *s, size_t size)
{
	assert(s && size > 0);
	char *ptr = realloc(s->ptr, size);
	if (ptr) {
		s->ptr = ptr;
		s->size = size;
		return true;
	}
	return false;
}

static inline void stream_printf(Stream *s, const char *fmt, ...)
{
	assert(s);
	va_list ap;
	va_start(ap, fmt);
	if (s->is_stdout) {
		int res = vprintf(fmt, ap);
		s->ss.index += res;
	} else {
		Stringbuf *ss = &s->ss;
		int ret = cs_vsnprintf(stringbuf_back(&s->ss),
				       ss->size - ss->index, fmt, ap);
		if (ret >= ss->size - ss->index) {
			if (!stringbuf_grow(ss, (ss->index + ret) * 2)) {
				assert(0);
				return;
			}
			ret = cs_vsnprintf(stringbuf_back(&s->ss), ret + 1, fmt,
					   ap);
		}
		ss->index += ret;
	}
	va_end(ap);
}

static inline void stream_print(Stream *s, const char *x)
{
	assert(s);
	if (s->is_stdout) {
		printf("%s", x);
	} else {
		size_t len = strlen(x);
		if (s->ss.size <= len + s->ss.index + 1) {
			if (!stringbuf_grow(&s->ss, (s->ss.size + len) * 2)) {
				assert(0);
				return;
			}
		}
		memcpy(stringbuf_back(&s->ss), x, len);
		s->ss.index += len;
	}
}

static inline void stream_close(Stream *s)
{
	assert(s);
	if (!s->is_stdout) {
		free(s->ss.ptr);
	}
	stream_init(s);
}

static inline char *stream_as_str(Stream *s)
{
	assert(s && !s->is_stdout);
	return s->ss.ptr;
}

static inline void stream_print_string_hex(Stream *s, const char *comment,
					   unsigned char *str, size_t len)
{
	assert(s);
	unsigned char *c;

	stream_printf(s, "%s", comment);
	for (c = str; c < str + len; c++) {
		stream_printf(s, "0x%02x ", *c & 0xff);
	}
	stream_printf(s, "\n");
}

static inline void stream_rtrim(Stream *s)
{
	assert(s && !s->is_stdout);
	char *back = stringbuf_back(&s->ss);
	while (isspace(*--back))
		;
	*(back + 1) = '\0';
}

void print_insn_detail_x86(csh handle, cs_insn *ins, Stream *steam);
void print_insn_detail_arm(csh handle, cs_insn *ins, Stream *steam);
void print_insn_detail_aarch64(csh handle, cs_insn *ins, Stream *steam);
void print_insn_detail_mips(csh handle, cs_insn *ins, Stream *steam);
void print_insn_detail_ppc(csh handle, cs_insn *ins, Stream *steam);
void print_insn_detail_sparc(csh handle, cs_insn *ins, Stream *steam);
void print_insn_detail_sysz(csh handle, cs_insn *ins, Stream *steam);
void print_insn_detail_xcore(csh handle, cs_insn *ins, Stream *steam);
void print_insn_detail_m68k(csh handle, cs_insn *ins, Stream *steam);
void print_insn_detail_tms320c64x(csh handle, cs_insn *ins, Stream *steam);
void print_insn_detail_m680x(csh handle, cs_insn *ins, Stream *steam);
void print_insn_detail_evm(csh handle, cs_insn *ins, Stream *steam);
void print_insn_detail_riscv(csh handle, cs_insn *ins, Stream *steam);
void print_insn_detail_wasm(csh handle, cs_insn *ins, Stream *steam);
void print_insn_detail_mos65xx(csh handle, cs_insn *ins, Stream *steam);
void print_insn_detail_bpf(csh handle, cs_insn *ins, Stream *steam);
void print_insn_detail_sh(csh handle, cs_insn *ins, Stream *steam);
void print_insn_detail_tricore(csh handle, cs_insn *ins, Stream *steam);
void print_insn_detail_alpha(csh handle, cs_insn *ins, Stream *steam);
void print_insn_detail_hppa(csh handle, cs_insn *ins, Stream *steam);
void print_insn_detail_xtensa(csh handle, cs_insn *ins, Stream *steam);
void print_insn_detail(csh handle, cs_arch arch, cs_mode md, cs_insn *ins,
		       Stream *steam);

typedef void (*func_print_insn_detail)(csh handle, cs_insn *ins, Stream *steam);

#endif //CAPSTONE_CSTOOL_CSTOOL_H_
