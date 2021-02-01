//Nikola Milosavljević RA5-2018, Datum : 8.6.2020.
#include <iostream>
#include <exception>

#include "LexicalAnalysis.h"
#include "SyntaxAnalysis.h"
#include "IR.h"

using namespace std;


int main()
{
	try
	{
		std::string f = "simple";
		std::string fileName = ".\\..\\examples\\" + f + ".mavn";
		bool retVal = false;

		LexicalAnalysis lex;

		if (!lex.readInputFile(fileName))
			throw runtime_error("\n[PROGRAM] : Exception! Failed to open input file!\n");

		lex.initialize();

		retVal = lex.Do();

		if (retVal)
		{
			cout << "[PROGRAM] : Lexical analysis finished successfully!" << endl;
			lex.printTokens();
		}
		else
		{
			lex.printLexError();
			throw runtime_error("\n[PROGRAM] : Exception! Lexical analysis failed!\n");
		}

		// Syntax analysis . . . 

		cout << endl << "[PROGRAM] : Syntax analysis in progress . . . " << endl << endl;
		SyntaxAnalysis syn(lex);

		retVal = syn.Do();

		if (retVal)
		{
			cout << endl <<"[PROGRAM] : Syntax analysis finished successfully!" << endl;
		}
		else
		{
			throw runtime_error("\n[PROGRAM] : Syntax analysis finished unsuccessfully!");
		}

		// Creating Variables, Functions and Labels . . . 

		cout << endl << "[PROGRAM] : Creating variables, functions and labels . . . " << endl << endl;

		createVariablesFuncsAndLabs(lex);
		
		printVariablesFuncsAndLabs();

		cout << endl << "[PROGRAM] : Variables, functions and labels created!" << endl << endl; 

		cout << endl << "[PROGRAM] : Creating instructions . . . " << endl << endl;

		createInsList(lex);

		cout << endl << "[PROGRAM] : Instructions created!" << endl << endl;

		// Liveness analysis . . .

		cout << endl << "[PROGRAM] : Attempting liveness analysis . . ." << endl << endl;

		livenessAnalysis();

		cout << endl << "[PROGRAM] : Liveness analysis successful!" << endl << endl;

		cout << endl << "[PROGRAM] : Priting instructions . . ." << endl << endl;

		printInstructions();

		// Resource allocation . . . 

		cout << endl << "[PROGRAM] : Attempting doing interference graph . . . " << endl << endl;

		doInterferenceGraph();

		printInterferenceGraph();

		cout << endl << "[PROGRAM] : Intereference graph drawn!" << endl << endl;

		cout << endl << "[PROGRAM] : Attempting to create the simplification stack . . . " << endl << endl;

		if (doSimplificationG(__REG_NUMBER__))
			cout << endl << "[PROGRAM] : Simplification finished!" << endl << endl;
		else
			throw runtime_error("\n[PROGRAM] : Cannot create simplification stack! Spill detected . . .");
		
		cout << endl << "[PROGRAM] : Attempting resource allocation . . . " << endl << endl;

		if (doResourceAllocation())
			cout << endl << "[PROGRAM] : Resource allocation successful!" << endl << endl;
		else
			throw runtime_error("\n[PROGRAM] : Impossible to do resource allocation . . .");

		// Outputting . . .

		cout << endl << "[PROGRAM] : Attempting to print translated version . . ." << endl << endl;

		createOutputString();

		cout << endl << "[PROGRAM] : Output created successfully!" << endl << endl;

		cout << endl << "[PROGRAM] : Printing output : " << endl << endl;

		printOutputString(f);

		cout << endl << "[PROGRAM] : Output printed!" << endl << endl;

		stlFree();
	}
	catch (runtime_error e)
	{
		cout << e.what() << endl;
		stlFree();
		return 1;
	}
	
	return 0;
}
