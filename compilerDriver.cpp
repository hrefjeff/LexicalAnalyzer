/********************************************************************
Name        : Jeffrey Allen
Class       : CS421, Otuhmatuh
Assignment  : Lexical Analyzer + Syntaxtic Analyzer
Description : This program demonstrates how a compiler analyzes
              source code lexically and syntactically
********************************************************************/

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include "LexicalAnalysis.h"

using namespace std;

//======Global Variables=======
LexicalAnalysis LINE_IN_PROGRAM;
ifstream PROGRAM_FILE;
string NEXTLINE;
int TOKEN;
//=============================

void printDiagnostics(int);
string convertToken(int t);

bool programNameRecognizer(char* next);
void term();
void expr();
void factor();
void assign();
void error(string);

unordered_map<string, int> errorTable;
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

int main()
{
	cout << "==================== Jeffrey's Parser ===================" << endl << endl;
	
	string  fileName;
	int lineNumber = 0;

	bool badFile = true;

	// Get file
	do
	{
		//cout << "Input file name (default is 'exampleProgram.txt'): ";
		fileName = "expr.txt";
		//cin >> fileName;

		PROGRAM_FILE.open(fileName);
		badFile = PROGRAM_FILE.fail();

	} while (badFile);

	getline(PROGRAM_FILE,NEXTLINE);
	LINE_IN_PROGRAM.setNewInput(NEXTLINE);
	TOKEN = SPACE;

	// Read entire program
	while (!PROGRAM_FILE.eof())
	{
		lineNumber++;
		LINE_IN_PROGRAM.preprocess();

		cout << "________________________________________________________" << endl;
		cout << lineNumber << "\t" << NEXTLINE << endl << endl;

		TOKEN = LINE_IN_PROGRAM.Lex();

		assign();

		// Read one line of program at a time
		// have to find new way to get next token (advance function?)
		//while (token != ENDOFLINE)
		//{

		//	LineInProgram.initBuffer();
		//	LineInProgram.Lex();

		//	// Handle multiline comment
		//	while (token == COMMENT)
		//	{
		//		lineNumber++;
		//		getline(file,nextLine);
		//		cout << "========SNNNNNNAP. THAT LINE WAS A COMMENT!=============" << endl;
		//		cout << lineNumber << "\t" << nextLine << endl << endl;
		//		LineInProgram.setNewInput(nextLine);
		//		LineInProgram.preprocess();
		//		LineInProgram.initBuffer();
		//		LineInProgram.charClass = COMMENT;
		//		token = LineInProgram.Lex();
		//	}

		//	if (token == ENDOFLINE) continue;

		//	if (token == ERROR)
		//	{
		//		cout << "\tInvalid Token" << " ------> " << LineInProgram.lexeme << endl;
		//		errorTable.insert(make_pair((LineInProgram.lexeme).append("Unrecognized token."), token));
		//	
		//	} else {

		//		cout << "\t" << token << "\t" << LineInProgram.lexeme << endl;
		//		symbolTable.insert(make_pair(LineInProgram.lexeme, token));
		//	
		//	}
		//}

		getline(PROGRAM_FILE,NEXTLINE);
		LINE_IN_PROGRAM.setNewInput(NEXTLINE);
		LINE_IN_PROGRAM.initBuffer();
		TOKEN = SPACE;
	}

	printDiagnostics(lineNumber);

	system("PAUSE");
	return 0;
}

/*-------------------------------------------------------------------
Name      : printDiagnostics
Writer(s) : Jeffrey Allen
Purpose   : Outputs a diagnostics message to the console, displaying
            the number of errors accumulated in the error table
Incoming  : int
Outgoing  : n/a
-------------------------------------------------------------------*/
void printDiagnostics(int numOfLines)
{
	int errorCount = 0;
	int tokenCount = 0;
	for (auto x : errorTable)
	{
		errorCount++;
	}

	cout << endl << endl;
	cout << "======================================" << endl;
	cout << "|       Program Diagnostic Report     |" << endl;
	cout << "======================================" << endl;
	cout << "Number of lines  : " << numOfLines << endl;
	cout << "Number of errors : " << errorCount << endl << endl;

	cout << endl << endl;
}

