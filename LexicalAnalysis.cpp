/********************************************************************
Name        : Jeffrey Allen
Class       : CS421, Otuhmatuh
Assignment  : Lexical Analyzer
Description : This is the implementation file for a lexical analyzer
********************************************************************/

#include <string>
#include "LexicalAnalysis.h"

LexicalAnalysis::LexicalAnalysis() : _input("")
{
	charClass = ERROR;
	buffer = ' ';
	PEAKingBuffer = NULL;
	lexeme = "";
	initBuffer();
}

LexicalAnalysis::LexicalAnalysis(string someInput)
{
	_input = someInput;
	charClass = ERROR;
	buffer = ' ';
	PEAKingBuffer = NULL;
	lexeme = "";
	initBuffer();
}

LexicalAnalysis::~LexicalAnalysis(){}

void LexicalAnalysis::_addCharToLexeme()
{
	lexeme += buffer;
}

void LexicalAnalysis::_getCharFromInput()
{
	// If there is a string to pull from
	if (_input.size() > 0)
	{
		// Get next character/make case insensitive
		buffer = _input[0];
		// Erase char from input
		_input.erase(0, 1);

		// peak ahead in input
		if (_input.size() > 0)
			PEAKingBuffer = &_input[0];
		else
			PEAKingBuffer = NULL;

		// Otherwise, we're at the end of the line
	}
	else {

		buffer = '$';
	}

	// Initializer
	charClass = ERROR;

	//cout << "Char is: " << buffer << ". Upper is: " << upperCaseChar << endl;

	// If it's letter, classify it as a letter
	if ((buffer > 64) && (buffer < 91))
		charClass = LETTER;

	// If it's numeric, set as digit
	if ((buffer > 47) && (buffer < 58))
		charClass = DIGIT;

	if (buffer == ' ') charClass = SPACE;
	if (buffer == '\t') charClass = TAB;

	if (buffer == '(')
	{
		if (*PEAKingBuffer == '*')
			charClass = COMMENT;
		else
			charClass = L_PAREN;
	}

	if (buffer == '*')
	{
		if (*PEAKingBuffer == ')')
			charClass = ENDCOMMENT;
		else
			charClass = OP_MULT;
	}

	if (buffer == ')') charClass = R_PAREN;
	if (buffer == '$') charClass = ENDOFLINE;
	if (buffer == '+') charClass = OP_PLUS;
	if (buffer == '-') charClass = OP_MINUS;
	if (buffer == ';') charClass = SEMICOLON;
	if (buffer == ':') 
	{

		if (*PEAKingBuffer == '=')
			charClass = OP_ASSIGN;
		else
			charClass = COLON;
	}

	if (buffer == ',') charClass = COMMA;
	if (buffer == '_') charClass = UNDERSCORE;

}

void LexicalAnalysis::_removeCharFromInput()
{
	_input.erase(0, 1);
}

void LexicalAnalysis::_retract()
{
	if (_input.size() > 0)
		PEAKingBuffer = &_input[0];
}

void LexicalAnalysis::initBuffer()
{
	if (_input.size() > 0)
	{
		buffer = toupper(_input[0]);

		if (_input.size() > 1)
			PEAKingBuffer = &_input[1];
	}
	else {

		buffer = '$';
	}

	// Initializer
	charClass = ERROR;

	// If it's letter, classify it as a letter
	if ((buffer > 64) && (buffer < 91))
		charClass = LETTER;

	// If it's numeric, set as digit
	if ((buffer > 47) && (buffer < 58))
		charClass = DIGIT;

	if (buffer == ' ') charClass = SPACE;

	if (buffer == '(')
	{
		if (*PEAKingBuffer == '*')
			charClass = COMMENT;
		else
			charClass = L_PAREN;
	}

	if (buffer == '*')
	{
		if (*PEAKingBuffer == ')')
			charClass = ENDCOMMENT;
		else
			charClass = OP_MULT;
	}

	if (buffer == ')') charClass = R_PAREN;
	if (buffer == '$') charClass = ENDOFLINE;
	if (buffer == '+') charClass = OP_PLUS;
	if (buffer == ';') charClass = SEMICOLON;
	if (buffer == ':')
	{
		if (*PEAKingBuffer == '=')
			charClass = OP_ASSIGN;
		else
			charClass = COLON;
	}

	if (buffer == ',')
		charClass = COMMA;
	if (buffer == '_') charClass = UNDERSCORE;
	if (buffer == '\t') charClass = TAB;
	if (buffer == ' ') charClass = SPACE;
}

