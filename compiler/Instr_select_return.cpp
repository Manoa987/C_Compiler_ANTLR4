#include "Instr_select_return.h"
using namespace std;


/**< x86 assembly code generation for this IR instruction */
void Instr_select_return::gen_asm_x86(ostream &o){
	o << "	movl -" << source << "(\%rbp), \%eax\n";
}


// virtual void gen_asm_arm(ostream &o); /**< arm assembly code generation for this IR instruction */
// virtual void gen_asm_msp430(ostream &o); /**< msp430 assembly code generation for this IR instruction */


