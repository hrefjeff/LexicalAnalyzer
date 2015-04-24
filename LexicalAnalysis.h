/********************************************************************
Name        : Jeffrey Allen
Class       : CS421, Otuhmatuh
Assignment  : Lexical Analyzer
Description : This is the interface file for a lexical analyzer
********************************************************************/

#ifndef _LEXICALANALYSIS_H
#define _LEXICALANALYSIS_H

#include <iostream>
#include <string>

using namespace std;

enum TOKENS
{
	// Tokens 0-5
	ZEROPLACEHOLDER, PROGRAM, VAR, BEGIN, END, END_PROG,

	// Tokens 6-10
	INTEGER, FOR, READ, WRITE, TO,

	// Tokens 11-15
	DO, SEMICOLON, COLON, COMMA, OP_ASSIGN,

	// Tokens 16-20
	OP_PLUS, OP_MINUS, OP_MULT, DIV, L_PAREN,

	// Tokens 21-25
	R_PAREN, IDENT, INT_LIT, COMMENT, ENDCOMMENT,

	// Tokens 26-30
	UNDERSCORE, LETTER, DIGIT, DELIMITER, ERROR,

	// Tokens 31-33
	SPACE, TAB, ENDOFLINE
};
enum INTERNAL_STATES
{ 
	q0, q1, q2, q3, q4, q5, q6, q7, qerr 
};

class LexicalAnalysis
{
	private:
		string _input;

		void _addCharToLexeme();
		void _getCharFromInput();
		void _removeCharFromInput();
		void _retract();

	public:
		char   buffer;
		char*  PEAKingBuffer;
		int    charClass;
		string lexeme;

		LexicalAnalysis();
		LexicalAnalysis(string);
		~LexicalAnalysis();
		
		void setNewInput(string in);
		void initBuffer();
		void preprocess();

		bool PROGRAM_recognizer();
		bool VAR_recognizer();
		bool BEGIN_recognizer();
		bool END_recognizer();
		bool END_PROG_recognizer();
		bool INTEGER_recognizer();
		bool FOR_recognizer();
		bool READ_recognizer();
		bool WRITE_recognizer();
		bool TO_recognizer();
		bool DO_recognizer();
		bool DIV_recognizer();

		int Lex();
};

#endif