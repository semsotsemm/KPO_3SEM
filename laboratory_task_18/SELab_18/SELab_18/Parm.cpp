#include "Parm.h"
#include "Error.h"

#include <cwchar>

namespace Parm
{
	PARM getparm(int argc, _TCHAR* argv[])
	{
		PARM parm;

		parm.in[0] = L'\0';
		parm.log[0] = L'\0';
		parm.out[0] = L'\0';

		for (int i = 1; i < argc; i++)
		{
			if (wcsstr(argv[i], PARM_IN))
			{
				wcscpy_s(parm.in, argv[i] + wcslen(PARM_IN));

				if (wcslen(parm.in) >= PARM_MAX_SIZE)
				{
					ERROR_THROW(104);
				}
			}
			else if (wcsstr(argv[i], PARM_LOG))
			{
				wcscpy_s(parm.log, argv[i] + wcslen(PARM_LOG));

				if (wcslen(parm.in) >= PARM_MAX_SIZE)
				{
					ERROR_THROW(104);
				}
			}
			else if (wcsstr(argv[i], PARM_OUT))
			{
				wcscpy_s(parm.out, argv[i] + wcslen(PARM_OUT));

				if (wcslen(parm.in) >= PARM_MAX_SIZE)
				{
					ERROR_THROW(104);
				}
			}
		}

		if (parm.in[0] == L'\0')
		{
			ERROR_THROW(100);
		}

		if (parm.log[0] == L'\0')
		{
			wcscpy_s(parm.log, parm.in);
			wcsncat_s(parm.log, PARM_MAX_SIZE, L".log", 4);
		}
		if (parm.out[0] == L'\0')
		{
			wcscpy_s(parm.out, parm.in);
			wcsncat_s(parm.out, PARM_MAX_SIZE, L".out", 4);
		}
		return parm;
	}
}