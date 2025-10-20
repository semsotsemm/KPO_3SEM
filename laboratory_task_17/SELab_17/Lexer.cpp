#include "Lexer.h"
#include "Error.h"
#include "In.h"

#include <map>
#include <string>
#include <iostream>

namespace Lexer
{
	void Analyze(In::IN& in, LT::LexTable& lt, IT::IdTable& it)
	{
		std::map<std::string, char> keywords;

        keywords["integer"] = LEX_INTEGER;
        keywords["string"] = LEX_STRING;
        keywords["function"] = LEX_FUNCTION;
        keywords["declare"] = LEX_DECLARE;
        keywords["return"] = LEX_RETURN;
        keywords["print"] = LEX_PRINT;
        keywords["main"] = LEX_MAIN;

        int line = 1;
        int position = 0;

        for (int  i = 0; i < in.size; i++, position++)
        {
            char c = in.text[i];

            if (c == '|')
            {
                ++line;
                position = 0;
                continue;
            }

            else if (isspace(c))
            {
                position++;
                continue;
            }

            else if (isalpha(c))
            {
                std::string word = "";
                while (i < in.size && isalpha(in.text[i]))
                {
                    position++;
                    word += in.text[i];
                    ++i;
                }
                position--;
                --i;    //вернул на последний символ слова

                if (keywords.count(word))
                {
                    //ключевое слово
                    LT::Entry entry = { keywords[word], line, LT_TI_NULLIDX };
                    LT::Add(lt, entry);
                }
                else
                {
                    //идетификатор
                    if (word.length() > ID_MAXSIZE)
                    {
                        word = word.substr(0, ID_MAXSIZE);
                    }

                    int identId = IT::IsId(it, (char*)word.c_str());

                    if (identId == TI_NULLIDX)  //проверка была-ли уже записана
                    {
                        IT::Entry entry;

                        strcpy_s(entry.id, word.c_str());
                        entry.idtype = IT::V;
                        entry.iddatatype = IT::UNDEF; //поумолчанию
                        entry.value.vint = TI_INT_DEFAULT;
                        entry.idxfirstLE = lt.size + 1; //строка в таблице лексем

                        IT::Add(it, entry);
                        identId = it.size - 1;      //строка в таблице идентификаторов
                    }

                    LT::Entry lexEntry = { LEX_ID, line, identId }; //тип, строка в файле, id в TI
                    LT::Add(lt, lexEntry);
                }
            }

            else if (isdigit(c))
            {
                //числовой литерал
                std::string number = "";
                while (i < in.size && isdigit(in.text[i]))
                {
                    position++;
                    number += in.text[i];
                    ++i;
                    position--;
                }
                position--;
                --i;

                int literal = std::stoi(number);

                if (literal > INT_MAX)
                {
                    ERROR_THROW(201);
                }

                IT::Entry entry;
                strcpy_s(entry.id, "lnum");
                entry.value.vint = literal;
                entry.idtype = IT::L;
                entry.iddatatype = IT::INT;
                entry.idxfirstLE = lt.size + 1;

                IT::Add(it, entry);
                int literalId = it.size - 1;

                LT::Entry lexEntry = { LEX_LITERAL, line, literalId };
                LT::Add(lt, lexEntry);
            }

            else if (c == '\'')
            {
                //строковый литерал
                std::string literal = "";
                i++;    //пропускаю \'
                
                while (i < in.size && in.text[i] != '\'')
                {
                    literal += in.text[i];
                    position++;
                    ++i;
                }

                if (i == in.size || in.text[i] != '\'')
                {
                    ERROR_THROW(203);
                    ERROR_THROW_IN(204, line, position);
                }

                if (literal.size() > TI_STR_MAXSIZE)
                {
                    ERROR_THROW(202);
                }

                IT::Entry entry;
                strcpy_s(entry.id, "Lstr");
                strcpy_s(entry.value.vstr.str, literal.c_str());
                entry.value.vstr.len = (char)literal.length();
                entry.idtype = IT::L;
                entry.iddatatype = IT::STR;
                entry.idxfirstLE = lt.size + 1;

                IT::Add(it, entry);
                int literalId = it.size - 1;

                LT::Entry lexEntry = { LEX_LITERAL, line, literalId };
                LT::Add(lt, lexEntry);

                continue;
            }
            else
            {
                position++;
                char symbol = 0;
                switch (in.text[i])
                {
                case ';': symbol = LEX_SEMICOLON;
                    break;
                case ',': symbol = LEX_COMMA;
                    break;
                case '{': symbol = LEX_LEFTBRACE;
                    break;
                case '}': symbol = LEX_RIGHTBRACE;
                    break;
                case '(': symbol = LEX_LEFTHESIS;
                    break;
                case ')': symbol = LEX_RIGHTTHESIS;
                    break;
                case '+': symbol = LEX_PLUS;
                    break;
                case '-': symbol = LEX_MINUS;
                    break;
                case '*': symbol = LEX_STAR;
                    break;
                case '/': symbol = LEX_DIRSLASH;
                    break;
                case '=': symbol = LEX_EQUAL;
                    break;
                }
                if (symbol == 0)
                {
                    std::cout << in.text[i];
                    ERROR_THROW(205, line, position);
                }
                LT::Entry lexEntry = { symbol, line, LT_TI_NULLIDX };
                LT::Add(lt, lexEntry);
            }
        }
	}
}