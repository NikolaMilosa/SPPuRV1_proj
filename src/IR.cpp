//Nikola Milosavljević RA5-2018, Datum : 8.6.2020.
#include <algorithm>
#include <ostream>
#include <iomanip>

#include "IR.h"

using namespace std;

//Global variables : 

Variables g_variables;
Instructions g_instructions;
FuncOrLabs g_funcOrLabs;
int g_position = 1;

vector<Variable*> g_regVars;

vector<vector<int>> g_interferenceGraph;
stack<Variable*> g_simplificationStack;

map<int,string> g_output_globl;
map<int, string> g_output_data;
map<int, string> g_output_text;

string g_output_str;



//FuncOrLab :

//Setters :
void FuncOrLab::setType(FuncOrLabType t)
{
	m_type = t;
}

void FuncOrLab::setName(string s)
{
	m_name = s;
}

void FuncOrLab::setPosition(int t)
{
	m_position = t;
}

//Getters :

string FuncOrLab::getName()
{
	return m_name;
}

int FuncOrLab::getPosition()
{
	return m_position;
}

FuncOrLabType FuncOrLab::getType()
{
	return m_type;
}

//Other functions :

void FuncOrLab::printer()
{
	cout << "-------- Function and Label printer --------" << endl;
	cout << string(5, ' ') << "Type : " << ((m_type == FUNC_TYPE) ? "Function" : "Label") << endl;
	cout << string(5, ' ') << "Name : " << m_name << endl;
	cout << string(5, ' ') << "Position : " << m_position << endl;
}



//Variable :

//Setters :
void Variable::setType(VariableType t) 
{
	m_type = t;
}

void Variable::setName(string s)
{
	m_name = s;
}

void Variable::setValue(int t)
{
	m_value = t;
}

void Variable::setPosition(int t)
{
	m_position = t;
}

void Variable::setAssignment(Regs r)
{
	m_assignment = r;
}

void Variable::setPosIgMat(int t)
{
	m_pos_ig_mat = t;
}

//Getters :

string Variable::getName()
{
	return m_name;
}

VariableType Variable::getType()
{
	return m_type;
}

int Variable::getPosIgMat()
{
	return m_pos_ig_mat;
}

Regs Variable::getAssignment()
{
	return m_assignment;
}

int Variable::getPosition()
{
	return m_position;
}

int Variable::getValue()
{
	return m_value;
}

//Others functions :

void Variable::printer()
{
	cout << "-------- Variable printer --------" << endl;
	cout << string(5, ' ') << "Variable type : " << ((m_type == MEM_VAR) ? "Memory variable" : "Registry variable") << endl;
	cout << string(5, ' ') << "Variable name : " << m_name << endl;
	cout << string(5, ' ') << "Variable value : " << m_value << endl;
	cout << string(5, ' ') << "Variable position : " << m_position << endl;
	cout << string(5, ' ') << "Variable assignment : " << m_assignment << endl << endl;
}



//Instruction :

//Setters :

void Instruction::setInstString(string t)
{
	m_instString = t;
}

//Getters :

InstructionType Instruction::getType()
{
	return m_type;
}

int Instruction::getPosition()
{
	return m_position;
}

string Instruction::getInstText()
{
	return m_instString;
}

Variable* Instruction::getFromSrc()
{
	auto it = m_src.begin();
	Variable* p = *it;
	m_src.erase(it);
	return p;
}

FuncOrLab* Instruction::getFuncOrLab()
{
	return *(m_funsLabs.begin());
}

Variables Instruction::getDest()
{
	return m_dst;
}

Variables Instruction::getSrc()
{
	return m_src;
}

Variables Instruction::getDef()
{
	return m_def;
}

Variables Instruction::getIn()
{
	return m_in;
}

Variables Instruction::getOut()
{
	return m_out;
}

ReturnValueVar Instruction::getItUse()
{
	ReturnValueVar p;
	p.itb = m_use.begin();
	p.ite = m_use.end();
	p.size = m_use.size();
	return p;
}

ReturnValueVar Instruction::getItOut()
{
	ReturnValueVar p;
	p.itb = m_out.begin();
	p.ite = m_out.end();
	p.size = m_out.size();
	return p;
}

ReturnValueVar Instruction::getItDef()
{
	ReturnValueVar p;
	p.itb = m_def.begin();
	p.ite = m_def.end();
	p.size = m_def.size();
	return p;
}

