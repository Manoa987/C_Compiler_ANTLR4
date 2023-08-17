#include "Instr_opposite.h"
using namespace std;


/**< x86 assembly code generation for this IR instruction */
void Instr_opposite::gen_asm_x86(ostream &o){
	// DEMANDER COMMENT GERER LES CONST ET LES REGISTRES
	o << "	movl -" << source << "(\%rbp), \%eax\n";
	o << "	negl \%eax\n";
	o << " 	movl " << "\%eax, -" << dest << "(\%rbp)\n";
}


// virtual void gen_asm_arm(ostream &o); /**< arm assembly code generation for this IR instruction */
// virtual void gen_asm_msp430(ostream &o); /**< msp430 assembly code generation for this IR instruction */


