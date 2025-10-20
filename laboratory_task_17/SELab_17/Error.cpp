#include "Error.h"

namespace Error
{
    /*����� ������: 0 - 99 - ��������� ������
                    100 - 109 - ������ ����������
                    110 - 119 - ������ �������� � ������ ������
                    120 - 129 - ������ ������ ��������������� � ������
                    200 - 209 - ������ ������������ �������*/
    ERROR errors[ERROR_MAX_ENTRY] =
    {
        ERROR_ENTRY(0, "������������ ��� ������"),
            ERROR_ENTRY(1, "��������� ����"),
            ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
            ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
            ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40), ERROR_ENTRY_NODEF10(50),
            ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),

            ERROR_ENTRY(100, "�������� -in ������ ���� �����"),
            ERROR_ENTRY_NODEF(101), ERROR_ENTRY_NODEF(102), ERROR_ENTRY_NODEF(103),
            ERROR_ENTRY(104, "��������� ����� �������� ���������"),
            ERROR_ENTRY_NODEF(105), ERROR_ENTRY_NODEF(106), ERROR_ENTRY_NODEF(107),
            ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),

            ERROR_ENTRY(110, "������ ��� �������� ����� � �������� ����� (-in)"),
            ERROR_ENTRY(111, "������������ ������ � �������� ����� (-in)"),
            ERROR_ENTRY(112, "������ ��� �������� ����� ���������(-log)"),
            ERROR_ENTRY_NODEF(113), ERROR_ENTRY_NODEF(114), ERROR_ENTRY_NODEF(115),
            ERROR_ENTRY_NODEF(116), ERROR_ENTRY_NODEF(117), ERROR_ENTRY_NODEF(118), ERROR_ENTRY_NODEF(119),

            ERROR_ENTRY(120, "�������� ���������� ������ ��� �������� �������� ���������������"),
            ERROR_ENTRY(121, "�������� ���������� ������ ��� �������� �������� ������"),
            ERROR_ENTRY(122, "������������ �������� ������"),
            ERROR_ENTRY(123, "������������ �������� ������"),
            ERROR_ENTRY(124, "���������� ������ ������� �� ������� ���������� �������� �������� ������"),
            ERROR_ENTRY(125, "���������� ������ ������� �� ������� ���������� �������� �������� ������"),
            ERROR_ENTRY_NODEF(126), ERROR_ENTRY_NODEF(127), ERROR_ENTRY_NODEF(128), ERROR_ENTRY_NODEF(129),
            
            ERROR_ENTRY_NODEF10(130), ERROR_ENTRY_NODEF10(140), ERROR_ENTRY_NODEF10(150),
            ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),

            ERROR_ENTRY_NODEF(200),
            ERROR_ENTRY(201, "�������� ������� ��������� ������������ �������� (65535)"),
            ERROR_ENTRY(202, "��������� ������� ��������� ������������ �����"),
            ERROR_ENTRY(203, "���������� ��������� �������"),
            ERROR_ENTRY(204, "��������� ' (��������� �������) ��� ���������� ��������"), 
            ERROR_ENTRY(205, "���������� ������"),
            ERROR_ENTRY_NODEF(206), ERROR_ENTRY_NODEF(207), ERROR_ENTRY_NODEF(208), ERROR_ENTRY_NODEF(209),
            
            ERROR_ENTRY_NODEF10(110), ERROR_ENTRY_NODEF10(120), ERROR_ENTRY_NODEF10(130), ERROR_ENTRY_NODEF10(140), 
            ERROR_ENTRY_NODEF10(150), ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), 
            ERROR_ENTRY_NODEF10(190),

            ERROR_ENTRY_NODEF100(300), ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500),
            ERROR_ENTRY_NODEF100(600), ERROR_ENTRY_NODEF100(700), ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900),

    };

    ERROR  geterror(int  id)
    {
        ERROR result;

        if (id < 0 || id >= ERROR_MAX_ENTRY)
        {
            result = errors[0];
        }
        else
        {
            result = errors[id];
        }

        return result;
    }
    ERROR  geterrorin(int  id, int  line = -1, int  col = -1)
    {
        ERROR result;

        if (id < 0 || id >= ERROR_MAX_ENTRY)
        {
            result = errors[0];
        }
        else
        {
            result = errors[id];

            result.inext.col = col;
            result.inext.line = line;
        }

        return result;
    }
};