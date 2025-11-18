#include "IT.h"
#include "Error.h"
#include <cstring>

namespace IT
{
	IdTable Create(int size)
	{
		if (size > TI_MAXSIZE)
		{
			ERROR_THROW(120);
		}
		IdTable idtable;
		idtable.maxsize = size;
		idtable.size = 0;
		idtable.table = new Entry[size];
		return idtable;
	}

	void Add(IdTable& idtable, Entry& entry)
	{
		if (idtable.size >= idtable.maxsize)
		{
			ERROR_THROW(122);
		}
		idtable.table[idtable.size] = entry;
		idtable.size++;
	}

	Entry GetEntry(IdTable& idtable, int n)
	{
		if (n < 0 || n >= idtable.size)
		{
			ERROR_THROW(124);
		}
		return idtable.table[n];
	}

	int IsId(IdTable& idtable, const char id[ID_MAXSIZE + 1])
	{
		for (int i = 0; i < idtable.size; ++i)
		{
			if (strcmp(idtable.table[i].id, id) == 0)
			{
				return i;
			}
		}
		return TI_NULLIDX;
	}

	const char* GetDataTypeString(int iddatatype) {
		switch (iddatatype) {
		case IT::INT: return "INT";
		case IT::STR: return "STR";
		default: return "UNKNOWN";
		}
	}

	const char* GetIdTypeString(int idtype) {
		switch (idtype) {
		case IT::V: return "(V)Переменная"; // V = Variable
		case IT::F: return "(F)Функция";    // F = Function
		case IT::P: return "(P)Параметр";   // P = Parameter
		case IT::L: return "(L)Литерал";    // L = Literal
		default: return "??";
		}
	}

	void Delete(IdTable& idtable)
	{
		if (idtable.table != nullptr)
		{
			delete[] idtable.table;
			idtable.table = nullptr;
		}
		idtable.maxsize = 0;
		idtable.size = 0;
	}
}
