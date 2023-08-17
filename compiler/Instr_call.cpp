#include "Instr_call.h"
using namespace std;


/**< x86 assembly code generation for this IR instruction */
void Instr_call::gen_asm_x86(ostream &o){
	o << "	subq  $" << nbOctetsUtilises <<", \%rsp\n";
	o << "	movl  $0, \%eax\n";
	o << "	call  " << nomFonction << "\n";
	o << "	movl  \%eax, -" << dest << "(\%rbp)\n";
}

// virtual void gen_asm_arm(ostream &o); /**< arm assembly code generation for this IR instruction */
// virtual void gen_asm_msp430(ostream &o); /**< msp430 assembly code generation for this IR instruction */
