/* Capstone testing regression */
/* By Do Minh Tuan <tuanit96@gmail.com>, 02-2019 */

#include "../../../cs_priv.h"
#include "cstool.h"
#include "capstone_test.h"

void test_single_MC(csh *handle, int mc_mode, char *line)
{
	char **list_part, **list_byte;
	int size_part, size_byte;
	int i, count;
	unsigned char *code;
	cs_insn *insn;
	char tmp[MAXMEM], tmp_mc[MAXMEM], origin[MAXMEM], tmp_noreg[MAXMEM];
	char **offset_opcode;
	int size_offset_opcode;
	unsigned long offset;
	char *p;

	list_part = split(line, " = ", &size_part);
	if (size_part <= 1) {
		free_strs(list_part, size_part);
		return;
	}

	offset_opcode = split(list_part[0], ": ", &size_offset_opcode);
	if (size_offset_opcode > 1) {
		offset = (unsigned int)strtol(offset_opcode[0], NULL, 16);
		list_byte = split(offset_opcode[1], ",", &size_byte);
	} else {
		offset = 0;
		list_byte = split(offset_opcode[0], ",", &size_byte);
	}

	code = (unsigned char *)malloc(size_byte * sizeof(char));
	for (i = 0; i < size_byte; ++i) {
		code[i] = (unsigned char)strtol(list_byte[i], NULL, 16);
	}

	((struct cs_struct *)(uintptr_t)*handle)->PrintBranchImmNotAsAddress =
		true;
	count = cs_disasm(*handle, code, size_byte, offset, 0, &insn);
	if (count == 0) {
		fprintf(stderr,
			"[  ERROR   ] --- %s --- Failed to disassemble given code!\n",
			list_part[0]);
		free_strs(list_part, size_part);
		free_strs(offset_opcode, size_offset_opcode);
		free_strs(list_byte, size_byte);
		free(code);
		_fail(__FILE__, __LINE__);
	}
	if (count > 1) {
		fprintf(stderr,
			"[  ERROR   ] --- %s --- Multiple instructions(%d) disassembling doesn't support!\n",
			list_part[0], count);
		free_strs(list_part, size_part);
		free_strs(offset_opcode, size_offset_opcode);
		free_strs(list_byte, size_byte);
		free(code);
		_fail(__FILE__, __LINE__);
	}

	for (p = list_part[1]; *p; ++p)
		*p = tolower(*p);
	for (p = list_part[1]; *p; ++p)
		if (*p == '\t')
			*p = ' ';
	trim_str(list_part[1]);
	strcpy(tmp_mc, list_part[1]);
	replace_hex(tmp_mc);
	replace_negative(tmp_mc, mc_mode);
	replace_tabs(tmp_mc);

	strcpy(tmp, insn[0].mnemonic);
	if (strlen(insn[0].op_str) > 0) {
		tmp[strlen(insn[0].mnemonic)] = ' ';
		strcpy(tmp + strlen(insn[0].mnemonic) + 1, insn[0].op_str);
	}

	trim_str(tmp);
	strcpy(origin, tmp);
	replace_hex(tmp);
	replace_negative(tmp, mc_mode);
	replace_tabs(tmp);
	for (p = tmp; *p; ++p)
		*p = tolower(*p);

	// Skip ARM because the duplicate disassembly messes with the IT/VPT states
	// and laeds to wrong results.
	cs_arch arch = ((struct cs_struct *)(uintptr_t)*handle)->arch;
	if (arch != CS_ARCH_ARM) {
		if (insn->detail) {
			free(insn->detail);
		}
		free(insn);
		cs_disasm(*handle, code, size_byte, offset, 0, &insn);

		strcpy(tmp_noreg, insn[0].mnemonic);
		if (strlen(insn[0].op_str) > 0) {
			tmp_noreg[strlen(insn[0].mnemonic)] = ' ';
			strcpy(tmp_noreg + strlen(insn[0].mnemonic) + 1,
			       insn[0].op_str);
		}

		trim_str(tmp_noreg);
		replace_hex(tmp_noreg);
		replace_negative(tmp_noreg, mc_mode);

		if (strcmp(tmp, tmp_mc) && strcmp(tmp_noreg, tmp_mc)) {
			fprintf(stderr,
				"[  ERROR   ] --- %s --- \"%s\" != \"%s\" ( \"%s\" != \"%s\" and \"%s\" != \"%s\" )\n",
				list_part[0], origin, list_part[1], tmp, tmp_mc,
				tmp_noreg, tmp_mc);
			free_strs(list_part, size_part);
			free_strs(offset_opcode, size_offset_opcode);
			free_strs(list_byte, size_byte);
			free(code);
			cs_free(insn, count);
			_fail(__FILE__, __LINE__);
		}
	} else if (strcmp(tmp, tmp_mc)) {
		fprintf(stderr,
			"[  ERROR   ] --- %s --- \"%s\" != \"%s\" ( \"%s\" != \"%s\" )\n",
			list_part[0], origin, list_part[1], tmp, tmp_mc);
		free_strs(list_part, size_part);
		free_strs(offset_opcode, size_offset_opcode);
		free_strs(list_byte, size_byte);
		free(code);
		cs_free(insn, count);
		_fail(__FILE__, __LINE__);
	}

	free_strs(list_part, size_part);
	free_strs(offset_opcode, size_offset_opcode);
	free_strs(list_byte, size_byte);
	free(code);
	cs_free(insn, count);
}

