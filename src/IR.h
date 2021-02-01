//Nikola Milosavljević RA5-2018, Datum : 8.6.2020.
#ifndef __IR__
#define __IR__

#include "Types.h"
#include "LexicalAnalysis.h"


class FuncOrLab;
class Instruction;
class Variable;

//For functions and labels
class FuncOrLab
{
public:

	FuncOrLab() : m_type(N_TYPE), m_name(""), m_position(-1) {}
	FuncOrLab(std::string name, int pos) : m_type(N_TYPE), m_name(name), m_position(pos) {}
	FuncOrLab(FuncOrLabType v, std::string n, int pos) : m_type(v), m_name(n), m_position(pos) {}

	void setType(FuncOrLabType t);
	void setName(std::string s);
	void setPosition(int t);

	std::string getName();
	int getPosition();
	FuncOrLabType getType();

	void printer();

private:
	FuncOrLabType m_type;
	std::string m_name;
	int m_position;
};

typedef std::list<FuncOrLab*> FuncOrLabs;

/**
 * This class represents one variable from program code.
 */
class Variable
{
public:

	Variable() : m_type(NO_TYPE), m_name(""), m_position(-1), m_assignment(no_assign) {}
	Variable(std::string name, int pos) : m_type(NO_TYPE), m_name(name), m_position(pos), m_assignment(no_assign) {}
	Variable(VariableType v, std::string n, int pos, int val, Regs m_assignment) : m_type(v), m_name(n), m_position(pos), m_value(val), m_assignment(no_assign) {}

	void setType(VariableType t);
	void setName(std::string s);
	void setValue(int t);
	void setPosition(int t);
	void setAssignment(Regs r);
	void setPosIgMat(int t);

	std::string getName();
	VariableType getType();
	int getPosIgMat();
	Regs getAssignment();
	int getPosition();
	int getValue();

	void printer();

private:
	VariableType m_type;
	std::string m_name;
	int m_position;
	int m_value;
	Regs m_assignment;
	int m_pos_ig_mat;
};

/**
 * This type represents list of variables from program code.
 */
typedef std::list<Variable*> Variables;

struct ReturnValueVar
{
	Variables::iterator itb;
	Variables::iterator ite;

	int size;
};

struct ReturnValueIns
{
	std::list<Instruction*>::iterator itb;
	std::list<Instruction*>::iterator ite;

	int size;
};

/**
 * This class represents one instruction in program code.
 */
class Instruction
{
public:
	Instruction () : m_position(0), m_type(I_NO_TYPE) {}
	Instruction (int pos, InstructionType type, Variables& dst, Variables& src) :
		m_position(pos), m_type(type), m_dst(dst), m_src(src) {}
	Instruction (int pos, InstructionType type) : m_position(pos), m_type(type) {}

	void setInstString(std::string t);
	
	InstructionType getType();
	int getPosition();
	std::string getInstText();
	Variable* getFromSrc();
	FuncOrLab* getFuncOrLab();
	Variables getDest();
	Variables getSrc();
	Variables getDef();
	Variables getIn();
	Variables getOut();
	ReturnValueVar getItUse();
	ReturnValueVar getItOut();
	ReturnValueVar getItDef();
	ReturnValueVar getItIn();
	ReturnValueIns getItSucc();
	
	void pushDst(Variable *t);
	void pushSrc(Variable *t);
	void pushFuncOrLab(FuncOrLab *t);
	void pushSucc(Instruction* t);
	void pushPred(Instruction* t);
	void pushUse(Variable* t);
	void pushDef(Variable* t);
	void pushIn(Variable* t);
	void pushOut(Variable* t);
	
	void inErase(Variables::iterator t);

	void myUnique();
	void mySort();

	void printer();

private:
	std::string m_instString;

	int m_position;
	InstructionType m_type;
	
	FuncOrLabs m_funsLabs;

	Variables m_dst;
	Variables m_src;

	Variables m_use;
	Variables m_def;

	Variables m_in;
	Variables m_out;

	std::list<Instruction*> m_succ;
	std::list<Instruction*> m_pred;
};

/**
* This type represents list of instructions from program code.
*/
typedef std::list<Instruction*> Instructions;

//Program functions :

bool compare(Variable* a, Variable* b);

void createRegVars();

void createVariablesFuncsAndLabs(LexicalAnalysis& lex);

void printVariablesFuncsAndLabs();

Variable* finderVar(Token t);

FuncOrLab* finderFuncOrLab(Token t);

void createInsList(LexicalAnalysis& lex);

Instructions::iterator instFinder(FuncOrLab* t);

void fillSuccAndPred();

void fillUseAndDef();

void livenessAnalysis();

int findGPosition(Variable* p);

void printInterferenceGraph();

void doInterferenceGraph();

bool doSimplificationG(int degree);

bool doResourceAllocation();

std::string makeFinString(Instruction* i);

void createOutputString();

void printOutputString(std::string f);

void printInstructions();

void addJr();

bool compareFuncOrLab(FuncOrLab* a, FuncOrLab* b);

void stlFree();


#endif
