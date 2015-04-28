/********************************************************************
Name        : Jeffrey Allen
Class       : CS421, Otuhmatuh
Assignment  : Lexical Analyzer + Syntactic Analyzer
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
int LINE_NUMBER = 1;
//=============================

void printDiagnostics(int);
string convertToken(int t);

//========Parser functions=====
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
void exp();
void term();
void factor();
void read();
void write();
void fer();
void indexExp();
void body();

void error(string, int);
//============================

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
	cout << "==================== Jeffrey's Scanner+Parser ===================" << endl << endl;
	
	string  fileName;
	bool badFile = true;

	// Get file
	do
	{
		cout << "Input file name (default is 'exampleProgram.txt'): ";
		cin >> fileName;
		PROGRAM_FILE.open(fileName);
		badFile = PROGRAM_FILE.fail();

	} while (badFile);

	
	// Read entire program
	while (!PROGRAM_FILE.eof())
	{
		getline(PROGRAM_FILE, NEXTLINE);
		cout << LINE_NUMBER << ":" << NEXTLINE << endl;
		WHOLE_PROGRAM.append(NEXTLINE);
		WHOLE_PROGRAM.append("$");
		LINE_NUMBER++;
	}

	LINE_NUMBER = 1;
	THE_PROGRAM.setNewInput(WHOLE_PROGRAM);
	THE_PROGRAM.preprocess();
	THE_PROGRAM.initBuffer();

	// initializer
	TOKEN = SPACE;

	// Program recognizer
	prog();

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
	stmtList();
	advance();

	if (TOKEN != END_PROG)
		error("'END.'!?!?!? BRUH! We can't finish the program without 'END.'!", LINE_NUMBER);
	
	LINE_NUMBER++;
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

	if (THE_PROGRAM.buffer == ';')
	{
		advance();
		advance();
		dec();
	}
}

/*-------------------------------------------------------------------
Name      : dec
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
			<type> ::== INTEGER
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
}

/*-------------------------------------------------------------------
Name      : stmtList
Writer(s) : Jeffrey Allen
Purpose   : Parses strings in the language generated by the rule:
			<stmt-list> ::== <stmt> { ; <stmt> }
Incoming  : n/a
Outgoing  : n/a
-------------------------------------------------------------------*/
void stmtList()
{
	stmt();

	if (THE_PROGRAM.buffer == ';')
	{
		advance();
		stmtList();
	}
}

/*-------------------------------------------------------------------
Name      : stmt
Writer(s) : Jeffrey Allen
Purpose   : Parses strings in the language generated by the rule:
			<stmt> ::= <assign> | <read> | <write> | <for>
Incoming  : n/a
Outgoing  : n/a
-------------------------------------------------------------------*/
void stmt()
{

	if (TOKEN == READ)
		read();

	else if (TOKEN == WRITE)
		write();

	else if (TOKEN == FOR)
		fer();

	else if (TOKEN == IDENT)
		assign();

	
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
	if (TOKEN != IDENT)
		error("Assign statment doesn't start with an identifier! >:O", LINE_NUMBER);

	advance();

	if (TOKEN != OP_ASSIGN)
		error("Assign statment missing assignment assign token! >:O", LINE_NUMBER);

	advance();

	exp();

	if (TOKEN != SEMICOLON)
		error("Assign statement missing a semicolon!! >:O", LINE_NUMBER);
}

/*-------------------------------------------------------------------
Name      : exp
Writer(s) : Jeffrey Allen
Purpose   : Parses strings in the language generated by the rule:
			exp ::= term { + term | - term }
Incoming  : n/a
Outgoing  : n/a
-------------------------------------------------------------------*/
void exp()
{
	term();
		
	if (TOKEN == OP_PLUS || TOKEN == OP_MINUS)
	{
		advance();
		term();
	}

}

/*-------------------------------------------------------------------
Name      : term
Writer(s) : Jeffrey Allen
Purpose   : Parses strings in the language generated by the rule:
			<term> ::== factor { * factor | DIV factor }
Incoming  : n/a
Outgoing  : string
-------------------------------------------------------------------*/
void term()
{
	factor();

	if (TOKEN == OP_MULT || TOKEN == DIV)
	{
		advance();
		advance();
		factor();
	}
}

