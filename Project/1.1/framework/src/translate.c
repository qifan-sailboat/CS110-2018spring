#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tables.h"
#include "translate_utils.h"
#include "translate.h"

/* Writes instructions during the assembler's first pass to OUTPUT. The case
   for general instructions has already been completed, but you need to write
   code to translate the li, bge and move pseudoinstructions. Your pseudoinstruction 
   expansions should not have any side effects.

   NAME is the name of the instruction, ARGS is an array of the arguments, and
   NUM_ARGS specifies the number of items in ARGS.

   Error checking for regular instructions are done in pass two. However, for
   pseudoinstructions, you must make sure that ARGS contains the correct number
   of arguments. You do NOT need to check whether the registers / label are 
   valid, since that will be checked in part two.

   Also for li:
    - make sure that the number is representable by 32 bits. (Hint: the number 
        can be both signed or unsigned).
    - if the immediate can fit in the imm field of an addiu instruction, then
        expand li into a single addiu instruction. Otherwise, expand it into 
        a lui-ori pair.

   And for bge and move:
    - your expansion should use the fewest number of instructions possible.

   MARS has slightly different translation rules for li, and it allows numbers
   larger than the largest 32 bit number to be loaded with li. You should follow
   the above rules if MARS behaves differently.

   Use fprintf() to write. If writing multiple instructions, make sure that 
   each instruction is on a different line.

   Returns the number of instructions written (so 0 if there were any errors).
 */
unsigned write_pass_one(FILE* output, const char* name, char** args, int num_args) {

  if (!output || !name || !args)
    return 0;


  if (strcmp(name, "li") == 0) {
    long int imm = strtol(args[1], NULL, 0);

    if (num_args != 2 || imm < -2147483648 || imm > 4294967295){
      /* Beyond the range of 32 bit */
      return 0;
    }else if (imm >= -32768 && imm <= 65535){
      /* Within the range of 16 bit, we need to optimize by using "addiu" */
      fprintf(output, "%s %s %s %s\n", "addiu", args[0], "$0", args[1]);
      /* addiu $1, $0, imm */
      return 1;
    }else{
      /* between 16 bit and 32 bit */
      uint16_t imm_p;
      /* We need a complete 16-bit unsigned integer to store the imm num*/
      imm_p = imm >> 16;
      /* Ignore last 16-bit and get the first 16-bit */
      fprintf(output, "%s %s %d\n", "lui", "$at", imm_p);
      /* first 16 bit added*/
      imm_p = imm & (0xffff);
      /* It means replace first 16-bit with 16-bit 0s */
      fprintf(output, "%s %s %s %d\n", "ori", args[0], "$at", imm_p);
      /* last 16 bit added */
      /* imm >> 16 << 16: replace the last 16 bit with 16 0s. */
      return 2;
    };
    
  }else if (strcmp(name, "bge") == 0) {
    /* YOUR CODE HERE */
    if (num_args != 3) return 0;
    fprintf(output, "%s %s %s %s\n", "slt", "$at", args[0], args[1]);
    /* slt $at 1 2 */
    fprintf(output, "%s %s %s %s\n", "beq", "$at", "$0", args[2]);
    /* beq $at $zero label */
    return 2;
    
  } else if (strcmp(name, "move") == 0 ) {
    /* YOUR CODE HERE */
    if (num_args != 2) return 0;
    fprintf(output, "%s %s %s %s\n", "addu", args[0], "$0", args[1]);
    /* addiu $1 $2 0 */
    return 1;
  }else {
    write_inst_string(output, name, args, num_args);
    return 1;
  }
}

