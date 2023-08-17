#include "Instr_xor_bin.h"
using namespace std;


/**< x86 assembly code generation for this IR instruction */
void Instr_xor_bin::gen_asm_x86(ostream &o){
	o << "	movl -" << x << "(\%rbp), \%eax\n";
	o << "	xorl -" << y << "(\%rbp), \%eax\n";
	o << " 	movl " << "\%eax, -" << dest << "(\%rbp)\n";
}


// virtual void gen_asm_arm(ostream &o); /**< arm assembly code generation for this IR instruction */
// virtual void gen_asm_msp430(ostream &o); /**< msp430 assembly code generation for this IR instruction */