void LexicalAnalysis::setNewInput(string someInput)
{
	_input = someInput;
}

void LexicalAnalysis::preprocess()
{
	// Make Case Insensitive
	for (unsigned int i = 0; i < _input.length(); i++)
	{
		_input[i] = toupper(_input[i]);
	}

	// Remove tabs and whitespaces that are more than two spaces long
	for (unsigned int j = 0; j < _input.length() - 1; j++)
	{
		if (_input[j] == ' ' && _input[j + 1] == ' ')
			_input.erase(j, 2);

		if (_input[j] == ' ' && _input[j + 1] == '\t')
			_input.erase(j, 2);

		if (_input[j] == '\t' && _input[j + 1] == ' ')
			_input.erase(j, 2);

		if (_input[j] == '\t' && _input[j + 1] == '\t')
			_input.erase(j, 2);

	}
}

bool LexicalAnalysis::PROGRAM_recognizer()
{
	int PROGRAM_stateDiagram[9][7] =
	{
		{   q1, qerr, qerr, qerr, qerr, qerr, qerr },
		{ qerr,   q2, qerr, qerr, qerr, qerr, qerr },
		{ qerr, qerr,   q3, qerr, qerr, qerr, qerr },
		{ qerr, qerr, qerr,   q4, qerr, qerr, qerr },
		{ qerr, qerr, qerr, qerr,   q5, qerr, qerr },
		{ qerr, qerr, qerr, qerr, qerr,   q6, qerr },
		{ qerr, qerr, qerr, qerr, qerr, qerr,   q7 },
		{ qerr, qerr, qerr, qerr, qerr, qerr, qerr },
		{ qerr, qerr, qerr, qerr, qerr, qerr, qerr }
	};

	enum INTEGER_LETTERS { P, R, O, G, SECOND_R, A, M };

	bool possible = true;
	int THE_LETTER;
	int current_state = q0;

	// Initial check
	if (buffer == 'P')
	{
		THE_LETTER = P;
		current_state = PROGRAM_stateDiagram[current_state][THE_LETTER];
	}

	do
	{
		possible = false;

		if (*PEAKingBuffer == 'R')
		{
			possible = true;
			THE_LETTER = R;
		}
		else if (*PEAKingBuffer == 'O')
		{
			possible = true;
			THE_LETTER = O;
		}

		if (!possible) break;

		 current_state = PROGRAM_stateDiagram[current_state][THE_LETTER];
		 PEAKingBuffer++;

	} while (possible);

	do
	{
		possible = false;

		 if (*PEAKingBuffer == 'G')
		{
			possible = true;
			THE_LETTER = G;
		}

		if (*PEAKingBuffer == 'R')
		{
			possible = true;
			THE_LETTER = SECOND_R;
		}
		else if (*PEAKingBuffer == 'A')
		{
			possible = true;
			THE_LETTER = A;
		}
		else if (*PEAKingBuffer == 'M')
		{
			possible = true;
			THE_LETTER = M;
		}
		if (!possible) break;

		current_state = PROGRAM_stateDiagram[current_state][THE_LETTER];
		PEAKingBuffer++;

	} while (possible);

	if (current_state == q7)
		return true;

	_retract();
	return false;
}

bool LexicalAnalysis::VAR_recognizer()
{
	int VAR_stateDiagram[5][3] =
	{
		{ q1, qerr, qerr },
		{ qerr, q2, qerr },
		{ qerr, qerr, q3 },
		{ qerr, qerr, qerr },
		{ qerr, qerr, qerr }
	};

	enum FOR_LETTERS { V, A, R };

	bool possible = true;
	int THE_LETTER;
	int current_state = q0;

	// Initial check
	if (buffer == 'V')
	{
		THE_LETTER = V;
		current_state = VAR_stateDiagram[current_state][THE_LETTER];
	}

	do
	{
		possible = false;
		if (*PEAKingBuffer == 'A')
		{
			possible = true;
			THE_LETTER = A;
		}
		else if (*PEAKingBuffer == 'R')
		{
			possible = true;
			THE_LETTER = R;
		}

		if (!possible) break;

		current_state = VAR_stateDiagram[current_state][THE_LETTER];
		PEAKingBuffer++;

	} while (possible);

	if (current_state == q3)
		return true;

	_retract();
	return false;
}

