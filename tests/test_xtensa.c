/* Capstone Disassembler Engine */
/// Capstone xtensa test, by billow <billow.fun@gmail.com>

#include <stdio.h>

#include <capstone/platform.h>
#include <capstone/capstone.h>
#include "../cstool/cstool.h"

struct platform {
	cs_arch arch;
	cs_mode mode;
	unsigned char *code;
	size_t size;
	char *comment;
};

static csh handle;

static void test()
{
#define TEST_CODE "\x60\x51\x60\x32\x51\x02"

	struct platform platforms[] = {
		{
			CS_ARCH_XTENSA,
			CS_MODE_LITTLE_ENDIAN,
			(unsigned char *)TEST_CODE,
			sizeof(TEST_CODE) - 1,
			"Xtensa",
		},
	};

	uint64_t address = 0x1000;
	cs_insn *insn;
	int i;
	size_t count;
	Stream stream = { 0 };
	stream_init_stdout(&stream);

	for (i = 0; i < sizeof(platforms) / sizeof(platforms[0]); i++) {
		cs_err err =
			cs_open(platforms[i].arch, platforms[i].mode, &handle);
		if (err) {
			printf("Failed on cs_open() with error returned: %u\n",
			       err);
			continue;
		}

		cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);

		count = cs_disasm(handle, platforms[i].code, platforms[i].size,
				  address, 0, &insn);
		if (count) {
			size_t j;

			printf("****************\n");
			printf("Platform: %s\n", platforms[i].comment);
			stream_print_string_hex(&stream,
						"Code:", platforms[i].code,
						platforms[i].size);
			printf("Disasm:\n");

			for (j = 0; j < count; j++) {
				printf("0x%" PRIx64 ":\t%s\t%s\n",
				       insn[j].address, insn[j].mnemonic,
				       insn[j].op_str);
				print_insn_detail_xtensa(handle, &insn[j],
							 &stream);
			}

			// free memory allocated by cs_disasm()
			cs_free(insn, count);
		} else {
			printf("****************\n");
			printf("Platform: %s\n", platforms[i].comment);
			stream_print_string_hex(&stream,
						"Code:", platforms[i].code,
						platforms[i].size);
			printf("ERROR: Failed to disasm given code!\n");
		}

		cs_close(&handle);
	}
}

int main()
{
	test();

	return 0;
}