/*-------------------------------------------------------------------
Name      : convertToken
Writer(s) : Jeffrey Allen
Purpose   : Converts an integer representation of token to string
Incoming  : int
Outgoing  : string
-------------------------------------------------------------------*/
string convertToken(int tok)
{
	switch (tok)
	{
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
		case OP_MINUS : return "OP_MINUS";
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

/*-------------------------------------------------------------------
Name      : prog
Writer(s) : Jeffrey Allen
Purpose   : Parses strings in the language generated by the rule:
			<prog> ::== PROGRAM <prog-name> VAR <dec-list> BEGIN <stmt-list END.>
Incoming  : n/a
Outgoing  : n/a
-------------------------------------------------------------------*/
void prog()
{
	LINE_IN_PROGRAM.Lex();

	if (LINE_IN_PROGRAM.charClass == PROGRAM)
	{
		LINE_IN_PROGRAM.Lex();
		progName();

	}
	else {

		cout << "" << endl;

	/*	if (LineInProgram.charClass == L_PAREN)
		{
			LineInProgram.Lex();
			expr();

			if (LineInProgram.charClass == R_PAREN)
				LineInProgram.Lex();

			else
				cout << "Error!!" << endl;
		}*/
	}
}

/*-------------------------------------------------------------------
Name      : progName
Writer(s) : Jeffrey Allen
Purpose   : Parses strings in the language generated by the rule:
			<prog-name> ::== id
Incoming  : int
Outgoing  : string
-------------------------------------------------------------------*/
bool progName()
{
	if (TOKEN == IDENT)
		return true;
	return false;
}

/*-------------------------------------------------------------------
Name      : decList
Writer(s) : Jeffrey Allen
Purpose   : Parses strings in the language generated by the rule:
			<dec-list> ::== <dec> { ; <dec> }
Incoming  : n/a
Outgoing  : n/a
-------------------------------------------------------------------*/
void decList()
{
	dec();

	LINE_IN_PROGRAM.initBuffer();
	TOKEN = LINE_IN_PROGRAM.Lex();

	if (*LINE_IN_PROGRAM.PEAKingBuffer == ';')
	{
		dec();
	}
}

/*-------------------------------------------------------------------
Name      :
Writer(s) : Jeffrey Allen
Purpose   : Parses strings in the language generated by the rule:
			<dec> ::== <id-list> : <type>
Incoming  : n/a
Outgoing  : n/a
-------------------------------------------------------------------*/
void dec()
{
	idList();

	LINE_IN_PROGRAM.initBuffer();
	TOKEN = LINE_IN_PROGRAM.Lex();

	if (TOKEN == COLON)
	{
		LINE_IN_PROGRAM.initBuffer();
		TOKEN = LINE_IN_PROGRAM.Lex();

		type();
	}
}

/*-------------------------------------------------------------------
Name      : type
Writer(s) : Jeffrey Allen
Purpose   : Parses strings in the language generated by the rule:
Incoming  : n/a
Outgoing  : n/a
-------------------------------------------------------------------*/
void type()
{

}
/*-------------------------------------------------------------------
Name      : idList
Writer(s) : Jeffrey Allen
Purpose   : Parses strings in the language generated by the rule:
Incoming  : n/a
Outgoing  : n/a
-------------------------------------------------------------------*/
void idList()
{

}
/*-------------------------------------------------------------------
Name      : stmtList
Writer(s) : Jeffrey Allen
Purpose   : Parses strings in the language generated by the rule:
Incoming  : n/a
Outgoing  : n/a
-------------------------------------------------------------------*/
void stmtList()
{

}
/*-------------------------------------------------------------------
Name      : stmt
Writer(s) : Jeffrey Allen
Purpose   : Parses strings in the language generated by the rule:
Incoming  : n/a
Outgoing  : n/a
-------------------------------------------------------------------*/
void stmt()
{

}
/*-------------------------------------------------------------------
Name      : assign
Writer(s) : Jeffrey Allen
Purpose   : Parses strings in the language generated by the rule:
<assign> ::== id := <exp>
Incoming  : n/a
Outgoing  : n/a
-------------------------------------------------------------------*/
void assign()
{
	if (TOKEN == IDENT)
	{
		LINE_IN_PROGRAM.initBuffer();
		TOKEN = LINE_IN_PROGRAM.Lex();

		if (TOKEN == OP_ASSIGN)
		{
			LINE_IN_PROGRAM.initBuffer();
			TOKEN = LINE_IN_PROGRAM.Lex();

			if (TOKEN == INT_LIT)
			{
				LINE_IN_PROGRAM.initBuffer();
				TOKEN = LINE_IN_PROGRAM.Lex();

				if (TOKEN == SEMICOLON)
				{
					cout << "Valid assign statement!" << endl;
				}
			}
		}
	}

}

/*-------------------------------------------------------------------
Name      : exp
Writer(s) : Jeffrey Allen
Purpose   : Parses strings in the language generated by the rule:
Incoming  : n/a
Outgoing  : n/a
-------------------------------------------------------------------*/
void exp()
{

}

/*-------------------------------------------------------------------
Name      : term
Writer(s) : Jeffrey Allen
Purpose   : Converts an integer representation of token to string
Incoming  : int
Outgoing  : string
-------------------------------------------------------------------*/
void term()
{
	factor();

	while (LINE_IN_PROGRAM.charClass == OP_MULT || LINE_IN_PROGRAM.charClass == DIV)
	{
		LINE_IN_PROGRAM.Lex();
		factor();
	}
}

/*-------------------------------------------------------------------
Name      : factor
Writer(s) : Jeffrey Allen
Purpose   : Converts an integer representation of token to string
Incoming  : int
Outgoing  : string
-------------------------------------------------------------------*/
void factor()
{
	if (LINE_IN_PROGRAM.charClass == IDENT || LINE_IN_PROGRAM.charClass == INT_LIT)
		LINE_IN_PROGRAM.Lex();

	else {

		if (LINE_IN_PROGRAM.charClass == L_PAREN)
		{
			LINE_IN_PROGRAM.Lex();
			expr();

			if (LINE_IN_PROGRAM.charClass == R_PAREN)
				LINE_IN_PROGRAM.Lex();

			else
				cout << "Error!!" << endl;
		}

	}
}

/*-------------------------------------------------------------------
Name      : read
Writer(s) : Jeffrey Allen
Purpose   : Parses strings in the language generated by the rule:
Incoming  : n/a
Outgoing  : n/a
-------------------------------------------------------------------*/
void read()
{

}

/*-------------------------------------------------------------------
Name      : write
Writer(s) : Jeffrey Allen
Purpose   : Parses strings in the language generated by the rule:
Incoming  : n/a
Outgoing  : n/a
-------------------------------------------------------------------*/
void write()
{

}
/*-------------------------------------------------------------------
Name      : fer
Writer(s) : Jeffrey Allen
Purpose   : Parses strings in the language generated by the rule:
Incoming  : n/a
Outgoing  : n/a
-------------------------------------------------------------------*/
void fer()
{

}
/*-------------------------------------------------------------------
Name      : indexExp
Writer(s) : Jeffrey Allen
Purpose   : Parses strings in the language generated by the rule:
Incoming  : n/a
Outgoing  : n/a
-------------------------------------------------------------------*/
void indexExp()
{

}
/*-------------------------------------------------------------------
Name      : body
Writer(s) : Jeffrey Allen
Purpose   : Parses strings in the language generated by the rule:
Incoming  : n/a
Outgoing  : n/a
-------------------------------------------------------------------*/
void body()
{

}

/*-------------------------------------------------------------------
Name      : convertToken
Writer(s) : Jeffrey Allen
Purpose   : Converts an integer representation of token to string
Incoming  : int
Outgoing  : string
-------------------------------------------------------------------*/
void expr()
{
	term();

	while (LINE_IN_PROGRAM.charClass == OP_PLUS || LINE_IN_PROGRAM.charClass == OP_MINUS)
	{
		LINE_IN_PROGRAM.Lex();
		term();
	}
}


/*-------------------------------------------------------------------
Name      : error
Writer(s) : Jeffrey Allen
Purpose   : Displays an error message
Incoming  : string
Outgoing  : n/a
-------------------------------------------------------------------*/
void error(string errorMessage)
{
	cout << "Error: " << errorMessage << endl;
}

