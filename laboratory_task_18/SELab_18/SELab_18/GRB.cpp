#include "GRB.h"

#include <vector>
#include <string> 
#include <cstdarg>

#define GRB_ERROR_SERIES 600

namespace GRB
{
#define NS(n) Rule::Chain::N(n)
#define TS(n) Rule::Chain::T(n)

	Greibach greibach(NS('S'), TS('$'), {	//стартовый символ, дно стека

			Rule(NS('S'), GRB_ERROR_SERIES + 0, {	//Неверная тсруктура программы

					 { TS('m'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';'), NS('S') },
					 { TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';'), NS('S') },
					 { TS('m'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';') }
			}),
			Rule(NS('N'), GRB_ERROR_SERIES + 1, {	//Ошибочный оператор

						 { TS('d'), TS('t'), TS('i'), TS(';') },
						 { TS('r'), NS('E'), TS(';') },
						 { TS('i'), TS('='), NS('E'), TS(';') },
						 { TS('d'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS(';') },
						 { TS('d'), TS('t'), TS('i'), TS(';'), NS('N') },
						 { TS('r'), NS('E'), TS(';'), NS('N') },
						 { TS('i'), TS('='), NS('E'), TS(';'), NS('N') },
						 { TS('d'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS(';'), NS('N') }
			}),
			Rule(NS('E'), GRB_ERROR_SERIES + 2, {	//Ошибка в выражении

						 { TS('i') },
						 { TS('l') },
						 { TS('('), NS('E'), TS(')') },
						 { TS('i'), TS('('), NS('W'), TS(')') },
						 { TS('i'), NS('M') },
						 { TS('l'), NS('M') },
						 { TS('('), NS('E'), TS(')'), NS('M') },
						 { TS('i'), TS('('), NS('W'), TS(')'), NS('M') }
			}),
			Rule(NS('M'), GRB_ERROR_SERIES + 3,	{	//Ошибка в действиях

						 { TS('v'), NS('E') },
						 { TS('v'), NS('E'), NS('M') }
			}),
			Rule(NS('F'), GRB_ERROR_SERIES + 4,	{	//Ошибка в объявоении

						 { TS('t'), TS('i') },
						 { TS('t'), TS('i'), TS(','), NS('F') }
			}),
			Rule(NS('W'), GRB_ERROR_SERIES + 5, {	//Ошибка в параметрах

						 { TS('i') },
						 { TS('l') },
						 { TS('i'), TS(','), NS('W') },
						 { TS('l'), TS(','), NS('W') }
			})
		});

	Greibach getGreibach()
	{
		return greibach;
	};

	short Greibach::getRule(GRBALPHABET pnn, Rule& prule)
	{
		for (int i = 0; i < rules.size(); i++)
		{
			if (rules[i].nn == pnn)
			{
				prule = rules[i];
				return i;
			}
		}
		return -1;
	};

	Rule Greibach::getRule(short n)
	{
		if (n >= 0 && n < rules.size()) 
		{
			return rules[n];
		}
		return Rule();
	}

	char* Rule::getCRule(char* b, short nchain)
	{
		if (nchain >= 0 && nchain < chains.size())
		{
			char bchain[200];
			b[0] = Chain::alphabet_to_char(nn);
			b[1] = '-';
			b[2] = '>';
			b[3] = 0x00;
			chains[nchain].getCChain(bchain);
			strcat_s(b, 205, bchain);
			return b;
		}
		b[0] = 0x00;
		return b;
	};

	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j)
	{
		for (short i = j; i < chains.size(); i++)
		{
			if (!chains[i].nt.empty() && chains[i].nt[0] == t) 
			{
				pchain = chains[i];
				return i;
			}
		}
		return -1;
	};

	char* Rule::Chain::getCChain(char* b)
	{
		for (int i = 0; i < nt.size(); i++)
		{
			b[i] = Chain::alphabet_to_char(nt[i]);
		}
		b[nt.size()] = 0x00;
		return b;
	}
}