bool LexicalAnalysis::BEGIN_recognizer()
{
	int BEGIN_stateDiagram[7][5] =
	{
		{ q1, qerr, qerr, qerr, qerr },
		{ qerr, q2, qerr, qerr, qerr },
		{ qerr, qerr, q3, qerr, qerr },
		{ qerr, qerr, qerr, q4, qerr },
		{ qerr, qerr, qerr, qerr, q5 },
		{ qerr, qerr, qerr, qerr, qerr },
		{ qerr, qerr, qerr, qerr, qerr }
	};

	enum BEGIN_LETTERS { B, E, G, I, N };

	bool possible = true;
	int THE_LETTER;
	int current_state = q0;

	// Initial check
	if (buffer == 'B')
	{
		THE_LETTER = B;
		current_state = BEGIN_stateDiagram[current_state][THE_LETTER];
	}

	do
	{
		possible = false;

		if (*PEAKingBuffer == 'E')
		{
			possible = true;
			THE_LETTER = E;
		}
		else if (*PEAKingBuffer == 'G')
		{
			possible = true;
			THE_LETTER = G;
		}
		else if (*PEAKingBuffer == 'I')
		{
			possible = true;
			THE_LETTER = I;
		}
		else if (*PEAKingBuffer == 'N')
		{
			possible = true;
			THE_LETTER = N;
		}

		if (!possible) break;

		current_state = BEGIN_stateDiagram[current_state][THE_LETTER];
		PEAKingBuffer++;

	} while (possible);

	if (current_state == q5)
		return true;

	_retract();
	return false;
}

bool LexicalAnalysis::END_recognizer()
{
	_retract();
	int END_stateDiagram[5][3] =
	{
		{ q1  , qerr, qerr },
		{ qerr,   q2, qerr },
		{ qerr, qerr,   q3 },
		{ qerr, qerr, qerr },
		{ qerr, qerr, qerr }
	};

	enum END_LETTERS { E, N, D };

	bool possible = true;
	int THE_LETTER;
	int current_state = q0;

	// Initial check
	if (buffer == 'E')
	{
		THE_LETTER = E;
		current_state = END_stateDiagram[current_state][THE_LETTER];
	}

	do
	{
		possible = false;

		if (*PEAKingBuffer == 'N')
		{
			possible = true;
			THE_LETTER = N;
		}
		else if (*PEAKingBuffer == 'D')
		{
			possible = true;
			THE_LETTER = D;
		}

		if (!possible) break;

		current_state = END_stateDiagram[current_state][THE_LETTER];
		PEAKingBuffer++;

	} while (possible);

	PEAKingBuffer++;
	if (*PEAKingBuffer != '.')
		if (current_state == q3)
			return true;

	_retract();
	return false;
}

bool LexicalAnalysis::END_PROG_recognizer()
{
	int END_PROG_stateDiagram[6][4] =
	{
		{ q1,   qerr, qerr, qerr },
		{ qerr,   q2, qerr, qerr },
		{ qerr, qerr,   q3, qerr },
		{ qerr, qerr, qerr,   q4 },
		{ qerr, qerr, qerr, qerr },
		{ qerr, qerr, qerr, qerr }
	};

	enum END_PROG_LETTERS { E, N, D, PERIOD };

	bool possible = true;
	int THE_LETTER;
	int current_state = q0;

	// Initial check
	if (buffer == 'E')
	{
		THE_LETTER = E;
		current_state = END_PROG_stateDiagram[current_state][THE_LETTER];
	}

	do
	{
		possible = false;
		
		if (*PEAKingBuffer == 'N')
		{
			possible = true;
			THE_LETTER = N;
		}
		else if (*PEAKingBuffer == 'D')
		{
			possible = true;
			THE_LETTER = D;
		}
		else if (*PEAKingBuffer == '.')
		{
			possible = true;
			THE_LETTER = PERIOD;
		}

		if (!possible) break;

		current_state = END_PROG_stateDiagram[current_state][THE_LETTER];
		PEAKingBuffer++;

	} while (possible);

	if (current_state == q4)
		return true;

	return false;
}