/* Writes the instruction in hexadecimal format to OUTPUT during pass #2.
   
   NAME is the name of the instruction, ARGS is an array of the arguments, and
   NUM_ARGS specifies the number of items in ARGS. 

   The symbol table (SYMTBL) is given for any symbols that need to be resolved
   at this step. If a symbol should be relocated, it should be added to the
   relocation table (RELTBL), and the fields for that symbol should be set to
   all zeros. 

   You must perform error checking on all instructions and make sure that their
   arguments are valid. If an instruction is invalid, you should not write 
   anything to OUTPUT but simply return -1. MARS may be a useful resource for
   this step.

   Note the use of helper functions. Consider writing your own! If the function
   definition comes afterwards, you must declare it first (see translate.h).

   Returns 0 on success and -1 on error. 
 */
int translate_inst(FILE* output, const char* name, char** args, size_t num_args, uint32_t addr, SymbolTable* symtbl, SymbolTable* reltbl) {
  /* this is a table implements the opcode / funct to functions and machine code change functions */
    if (strcmp(name, "addu") == 0)       return write_rtype (0x21, output, args, num_args);
    /* addu 0x21, r */
    else if (strcmp(name, "or") == 0)    return write_rtype (0x25, output, args, num_args);
    /* or 0x25, r */
    else if (strcmp(name, "sll") == 0)   return write_shift (0x0, output, args, num_args);
    /* all 0x0, r */
    else if (strcmp(name, "slt") == 0)   return write_rtype (0x2a, output, args, num_args);
    /* slt 0x2a, r */
    else if (strcmp(name, "sltu") == 0)  return write_rtype (0x2b, output, args, num_args);
    /* sltu 0x2b, r */
    else if (strcmp(name, "jr") == 0)    return write_jr    (0x8, output, args, num_args);
    /* jr 0x8, i */
    else if (strcmp(name, "addiu") == 0) return write_addiu (0x9, output, args, num_args);
    /* addiu 0x9, i */
    else if (strcmp(name, "ori") == 0)   return write_ori   (0xd, output, args, num_args);
    /* ori 0xd, i */
    else if (strcmp(name, "lui") == 0)   return write_lui   (0xf, output, args, num_args);
    /* lui 0xf, i */
    else if (strcmp(name, "lb") == 0)    return write_mem   (0x20, output, args, num_args);
    /* lb 0x20, r */
    else if (strcmp(name, "lbu") == 0)   return write_mem   (0x24, output, args, num_args);
    /* lbu 0x24, r */
    else if (strcmp(name, "lw") == 0)    return write_mem   (0x23, output, args, num_args);
    /* lw 0x23, r */
    else if (strcmp(name, "sb") == 0)    return write_mem   (0x28, output, args, num_args);
    /* sb 0x28, r */
    else if (strcmp(name, "sw") == 0)    return write_mem   (0x2b, output, args, num_args);
    /* sw 0x2b, r */
    else if (strcmp(name, "beq") == 0)   return write_branch(0x4, output, args, num_args, addr, symtbl);
    /* beq 0x4, i */
    else if (strcmp(name, "bne") == 0)   return write_branch(0x5, output, args, num_args, addr, symtbl);
    /* bne 0x5, i */
    else if (strcmp(name, "j") == 0)     return write_jump  (0x2, output, args, num_args, addr, reltbl);
    /* j 0x2, i */
    else if (strcmp(name, "jal") == 0)   return write_jump  (0x3, output, args, num_args, addr, reltbl);
    /* jal 0x3, i */

    else                                 return -1;
}

/* A helper function for writing most R-type instructions. You should use
   translate_reg() to parse registers and write_inst_hex() to write to 
   OUTPUT. Both are defined in translate_utils.h.

   This function is INCOMPLETE. Complete the implementation below. You will
   find bitwise operations to be the cleanest way to complete this function.
 */
int write_rtype(uint8_t funct, FILE* output, char** args, size_t num_args) {
  /*  Perhaps perform some error checking?  */
  int rd, rs, rt;
  uint32_t instruction;
  /* varibles */

  if (num_args != 3) return -1;
  /* For this r typr instructions, there should be 3 arguments */
  /* rd rs rt */

  rd = translate_reg(args[0]);
  rs = translate_reg(args[1]);
  rt = translate_reg(args[2]);

  
  
  /* YOUR CODE HERE */

  /* error checking for register ids */

  /* generate instruction */
  instruction = (rs << 21) + (rt << 16) + (rd << 11) + funct;
  /* According to the Green sheet guideline */
  if (rd == -1 || rs == -1 || rt == -1) return -1;
  /* if any reg is invalid, it is invalid */

  write_inst_hex(output, instruction);
  return 0;
}