ReturnValueVar Instruction::getItIn()
{
	ReturnValueVar p;
	p.itb = m_in.begin();
	p.ite = m_in.end();
	p.size = m_in.size();
	return p;
}

ReturnValueIns Instruction::getItSucc()
{
	ReturnValueIns p;
	p.itb = m_succ.begin();
	p.ite = m_succ.end();
	p.size = m_succ.size();
	return p;
}

//Other functions : 

void Instruction::pushDst(Variable* t)
{
	m_dst.push_back(t);
}

void Instruction::pushSrc(Variable* t)
{
	m_src.push_back(t);
}

void Instruction::pushFuncOrLab(FuncOrLab* t)
{
	m_funsLabs.push_back(t);
}

void Instruction::pushSucc(Instruction* t)
{
	m_succ.push_back(t);
}

void Instruction::pushPred(Instruction* t)
{
	m_pred.push_back(t);
}

void Instruction::pushUse(Variable* t)
{
	m_use.push_back(t);
}

void Instruction::pushDef(Variable* t)
{
	m_def.push_back(t);
}

void Instruction::pushIn(Variable* t)
{
	m_in.push_back(t);
}

void Instruction::pushOut(Variable* t)
{
	m_out.push_back(t);
}

void Instruction::inErase(Variables::iterator t)
{
	m_in.erase(t);
}

void Instruction::mySort()
{
	m_in.sort(compare);
	m_out.sort(compare);
}

void Instruction::myUnique()
{
	m_in.unique();
	m_out.unique();
}

void Instruction::printer()
{
	cout << "-------- Instruction printer --------" << endl;
	cout << string(5, ' ') << "Position : " << m_position << endl;
	cout << string(5, ' ') << "Instruction text : " << m_instString;
	cout << string(5, ' ') << "Destinations  : ";
	for (auto it = m_dst.begin(); it != m_dst.end(); it++)
		cout << (*it)->getName() << " ";
	cout << endl;
	cout << string(5, ' ') << "Sources : ";
	for (auto it = m_src.begin(); it != m_src.end(); it++)
		cout << (*it)->getName() << " ";
	cout << endl;

	cout << string(5, ' ') << "Uses : ";
	for (auto it = m_use.begin(); it != m_use.end(); it++)
		cout << (*it)->getName() << " ";
	cout << endl;

	cout << string(5, ' ') << "Defines : ";
	for (auto it = m_def.begin(); it != m_def.end(); it++)
		cout << (*it)->getName() << " ";
	cout << endl;

	cout << string(5, ' ') << "In : ";
	for (auto it = m_in.begin(); it != m_in.end(); it++)
		cout << (*it)->getName() << " ";
	cout << endl;

	cout << string(5, ' ') << "Out : ";
	for (auto it = m_out.begin(); it != m_out.end(); it++)
		cout << (*it)->getName() << " ";
	cout << endl;

	cout << string(5, ' ') << "Successor position : ";
	for (auto it = m_succ.begin(); it != m_succ.end(); it++)
		cout << (*it)->getPosition() << " ";
	cout << endl;

	cout << string(5, ' ') << "Predecessor position : ";
	for (auto it = m_pred.begin(); it != m_pred.end(); it++)
		cout << (*it)->getPosition() << " ";
	cout << endl;
}



//Program functions:

bool compare(Variable* a, Variable* b)
{
	if (a->getName().compare(b->getName()) < 0)
		return true;
	return false;
}

void createRegVars()
{
	g_variables.sort(compare);

	for (auto it = g_variables.begin(); it != g_variables.end(); it++)
		if ((*it)->getType() == REG_VAR)
			g_regVars.push_back(*it);
}

