#include <iostream>
#include <fstream>
#include <cstdarg>
#include <iomanip>

#include "Log.h"
#include "Parm.h"
#include "In.h"
#include "IT.h"
#include "LT.h"

namespace Log {
	LOG getlog(wchar_t logfile[]) {
		LOG log;

		wcscpy_s(log.logfile, PARM_MAX_SIZE, logfile);

		log.stream = new std::ofstream(logfile);

		if (!log.stream || !log.stream->is_open())
		{
			ERROR_THROW(112);
		}

		return log;
	}

	void WriteLine(LOG log, char* c, ...)
	{
		va_list args;
		va_start(args, c);

		char* s = c;
		while (s && *s != '\0')
		{
			*log.stream << s;
			s = va_arg(args, char*);
		}

		*log.stream << std::endl;
		va_end(args);
	}

	void WriteLine(LOG log, const wchar_t* first, ...) {
		va_list args;
		va_start(args, first);

		const wchar_t* s = first;
		char buffer[4096];
		size_t converted = 0;

		while (s != nullptr && std::wcscmp(s, L"") != 0) {
			wcstombs_s(&converted, buffer, sizeof(buffer), s, _TRUNCATE);
			*log.stream << buffer;
			s = va_arg(args, const wchar_t*);
		}

		*log.stream << std::endl;
		va_end(args);
	}

	void WriteLog(LOG log)
	{
		char time_buffer[100];
		time_t now = time(NULL);

		tm t;
		localtime_s(&t, &now);
		strftime(time_buffer, sizeof(time_buffer), "%d.%m.%Y в %H:%M:%S", &t);

		WriteLine(log, (char*)"Дата создания протокола: ", (char*)time_buffer, (char*)"\n", "");
	}

	void WriteParm(LOG log, Parm::PARM parm)
	{
		WriteLine(log, (char*)"------------- Параметры -------------\n", "");
		WriteLine(log, (wchar_t*)L"-in :  ", (wchar_t*)parm.in, L"\n",
			(wchar_t*)L"-log:  ", (wchar_t*)parm.log, L"\n",
			(wchar_t*)L"-out:  ", (wchar_t*)parm.out, L"\n", L"-------------------------------------", L"");
	}

	void WriteIn(LOG log, In::IN in)
	{

		wchar_t size_buf[20];
		wchar_t ignor_buf[20];
		wchar_t lines_buf[20];

		_itow_s(in.size, size_buf, 10);
		_itow_s(in.ignor, ignor_buf, 10);
		_itow_s(in.lines, lines_buf, 10);

		WriteLine(log, (char*)"---------- Исходные данные ----------\n", "");
		WriteLine(log, (wchar_t*)L"Количество символов: ", (wchar_t*)size_buf, L"\n",
			(wchar_t*)L"Проигнорировано    : ", (wchar_t*)ignor_buf, L"\n",
			(wchar_t*)L"Количество строк   : ", (wchar_t*)lines_buf, L"\n", L"-------------------------------------", L"");
	}

	void WriteError(LOG log, Error::ERROR error)
	{
		if (log.stream != nullptr && log.stream->is_open())
		{
			char idbuf[20];
			char linebuf[20];
			char colbuf[20];

			_itoa_s(error.id, idbuf, 10);
			if (error.inext.line >= 0 && error.inext.col >= 0)
			{
				_itoa_s(error.inext.line, linebuf, 10);
				_itoa_s(error.inext.col, colbuf, 10);

				WriteLine(log, (char*)"Ошибка: ", error.message,
					(char*)" строка: ", (char*)linebuf,
					(char*)" позиция: ", (char*)colbuf, "");
			}
			else
			{
				WriteLine(log, (char*)"Ошибка: ", error.message, "");
			}
		}
		else
		{
			std::cerr << "Ошибка " << error.id << ": " << error.message;
			if (error.inext.line >= 0 && error.inext.col >= 0)
			{
				std::cerr << " (строка " << error.inext.line
					<< ", позиция " << error.inext.col << ")";
			}
			std::cerr << std::endl;
		}
	}

	void WriteLexTable(LOG log, LT::LexTable& lt)
	{
		char ltsize[32];
		sprintf_s(ltsize, "%d", lt.size);

		WriteLine(log, (char*)"--------- Таблица лексем ---------\n", "");
		WriteLine(log, (char*)"Всего лексем: ", (char*)ltsize, "");

		int pSn = 1;
		*log.stream << "\n";

		for (int i = 0; i < lt.size; i++)
		{
			LT::Entry& entry = lt.table[i];
			int currSn = entry.sn;

			if (pSn != currSn)
			{
				*log.stream << "\n";
			}

			*log.stream << entry.lexema[0] << "(" << entry.sn << ", " << entry.idxTI << ")";

			pSn = currSn;
		}
		*log.stream << "\n\n";
	}
	
	void WriteIdTable(LOG log, IT::IdTable& it)
	{
		WriteLine(log, (char*)"----- Таблица идентификаторов -----\n", "");

		char itsize[32];
		sprintf_s(itsize, "%d", it.size);

		WriteLine(log, (char*)"Всего идентификаторов: ", (char*)itsize, (char*)"\n", "");

		const int W_INDEX = 8;
		const int W_NAME = 12;
		const int W_TYPE = 16;
		const int W_DATATYPE = 15;
		const int W_VALUE = 12;

		*log.stream << std::left
			<< std::setw(W_INDEX) << "Индекс"
			<< std::setw(W_NAME) << "Имя"
			<< std::setw(W_TYPE) << "Тип"
			<< std::setw(W_DATATYPE) << "Тип данных"
			<< std::setw(W_VALUE) << "Значение"
			<< std::endl;

		std::string separator = std::string(W_INDEX + W_NAME + W_TYPE + W_DATATYPE + W_VALUE, '-');
		*log.stream << separator << std::endl;

		for (int i = 0; i < it.size; i++)
		{
			IT::Entry& entry = it.table[i];

			*log.stream << std::left
				<< std::setw(W_INDEX) << entry.idxfirstLE
				<< std::setw(W_NAME) << entry.id
				<< std::setw(W_TYPE) << GetIdTypeString(entry.idtype)
				<< std::setw(W_DATATYPE) << GetDataTypeString(entry.iddatatype);

			*log.stream << std::right << std::setw(W_VALUE);
			if (entry.iddatatype == IT::INT)
			{
				*log.stream << entry.value.vint;
			}
			else if (entry.iddatatype == IT::STR)
			{
				*log.stream << std::left << entry.value.vstr.str;
			}
			else
			{
				*log.stream << "UNDEF";
			}
			*log.stream << "\n";
		}
		*log.stream << "\n\n";
	}

	void Close(LOG log)
	{
		if (log.stream)
		{
			log.stream->close();
			delete log.stream;
		}
	}
}