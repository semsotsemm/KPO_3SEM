#include <iostream>
#include <locale>
#include <tchar.h>
#include <stack>
#include <vector>
#include <string>

#include "LT.h"
#include "IT.h"
#include "MFST.h"	
#include "GRB.h"	

#include "Lexer.h"
#include "Error.h"
#include "Parm.h"
#include "Log.h"
#include "In.h"

const char FILLER_CHAR = '@';


int GetPriority(char op) {
    if (op == '(') return 0;
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == 'v') return 3;
    return -1;
}


bool PolishNotation(
    int expression_pos,
    LT::LexTable& lextable,
    IT::IdTable& idtable
) {
    std::stack<LT::Entry> operators;
    std::vector<LT::Entry> output_queue;
    int current_pos = expression_pos;
    int start_pos = expression_pos;

    while (current_pos < lextable.size &&
        lextable.table[current_pos].lexema != ';' &&
        lextable.table[current_pos].lexema != '{')
    {
        LT::Entry current_lexeme = lextable.table[current_pos];
        char token = current_lexeme.lexema;

        if (token == 'i' || token == 'l') { 
            bool is_function_call = false;

            if (token == 'i' && (current_pos + 1) < lextable.size && lextable.table[current_pos + 1].lexema == '(') {
                is_function_call = true;
            }

            if (is_function_call) {
                operators.push(current_lexeme);
            }
            else {
                output_queue.push_back(current_lexeme);
            }
        }
        else if (token == '(') {
            operators.push(current_lexeme);
        }
        else if (token == ')') {
            while (!operators.empty() && operators.top().lexema != '(') {
                output_queue.push_back(operators.top());
                operators.pop();
            }
            if (operators.empty() || operators.top().lexema != '(') {
                std::cerr << "Ошибка ПОЛИЗ: Непарная скобка в выражении (позиция " << expression_pos << ")" << std::endl;
                return false;
            }
            operators.pop(); 

            if (!operators.empty() && operators.top().lexema == 'i') {
                output_queue.push_back(operators.top());
                operators.pop();
            }
        }
        else if (GetPriority(token) > 0) { 
            int current_priority = GetPriority(token);

            while (!operators.empty() && operators.top().lexema != '(' && GetPriority(operators.top().lexema) >= current_priority) {
                output_queue.push_back(operators.top());
                operators.pop();
            }
            operators.push(current_lexeme);
        }
        else if (token == ',') { 
            while (!operators.empty() && operators.top().lexema != '(') {
                output_queue.push_back(operators.top());
                operators.pop();
            }
        }

        current_pos++;
    }

    while (!operators.empty()) {
        if (operators.top().lexema == '(') {
            std::cerr << "Ошибка ПОЛИЗ: Непарная скобка в конце выражения" << std::endl;
            return false;
        }
        output_queue.push_back(operators.top());
        operators.pop();
    }


    int output_len = output_queue.size();

    for (int i = 0; i < output_len; ++i) {
        lextable.table[start_pos + i] = output_queue[i];
    }

    for (int i = start_pos + output_len; i < current_pos; ++i) {
        LT::Entry filler = {};
        filler.lexema = FILLER_CHAR;
        if (i < lextable.size) filler.sn = lextable.table[i].sn;
        lextable.table[i] = filler;
    }

    std::cout << "ПОЛИЗ (на позиции " << expression_pos << "): ";
    for (const auto& lex : output_queue) {
        std::cout << lex.lexema << " ";
    }
    std::cout << std::endl;

    return true;
}


int _tmain(int argc, _TCHAR* argv[])
{
    setlocale(LC_ALL, "rus");

    Log::LOG log = Log::INITLOG;

    LT::LexTable lextable = LT::Create(LT_MAXSIZE);
    IT::IdTable idtable = IT::Create(TI_MAXSIZE);

    try
    {
        Parm::PARM parm = Parm::getparm(argc, argv);

        log = Log::getlog(parm.log);
        Log::WriteLog(log);
        Log::WriteParm(log, parm);

        In::IN in = In::getin(parm.in, parm.out);

        Log::WriteLine(log, (char*)"---------- Лексический анализ --------\n", "");

        Lexer::Analyze(in, lextable, idtable);

        Log::WriteLine(log, (char*)"--------- Результаты анализа --------\n", "");

        Log::WriteLexTable(log, lextable);
        Log::WriteIdTable(log, idtable);

        Log::WriteLine(log, (char*)"Успешное завершение лексического анализа.", "");

        MFST_TRACE_START
            MFST::Mfst mfst(lextable, GRB::getGreibach());

        bool ok = mfst.start();
        if (ok)
        {
            std::cout << "Синтаксический анализ прошёл успешно." << std::endl;
        }
        else
        {
            std::cout << "Синтаксический анализ обнаружил ошибки." << std::endl;
        }

        mfst.savededucation();
        mfst.printrules();

        Log::WriteLine(log, (char*)"---------- Построение Обратной Польской Записи --------\n", "");
        std::cout << "\n--- Построение ПОЛИЗ ---\n";

        std::vector<int> expression_starts;
        for (int i = 0; i < lextable.size; ++i)
        {
            char current_lex = lextable.table[i].lexema;

            if (current_lex == '=') {
                expression_starts.push_back(i + 1);
            }
            else if (current_lex == 'r' && (i + 1) < lextable.size && lextable.table[i + 1].lexema != ';') {
                expression_starts.push_back(i + 1);
            }
        }

        if (expression_starts.empty()) {
            std::cout << "Выражения для преобразования не найдены." << std::endl;
        }

        int exp_count = 1;
        for (int start_pos : expression_starts) {
            std::cout << "--- Выражение #" << exp_count << " (позиция " << start_pos << ") ---\n";
            if (PolishNotation(start_pos, lextable, idtable)) {
                std::cout << "Успешно преобразовано в ПОЛИЗ." << std::endl;
            }
            else {
                std::cout << "Ошибка при построении ПОЛИЗ." << std::endl;
            }
            exp_count++;
        }

        Log::WriteLine(log, (char*)"--------- Таблица лексем после ПОЛИЗ --------\n", "");
        Log::WriteLexTable(log, lextable);

        Log::WriteLine(log, (char*)"Успешное завершение.", "");
        Log::Close(log);

        LT::Delete(lextable);
        IT::Delete(idtable);
    }
    catch (Error::ERROR e)
    {
        std::cout << "Ошибка " << e.id << ": " << e.message << std::endl;
        Log::WriteError(log, e);
        Log::Close(log);
        return -1;
    }

    return 0;
}