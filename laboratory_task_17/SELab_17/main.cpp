#include <iostream>

#include "Lexer.h"
#include "Error.h"
#include "Parm.h"
#include "Log.h"
#include "LT.h"
#include "IT.h"
#include "In.h"

using namespace std;

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
        
        Log::WriteLine(log, (char*)"---------- Лесический анализ --------\n", "");

        Lexer::Analyze(in, lextable, idtable);

        Log::WriteLine(log, (char*)"--------- Результаты анализа --------\n", "");

        Log::WriteLexTable(log, lextable);
        Log::WriteIdTable(log, idtable);

        Log::WriteLine(log, (char*)"Успешное завершение.", "");
        Log::Close(log);

        LT::Delete(lextable);
        IT::Delete(idtable);
    }

    catch (Error::ERROR e) 
    {
        cout << e.message << endl;
        Log::WriteError(log, e);

        LT::Delete(lextable);
        IT::Delete(idtable);

        Log::Close(log);
        return -2;
    }

    Log::Close(log);
    return 0;
}