bool LexicalAnalysis::INTEGER_recognizer()
{
	int INTEGER_stateDiagram[9][7] =
	{
		{   q1, qerr, qerr, qerr, qerr, qerr, qerr },
		{ qerr,   q2, qerr, qerr, qerr, qerr, qerr },
		{ qerr, qerr,   q3, qerr, qerr, qerr, qerr },
		{ qerr, qerr, qerr,   q4, qerr, qerr, qerr },
		{ qerr, qerr, qerr, qerr,   q5, qerr, qerr },
		{ qerr, qerr, qerr, qerr, qerr,   q6, qerr },
		{ qerr, qerr, qerr, qerr, qerr, qerr,   q7 },
		{ qerr, qerr, qerr, qerr, qerr, qerr, qerr },
		{ qerr, qerr, qerr, qerr, qerr, qerr, qerr }
	};

	enum INTEGER_LETTERS { I, N, T, E, G, SECOND_E, R };

	bool possible = true;
	int THE_LETTER;
	int current_state = q0;

	// Initial check
	if (buffer == 'I')
	{
		THE_LETTER = I;
		current_state = INTEGER_stateDiagram[current_state][THE_LETTER];
	}

	do
	{
		possible = false;

		if (*PEAKingBuffer == 'N')
		{
			possible = true;
			THE_LETTER = N;
		}
		else if (*PEAKingBuffer == 'T')
		{
			possible = true;
			THE_LETTER = T;
		}
		else if (*PEAKingBuffer == 'E')
		{
			possible = true;
			THE_LETTER = E;
		}

		if (!possible) break;

		current_state = INTEGER_stateDiagram[current_state][THE_LETTER];
		PEAKingBuffer++;

	} while (possible);

	do
	{
		possible = false;

		if (*PEAKingBuffer == 'G')
		{
			possible = true;
			THE_LETTER = G;
		}

		else if (*PEAKingBuffer == 'E')
		{
			possible = true;
			THE_LETTER = SECOND_E;
		}
		else if (*PEAKingBuffer == 'R')
		{
			possible = true;
			THE_LETTER = R;
		}

		if (!possible) break;

		current_state = INTEGER_stateDiagram[current_state][THE_LETTER];
		PEAKingBuffer++;

	} while (possible);

	if (current_state == q7)
		return true;

	_retract();
	return false;
}

bool LexicalAnalysis::FOR_recognizer()
{
	int FOR_stateDiagram[5][3] =
	{
		{ q1, qerr, qerr },
		{ qerr, q2, qerr },
		{ qerr, qerr, q3 },
		{ qerr, qerr, qerr },
		{ qerr, qerr, qerr }
	};

	enum FOR_LETTERS { F, O, R };

	bool possible = true;
	int THE_LETTER;
	int current_state = q0;

	// Initial check
	if (buffer == 'F')
	{
		THE_LETTER = F;
		current_state = FOR_stateDiagram[current_state][THE_LETTER];
	}

	do
	{
		possible = false;
		if (*PEAKingBuffer == 'O')
		{
			possible = true;
			THE_LETTER = O;
		}
		else if (*PEAKingBuffer == 'R')
		{
			possible = true;
			THE_LETTER = R;
		}

		if (!possible) break;

		current_state = FOR_stateDiagram[current_state][THE_LETTER];
		PEAKingBuffer++;

	} while (possible);

	if (current_state == q3)
		return true;

	_retract();
	return false;
}

