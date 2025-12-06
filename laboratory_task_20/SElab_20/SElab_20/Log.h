#pragma once

#include <fstream>
#include "In.h"
#include "LT.h"
#include "IT.h"
#include "Parm.h"
#include "Error.h"

//работа с протоколом
namespace Log
{
	struct LOG
	{
		// протокол
		wchar_t logfile[PARM_MAX_SIZE]; // имя файла протокола
		std::ofstream* stream;          // выходной поток протокола
	};

	static const LOG INITLOG = { L"", NULL };	//начальная инициализации LOG
	LOG getlog(wchar_t logfile[]);				// сформировать структуру LOG
	void WriteLine(LOG log, char* c, ...);      // вывести в протокол конкатенацию строк
	void WriteLine(LOG log, const wchar_t* c, ...);   // вывести в протокол конкатенацию строк
	void WriteLog(LOG log);                     // вывести в протокол заголовок
	void WriteParm(LOG log, Parm::PARM parm);   // вывести в протокол информацию о входных параметрах
	void WriteIn(LOG log, In::IN in);           // вывести в протокол информацию о входном потоке
	void WriteError(LOG log, Error::ERROR error); // вывести в протокол информацию об ошибке
	void Close(LOG log);                        // закрыть протокол

	void WriteLexTable(LOG log, LT::LexTable& lt);
	void WriteIdTable(LOG log, IT::IdTable& it);
}