void createVariablesFuncsAndLabs(LexicalAnalysis& lex)
{
	TokenList list = lex.getTokenList();
	Token currentToken;

	for(auto it = list.begin(); it != list.end(); it++)
	{
		if ((*it).getType() == T_MEM)
		{
			it++;
			string varName = (*it).getValue();

			it++;
			int varValue = stoi((*it).getValue());

			if (varName[0] != 'm')
				throw runtime_error("\n[PROGRAM] : Syntax error! Memory variable : " + varName + " not declared correctly!");
			else
			{
				bool exists = false;
				for (auto it_var = g_variables.begin(); it_var != g_variables.end(); it_var++)
					if ((*it_var)->getName().compare(varName) == 0)
						exists = true;
				if (!exists)
				{
					Variable* temp = new Variable(MEM_VAR, varName, g_position, varValue, no_assign);
					g_variables.push_back(temp);
				}
				else
					throw runtime_error("\n[PROGRAM] : Error! Memory variable : " + varName + " already defined!\n");
			}
		}
		else if ((*it).getType() == T_REG)
		{
			it++;
			string varName = (*it).getValue();

			if (varName[0] != 'r')
				throw runtime_error("\n[PROGRAM] : Syntax error! Registry variable : " + varName + " not declared correctly!");
				
			else
			{
				bool exists = false;
				for (auto it_var = g_variables.begin(); it_var != g_variables.end(); it_var++)
					if ((*it_var)->getName().compare(varName) == 0)
						exists = true;
				if (!exists)
				{
					Variable* temp = new Variable(REG_VAR, varName, g_position, 0, no_assign);
					g_variables.push_back(temp);
				}
				else
					throw runtime_error("\n[PROGRAM] : Error! Registry variable : " + varName + " already defined!\n");
			}
		}
		else if ((*it).getType() == T_SEMI_COL || (*it).getType() == T_COL)
			g_position++;
		else if ((*it).getType() == T_FUNC)
		{
			it++;
			string funcName = (*it).getValue();
			if (funcName[0] < 'a' && funcName[0] > 'z')
				throw runtime_error("\n[PROGRAM] : Syntax error! Function : " + funcName + " not declared correctly!");
			else
			{
				bool exists = false;
				for (auto it_var = g_funcOrLabs.begin(); it_var != g_funcOrLabs.end(); it_var++)
					if ((*it_var)->getName().compare(funcName) == 0)
						exists = true;
				if (!exists)
				{
					FuncOrLab* temp = new FuncOrLab(FUNC_TYPE, funcName, g_position);
					g_funcOrLabs.push_back(temp);
				}
				else
					throw runtime_error("\n[PROGRAM] : Error! Function : " + funcName + " already defined!\n");
			}
		}
		else if ((*it).getType() == T_ID)
		{
			string labelName = (*it).getValue();

			it++;
			if ((*it).getType() != T_COL)
			{
				g_position++;
				continue;
			}
			bool exists = false;
			for (auto it_var = g_funcOrLabs.begin(); it_var != g_funcOrLabs.end(); it_var++)
				if ((*it_var)->getName().compare(labelName) == 0)
					exists = true;
			if (!exists)
			{
				FuncOrLab* temp = new FuncOrLab(LAB_TYPE, labelName, g_position);
				g_funcOrLabs.push_back(temp);
				g_position++;
			}
			else
				throw runtime_error("\n[PROGRAM] : Error! Label : " + labelName + " already defined!\n");

		}

	}
	createRegVars();
}

void printVariablesFuncsAndLabs() 
{
	auto it = g_variables.begin();
	for (it; it != g_variables.end(); it++)
		(*it)->printer();
	auto it_fl = g_funcOrLabs.begin();
	for (it_fl; it_fl != g_funcOrLabs.end(); it_fl++)
		(*it_fl)->printer();
}

Variable* finderVar(Token t)
{ 
	Variable* temp = nullptr;

	for (auto it = g_variables.begin(); it != g_variables.end(); it++)
		if (t.getValue().compare((*it)->getName()) == 0)
			temp = *it;

	if (temp == nullptr)
		throw runtime_error("\n[PROGRAM] : Using nonexistant variables or functions! Name : " + t.getValue());
	return temp;
}

FuncOrLab* finderFuncOrLab(Token t)
{
	FuncOrLab* temp = nullptr;

	for (auto it = g_funcOrLabs.begin(); it != g_funcOrLabs.end(); it++)
		if (t.getValue().compare((*it)->getName()) == 0)
			temp = *it;

	if (temp == nullptr)
		throw runtime_error("\n[PROGRAM] : Using nonexistant variables or functions! Name : " + t.getValue());
	return temp;
}

