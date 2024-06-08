#include "cstool.h"
#include "priv.h"

void print_insn_detail(csh handle, cs_arch arch, cs_mode md, cs_insn *ins,
		       Stream *steam)
{
	func_print_insn_detail print_detail = NULL;
	printf("\tID: %u (%s)\n", ins->id, cs_insn_name(handle, ins->id));
	if (ins->is_alias) {
		printf("\tIs alias: %" PRIu64 " (%s) ", ins->alias_id,
		       cs_insn_name(handle, ins->alias_id));
		printf("with %s operand set\n",
		       ins->usesAliasDetails ? "ALIAS" : "REAL");
	}

	switch (arch) {
	case CS_ARCH_X86:
		print_detail = print_insn_detail_x86;
		break;
	case CS_ARCH_ARM:
		print_detail = print_insn_detail_arm;
		break;
	case CS_ARCH_AARCH64:
		print_detail = print_insn_detail_aarch64;
		break;
	case CS_ARCH_MIPS:
		print_detail = print_insn_detail_mips;
		break;
	case CS_ARCH_PPC:
		print_detail = print_insn_detail_ppc;
		break;
	case CS_ARCH_SPARC:
		print_detail = print_insn_detail_sparc;
		break;
	case CS_ARCH_SYSZ:
		print_detail = print_insn_detail_sysz;
		break;
	case CS_ARCH_XCORE:
		print_detail = print_insn_detail_xcore;
		break;
	case CS_ARCH_M68K:
		print_detail = print_insn_detail_m68k;
		break;
	case CS_ARCH_TMS320C64X:
		print_detail = print_insn_detail_tms320c64x;
		break;
	case CS_ARCH_M680X:
		print_detail = print_insn_detail_m680x;
		break;
	case CS_ARCH_EVM:
		print_detail = print_insn_detail_evm;
		break;
	case CS_ARCH_WASM:
		print_detail = print_insn_detail_wasm;
		break;
	case CS_ARCH_MOS65XX:
		print_detail = print_insn_detail_mos65xx;
		break;
	case CS_ARCH_BPF:
		print_detail = print_insn_detail_bpf;
		break;
	case CS_ARCH_RISCV:
		print_detail = print_insn_detail_riscv;
		break;
	case CS_ARCH_SH:
		print_detail = print_insn_detail_sh;
		break;
	case CS_ARCH_TRICORE:
		print_detail = print_insn_detail_tricore;
		break;
	case CS_ARCH_ALPHA:
		print_detail = print_insn_detail_alpha;
		break;
	case CS_ARCH_HPPA:
		print_detail = print_insn_detail_hppa;
		break;
	case CS_ARCH_XTENSA: {
		print_detail = print_insn_detail_xtensa;
		break;
	}
	default:
		break;
	}

	if (print_detail) {
		print_detail(handle, ins, steam);
	} else {
		assert(0);
	}

	if (ins->detail && ins->detail->groups_count) {
		int j;

		printf("\tGroups: ");
		for (j = 0; j < ins->detail->groups_count; j++) {
			printf("%s ",
			       cs_group_name(handle, ins->detail->groups[j]));
		}
		printf("\n");
	}

	printf("\n");
}
