#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
using namespace std;

int main()
{
	string s = "qwertyuiopasdfghjklzxcvbnmйцукенгшщзхъфывапролджэячсмитьбюё0123456789+-*/=;,(){}'";

	char c;
	unsigned char uc;

	for (size_t i = 0; i < 16; i++)
	{
		for (size_t j = 0; j < 16; j++)
		{
			uc = 16 * i + j;
			switch (1)
			{
			case 0:
				if (s.find(uc) != string::npos)
				{
					cout << "IN::T, ";
				}
				else if (uc == '\n')
				{
					cout << "SEPORATOR, ";
				}
				else if (uc == '\r')
				{
					cout << "IN::I, ";
				}
				else if (uc == '\t')
				{
					cout << "IN::I, ";
				}
				else if (uc == ' ')
				{
					cout << "IN::I, ";
				}
				else
				{
					cout << "IN::F, ";
				}
				break;
			case 1:
				cout << "\" " << uc << " \" ";
				break;
			}
		}
		cout << "\\\n";
	}
}
