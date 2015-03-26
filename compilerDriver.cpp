/********************************************************************
Name        : Jeffrey Allen
Class       : CS421, Otuhmatuh
Assignment  : Lexical Analyzer
Description : Compiler
********************************************************************/

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include "LexicalAnalysis.h"

using namespace std;

string convertToken(int t);

unordered_map<string, int> symbolTable =
{
	{ "PROGRAM", 1 },
	{ "VAR",     2 },
	{ "BEGIN",   3 },
	{ "END",     4 },
	{ "END.",    5 },
	{ "INTEGER", 6 },
	{ "FOR",     7 },
	{ "READ",    8 },
	{ "WRITE",   9 },
	{ "TO",     10 },
	{ "DO",     11 },
	{ ";",      12 },
	{ ":",      13 },
	{ ",",      14 },
	{ ":=",     15 },
	{ "+",      16 },
	{ "-",      17 },
	{ "*",      18 },
	{ "DIV",    19 },
	{ "(",      20 },
	{ ")",      21 }
};

pair<string, int> tableEntry;

int main()
{
	cout << "==================== Jeffrey's Lexical Analysis ===================" << endl;
	int token = SPACE;

	ifstream file;
	string  fileName;
	string  nextLine;
	int lineNumber = 1;

	bool badFile = true;

	// Get file
	do
	{
		cout << "Input file name (default is 'exampleProgram.txt'): ";
		cin >> fileName;

		file.open(fileName);
		badFile = file.fail();

	} while (badFile);

	cout << endl << endl;

	getline(file,nextLine);
	LexicalAnalysis LineInProgram(nextLine);

	// Read entire program
	while (!file.eof())
	{
		LineInProgram.preprocess();

		cout << "________________________________________________________" << endl;
		cout << lineNumber << "\t" << nextLine << endl << endl;

		// Read one line of program at a time
		while (token != ENDOFLINE)
		{
			token = LineInProgram.Analyze();

			if (token == ENDOFLINE)
				continue;

			if (token == ERROR)
				cout << "\tERROR" << "\t" << LineInProgram.lexeme << endl;
			else 
				cout << "\t" << token << "\t" << LineInProgram.lexeme << endl;

			symbolTable.insert(make_pair(LineInProgram.lexeme, token));
		}

		getline(file,nextLine);
		LineInProgram.setNewInput(nextLine);
		lineNumber++;
		token = SPACE;
	}

	system("PAUSE");
	return 0;
}

string convertToken(int tok)
{
	switch (tok)
	{
		//auto search = symbolTable.find("DIV");
		case PROGRAM  : return "PROGRAM";
		case VAR      : return "VAR";
		case BEGIN    : return "BEGIN";
		case END      : return "END";
		case END_PROG : return "END.";
		case INTEGER  : return "INTEGER";
		case FOR      : return "FOR";
		case READ     : return "READ";
		case WRITE    : return "WRITE";
		case TO       : return "TO";
		case DO       : return "DO";
		case SEMICOLON: return ";";
		case COMMA	  : return ",";
		case OP_ASSIGN: return ":=";
		case COLON    : return ":";
		case SPACE    : return "SPACE";
		case LETTER   : return "LETTER";
		case DIGIT    : return "DIGIT";
		case ERROR    : return "ERROR";
		case OP_PLUS  : return "OP_PLUS";
		case OP_MULT  : return "OP_MULT";
		case DIV      : return "DIV";
		case COMMENT  : return "COMMENT_BEG";
		case ENDCOMMENT : return "COMMENT_END";
		case UNDERSCORE : return "UNDERSCORE";
		case L_PAREN    : return "L_PAREN";
		case R_PAREN    : return "R_PAREN";
		case IDENT      : return "id";
		case INT_LIT    : return "int";
		case ENDOFLINE: return "-------------------------------";
		default         : return "o_____o";
	}
}

//cout << search->first << " has " << search->second << endl;

// range based loop. auto uses type inferences. 
// uses initialization expression to determine it's type.
// neato.
/*for (auto x : symbolTable)
cout << x.first << "\t:\t" << x.second << endl;

auto search = symbolTable.find("END.");

cout << search->first << " : " << search->second << endl;
*/