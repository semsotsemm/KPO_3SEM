#include "LT.h"
#include "Error.h"

namespace LT 
{
	LexTable Create(int size)
	{
		if (size > LT_MAXSIZE)
		{
			ERROR_THROW(122);
		}
		LexTable lextable;
		lextable.maxsize = size;
		lextable.size = 0;
		lextable.table = new Entry[size];
		return lextable;
	}

	void Add(LexTable& lextable, Entry entry)
	{
		if (lextable.size >= lextable.maxsize)
		{
			ERROR_THROW(123);
		}
		lextable.table[lextable.size] = entry;
		lextable.size++;
	}

	Entry GetEntry(LexTable& lextable, int n)
	{
		if (n < 0 || n >= lextable.size)
		{
			ERROR_THROW(125);
		}
		return lextable.table[n];
	}

	void Delete(LexTable& lextable)
	{
		if (lextable.table != nullptr)
		{
			delete[] lextable.table;
			lextable.table = nullptr;
		}
		lextable.size = 0;
		lextable.maxsize = 0;

	}
}