void createInsList(LexicalAnalysis& lex)
{
	TokenList list = lex.getTokenList();
	Token currentToken;
	g_position = 1;

	auto it = list.begin();
	for (it; it != list.end(); it++)
	{
		switch ((*it).getType())
		{
		case T_SEMI_COL:
		{
			g_position++;
			break;
		}
		case T_COL:
		{
			g_position++;
			break;
		}
		case T_ADD:
		{
			Instruction* inst = new Instruction(g_position, I_ADD);
			it++;
			inst->pushDst(finderVar(*it));

			advance(it, 2);
			inst->pushSrc(finderVar(*it));

			advance(it, 2);
			inst->pushSrc(finderVar(*it));

			inst->setInstString("add   ~d, ~s, ~s\n");
			g_instructions.push_back(inst);
			break;
		}
		case T_ADDI:
		{
			Instruction* inst = new Instruction(g_position, I_ADDI);
			it++;
			inst->pushDst(finderVar(*it));

			advance(it, 2);
			inst->pushSrc(finderVar(*it));

			advance(it, 2);
			string cons = (*it).getValue();

			inst->setInstString("addi  ~d, ~s, " + cons + "\n");
			g_instructions.push_back(inst);
			break;
		}
		case T_SUB:
		{
			Instruction* inst = new Instruction(g_position, I_SUB);
			it++;
			inst->pushDst(finderVar(*it));

			advance(it, 2);
			inst->pushSrc(finderVar(*it));

			advance(it, 2);
			inst->pushSrc(finderVar(*it));

			inst->setInstString("sub   ~d, ~s, ~s\n");
			g_instructions.push_back(inst);
			break;
		}
		case T_LA:
		{
			Instruction* inst = new Instruction(g_position, I_LA);
			it++;
			inst->pushDst(finderVar(*it));
			
			advance(it, 2);
			inst->pushSrc(finderVar(*it));

			inst->setInstString("la    ~d, ~s\n");
			g_instructions.push_back(inst);
			break;
		}
		case T_LI:
		{
			Instruction* inst = new Instruction(g_position, I_LI);
			it++;
			inst->pushDst(finderVar(*it));
			
			advance(it, 2);
			string cons = (*it).getValue();

			inst->setInstString("li    ~d, " + cons + "\n");
			g_instructions.push_back(inst);
			break;
		}
		case T_LW:
		{
			Instruction* inst = new Instruction(g_position, I_LW);
			it++;
			inst->pushDst(finderVar(*it));
			
			advance(it, 2);
			string cons = (*it).getValue();
			
			advance(it, 2);
			inst->pushSrc(finderVar(*it));

			inst->setInstString("lw    ~d, " + cons + "(~s)\n");
			g_instructions.push_back(inst);
			break;
		}
		case T_SW:
		{
			Instruction* inst = new Instruction(g_position, I_SW);
			it++;
			inst->pushSrc(finderVar(*it));
			
			advance(it, 2);
			string cons = (*it).getValue();
			
			advance(it, 2);
			inst->pushSrc(finderVar(*it));

			inst->setInstString("sw    ~s, " + cons + "(~s)\n");
			g_instructions.push_back(inst);
			break;
		}
		case T_BLTZ:
		{
			Instruction* inst = new Instruction(g_position, I_BLTZ);
			it++;
			inst->pushSrc(finderVar(*it));
			
			advance(it, 2);
			inst->pushFuncOrLab(finderFuncOrLab(*it));

			inst->setInstString("bltz  ~s, ~d\n");
			g_instructions.push_back(inst);
			break;
		}
		case T_B:
		{
			Instruction* inst = new Instruction(g_position, I_B);
			it++;
			inst->pushFuncOrLab(finderFuncOrLab(*it));

			inst->setInstString("b     ~d\n");
			g_instructions.push_back(inst);
			break;
		}
		case T_NOP:
		{
			Instruction* inst = new Instruction(g_position, I_NOP);

			inst->setInstString("nop\n");
			g_instructions.push_back(inst);
			break;
		}
		case T_DIV:
		{
			Instruction* inst = new Instruction(g_position, I_DIV);
			it++;
			inst->pushDst(finderVar(*it));

			advance(it, 2);
			inst->pushSrc(finderVar(*it));

			advance(it, 2);
			inst->pushSrc(finderVar(*it));

			inst->setInstString("div   ~d, ~s, ~s\n");
			g_instructions.push_back(inst);
			break;
		}
		case T_OR:
		{
			Instruction* inst = new Instruction(g_position, I_OR);
			it++;
			inst->pushDst(finderVar(*it));

			advance(it, 2);
			inst->pushSrc(finderVar(*it));

			advance(it, 2);
			inst->pushSrc(finderVar(*it));

			inst->setInstString("or    ~d, ~s, ~s\n");
			g_instructions.push_back(inst);
			break;
		}
		case T_BLEZ:
		{
			Instruction* inst = new Instruction(g_position, I_BLEZ);
			it++;
			inst->pushSrc(finderVar(*it));

			advance(it, 2);
			inst->pushFuncOrLab(finderFuncOrLab(*it));

			inst->setInstString("blez  ~s, ~d\n");
			g_instructions.push_back(inst);
			break;
		}
		}
	}

	fillSuccAndPred();
	fillUseAndDef();
}