bool LexicalAnalysis::READ_recognizer()
{
	int READ_stateDiagram[6][4] =
	{
		{ q1, qerr, qerr, qerr },
		{ qerr, q2, qerr, qerr },
		{ qerr, qerr, q3, qerr },
		{ qerr, qerr, qerr, q4 },
		{ qerr, qerr, qerr, qerr },
		{ qerr, qerr, qerr, qerr }
	};

	enum END_PROG_LETTERS { R, E, A, D };

	bool possible = true;
	int THE_LETTER;
	int current_state = q0;

	// Initial check
	if (buffer == 'R')
	{
		THE_LETTER = R;
		current_state = READ_stateDiagram[current_state][THE_LETTER];
	}

	do
	{
		possible = false;

		if (*PEAKingBuffer == 'E')
		{
			possible = true;
			THE_LETTER = E;
		}
		else if (*PEAKingBuffer == 'A')
		{
			possible = true;
			THE_LETTER = A;
		}
		else if (*PEAKingBuffer == 'D')
		{
			possible = true;
			THE_LETTER = D;
		}

		if (!possible) break;

		current_state = READ_stateDiagram[current_state][THE_LETTER];
		PEAKingBuffer++;

	} while (possible);

	if (current_state == q4)
		return true;

	return false;
}

bool LexicalAnalysis::WRITE_recognizer()
{
	int WRITE_stateDiagram[7][5] =
	{
		{   q1, qerr, qerr, qerr, qerr },
		{ qerr,   q2, qerr, qerr, qerr },
		{ qerr, qerr,   q3, qerr, qerr },
		{ qerr, qerr, qerr,   q4, qerr },
		{ qerr, qerr, qerr, qerr,   q5 },
		{ qerr, qerr, qerr, qerr, qerr },
		{ qerr, qerr, qerr, qerr, qerr }
	};

	enum WRITE_LETTERS { W, R, I, T, E };

	bool possible = true;
	int THE_LETTER;
	int current_state = q0;

	// Initial check
	if (buffer == 'W')
	{
		THE_LETTER = W;
		current_state = WRITE_stateDiagram[current_state][THE_LETTER];
	}

	do
	{
		possible = false;

		if (*PEAKingBuffer == 'R')
		{
			possible = true;
			THE_LETTER = R;
		}
		else if (*PEAKingBuffer == 'I')
		{
			possible = true;
			THE_LETTER = I;
		}
		else if (*PEAKingBuffer == 'T')
		{
			possible = true;
			THE_LETTER = T;
		}
		else if (*PEAKingBuffer == 'E')
		{
			possible = true;
			THE_LETTER = E;
		}

		if (!possible) break;

		current_state = WRITE_stateDiagram[current_state][THE_LETTER];
		PEAKingBuffer++;

	} while (possible);

	if (current_state == q5)
		return true;

	_retract();
	return false;
}

bool LexicalAnalysis::TO_recognizer()
{
	int TO_stateDiagram[4][2] =
	{
		{ q1  , qerr },
		{ qerr,   q2 },
		{ qerr, qerr },
		{ qerr, qerr }
	};

	enum TO_LETTERS { T, O };

	int THE_LETTER;
	int current_state = q0;

	if (buffer == 'T') THE_LETTER = T;

	current_state = TO_stateDiagram[current_state][THE_LETTER];

	if (current_state == q1)
		if (*PEAKingBuffer == 'O')
			current_state = q2;

	if (current_state == q2)
		return true;

	return false;
}

bool LexicalAnalysis::DO_recognizer()
{
	int DO_stateDiagram[4][2] =
	{
		{   q1, qerr },
		{ qerr,   q2 },
		{ qerr, qerr },
		{ qerr, qerr }
	};

	enum TO_LETTERS { D, O };

	int THE_LETTER;
	int current_state = q0;

	if (buffer == 'D')
		THE_LETTER = D;

	current_state = DO_stateDiagram[current_state][THE_LETTER];

	if (current_state == q1)
		if (*PEAKingBuffer == 'O')
			current_state = q2;
	
	if (current_state == q2)
		return true;

	return false;
}

