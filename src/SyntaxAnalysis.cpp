//Nikola Milosavljević RA5-2018, Datum : 8.6.2020.
#include <iostream>
#include <iomanip>

#include "SyntaxAnalysis.h"

using namespace std;


SyntaxAnalysis::SyntaxAnalysis(LexicalAnalysis& lex)
	: m_lexicalAnalysis(lex), m_errorFound(false), m_tokenIterator(m_lexicalAnalysis.getTokenList().begin())
{
}


bool SyntaxAnalysis::Do()
{
	m_currentToken = getNextToken();

	Q();

	return !m_errorFound;
}


void SyntaxAnalysis::printSyntaxError(Token token)
{
	cout << "Syntax error! Token: " << token.getValue() << " unexpected" << endl;
}


void SyntaxAnalysis::eat(TokenType t)
{
	if (m_errorFound == false)
	{
		if (m_currentToken.getType() == t)
		{			
			m_currentToken = getNextToken();
		}
		else
		{
			m_errorFound = true;
		}
	}
}


Token SyntaxAnalysis::getNextToken()
{
	if (m_tokenIterator == m_lexicalAnalysis.getTokenList().end())
		throw runtime_error("\n[PROGRAM] : End of input file reached");
	return *m_tokenIterator++;
}


void SyntaxAnalysis::Q()
{
	S();
	if (m_errorFound)
		return;
	if (m_currentToken.getType() == T_SEMI_COL)
		eat(T_SEMI_COL);
	else
	{
		m_errorFound = true;
		printSyntaxError(m_currentToken);
		return;
	}
		
	L();
}


void SyntaxAnalysis::S()
{
	if (m_errorFound)
		return;
	if (m_currentToken.getType() == T_MEM)
	{
		eat(T_MEM);
		if (m_currentToken.getType() == T_M_ID)
		{
			eat(T_M_ID);
			if (m_currentToken.getType() == T_NUM)
				eat(T_NUM);
			else
			{
				m_errorFound = true;
				printSyntaxError(m_currentToken);
				return;
			}
		}
		else
		{
			m_errorFound = true;
			printSyntaxError(m_currentToken);
			return;
		}
	}
	else if (m_currentToken.getType() == T_REG)
	{
		eat(T_REG);
		if (m_currentToken.getType() == T_R_ID)
			eat(T_R_ID);
		else
		{
			m_errorFound = true;
			printSyntaxError(m_currentToken);
			return;
		}
	}
	else if (m_currentToken.getType() == T_FUNC)
	{
		eat(T_FUNC);
		if (m_currentToken.getType() == T_ID)
			eat(T_ID);
		else
		{
			m_errorFound = true;
			printSyntaxError(m_currentToken);
			return;
		}
	}
	else if (m_currentToken.getType() == T_ID)
	{
		eat(T_ID);
		if (m_currentToken.getType() == T_COL)
		{
			eat(T_COL);
			E();
		}
		else
		{
			m_errorFound = true;
			printSyntaxError(m_currentToken);
			return;
		}
	}
	else
		E();
		
}


void SyntaxAnalysis::L()
{
	if (m_errorFound)
		return;
	if (m_currentToken.getType() == T_END_OF_FILE)
		return;                          
	else
		Q();
}