Instructions::iterator instFinder(FuncOrLab* t)
{
	int pos = t->getPosition();
	pos++;
	for (auto it = g_instructions.begin(); it != g_instructions.end(); it++)
		if ((*it)->getPosition() == pos)
			return it;
}

void fillSuccAndPred()
{
	auto it = g_instructions.begin();
	auto it_next = g_instructions.begin();

	for (it; it != g_instructions.end(); it++)
	{
		if ((*it)->getType() == I_BLTZ || (*it)->getType() == I_B || (*it)->getType() == I_BLEZ)
		{
			it_next = instFinder((*it)->getFuncOrLab());

			(*it)->pushSucc(*it_next);
			(*it_next)->pushPred(*it);

			it_next = it;
			it_next++;
			if (it_next != g_instructions.end())
			{
				(*it)->pushSucc(*it_next);
				(*it_next)->pushPred(*it);
			}
		}
		else
		{
			it_next = it;
			it_next++;
			if (it_next != g_instructions.end())
			{
				(*it)->pushSucc(*it_next);
				(*it_next)->pushPred(*it);
			}
		}
	}
}

void fillUseAndDef()
{
	auto it = g_instructions.begin();
	for (it; it != g_instructions.end(); it++)
	{
		Variables temp = (*it)->getSrc();
		auto it_t = temp.begin();
		for (it_t; it_t != temp.end(); it_t++)
			if((*it_t)->getType() == REG_VAR)
				(*it)->pushUse(*it_t);

		temp = (*it)->getDest();
		it_t = temp.begin();
		for (it_t; it_t != temp.end(); it_t++)
			if ((*it_t)->getType() == REG_VAR)
				(*it)->pushDef(*it_t);
	}
}

void livenessAnalysis()
{
	for (int i = 0; i < g_instructions.size(); i++) 
	{
		bool checker = true;
		for (auto rit = g_instructions.rbegin(); rit != g_instructions.rend(); rit++) {
			Variables in_p = (*rit)->getIn();
			Variables out_p = (*rit)->getOut();

			ReturnValueIns succ = (*rit)->getItSucc();
			for (auto it = succ.itb; it != succ.ite; it++)
			{
				ReturnValueVar in = (*it)->getItIn();
				for (auto it_1 = in.itb; it_1 != in.ite; it_1++)
					(*rit)->pushOut(*it_1);
			}

			ReturnValueVar out = (*rit)->getItOut();
			ReturnValueVar in = (*rit)->getItIn();
			ReturnValueVar use = (*rit)->getItUse();
			ReturnValueVar def = (*rit)->getItDef();

			for (auto it = out.itb; it != out.ite; it++)
				(*rit)->pushIn(*it);

			(*rit)->mySort();
			(*rit)->myUnique();

			in = (*rit)->getItIn();

			for (int j = 0; j < in.size; j++) {
				auto iter = in.itb;
				std::advance(iter, j);
				for (auto it_1 = def.itb; it_1 != def.ite; it_1++)
					if ((*it_1)->getName().compare((*iter)->getName()) == 0)
					{
						(*rit)->inErase(iter);
					}
				in = (*rit)->getItIn();
			}
			
			in = (*rit)->getItIn();

			for (auto it = use.itb; it != use.ite; it++)
				(*rit)->pushIn(*it);

			(*rit)->mySort();
			(*rit)->myUnique();
			
			if (in_p == (*rit)->getIn() && out_p == (*rit)->getOut())
				checker = checker & true;
			else
				checker = checker & false;
			
		}
		if (checker)
			break;
	}
	
}

int findGPosition(Variable* p)
{
	int i = 0;
	for (auto it = g_regVars.begin(); it != g_regVars.end(); it++)
	{
		if ((*it)->getName().compare(p->getName()) == 0)
			return i;
		i++;
	}
}

