#include "In.h"
#include "Parm.h"
#include "Error.h"

#include <iostream>
#include <fstream>
#include <vector>

namespace In
{
    IN getin(wchar_t infile[], wchar_t outfile[])
    {
        IN in;

        in.size = 0;
        in.lines = 1;
        in.ignor = 0;
        in.text = nullptr;

        std::ifstream file(infile);

        if (!file.is_open())
        {
            ERROR_THROW(110);
        }
        else
        {
            std::vector <unsigned char> text_buffer;

            int line = 1;
            int position = 0;

            char c;
            while (file.get(c))
            {
                unsigned char uc = (unsigned char)c;
                ++position;

                int symbol = in.code[uc];

                switch (symbol)
                {
                case IN::T:
                    text_buffer.push_back(uc);
                    break;

                case IN::F:
                    ERROR_THROW_IN(111, line, position);
                    break;

                case IN::I:
                    in.ignor++;
                    break;

                default:
                    text_buffer.push_back((unsigned char)symbol);
                    break;
                }

                if (uc == ' ' || uc == '\t')
                {
                    if (!text_buffer.empty() && text_buffer.back() != ' ')
                    {
                        text_buffer.push_back(' ');
                    }
                }
                else if (uc == IN_CODE_ENDL)
                {
                    ++line;
                    in.lines++;
                    position = 0;
                }
            }

            std::ofstream ofile(outfile);

            in.size = text_buffer.size();
            in.text = new unsigned char[in.size + 1];

            for (int i = 0; i < in.size; i++)
            {
                in.text[i] = (unsigned char)text_buffer[i];
                ofile << (unsigned char)text_buffer[i];
            }
            in.text[in.size] = '\0';
        }
        file.close();
        return in;
    }
}