/* A helper function for writing shift instructions. You should use 
   translate_num() to parse numerical arguments. translate_num() is defined
   in translate_utils.h.

   This function is INCOMPLETE. Complete the implementation below. You will
   find bitwise operations to be the cleanest way to complete this function.
 */
int write_shift(uint8_t funct, FILE* output, char** args, size_t num_args) {
  /* Perhaps perform some error checking? */
  long int shamt;
  int rd ;
  int rt ;
  int err;
  uint32_t instruction;
  /* variables */

  if (num_args != 3) return -1;
  /* For this r typr instructions, there should be 3 arguments */

  rd = translate_reg(args[0]);
  rt = translate_reg(args[1]);
  err = translate_num(&shamt, args[2], 31, 0);
  /* err is used to record whether the num is translated successfully. */
  
  /* YOUR CODE HERE */
  if (err == -1 || rd == -1 || rt == -1) return -1;
  /* if any reg is invalid, it is invalid */
  /* error check for register id */

  /* generate instruction */
  instruction = (rt << 16) + (rd << 11) + (shamt << 6) + funct;
  /* According to the Green sheet guideline */
  write_inst_hex(output, instruction);
  return 0;
}

int write_jr(uint8_t funct, FILE* output, char** args, size_t num_args) {
  /* YOUR CODE HERE */
  int rs;
  uint32_t instruction;
  /* varibles */

  if (num_args != 1) return -1;
  /* there should be only one element */
  rs = translate_reg(args[0]);
  /* translate the registers */

  if (rs == -1) return -1;
  /* if any reg is invalid, it is invalid */

  instruction = (rs << 21) + funct;
  /* According to the Green sheet guideline */
  write_inst_hex(output, instruction);

  return 0;
}

int write_addiu(uint8_t opcode, FILE* output, char** args, size_t num_args) {
  /* YOUR CODE HERE */
  long int imm;
  uint16_t imm_16;
  int rt;
  int rs;
  int err;
  uint32_t instruction;
  /* varibles */

  if (num_args != 3) return -1;

  rt = translate_reg(args[0]);
  rs = translate_reg(args[1]);
  err = translate_num(&imm, args[2], 32767, -32768);
  /* translate each arg. */
  imm_16 = imm;
  /* force the last 16 imm number to be 16-bit long. */

  if (err == -1 || rt == -1 || rs == -1) return -1;
  /* if there is one element is invalid, it is invalid. */

  instruction = (opcode << 26) + (rs << 21) + (rt << 16) + imm_16;
   /* According to the Green sheet guideline */
  write_inst_hex(output, instruction);
  return 0;
}

int write_ori(uint8_t opcode, FILE* output, char** args, size_t num_args) {
  /* YOUR CODE HERE */
  long int imm;
  uint16_t imm_16;
  int rt;
  int rs;
  int err;
  uint32_t instruction;
  /* varibles */

  if (num_args != 3) return -1;
  /* There should be only 3 args */

  rt = translate_reg(args[0]);
  rs = translate_reg(args[1]);
  err = translate_num(&imm, args[2], 65535, 0);
  /* translate each arg. */
  imm_16 = imm;
  /* force the last 16 imm number to be 16-bit long. */
  if (err == -1 || rt == -1 || rs == -1) return -1;
  /* if there is one element is invalid, it is invalid. */

  instruction = (opcode << 26) + (rs << 21) + (rt << 16) + imm_16;
  /* According to the Green sheet guideline */
  write_inst_hex(output, instruction);
  return 0;
}