void printInterferenceGraph() 
{
	for (int i = 0; i < g_interferenceGraph.size(); i++)
	{
		cout << string(5, ' ');
		for (int j = 0; j < g_interferenceGraph[0].size(); j++)
			cout << ((g_interferenceGraph[i][j] == __INTERFERENCE__) ? "1" : "0") << " ";
		cout << endl;
	}
}

void doInterferenceGraph()
{
	for (int i = 0; i < g_regVars.size(); i++)
		g_regVars[i]->setPosIgMat(i);

	g_interferenceGraph.resize(g_regVars.size());

	for (auto it = g_interferenceGraph.begin(); it != g_interferenceGraph.end(); it++)
		(*it).assign(g_regVars.size(), __EMPTY__);

	for (auto it = g_instructions.begin(); it != g_instructions.end(); it++)
	{
		Variables tempDef = (*it)->getDef();
		Variables tempOut = (*it)->getOut();

		for(auto it_d = tempDef.begin(); it_d != tempDef.end(); it_d++)
			for (auto it_o = tempOut.begin(); it_o != tempOut.end(); it_o++)
			{
				int i = findGPosition(*it_d);
				int j = findGPosition(*it_o);
				if (i != j)
				{
					g_interferenceGraph[i][j] = __INTERFERENCE__;
					g_interferenceGraph[j][i] = __INTERFERENCE__;
				}
			}
	}
}

bool doSimplificationG(int degree)
{
	bool not_finished = true;
	int count;
	int k = degree - 1;
	int k_temp = k;
	int place_in_regvars;

	vector<bool> flags(g_regVars.size());

	for (auto it = flags.begin(); it != flags.end(); it++)
		(*it) = false;
	do 
	{
		place_in_regvars = -1;
		int checker = 0;
		for (int i = 0; i < g_interferenceGraph.size(); i++,checker++)
		{
			count = 0;
			if (flags[checker])
				continue;
			for (int j = 0; j < g_interferenceGraph.size(); j++)
				if (g_interferenceGraph[i][j] == __INTERFERENCE__)
					count++;
			if (count == k_temp)
			{
				place_in_regvars = i;
				break;
			}
		}

		if (place_in_regvars != -1)
		{
			flags[place_in_regvars] = true;
			g_simplificationStack.push(g_regVars[place_in_regvars]);
			
			
			for (int i = 0; i < g_interferenceGraph.size(); i++)
			{
				g_interferenceGraph[place_in_regvars][i] = __CHECKED_INT__;
				g_interferenceGraph[i][place_in_regvars] = __CHECKED_INT__;
			}			

			bool temp_fin = true;
			for (int i = 0; i < flags.size(); i++)
				temp_fin &= flags[i];
			if (temp_fin)
				not_finished = false;

			k_temp = k;
		}
		else
		{
			k_temp--;
			if (k_temp == -1)
				break;
		}
	} while (not_finished);

	doInterferenceGraph();

	if (not_finished)
		return false;
	else
		return true;	
}

bool doResourceAllocation()
{
	int tekuci_reg = 0;                                                               

	list<Variable*> obradjeni;                                                        
																					  
	while (!g_simplificationStack.empty()) {
		Variable* var = g_simplificationStack.top();
		g_simplificationStack.pop();

		bool ima_smetnje = false;                                                     
																					 
		for (auto it = obradjeni.begin(); it != obradjeni.end(); it++)
			if (g_interferenceGraph[var->getPosIgMat()][(*it)->getPosIgMat()] == __INTERFERENCE__) {               
				ima_smetnje = true;
				break;
			}

		if (ima_smetnje) {
			if (tekuci_reg != (__REG_NUMBER__ - 1))                                   
				tekuci_reg++;
			else
				return false;
		}
		var->setAssignment((Regs)tekuci_reg);
		obradjeni.push_back(var);
	}

	cout << endl << "[PROGRAM] : Printing newly assigned registers . . ." << endl << endl;

	for (auto it = g_regVars.begin(); it != g_regVars.end(); it++)
		(*it)->printer();

	return true;

}

