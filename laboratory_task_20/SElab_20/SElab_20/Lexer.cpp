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

        //  онтекст дл€ пометки типов идентификаторов
        bool afterFunctionKeyword = false; // следующа€ идентификаторна€ лексема Ч им€ функции
        bool afterFunctionName = false;    // после имени функции Ч ожидаем '(' дл€ заголовка
        bool inFunctionHeader = false;     // внутри (...), параметры функции
        int funcHeaderParenDepth = 0;
        bool afterDeclare = false;         // после "declare" Ч следующа€ пара (type id) Ч id это переменна€
        char prevToken = 0; // последний выданный токен (лексема), нужен дл€ контекстных решений

        for (int i = 0; i < in.size; ++i)
        {
            char c = in.text[i];

            //нова€ строка
            if (c == '|')
            {
                ++line;
                position = 0;
                continue;
            }

            //пробельные символы
            else if (isspace(c))
            {
                ++position;
                continue;
            }

            //буквы -> ключевое слово/идентификатор
            else if (isalpha(c))
            {
                std::string word;
                int startPos = position;
                while (i < in.size && isalpha(in.text[i]))
                {
                    word += in.text[i];
                    ++i;
                    ++position;
                }
                --i;    //вернЄм на последний символ слова

                if (keywords.count(word))   //ключевое слово
                {
                    char kw = keywords[word];

                    LT::Entry entry;
                    entry.lexema = kw;
                    entry.sn = line;
                    entry.idxTI = LT_TI_NULLIDX;
                    LT::Add(lt, entry);

                    //контекстные флаги
                    if (kw == LEX_FUNCTION)
                    {
                        afterFunctionKeyword = true;
                    }
                    else if (kw == LEX_DECLARE)
                    {
                        afterDeclare = true;
                    }

                    prevToken = kw;
                    continue;
                }
                else    //идетификатор
                {
                    std::string idname = word;
                    if (idname.length() > ID_MAXSIZE)
                    {
                        idname = idname.substr(0, ID_MAXSIZE);
                    }

                    //определим тип идентификатора
                    IT::IDTYPE decidedType = IT::V;  //по умолчанию
                    IT::IDDATATYPE decidedDataType = IT::UNDEF;  //по умолчанию

                    if (afterFunctionKeyword)   //им€ функции
                    {
                        decidedType = IT::F;
                        afterFunctionKeyword = false;
                        afterFunctionName = true;
                    }
                    else if (
                        inFunctionHeader &&
                        (prevToken == LEX_INTEGER || prevToken == LEX_STRING)
                        )   //параметр
                    {
                        decidedType = IT::P;
                    }
                    else if (afterDeclare)
                    {
                        decidedType = IT::V;
                        afterDeclare = false;
                    }

                    if (prevToken == LEX_INTEGER)
                    {
                        decidedDataType = IT::INT;
                    }
                    else if (prevToken == LEX_STRING)
                    {
                        decidedDataType = IT::STR;
                    }
                    else
                    {
                        decidedDataType = IT::UNDEF;
                    }

                    int identId = IT::IsId(it, (char*)idname.c_str());

                    if (identId == TI_NULLIDX)  //проверка был-ли уже записан
                    {
                        IT::Entry entry;

                        strcpy_s(entry.id, sizeof(entry.id), idname.c_str());

                        entry.idtype = decidedType;
                        entry.iddatatype = decidedDataType; //определитс€ парсером?

                        if (decidedDataType == IT::INT)
                        {
                            entry.value.vint = TI_INT_DEFAULT;
                        }
                        else if (decidedDataType == IT::STR)
                        {
                            entry.value.vstr.len = 0;   //по умолчанию
                            strcpy_s(entry.value.vstr.str, sizeof(entry.value.vstr.str), TI_STR_DEFAULT);
                        }

                        entry.idxfirstLE = lt.size + 1; //строка в таблице лексем

                        IT::Add(it, entry);
                        identId = it.size - 1;      //строка в таблице идентификаторов
                    }
                    else    //возможно сможем определить ранее не известные пол€
                    {
                        IT::Entry& existing = it.table[identId];

                        if (existing.idtype != decidedType && decidedType != IT::V)
                        {
                            existing.idtype = decidedType;
                        }

                        if (existing.iddatatype == IT::UNDEF && decidedDataType != IT::UNDEF)
                        {
                            existing.iddatatype = decidedDataType;
                        }
                    }

                    LT::Entry lexEntry;
                    lexEntry.lexema = LEX_ID;
                    lexEntry.sn = line;
                    lexEntry.idxTI = identId;
                    LT::Add(lt, lexEntry);

                    prevToken = LEX_ID;
                    continue;
                }
            }

            //числовой литерал
            else if (isdigit(c))
            {
                std::string number;
                while (i < in.size && isdigit(in.text[i]))
                {
                    number += in.text[i];
                    ++i;
                    ++position;
                }
                --i;


                if (number.size() > 10)
                {
                    ERROR_THROW(201);
                }

                long long literal = std::stoll(number);
                if (literal > 2147483648)
                {
                    ERROR_THROW(201);
                }

                IT::Entry entry;
                strcpy_s(entry.id, "Lnum");
                entry.value.vint = (int)literal;
                entry.idtype = IT::L;
                entry.iddatatype = IT::INT;
                entry.idxfirstLE = lt.size + 1;

                IT::Add(it, entry);
                int literalId = it.size - 1;

                LT::Entry lexEntry;
                lexEntry.lexema = LEX_LITERAL;
                lexEntry.sn = line;
                lexEntry.idxTI = literalId;
                LT::Add(lt, lexEntry);

                prevToken = LEX_LITERAL;
                continue;
            }

            //строковый литерал
            else if (c == '\'')
            {
                i++; ++position;//пропускаю \'
                std::string literal;

                while (i < in.size && in.text[i] != '\'')
                {
                    literal += in.text[i];
                    ++i;
                    ++position;
                }

                if (i == in.size || in.text[i] != '\'')
                {
                    ERROR_THROW_IN(204, line, position);
                }

                if (literal.size() > TI_STR_MAXSIZE)
                {
                    ERROR_THROW(202);
                }

                IT::Entry entry;
                strcpy_s(entry.id, "Lstr");
                strcpy_s(entry.value.vstr.str, sizeof(entry.value.vstr.str), literal.c_str());
                entry.value.vstr.len = (char)literal.length();
                entry.idtype = IT::L;
                entry.iddatatype = IT::STR;
                entry.idxfirstLE = lt.size + 1;

                IT::Add(it, entry);
                int literalId = it.size - 1;

                LT::Entry lexEntry;
                lexEntry.lexema = LEX_LITERAL;
                lexEntry.sn = line;
                lexEntry.idxTI = literalId;
                LT::Add(lt, lexEntry);

                prevToken = LEX_LITERAL;
                ++position;
                continue;
            }
            else
            {
                ++position;
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
                default:
                    ERROR_THROW(205, line, position);
                }

                //контекстное обновление
                if (symbol == LEX_LEFTHESIS)
                {
                    if (afterFunctionName)
                    {
                        inFunctionHeader = true;
                        funcHeaderParenDepth = 1;
                        afterFunctionName = false;
                    }
                    else if (inFunctionHeader)
                    {
                        ++funcHeaderParenDepth;
                    }
                }
                else if (symbol == LEX_RIGHTTHESIS)
                {
                    if (inFunctionHeader)
                    {
                        --funcHeaderParenDepth;
                        if (funcHeaderParenDepth <= 0)
                        {
                            inFunctionHeader = false;
                            funcHeaderParenDepth = 0;
                        }
                    }
                }

                if (symbol != 0)
                {
                    LT::Entry lexEntry = { symbol, line, LT_TI_NULLIDX };
                    LT::Add(lt, lexEntry);
                    prevToken = symbol;
                }
            }
        }
    }
}