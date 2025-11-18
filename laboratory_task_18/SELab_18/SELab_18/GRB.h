#pragma once

#include "Error.h"
#include <vector>
#include <initializer_list>

typedef short GRBALPHABET;	//СИМВОЛЫ АЛФАВИТА T > 0, N < 0

namespace GRB
{
	/* ПРАВИЛО(правило в граматике)*/
	struct Rule
	{
		struct Chain;

		GRBALPHABET nn;				//нетерминал < 0
		int iderror;				//id диагностического сообщения
		std::vector<Chain> chains;	//вектор правых частей

		Rule() = default;

		Rule(GRBALPHABET pnn, int piderror, std::initializer_list<Chain> pchains)
			: nn(pnn), iderror(piderror), chains(pchains) {}

		//Получить правило в виде N->цепочка (для печати)
		char* getCRule(
			char* b,	//буфер
			short nchain//номер цепочки(правой части) в правиле
		);
		//Получить следующую за j подходящую цепочку? вернуть её номер : -1
		short getNextChain(
			GRBALPHABET t,			//первый символ цепочки
			Rule::Chain& pchain,	//возвращаемая цепочка
			short j					//номер цепочки
		);
	};

	/*ЦЕПОЧКА(правая чать правила)
	chains - вектор цепочек(правых частей правила)*/
	struct Rule::Chain
	{
		std::vector<GRBALPHABET> nt;	//цепочка терминалов (> 0) и нетерминалов (< 0)

		Chain() = default;

		Chain(std::initializer_list<GRBALPHABET> symbols) 
			: nt(symbols) {};

		//Получить правую сторону правила
		char* getCChain(char* b);

		//Терминал > 0
		static GRBALPHABET T(char t) { return GRBALPHABET(t); }
		//Не терминал < 0
		static GRBALPHABET N(char n) { return -GRBALPHABET(n); }

		//Терминал?
		static bool isT(GRBALPHABET s) { return s > 0; }
		//Не терминал?
		static bool isN(GRBALPHABET s) { return !isT(s); };

		//GRBALPHABET -> char
		static char alphabet_to_char(GRBALPHABET s) { return isT(s) ? char(s) : char(-s); };
	};

	/*ГРАМАТИКА ГРЕЙБАХА*/
	struct Greibach
	{
		GRBALPHABET startN;			//стартовый символ
		GRBALPHABET stbottomT;		//дно стека
		std::vector<Rule> rules;	//множество правил

		Greibach() = default;

		Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottomT, std::initializer_list<Rule> prules)
			: startN(pstartN), stbottomT(pstbottomT), rules(prules) {};

		//Получить номер правила? номер : -1
		short getRule(
			GRBALPHABET pnn,	//левый символ правила
			Rule& prule			//возвращаемое правило граматики
		);
		//Получить правило по индексу? правило : пустой Rule
		Rule getRule(short n);
	};

	//Получить граматику
	Greibach getGreibach();
}
