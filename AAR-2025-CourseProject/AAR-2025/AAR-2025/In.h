#pragma once

#define IN_MAX_LEN_TEXT 1024*1024
#define SEPORATOR '|'
#define IN_CODE_ENDL '\n'
#define IN_CODE_QUOTES '\"'
#define IN_CODE_SPACE ' '
#define MINUS '-'
#define REPEAT 'n'
#define BRACKET '('
#define COMMA ','

/*
	 '\0'   '\x1'  '\x2'  '\x3'  '\x4'  '\x5'  '\x9'  '\a'   '\b'   '\t'   '\n'   '\v'   '\f'   '\r'   '\xe'  '\xf'
	 '\x10' '\x11' '\x12' '\x13' '\x14' '\x15' '\x16' '\x17' '\x18' '\x19' '\x1a' '\x1b' '\x1c' '\x1d' '\x1e' '\x1f'
	 ''     '!'    '\"'   '#'    '$'    '%'    '&'    '''    '('    ')'    '*'    '+'     ','   '-'    '.'    '/' 
	 '0'    '1'    '2'    '3'    '4'    '5'    '6'    '7'    '8'    '9'    ':'    ';'     '<'   '='    '>'    '?'
	 '@'    'A'    'B'    'C'    'D'    'E'    'F'    'G'    'H'    'I'    'J'    'K'     'L'   'M'    'N'    'O' 
	 'P'    'Q'    'R'    'S'    'T'    'U'    'V'    'W'    'X'    'Y'    'Z'    '['     '\'   ']'    '^'    '_'
	 '`'    'a'    'b'    'c'    'd'	'e'    'f'    'g'    'h'    'i'    'j'    'k'     'l'   'm'    'n'    'o'
	 'p'    'q'    'r'    's'    't'    'u'    'v'    'w'    'x'    'y'    'z'    '{'    '|'    '}'    '~'    '' 
	 
	 '?'    '?'    '''    '?'    '"'    ':'    '┼'    '╪'    '?'    '%'    '?'    '<'    '?'    '?'    '?'    '?'
	 '?'    '''    '''    '"'    '"'    ''     '-'    '-'    '?'    'T'    '?'    '>'    '?'    '?'    '?'    '?' 
	 ' '    'Ў'    'ў'    '?'    '¤'    '?'    '│'    ''     'Ё'    'c'    'Є'    '<'    '┐'    '-'    'R'    'Ї'   
	 '°'    '+'    '?'    '?'    '?'    'ч'    ''     '·'    'ё'    '№'    'є'    '>'    '?'    '?'    '?'    'ї' 
	 'А'    'Б'    'В'    'Г'    'Д'    'Е'    'Ж'    'З'    'И'    'Й'    'К'    'Л'    'М'    'Н'    'О'    'П'
	 'Р'    'С'    'Т'    'У'    'Ф'    'Х'    'Ц'    'Ч'    'Ш'    'Щ'    'Ъ'    'Ы'    'Ь'    'Э'    'Ю'    'Я'
	 'а'    'б'    'в'    'г'    'д'    'е'    'ж'    'з'    'и'    'й'    'к'    'л'    'м'    'н'    'о'    'п' 
	 'р'    'с'    'т'    'у'    'ф'    'х'    'ц'    'ч'    'ш'    'щ'    'ъ'    'ы'    'ь'    'э'    'ю'    'я'
*/

#define IN_CODE_TABLE  {\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::I, SEPORATOR, IN::F, IN::F, IN::I, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::I, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::F, IN::T, \
	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::F, IN::T, IN::F, IN::F, \
	IN::F, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, \
	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, \
	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::F, IN::T, IN::F, IN::F, \
																													\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::T, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, \
	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, \
	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, \
	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, \
}


namespace In
{
	struct IN
	{
		enum { T = 1024, F = 2048, I = 4096 };
		int size;
		int lines;
		int ignor;
		unsigned char* text;
		int code[256] = IN_CODE_TABLE;
	};
	IN getin(wchar_t infile[], wchar_t outfile[]);
}