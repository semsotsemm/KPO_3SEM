#pragma once

#include "Error.h"
#include <vector>
#include <initializer_list>

typedef short GRBALPHABET;	
namespace GRB
{
	struct Rule
	{
		struct Chain;

		GRBALPHABET nn;				
		int iderror;				
		std::vector<Chain> chains;	

		Rule() = default;

		Rule(GRBALPHABET pnn, int piderror, std::initializer_list<Chain> pchains)
			: nn(pnn), iderror(piderror), chains(pchains) {
		}

		char* getCRule(
			char* b,	
			short nchain
		);
		short getNextChain(
			GRBALPHABET t,			
			Rule::Chain& pchain,	
			short j					
		);
	};

	struct Rule::Chain
	{
		std::vector<GRBALPHABET> nt;

		Chain() = default;

		Chain(std::initializer_list<GRBALPHABET> symbols)
			: nt(symbols) {
		};

		char* getCChain(char* b);

		static GRBALPHABET T(char t) { return GRBALPHABET(t); }
		static GRBALPHABET N(char n) { return -GRBALPHABET(n); }

		static bool isT(GRBALPHABET s) { return s > 0; }
		static bool isN(GRBALPHABET s) { return !isT(s); };

		static char alphabet_to_char(GRBALPHABET s) { return isT(s) ? char(s) : char(-s); };
	};
	struct Greibach
	{
		GRBALPHABET startN;			
		GRBALPHABET stbottomT;		
		std::vector<Rule> rules;	

		Greibach() = default;

		Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottomT, std::initializer_list<Rule> prules)
			: startN(pstartN), stbottomT(pstbottomT), rules(prules) {
		};

		short getRule(
			GRBALPHABET pnn,	
			Rule& prule			
		);
		Rule getRule(short n);
	};

	Greibach getGreibach();
}