string makeFinString(Instruction* i)
{
	string retVal = string(8, ' ');
	string temp = i->getInstText();

	for (auto it = temp.begin(); it != temp.end(); it++)
	{
		if ((*it) == '~')
		{
			it++;
			if ((*it) == 'd')
			{
				if (i->getType() != I_B && i->getType() != I_BLTZ && i->getType() != I_BLEZ)
				{
					Variable* p = *(i->getDest().begin());

					if (p->getType() == MEM_VAR)
						retVal += p->getName();
					else
						switch (p->getAssignment())
						{
						case t0:
						{
							retVal += "$t0";
							break;
						}
						case t1:
						{
							retVal += "$t1";
							break;
						}
						case t2:
						{
							retVal += "$t2";
							break;
						}
						case t3:
						{
							retVal += "$t3";
							break;
						}
						}
				}
				else
				{
					FuncOrLab* p = i->getFuncOrLab();

					retVal += p->getName();
				}
			}
			else
			{
				Variable* p = i->getFromSrc();

				if (p->getType() == MEM_VAR)
					retVal += p->getName();
				else
					switch (p->getAssignment())
					{
					case t0:
					{
						retVal += "$t0";
						break;
					}
					case t1:
					{
						retVal += "$t1";
						break;
					}
					case t2:
					{
						retVal += "$t2";
						break;
					}
					case t3:
					{
						retVal += "$t3";
						break;
					}
					}
			}
		}
		else
			retVal += *it;
	}

	return retVal;
}

void createOutputString()
{
	for (auto it = g_funcOrLabs.begin(); it != g_funcOrLabs.end(); it++)
		if ((*it)->getType() == FUNC_TYPE)
			g_output_globl[(*it)->getPosition()] = ".globl " + (*it)->getName() + "\n";

	g_output_data[0] = ".data\n";
	for (auto it = g_variables.begin(); it != g_variables.end(); it++)
		if ((*it)->getType() == MEM_VAR)
			g_output_data[(*it)->getPosition()] = (*it)->getName() + ":" + string(5, ' ') + ".word " + to_string((*it)->getValue()) + "\n";

	addJr();
	g_output_text[0] = ".text\n";
	for (auto it = g_funcOrLabs.begin(); it != g_funcOrLabs.end(); it++)
		g_output_text[(*it)->getPosition()] = (*it)->getName() + ":\n";

	for (auto it = g_instructions.begin(); it != g_instructions.end(); it++)
		g_output_text[(*it)->getPosition()] = makeFinString(*it);
}

void printOutputString(string f)
{
	for (auto it = g_output_globl.begin(); it != g_output_globl.end(); it++)
		g_output_str += it->second;
	g_output_str += "\n";
	for (auto it = g_output_data.begin(); it != g_output_data.end(); it++)
		g_output_str += it->second;
	g_output_str += "\n";
	for (auto it = g_output_text.begin(); it != g_output_text.end(); it++)
		g_output_str += it->second;

	g_output_str += "\tjr    $ra\n";

	cout << g_output_str;

	ofstream outputFile;
	string filename = ".\\..\\outputs\\" + f + ".s";
	
	outputFile.open(filename, ofstream::out | ofstream::trunc);

	if (!outputFile.is_open())
		throw runtime_error("\n[PROGRAM] : Cannot create output file!");

	outputFile << g_output_str;

	outputFile.close();

}

void printInstructions()
{
	for (auto it = g_instructions.begin(); it != g_instructions.end(); it++)
		(*it)->printer();
}

void addJr()
{
	g_funcOrLabs.sort(compareFuncOrLab);

	int count = 0;
	for (auto it = g_funcOrLabs.begin(); it != g_funcOrLabs.end(); it++)
		if ((*it)->getType() == FUNC_TYPE)
			count++;
	if (count > 1)
	{
		auto it = g_funcOrLabs.begin();
		it++;
		for (it; it != g_funcOrLabs.end(); it++)
			if ((*it)->getType() == FUNC_TYPE)
				(*it)->setName("\tjr    $ra\n" + (*it)->getName());
	}
}

bool compareFuncOrLab(FuncOrLab* a, FuncOrLab* b)
{
	if (a->getPosition() < b->getPosition())
		return true;
	return false;
}

void stlFree()
{
	for (auto it = g_instructions.begin(); it != g_instructions.end(); it++)
		delete (*it);
	for (auto it = g_variables.begin(); it != g_variables.end(); it++)
		delete (*it);
	for (auto it = g_funcOrLabs.begin(); it != g_funcOrLabs.end(); it++)
		delete (*it);
}
