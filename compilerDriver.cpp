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
string WHOLE_PROGRAM;
LexicalAnalysis THE_PROGRAM;
ifstream PROGRAM_FILE;
string NEXTLINE;
int TOKEN;
int LINE_NUMBER = 0;
//=============================

void printDiagnostics(int);
string convertToken(int t);

void advance();
void prog();
void progName();
void decList();
void dec();
void type();
void idList();
void stmtList();
void stmt();
void assign();
void expr();
void term();
void factor();
void read();
void write();
void fer();
void indexExp();
void body();

void error(string,int);

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

	
	// Read entire program
	while (!PROGRAM_FILE.eof())
	{
		getline(PROGRAM_FILE, NEXTLINE);
		WHOLE_PROGRAM.append(NEXTLINE);
		WHOLE_PROGRAM.append("$");
	}


	THE_PROGRAM.setNewInput(WHOLE_PROGRAM);
	THE_PROGRAM.preprocess();
	THE_PROGRAM.initBuffer();

	TOKEN = SPACE;

	prog();

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

	/*	getline(PROGRAM_FILE,NEXTLINE);
		LINE_IN_PROGRAM.setNewInput(NEXTLINE);
		LINE_IN_PROGRAM.initBuffer();
		TOKEN = SPACE;
	}*/

	printDiagnostics(LINE_NUMBER);

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
		cout << x.second << ": " << x.first << endl;
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

void advance()
{
	THE_PROGRAM.initBuffer();
	TOKEN = THE_PROGRAM.Lex();
	if (TOKEN == ENDOFLINE)
	{
		LINE_NUMBER++;
		THE_PROGRAM.initBuffer();
		TOKEN = THE_PROGRAM.Lex();
	}
}

/*-------------------------------------------------------------------
Name      : prog
Writer(s) : Jeffrey Allen
Purpose   : Parses strings in the language generated by the rule:
			<prog> ::== PROGRAM <prog-name> VAR <dec-list> BEGIN <stmt-list> END.
Incoming  : n/a
Outgoing  : n/a
-------------------------------------------------------------------*/
void prog()
{
	advance();

	if (TOKEN != PROGRAM)
	{
		error("PROGRAM keyword not detected >:O", LINE_NUMBER);
	} 
	
	advance();

	progName();
	
	advance();

	if (TOKEN != VAR)
	{
		error("VAR keyword not detected >:O", LINE_NUMBER);
	}

	advance();

	decList();

	advance();

	if (TOKEN != BEGIN)
		error("BEGIN keyword not detected >:O", LINE_NUMBER);

	advance();

	if (TOKEN != END_PROG)
		error("BRUH! We can't finish the program without END.", LINE_NUMBER);
	
}

/*-------------------------------------------------------------------
Name      : progName
Writer(s) : Jeffrey Allen
Purpose   : Parses strings in the language generated by the rule:
			<prog-name> ::== id
Incoming  : int
Outgoing  : string
-------------------------------------------------------------------*/
void progName()
{
	if (TOKEN != IDENT) 
		error("Invalid program name identifier!!", LINE_NUMBER);
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

	advance();

	if (THE_PROGRAM.buffer == ';')
		dec();
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

	advance();

	if (TOKEN != COLON) error("Where's the colon!? >:O", LINE_NUMBER);

	advance();

	type();
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
	if (TOKEN != INTEGER) error("Invalid type! >:O", LINE_NUMBER);
}

/*-------------------------------------------------------------------
Name      : idList
Writer(s) : Jeffrey Allen
Purpose   : Parses strings in the language generated by the rule:
			<id-list> ::== id {, id}
Incoming  : n/a
Outgoing  : n/a
-------------------------------------------------------------------*/
void idList()
{
	if (TOKEN != IDENT) error("Invalid ID in IDENT-list >:O", LINE_NUMBER);
	
	if (*THE_PROGRAM.PEAKingBuffer == ',')
	{
		advance();
		advance();
		idList();
	}

	return;
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
	return;
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
	return;
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
		THE_PROGRAM.initBuffer();
		TOKEN = THE_PROGRAM.Lex();

		if (TOKEN == OP_ASSIGN)
		{
			THE_PROGRAM.initBuffer();
			TOKEN = THE_PROGRAM.Lex();

			if (TOKEN == INT_LIT)
			{
				THE_PROGRAM.initBuffer();
				TOKEN = THE_PROGRAM.Lex();

				if (TOKEN == SEMICOLON)
				{
					error("Invalid type! >:O", LINE_NUMBER);
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
	return;
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

	while (THE_PROGRAM.charClass == OP_MULT || THE_PROGRAM.charClass == DIV)
	{
		THE_PROGRAM.Lex();
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
	if (THE_PROGRAM.charClass == IDENT || THE_PROGRAM.charClass == INT_LIT)
		THE_PROGRAM.Lex();

	else {

		if (THE_PROGRAM.charClass == L_PAREN)
		{
			THE_PROGRAM.Lex();
			expr();

			if (THE_PROGRAM.charClass == R_PAREN)
				THE_PROGRAM.Lex();

			else
				error("Invalid type! >:O", LINE_NUMBER);
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
	return;
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
	return;
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
	return;
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
	return;
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
	return;
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

	while (THE_PROGRAM.charClass == OP_PLUS || THE_PROGRAM.charClass == OP_MINUS)
	{
		THE_PROGRAM.Lex();
		term();
	}
}


/*-------------------------------------------------------------------
Name      : error
Writer(s) : Jeffrey Allen
Purpose   : Inserts an error inside of the error table along with its
            line num
Incoming  : string, int
Outgoing  : n/a
-------------------------------------------------------------------*/
void error(string errorMessage, int lineNum)
{
	errorTable.insert(make_pair(errorMessage, lineNum));
}