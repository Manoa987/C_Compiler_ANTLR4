#include "CFG.h"
using namespace std;

void CFG::add_bb(BasicBlock *bb)
{
	bbs.push_back(bb);
}

// x86 code generation: could be encapsulated in a processor class in a retargetable compiler
void CFG::gen_asm_x86(ostream &o)
{
	gen_asm_x86_prologue(o);
	for (BasicBlock* block : bbs)
	{
		if (block->label!="") o << block->label << ":\n";
		block->gen_asm_x86(o);
		if (block->exit_true != nullptr && block->exit_true->exit_true == nullptr) gen_asm_x86_epilogue(o);
	}
}

void CFG::gen_asm_arm(ostream &o)
{
	gen_asm_arm_prologue(o);
	for (BasicBlock* block : bbs)
	{
		if (block->label!="") o << block->label << ":\n";
		block->gen_asm_arm(o);
		if (block->exit_true != nullptr && block->exit_true->exit_true == nullptr) gen_asm_arm_epilogue(o);
	}
}

void CFG::gen_asm_msp430(ostream &o)
{
	gen_asm_msp430_prologue(o);
	for (BasicBlock* block : bbs)
	{
		if (block->label!="") o << block->label << ":\n";
		block->gen_asm_msp430(o);
		if (block->exit_true != nullptr && block->exit_true->exit_true == nullptr) gen_asm_msp430_epilogue(o);
	}
}
// string IR_reg_to_asm_x86(string reg); /**< helper method: inputs a IR reg or input variable, returns e.g. "-24(%rbp)" for the proper value of 24 */
void CFG::gen_asm_x86_prologue(ostream &o)
{
	o << ".globl	"<< nomCFG <<"\n"
			 " " << nomCFG << ": \n"
			 "	pushq %rbp \n"
			 "	movq %rsp, %rbp \n";
}
void CFG::gen_asm_x86_epilogue(ostream &o)
{
	o << "	leave \n"
			 " 	ret\n";
}

void CFG::gen_asm_arm_prologue(ostream &o)
{

}
void CFG::gen_asm_arm_epilogue(ostream &o)
{

}

void CFG::gen_asm_msp430_prologue(ostream &o)
{

}
void CFG::gen_asm_msp430_epilogue(ostream &o)
{

}

// symbol table methods
void CFG::add_to_symbol_table(string name, Type t) {
    if (SymbolIndex.find(name) == SymbolIndex.end())
    {
        switch (t){
            case _INT:
                lastIndex += 4; // +4 pour type int
                break;
            case _CHAR:
                lastIndex += 1;
			case _DOUBLE:
                lastIndex += 8;
            break;
        }
        SymbolIndex[name] = lastIndex;
        SymbolType[name] = t;
        tableSymbolesUtilises[name] = false;
    }
    else
    {
        throw runtime_error("Variable déjà définie dans la fonction");
    }
}

string CFG::create_new_tempvar(Type t)
{
	string key = "$" + to_string(compteurRegistre);
	compteurRegistre++;
	add_to_symbol_table(key, t);
	tableSymbolesUtilises[key] = true;
	return key;
}

int CFG::get_var_index(string name)
{
	auto var = SymbolIndex.find(name);
	if (var != SymbolIndex.end()) {
		return var->second;
	} else {
		throw runtime_error("Variable non déclarée");
	}
}

Type CFG::get_var_type(string name)
{
	Type t = SymbolType.find(name)->second;
	return t;
}

void CFG::setVariableUtilisee(string var) {
	tableSymbolesUtilises[var] = true;
}

void CFG::verifierAffectationSymboles() {
	map<string, bool>::iterator it;
	for (it = tableSymbolesUtilises.begin(); it != tableSymbolesUtilises.end(); ++it ) {
		if (it->second == false) cout << "#WARNING : Variable " << it->first << " declared but not used !" << endl;
	}
}

// basic block management
string CFG::newBlockLabel(){
	BBnumberIndex++;
	return ".L"+ to_string(BBnumberIndex);
}

BasicBlock* CFG::findBlock(string label) {
    for (auto block : bbs) {
        if (block->label == label) {
            return block;
        }
    }
    return nullptr;
}