int get_value(single_dict d[], unsigned int size, const char *str)
{
	int i;

	for (i = 0; i < size; ++i)
		if (!strcmp(d[i].str, str))
			return d[i].value;
	return -1;
}

int get_index(double_dict d[], unsigned int size, const char *s)
{
	int i;

	for (i = 0; i < size; ++i) {
		if (!strcmp(s, d[i].str))
			return i;
	}
	return -1;
}

void test_single_issue(csh *handle, cs_mode mode, char *line, int detail)
{
	char **list_part, **list_byte, **list_part_issue_result;
	int size_part, size_byte, size_part_issue_result;
	int i, count, j;
	unsigned char *code;
	cs_insn *insn;
	char *p;
	char **offset_opcode;
	int size_offset_opcode;
	unsigned long offset;

	Stream steam = { 0 };
	stream_init_stream(&steam);
	list_part = split(line, " == ", &size_part);

	offset_opcode = split(list_part[0], ": ", &size_offset_opcode);
	if (size_offset_opcode > 1) {
		offset = (unsigned int)strtol(offset_opcode[0], NULL, 16);
		list_byte = split(offset_opcode[1], ",", &size_byte);
	} else {
		offset = 0;
		list_byte = split(offset_opcode[0], ",", &size_byte);
	}
	free_strs(offset_opcode, size_offset_opcode);

	code = (unsigned char *)malloc(sizeof(char) * size_byte);
	for (i = 0; i < size_byte; ++i) {
		code[i] = (unsigned char)strtol(list_byte[i], NULL, 16);
	}

	count = cs_disasm(*handle, code, size_byte, offset, 0, &insn);
	free_strs(list_byte, size_byte);
	free(code);
	for (i = 0; i < count; ++i) {
		stream_print(&steam, insn[i].mnemonic);
		if (strlen(insn[i].op_str) > 0) {
			stream_printf(&steam, " %s", insn[i].op_str);
		}
		if (detail && insn->detail) {
			stream_print(&steam, "\n");
			print_insn_detail(*handle, ((cs_struct *)*handle)->arch,
					  ((cs_struct *)*handle)->mode,
					  insn + i, &steam);
		}
	}

	if (detail == 1 && insn->detail) {
		if (insn->detail->groups_count) {
			stream_print(&steam, " ; Groups: ");
			for (j = 0; j < insn->detail->groups_count; j++) {
				stream_printf(
					&steam, "%s ",
					cs_group_name(*handle,
						      insn->detail->groups[j]));
			}
		}
	}

	//	list_part_cs_result = split(cs_result, " ; ", &size_part_cs_result);
	for (p = list_part[1]; *p; ++p)
		if (*p == '\t')
			*p = ' ';
	list_part_issue_result =
		split(list_part[1], " ; ", &size_part_issue_result);

	for (i = 0; i < size_part_issue_result; ++i) {
		trim_str(list_part_issue_result[i]);

		if ((strstr(stream_as_str(&steam),
			    list_part_issue_result[i])) == NULL) {
			fprintf(stderr,
				"[  ERROR   ] --- %s --- \"%s\" not in \"%s\"\n",
				list_part[0], list_part_issue_result[i],
				stream_as_str(&steam));
			cs_free(insn, count);
			free_strs(list_part, size_part);
			stream_close(&steam);
			//	free_strs(list_part_cs_result, size_part_cs_result);
			free_strs(list_part_issue_result,
				  size_part_issue_result);
			_fail(__FILE__, __LINE__);
		}
	}

	cs_free(insn, count);
	free_strs(list_part, size_part);
	stream_close(&steam);
	//	free_strs(list_part_cs_result, size_part_cs_result);
	free_strs(list_part_issue_result, size_part_issue_result);
}