/*-------------------------------------------------------------------
Name      : factor
Writer(s) : Jeffrey Allen
Purpose   : Parses strings in the language generated by the rule:
			<factor> ::== factor ::= id | int | ( exp )
Incoming  : n/a
Outgoing  : n/a
-------------------------------------------------------------------*/
void factor()
{
	if (TOKEN == IDENT || TOKEN == INT_LIT)
	{
		advance();

	} else {

		if (TOKEN == L_PAREN)
		{
			advance();
			exp();

			if (TOKEN == R_PAREN) advance();
			else error("Where's the closing paren!? >:O", LINE_NUMBER);
		}

	}
}

/*-------------------------------------------------------------------
Name      : read
Writer(s) : Jeffrey Allen
Purpose   : Parses strings in the language generated by the rule:
			<read> ::== READ ( <id-list>)
Incoming  : n/a
Outgoing  : n/a
-------------------------------------------------------------------*/
void read()
{
	if (TOKEN != READ) error("READ doesn't even start with the keyword READ! >:O",LINE_NUMBER);
	
	advance();

	if (TOKEN == L_PAREN)
	{
		advance();
		idList();
		advance();

		if (TOKEN != R_PAREN)
			error("READ statement missing a closing paren! >:O",LINE_NUMBER);
	
	} else {
	
		error("READ missing an opening paren!! >:O",LINE_NUMBER);
	
	}

	advance();
}

/*-------------------------------------------------------------------
Name      : write
Writer(s) : Jeffrey Allen
Purpose   : Parses strings in the language generated by the rule:
			<read> ::== WRITE ( <id-list>)
Incoming  : n/a
Outgoing  : n/a
-------------------------------------------------------------------*/
void write()
{
	if (TOKEN != WRITE) error("WRITE doesn't even start with the keyword READ! >:O", LINE_NUMBER);

	advance();

	if (TOKEN == L_PAREN)
	{
		advance();
		idList();
		advance();

		if (TOKEN != R_PAREN)
			error("WRITE statement missing a closing paren! >:O", LINE_NUMBER);

	} else {

		error("WRITE missing an opening paren!! >:O", LINE_NUMBER);

	}
}

/*-------------------------------------------------------------------
Name      : fer
Writer(s) : Jeffrey Allen
Purpose   : Parses strings in the language generated by the rule:
			<for> ::== FOR <index-exp> DO <body>
Incoming  : n/a
Outgoing  : n/a
-------------------------------------------------------------------*/
void fer()
{
	if (TOKEN != FOR)
		error("FOR token not found!! >:O",LINE_NUMBER);

	advance();

	indexExp();

	if (TOKEN != DO)
		error("DO keyword not found in for loop! >:O", LINE_NUMBER);

	advance();

	body();

	advance();
}

/*-------------------------------------------------------------------
Name      : indexExp
Writer(s) : Jeffrey Allen
Purpose   : Parses strings in the language generated by the rule:
			<index-exp> ::== id := <exp> TO <exp>
Incoming  : n/a
Outgoing  : n/a
-------------------------------------------------------------------*/
void indexExp()
{
	if (TOKEN != IDENT)
		error("id not found for <index-exp>!!! >:O", LINE_NUMBER);

	exp();

	if (TOKEN != OP_ASSIGN)
		error("No assign statement in <index-exp!!!> >:O", LINE_NUMBER);

	exp();

	// get int literal
	advance();

	// get TO
	advance();

	if (TOKEN != TO)
		error("TO not found for <index-exp>!!! >:O", LINE_NUMBER);

	advance();

	exp();
}

/*-------------------------------------------------------------------
Name      : body
Writer(s) : Jeffrey Allen
Purpose   : Parses strings in the language generated by the rule:
			body ::== <stmt> | BEGIN <stmt-list> END
Incoming  : n/a
Outgoing  : n/a
-------------------------------------------------------------------*/
void body()
{
	if (TOKEN == BEGIN)
	{
		advance();
		stmtList();

		if (TOKEN != END)
		{
			error("Missing END in body!", LINE_NUMBER);

		} else {

			return;

		}
	} else {
		stmt();
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