void SyntaxAnalysis::E()
{
	if (m_errorFound)
		return;
	if (m_currentToken.getType() == T_ADD)
	{
		eat(T_ADD);
		if (m_currentToken.getType() == T_R_ID)
		{
			eat(T_R_ID);
			if (m_currentToken.getType() == T_COMMA)
			{
				eat(T_COMMA);
				if (m_currentToken.getType() == T_R_ID)
				{
					eat(T_R_ID);
					if (m_currentToken.getType() == T_COMMA)
					{
						eat(T_COMMA);
						if (m_currentToken.getType() == T_R_ID)
							eat(T_R_ID);
						else
						{
							m_errorFound = true;
							printSyntaxError(m_currentToken);
							return;
						}
					}
					else
					{
						m_errorFound = true;
						printSyntaxError(m_currentToken);
						return;
					}
				}
				else
				{
					m_errorFound = true;
					printSyntaxError(m_currentToken);
					return;
				}
			}
			else
			{
				m_errorFound = true;
				printSyntaxError(m_currentToken);
				return;
			}
		}
		else
		{
			m_errorFound = true;
			printSyntaxError(m_currentToken);
			return;
		}
	}
	else if (m_currentToken.getType() == T_ADDI)
	{
		eat(T_ADDI);
		if (m_currentToken.getType() == T_R_ID)
		{
			eat(T_R_ID);
			if (m_currentToken.getType() == T_COMMA)
			{
				eat(T_COMMA);
				if (m_currentToken.getType() == T_R_ID)
				{
					eat(T_R_ID);
					if (m_currentToken.getType() == T_COMMA)
					{
						eat(T_COMMA);
						if (m_currentToken.getType() == T_NUM)
							eat(T_NUM);
						else
						{
							m_errorFound = true;
							printSyntaxError(m_currentToken);
							return;
						}
					}
					else
					{
						m_errorFound = true;
						printSyntaxError(m_currentToken);
						return;
					}
				}
				else
				{
					m_errorFound = true;
					printSyntaxError(m_currentToken);
					return;
				}
			}
			else
			{
				m_errorFound = true;
				printSyntaxError(m_currentToken);
				return;
			}
		}
		else
		{
			m_errorFound = true;
			printSyntaxError(m_currentToken);
			return;
		}
	}
	else if (m_currentToken.getType() == T_SUB)
	{
		eat(T_SUB);
		if (m_currentToken.getType() == T_R_ID)
		{
			eat(T_R_ID);
			if (m_currentToken.getType() == T_COMMA)
			{
				eat(T_COMMA);
				if (m_currentToken.getType() == T_R_ID)
				{
					eat(T_R_ID);
					if (m_currentToken.getType() == T_COMMA)
					{
						eat(T_COMMA);
						if (m_currentToken.getType() == T_R_ID)
							eat(T_R_ID);
						else
						{
							m_errorFound = true;
							printSyntaxError(m_currentToken);
							return;
						}
					}
					else
					{
						m_errorFound = true;
						printSyntaxError(m_currentToken);
						return;
					}
				}
				else
				{
					m_errorFound = true;
					printSyntaxError(m_currentToken);
					return;
				}
			}
			else
			{
				m_errorFound = true;
				printSyntaxError(m_currentToken);
				return;
			}
		}
		else
		{
			m_errorFound = true;
			printSyntaxError(m_currentToken);
			return;
		}
	}
	else if (m_currentToken.getType() == T_LA)
	{
		eat(T_LA);
		if (m_currentToken.getType() == T_R_ID)
		{
			eat(T_R_ID);
			if (m_currentToken.getType() == T_COMMA)
			{
				eat(T_COMMA);
				if (m_currentToken.getType() == T_M_ID)
					eat(T_M_ID);
				else
				{
					m_errorFound = true;
					printSyntaxError(m_currentToken);
					return;
				}
			}
			else
			{
				m_errorFound = true;
				printSyntaxError(m_currentToken);
				return;
			}
		}
		else
		{
			m_errorFound = true;
			printSyntaxError(m_currentToken);
			return;
		}
	}
	else if (m_currentToken.getType() == T_LW)
	{
		eat(T_LW);
		if (m_currentToken.getType() == T_R_ID)
		{
			eat(T_R_ID);
			if (m_currentToken.getType() == T_COMMA)
			{
				eat(T_COMMA);
				if (m_currentToken.getType() == T_NUM)
				{
					eat(T_NUM);
					if (m_currentToken.getType() == T_L_PARENT)
					{
						eat(T_L_PARENT);
						if (m_currentToken.getType() == T_R_ID)
						{
							eat(T_R_ID);
							if (m_currentToken.getType() == T_R_PARENT)
								eat(T_R_PARENT);
							else
							{
								m_errorFound = true;
								printSyntaxError(m_currentToken);
								return;
							}
						}
						else
						{
							m_errorFound = true;
							printSyntaxError(m_currentToken);
							return;
						}
					}
					else
					{
						m_errorFound = true;
						printSyntaxError(m_currentToken);
						return;
					}
				}
				else
				{
					m_errorFound = true;
					printSyntaxError(m_currentToken);
					return;
				}
			}
			else
			{
				m_errorFound = true;
				printSyntaxError(m_currentToken);
				return;
			}
		}
		else
		{
			m_errorFound = true;
			printSyntaxError(m_currentToken);
			return;
		}
	}
	else if (m_currentToken.getType() == T_LI)
	{
		eat(T_LI);
		if (m_currentToken.getType() == T_R_ID)
		{
			eat(T_R_ID);
			if (m_currentToken.getType() == T_COMMA)
			{
				eat(T_COMMA);
				if (m_currentToken.getType() == T_NUM)
					eat(T_NUM);
				else
				{
					m_errorFound = true;
					printSyntaxError(m_currentToken);
					return;
				}
			}
			else
			{
				m_errorFound = true;
				printSyntaxError(m_currentToken);
				return;
			}
		}
		else
		{
			m_errorFound = true;
			printSyntaxError(m_currentToken);
			return;
		}
	}
	else if (m_currentToken.getType() == T_SW)
	{
		eat(T_SW);
		if (m_currentToken.getType() == T_R_ID)
		{
			eat(T_R_ID);
			if (m_currentToken.getType() == T_COMMA)
			{
				eat(T_COMMA);
				if (m_currentToken.getType() == T_NUM)
				{
					eat(T_NUM);
					if (m_currentToken.getType() == T_L_PARENT)
					{
						eat(T_L_PARENT);
						if (m_currentToken.getType() == T_R_ID)
						{
							eat(T_R_ID);
							if (m_currentToken.getType() == T_R_PARENT)
								eat(T_R_PARENT);
							else
							{
								m_errorFound = true;
								printSyntaxError(m_currentToken);
								return;
							}
						}
						else
						{
							m_errorFound = true;
							printSyntaxError(m_currentToken);
							return;
						}
					}
					else
					{
						m_errorFound = true;
						printSyntaxError(m_currentToken);
						return;
					}
				}
				else
				{
					m_errorFound = true;
					printSyntaxError(m_currentToken);
					return;
				}
			}
			else
			{
				m_errorFound = true;
				printSyntaxError(m_currentToken);
				return;
			}
		}
		else
		{
			m_errorFound = true;
			printSyntaxError(m_currentToken);
			return;
		}
	}
	else if (m_currentToken.getType() == T_B)
	{
		eat(T_B);
		if (m_currentToken.getType() == T_ID)
			eat(T_ID);
		else
		{
			m_errorFound = true;
			printSyntaxError(m_currentToken);
			return;
		}
	}
	else if (m_currentToken.getType() == T_BLTZ)
	{
		eat(T_BLTZ);
		if (m_currentToken.getType() == T_R_ID)
		{
			eat(T_R_ID);
			if (m_currentToken.getType() == T_COMMA)
			{
				eat(T_COMMA);
				if (m_currentToken.getType() == T_ID)
					eat(T_ID);
				else
				{
					m_errorFound = true;
					printSyntaxError(m_currentToken);
					return;
				}
			}
			else
			{
				m_errorFound = true;
				printSyntaxError(m_currentToken);
				return;
			}
		}
		else
		{
			m_errorFound = true;
			printSyntaxError(m_currentToken);
			return;
		}
	}
	else if (m_currentToken.getType() == T_NOP)
	{
		eat(T_NOP);
	}
	else if (m_currentToken.getType() == T_DIV)
	{
		eat(T_DIV);
		if (m_currentToken.getType() == T_R_ID)
		{
			eat(T_R_ID);
			if (m_currentToken.getType() == T_COMMA)
			{
				eat(T_COMMA);
				if (m_currentToken.getType() == T_R_ID)
				{
					eat(T_R_ID);
					if (m_currentToken.getType() == T_COMMA)
					{
						eat(T_COMMA);
						if (m_currentToken.getType() == T_R_ID)
							eat(T_R_ID);
						else
						{
							m_errorFound = true;
							printSyntaxError(m_currentToken);
							return;
						}
					}
					else
					{
						m_errorFound = true;
						printSyntaxError(m_currentToken);
						return;
					}
				}
				else
				{
					m_errorFound = true;
					printSyntaxError(m_currentToken);
					return;
				}
			}
			else
			{
				m_errorFound = true;
				printSyntaxError(m_currentToken);
				return;
			}
		}
		else
		{
			m_errorFound = true;
			printSyntaxError(m_currentToken);
			return;
		}
	}
	else if (m_currentToken.getType() == T_OR)
	{
		eat(T_OR);
		if (m_currentToken.getType() == T_R_ID)
		{
			eat(T_R_ID);
			if (m_currentToken.getType() == T_COMMA)
			{
				eat(T_COMMA);
				if (m_currentToken.getType() == T_R_ID)
				{
					eat(T_R_ID);
					if (m_currentToken.getType() == T_COMMA)
					{
						eat(T_COMMA);
						if (m_currentToken.getType() == T_R_ID)
							eat(T_R_ID);
						else
						{
							m_errorFound = true;
							printSyntaxError(m_currentToken);
							return;
						}
					}
					else
					{
						m_errorFound = true;
						printSyntaxError(m_currentToken);
						return;
					}
				}
				else
				{
					m_errorFound = true;
					printSyntaxError(m_currentToken);
					return;
				}
			}
			else
			{
				m_errorFound = true;
				printSyntaxError(m_currentToken);
				return;
			}
		}
		else
		{
			m_errorFound = true;
			printSyntaxError(m_currentToken);
			return;
		}
	}
	else if (m_currentToken.getType() == T_BLEZ)
	{
		eat(T_BLEZ);
		if (m_currentToken.getType() == T_R_ID)
		{
			eat(T_R_ID);
			if (m_currentToken.getType() == T_COMMA)
			{
				eat(T_COMMA);
				if (m_currentToken.getType() == T_ID)
					eat(T_ID);
				else
				{
					m_errorFound = true;
					printSyntaxError(m_currentToken);
					return;
				}
			}
			else
			{
				m_errorFound = true;
				printSyntaxError(m_currentToken);
				return;
			}
		}
		else
		{
			m_errorFound = true;
			printSyntaxError(m_currentToken);
			return;
		}
	}
	else
	{
		m_errorFound = true;
		printSyntaxError(m_currentToken);
		return;
	}
}