bool LexicalAnalysis::DIV_recognizer()
{
	_retract();
	int DIV_stateDiagram[5][3] =
	{
		{ q1, qerr, qerr },
		{ qerr, q2, qerr },
		{ qerr, qerr, q3 },
		{ qerr, qerr, qerr },
		{ qerr, qerr, qerr }
	};

	enum DIV_LETTERS { D, I, V };

	bool possible = true;
	int THE_LETTER;
	int current_state = q0;

	// Initial check
	if (buffer == 'D')
	{
		THE_LETTER = D;
		current_state = DIV_stateDiagram[current_state][THE_LETTER];
	}

	do
	{
		possible = false;

		if (*PEAKingBuffer == 'I')
		{
			possible = true;
			THE_LETTER = I;
		}
		else if (*PEAKingBuffer == 'V')
		{
			possible = true;
			THE_LETTER = V;
		}

		if (!possible) break;

		current_state = DIV_stateDiagram[current_state][THE_LETTER];
		PEAKingBuffer++;

	} while (possible);

	
	if (current_state == q3)
		return true;

	_retract();
	return false;
}

/*============================

	1) Initialize
	2) Huge Switch statement for recognizers
	   
============================*/
int LexicalAnalysis::Lex()
{
	/*.........1) Initialize...........*/
	lexeme = "";

	if (charClass == SPACE)
	{
		_removeCharFromInput();
		initBuffer();
	}

	if (charClass == TAB)
	{
		_removeCharFromInput();
		initBuffer();
	}

	if (charClass == COMMENT)
	{
		while (charClass != ENDCOMMENT)
		{
			// if EOL, return comment cuz it's a multiline comment
			if (PEAKingBuffer == NULL) return COMMENT;

			_getCharFromInput();
		}

		_getCharFromInput();
		_getCharFromInput();
	}

	if (charClass == ERROR)
	{
		_addCharToLexeme();
		_getCharFromInput();
		return ERROR;
	}

	if (charClass == ENDOFLINE)
		return ENDOFLINE;

	/*.........2) Huge Switch Statement...........*/
	switch (charClass)
	{
		case UNDERSCORE:
		case LETTER:
		{
			_addCharToLexeme();
			_getCharFromInput();

			if (buffer == 'P' && PROGRAM_recognizer())
			{
				// Get R
				_getCharFromInput();
				_addCharToLexeme();
				// Get O
				_getCharFromInput();
				_addCharToLexeme();
				// Get G
				_getCharFromInput();
				_addCharToLexeme();
				// Get R
				_getCharFromInput();
				_addCharToLexeme();
				// Get A
				_getCharFromInput();
				_addCharToLexeme();
				// Get M
				_getCharFromInput();
				_addCharToLexeme();
				
				return PROGRAM;
			}

			if (buffer == 'V' && VAR_recognizer())
			{
				// Get A
				_getCharFromInput();
				_addCharToLexeme();
				// Get R
				_getCharFromInput();
				_addCharToLexeme();
				
				return VAR;
			}

			if (buffer == 'B' && BEGIN_recognizer())
			{
				// Get E
				_getCharFromInput();
				_addCharToLexeme();
				// Get G
				_getCharFromInput();
				_addCharToLexeme();
				// Get I
				_getCharFromInput();
				_addCharToLexeme();
				// Get N
				_getCharFromInput();
				_addCharToLexeme();
				
				return BEGIN;
			}

			if (buffer == 'E' && END_PROG_recognizer())
			{
				// Get N
				_getCharFromInput();
				_addCharToLexeme();
				// Get D
				_getCharFromInput();
				_addCharToLexeme();
				// Get .
				_getCharFromInput();
				_addCharToLexeme();
				
				return END_PROG;
			}

			if (buffer == 'E' && END_recognizer())
			{
				// Get N
				_getCharFromInput();
				_addCharToLexeme();
				// Get D
				_getCharFromInput();
				_addCharToLexeme();
				
				return END;
			}

			if (buffer == 'I' && INTEGER_recognizer())
			{
				// Get N
				_getCharFromInput();
				_addCharToLexeme();
				// Get T
				_getCharFromInput();
				_addCharToLexeme();
				// Get E
				_getCharFromInput();
				_addCharToLexeme();
				// Get G
				_getCharFromInput();
				_addCharToLexeme();
				// Get E
				_getCharFromInput();
				_addCharToLexeme();
				// Get R
				_getCharFromInput();
				_addCharToLexeme();

				return INTEGER;
			}

			if (buffer == 'F' && FOR_recognizer())
			{
				// Get O
				_getCharFromInput();
				_addCharToLexeme();
				// Get R
				_getCharFromInput();
				_addCharToLexeme();
				return FOR;
			}

			if (buffer == 'R' && READ_recognizer())
			{
				// Get E
				_getCharFromInput();
				_addCharToLexeme();
				// Get A
				_getCharFromInput();
				_addCharToLexeme();
				// Get D
				_getCharFromInput();
				_addCharToLexeme();
				return READ;
			}


			if (buffer == 'W' && WRITE_recognizer())
			{
				// Get R
				_getCharFromInput();
				_addCharToLexeme();
				// Get I
				_getCharFromInput();
				_addCharToLexeme();
				// Get T
				_getCharFromInput();
				_addCharToLexeme();
				// Get E
				_getCharFromInput();
				_addCharToLexeme();
				return WRITE;
			}

			if (buffer == 'T' && TO_recognizer())
			{
				_getCharFromInput();
				_addCharToLexeme();
				return TO;
			}

			if (buffer == 'D' && DO_recognizer())
			{
				_getCharFromInput();
				_addCharToLexeme();
				return DO;
			}

			if (buffer == 'D' && DIV_recognizer())
			{
				// Get I
				_getCharFromInput();
				_addCharToLexeme();
				// Get V
				_getCharFromInput();
				_addCharToLexeme();
				return DIV;
			}


			do
			{
				if (PEAKingBuffer == NULL)
					return IDENT;
				if (*PEAKingBuffer == ' ')
					return IDENT;
				if (*PEAKingBuffer == ';')
					return IDENT;
				if (*PEAKingBuffer == ':')
					return IDENT;
				if (*PEAKingBuffer == ',')
					return IDENT;
				if (*PEAKingBuffer == ')')
					return IDENT;

				_getCharFromInput();
				_addCharToLexeme();

			} while (charClass == LETTER || charClass == DIGIT || charClass == UNDERSCORE);

			return IDENT;
			break;
		}

		case DIGIT:

			_addCharToLexeme();
			_getCharFromInput();

			do 
			{	
				if (PEAKingBuffer == NULL)
					return INT_LIT;
				if (*PEAKingBuffer == ' ')
					return INT_LIT;
				if (*PEAKingBuffer == ';')
					return INT_LIT;
				if (*PEAKingBuffer == ':')
					return INT_LIT;
				if (*PEAKingBuffer == ',')
					return INT_LIT;

				if (charClass == LETTER || charClass == UNDERSCORE)
				{
					do
					{
						if (*PEAKingBuffer == ' ')
							return IDENT;
						if (*PEAKingBuffer == ';')
							return IDENT;
						if (*PEAKingBuffer == ':')
							return IDENT;
						if (*PEAKingBuffer == ',')
							return IDENT;
						if (PEAKingBuffer == NULL)
							return IDENT;

						_addCharToLexeme();
						_getCharFromInput();

					} while (charClass == LETTER || charClass == DIGIT || charClass == UNDERSCORE);

					return IDENT;
				}

				_getCharFromInput();
				_addCharToLexeme();

			} while (charClass == DIGIT);

			return INT_LIT;
			break;

		case OP_PLUS:
			_addCharToLexeme();
			_getCharFromInput();

			return OP_PLUS;
			break;

		case OP_MINUS:
			_addCharToLexeme();
			_getCharFromInput();

			return OP_MINUS;
			break;

		case SEMICOLON:
			_addCharToLexeme();
			_getCharFromInput();

			return SEMICOLON;
			break;

		case COLON:
			_addCharToLexeme();
			_getCharFromInput();

			return COLON;
			break;

		case COMMA:
			_addCharToLexeme();
			_getCharFromInput();

			return COMMA;
			break;

		case OP_ASSIGN:
			_addCharToLexeme();
			_getCharFromInput();
			_getCharFromInput();
			_addCharToLexeme();

			return OP_ASSIGN;
			break;

		case OP_MULT:
			_addCharToLexeme();
			_getCharFromInput();

			return OP_MULT;
			break;

		case L_PAREN:
			_addCharToLexeme();
			_getCharFromInput();

			return L_PAREN;
			break;

		case R_PAREN:
			_addCharToLexeme();
			_getCharFromInput();

			return R_PAREN;
			break;

		return ERROR;
	}

	return ERROR;
}