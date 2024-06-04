#ifndef CAPSTONE_XTENSA_H
#define CAPSTONE_XTENSA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cs_operand.h"
#include "platform.h"

/// Xtensa registers
typedef enum xtensa_reg {
	// generated content <XtensaGenCSRegEnum.inc> begin
	// clang-format off

	Xtensa_REG_INVALID = 0,
	Xtensa_REG_SAR = 1,
	Xtensa_REG_SP = 2,
	Xtensa_REG_A0 = 3,
	Xtensa_REG_A2 = 4,
	Xtensa_REG_A3 = 5,
	Xtensa_REG_A4 = 6,
	Xtensa_REG_A5 = 7,
	Xtensa_REG_A6 = 8,
	Xtensa_REG_A7 = 9,
	Xtensa_REG_A8 = 10,
	Xtensa_REG_A9 = 11,
	Xtensa_REG_A10 = 12,
	Xtensa_REG_A11 = 13,
	Xtensa_REG_A12 = 14,
	Xtensa_REG_A13 = 15,
	Xtensa_REG_A14 = 16,
	Xtensa_REG_A15 = 17,
	Xtensa_REG_ENDING, // 18

	// clang-format on
	// generated content <XtensaGenCSRegEnum.inc> end
} xtensa_reg;

/// Xtensa registers
typedef enum xtensa_insn {
	// generated content <XtensaGenCSInsnEnum.inc> begin
	// clang-format off

  Xtensa_INS_INVALID,
	Xtensa_INS_ABS,
	Xtensa_INS_ADD,
	Xtensa_INS_ADDI,
	Xtensa_INS_ADDMI,
	Xtensa_INS_ADDX2,
	Xtensa_INS_ADDX4,
	Xtensa_INS_ADDX8,
	Xtensa_INS_AND,
	Xtensa_INS_BALL,
	Xtensa_INS_BANY,
	Xtensa_INS_BBC,
	Xtensa_INS_BBCI,
	Xtensa_INS_BBS,
	Xtensa_INS_BBSI,
	Xtensa_INS_BEQ,
	Xtensa_INS_BEQI,
	Xtensa_INS_BEQZ,
	Xtensa_INS_BGE,
	Xtensa_INS_BGEI,
	Xtensa_INS_BGEU,
	Xtensa_INS_BGEUI,
	Xtensa_INS_BGEZ,
	Xtensa_INS_BLT,
	Xtensa_INS_BLTI,
	Xtensa_INS_BLTU,
	Xtensa_INS_BLTUI,
	Xtensa_INS_BLTZ,
	Xtensa_INS_BNALL,
	Xtensa_INS_BNE,
	Xtensa_INS_BNEI,
	Xtensa_INS_BNEZ,
	Xtensa_INS_BNONE,
	Xtensa_INS_CALL0,
	Xtensa_INS_CALLX0,
	Xtensa_INS_DSYNC,
	Xtensa_INS_ESYNC,
	Xtensa_INS_EXTUI,
	Xtensa_INS_EXTW,
	Xtensa_INS_ISYNC,
	Xtensa_INS_J,
	Xtensa_INS_JX,
	Xtensa_INS_L16SI,
	Xtensa_INS_L16UI,
	Xtensa_INS_L32I,
	Xtensa_INS_L32R,
	Xtensa_INS_L8UI,
	Xtensa_INS_MEMW,
	Xtensa_INS_MOVEQZ,
	Xtensa_INS_MOVGEZ,
	Xtensa_INS_MOVI,
	Xtensa_INS_MOVLTZ,
	Xtensa_INS_MOVNEZ,
	Xtensa_INS_NEG,
	Xtensa_INS_NOP,
	Xtensa_INS_OR,
	Xtensa_INS_RET,
	Xtensa_INS_RSR,
	Xtensa_INS_RSYNC,
	Xtensa_INS_S16I,
	Xtensa_INS_S32I,
	Xtensa_INS_S8I,
	Xtensa_INS_SLL,
	Xtensa_INS_SLLI,
	Xtensa_INS_SRA,
	Xtensa_INS_SRAI,
	Xtensa_INS_SRC,
	Xtensa_INS_SRL,
	Xtensa_INS_SRLI,
	Xtensa_INS_SSA8L,
	Xtensa_INS_SSAI,
	Xtensa_INS_SSL,
	Xtensa_INS_SSR,
	Xtensa_INS_SUB,
	Xtensa_INS_SUBX2,
	Xtensa_INS_SUBX4,
	Xtensa_INS_SUBX8,
	Xtensa_INS_WSR,
	Xtensa_INS_XOR,
	Xtensa_INS_XSR,

	// clang-format on
	// generated content <XtensaGenCSInsnEnum.inc> end
} xtensa_insn;

typedef enum xtensa_feature {
	Xtensa_GRP_INVALID = 0,
	Xtensa_GRP_CALL,
	Xtensa_GRP_JUMP,
	Xtensa_GRP_RET,
	// generated content <XtensaGenCSFeatureEnum.inc> begin
	// clang-format off

Xtensa_FEATURE_HasDensity = 128,

	// clang-format on
	// generated content <XtensaGenCSFeatureEnum.inc> end
	Xtensa_GRP_ENDING, ///< mark the end of the list of features
} xtensa_feature;

typedef enum cs_xtensa_op_type {
	Xtensa_OP_INVALID = CS_OP_INVALID, ///< = CS_OP_INVALID (Uninitialized).
	Xtensa_OP_REG = CS_OP_REG,	   ///< = CS_OP_REG (Register operand).
	Xtensa_OP_IMM = CS_OP_IMM,	   ///< = CS_OP_IMM (Immediate operand).
	Xtensa_OP_MEM = CS_OP_MEM,	   ///< = CS_OP_MEM (Memory operand).
} cs_xtensa_op_type;

typedef struct cs_xtensa_op_mem {
	uint8_t base;
	uint8_t index;
} cs_xtensa_op_mem;

typedef struct cs_xtensa_operand {
	uint8_t type;
	uint8_t access;

	union {
		uint8_t reg;
		int32_t imm;
		cs_xtensa_op_mem mem;
	};
} cs_xtensa_op;

#define MAX_XTENSA_OPS 8

typedef struct cs_xtensa {
	uint8_t op_count;
	cs_xtensa_op operands[MAX_XTENSA_OPS];
} cs_xtensa;

#ifdef __cplusplus
}
#endif

#endif
