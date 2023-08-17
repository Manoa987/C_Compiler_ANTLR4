/** The class for the control flow graph, also includes the symbol table */

/* A few important comments:
	 The entry block is the one with the same label as the AST function name.
	   (it could be the first of bbs, or it could be defined by an attribute value)
	 The exit block is the one with both exit pointers equal to nullptr.
     (again it could be identified in a more explicit way)

 */
#ifndef CFG_H
#define CFG_H

#include <vector>
#include <string>
#include <initializer_list>
#include <map>
#include "BasicBlock.h"
#include "Types.h"

class BasicBlock;
using namespace std;

class CFG {
 public:
	CFG(string nom): nomCFG(nom){};
	void add_bb(BasicBlock* bb); 

	// x86 code generation: could be encapsulated in a processor class in a retargetable compiler
	void gen_asm_x86(ostream& o);
	void gen_asm_arm(ostream& o);
	void gen_asm_msp430(ostream& o);
	//string IR_reg_to_asm_x86(string reg); /**< helper method: inputs a IR reg or input variable, returns e.g. "-24(%rbp)" for the proper value of 24 */
	void gen_asm_x86_prologue(ostream& o);
	void gen_asm_x86_epilogue(ostream& o);
	void gen_asm_arm_prologue(ostream &o);
	void gen_asm_arm_epilogue(ostream &o);
	void gen_asm_msp430_prologue(ostream &o);
	void gen_asm_msp430_epilogue(ostream &o);

	// symbol table methods
	void add_to_symbol_table(string name, Type t);
	string create_new_tempvar(Type t);
	int get_var_index(string name);
	Type get_var_type(string name);
	void setVariableUtilisee(string var);
	void verifierAffectationSymboles();

	// basic block management
	BasicBlock* current_bb;
	string newBlockLabel();
	vector <BasicBlock*> bbs; /**< all the basic blocks of this CFG*/
	BasicBlock* findBlock(string label);

 protected:	
	map <string, Type> SymbolType; /**< part of the symbol table  */
	map <string, int> SymbolIndex; /**< part of the symbol table  */
	int BBnumberIndex = 1; /**< just for naming */
	int lastIndex = 0;
	int compteurRegistre = 0;
	map<string, bool> tableSymbolesUtilises;
	string nomCFG;
};

#endif