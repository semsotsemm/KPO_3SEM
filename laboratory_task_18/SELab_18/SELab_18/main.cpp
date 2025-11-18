#include <iostream>
#include <locale>
#include <tchar.h>

#include "LT.h"
#include "IT.h"
#include "MFST.h"	//магазинный автомат
#include "GRB.h"	
#include <iostream>

#include "Lexer.h"
#include "Error.h"
#include "Parm.h"
#include "Log.h"
#include "In.h"

int _tmain(int argc, _TCHAR* argv[])
{
    setlocale(LC_ALL, "rus");

    Log::LOG log = Log::INITLOG;    //начальная инициализация LOG

    LT::LexTable lextable = LT::Create(LT_MAXSIZE);
    IT::IdTable idtable = IT::Create(TI_MAXSIZE);

    try
    {
        Parm::PARM parm = Parm::getparm(argc, argv);

        log = Log::getlog(parm.log);//создание поотока для записи
        Log::WriteLog(log);         //время создания
        Log::WriteParm(log, parm);

        In::IN in = In::getin(parm.in, parm.out);   //преобразование исходного текста

        Log::WriteLine(log, (char*)"---------- Лесический анализ --------\n", "");

        Lexer::Analyze(in, lextable, idtable);

        Log::WriteLine(log, (char*)"--------- Результаты анализа --------\n", "");

        Log::WriteLexTable(log, lextable);
        Log::WriteIdTable(log, idtable);

        Log::WriteLine(log, (char*)"Успешное завершение.", "");
        Log::Close(log);

        MFST_TRACE_START
        MFST::Mfst mfst(lextable, GRB::getGreibach());

        bool ok = mfst.start(); // запустить анализ
        if (ok)
        {
            std::cout << "Синтаксический анализ прошёл успешно." << std::endl;
        }
        else
        {
            std::cout << "Синтаксический анализ обнаружил ошибки." << std::endl;
        }

        mfst.savededucation();						//сохранить вывести правила вывода

        mfst.printrules();							//отладка: вывести правила вывода


        LT::Delete(lextable);
        IT::Delete(idtable);
    }

    catch (Error::ERROR e)
    {
        std::cout << e.message << std::endl;
        Log::WriteError(log, e);


        Log::Close(log);
        return -2;
    }

    Log::Close(log);
    return 0;
}