int write_lui(uint8_t opcode, FILE* output, char** args, size_t num_args) {
  /* YOUR CODE HERE */
  long int imm;
  uint16_t imm_16;
  int rt;
  int err;  
  uint32_t instruction;
  /* varibles */

  if (num_args != 2) return -1;
  /* There should be only 2 args */

  rt = translate_reg(args[0]);
  err = translate_num(&imm, args[1], 65535, 0);
  /* translate each arg. */
  imm_16 = imm;
  /* force the last 16 imm number to be 16-bit long. */

  if (err == -1 || rt == -1) return -1;
  /* if there is one element is invalid, it is invalid. */

  instruction = (opcode << 26) + (rt << 16) + imm_16;
  /* According to the Green sheet guideline */
  write_inst_hex(output, instruction);
  return 0 ;
}


int write_mem(uint8_t opcode, FILE* output, char** args, size_t num_args) {
  /* YOUR CODE HERE */
  uint32_t instruction;
  long int offset;
  uint16_t offset_16;
  int rt;
  int rs;
  int err;
  /* varibles */

  if (num_args != 3) return -1;
  /* There should be only 3 args */

  rt = translate_reg(args[0]);
  rs = translate_reg(args[2]);
  err = translate_num(&offset, args[1], 32767, -32768);
  /* translate each arg. */
  offset_16 = offset;
  /* force the last 16 imm number to be 16-bit long. */

  if (err == -1 || rs == -1 || rt == -1) return -1;
  /* if there is one element is invalid, it is invalid. */

  instruction = (opcode << 26) + (rs << 21) + (rt << 16) + offset_16;
  /* According to the Green sheet guideline */
  write_inst_hex(output, instruction);

  return 0;
}

/* Hint: the way for branch to calculate relative address. e.g. bne
     bne $rs $rt label
   assume the byte_offset(addr) of label is L, 
   current instruction byte_offset(addr) is A
   the relative address I  for label satisfy:
     L = (A + 4) + I * 4
   so the relative addres is 
     I = (L - A - 4) / 4;  */
int write_branch(uint8_t opcode, FILE* output, char** args, size_t num_args, 
		 uint32_t addr, SymbolTable* symtbl) {
  /* YOUR CODE HERE */
    int rt;
    int rs;
    char* temp;
    int64_t target_addr;
    uint16_t gap;
    uint32_t instruction;
    /* varibles */

    if (num_args != 3) return -1;
    /* There should be only 3 args */

    rt = translate_reg(args[1]);
    rs = translate_reg(args[0]);
    temp = args[2];
    /* translate each arg. */

    target_addr = get_addr_for_symbol(symtbl, temp);
    /* search for the address we want to jump to. */

    gap = (target_addr - addr - 4) / 4;
    /* This is the distance between where we jump and where we jump to. */

    if (target_addr == -1 || rs == -1 || rt == -1) return -1;
    /* if there is one element is invalid, it is invalid. */

    instruction = (opcode << 26) + (rs << 21) + (rt << 16) + gap;
    /* According to the Green sheet guideline */
    write_inst_hex(output, instruction);

    return 0;
}

/* Hint: the relocation table should record
   1. the current instruction byte_offset(addr)
   2. the unsolved LABEL in the jump instruction  */
int write_jump(uint8_t opcode, FILE* output, char** args, size_t num_args, 
	       uint32_t addr, SymbolTable* reltbl) {
  /* YOUR CODE HERE */
    char* temp = args[0];
    int64_t target_addr;
    uint32_t instruction;
    /* varibles */

    if (num_args != 1) return -1;
    /* There should be only one element, or it will be invalid. */

    target_addr = add_to_table(reltbl, temp, addr);
    /* add the address and label to the Symbol table storing them */
    if (target_addr == -1) return -1;
    /* if there is one element is invalid, it is invalid. */

    instruction = (opcode << 26);
    /* According to the Green sheet guideline */
    write_inst_hex(output, instruction);
    return 0;

}
