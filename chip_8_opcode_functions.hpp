
#ifndef _chip_8_opcode_functions_H__
#define _chip_8_opcode_functions_H__

#include "Chip_8.hpp"

class Chip_8;
using opcode_function_executer = void (*)(Chip_8 &);

void opcode_0_fn_sets(Chip_8 &);
void opcode_1_fn_sets(Chip_8 &);
void opcode_2_fn_sets(Chip_8 &);
void opcode_3_fn_sets(Chip_8 &);
void opcode_4_fn_sets(Chip_8 &);
void opcode_5_fn_sets(Chip_8 &);
void opcode_6_fn_sets(Chip_8 &);
void opcode_7_fn_sets(Chip_8 &);
void opcode_8_fn_sets(Chip_8 &);
void opcode_9_fn_sets(Chip_8 &);
void opcode_A_fn_sets(Chip_8 &);
void opcode_B_fn_sets(Chip_8 &);
void opcode_C_fn_sets(Chip_8 &);
void opcode_D_fn_sets(Chip_8 &);
void opcode_E_fn_sets(Chip_8 &);
void opcode_F_fn_sets(Chip_8 &);

extern const opcode_function_executer opcode_table[];



#endif /*  _chip_8_opcode_functions_H__ */