#pragma once

#define LEXEME_FIXSIZE	1
#define LT_MAXSIZE		4096
#define LT_TI_NULLIDX	0xffffffff
#define LEX_INTEGER		'n'
#define LEX_BOOL        'b'
#define LEX_STRING		't'
#define LEX_ID			'i'
#define LEX_LITERAL		'l'
#define LEX_FUNCTION	'f'
#define LEX_RETURN		'r'
#define LEX_PRINT		'p'
#define LEX_MAIN		'm'
#define LEX_SEMICOLON	';'
#define LEX_COLON   	':'
#define LEX_COMMA		','
#define LEX_LEFTBRACE	'{'
#define LEX_RIGHTBRACE	'}'
#define LEX_LEFTHESIS	'('
#define LEX_RIGHTTHESIS ')'
#define LEX_PLUS		'+'
#define LEX_MINUS		'-'
#define LEX_STAR		'*'
#define LEX_PECENT      '%'
#define LEX_DIRSLASH	'/'
#define LEX_EQUAL		'='

#define LEX_IF	    	'c'
#define LEX_ELSE	    'e'
#define LEX_SWITCH	    's'
#define LEX_CASE	    'a'
#define LEX_DEFAULT	    'u'
#define LEX_TRUE	    '1'
#define LEX_FALSE	    '0'

//таблица лексем
namespace LT {
    struct Entry
    {
        char lexema[LEXEME_FIXSIZE];
        int sn;     //номер строки
        int idxTI;  //индекс  в таблице идентификаторов
    };

    struct LexTable
    {
        int maxsize;    
        int size;
        Entry* table;
    };

    LexTable Create(int size);
    void Add(LexTable& lextable, Entry entry);
    Entry GetEntry(LexTable& lextable, int n);
    void Delete(LexTable& lextable);
}
