#pragma once
#include "Defs.h"
#include <fstream>

class Scaner {
private:
    std::string text; //�������� (�����������) �����
    size_t uk; //������� ������� � ������
    int line; //����� ������ ��� ������ ������
    int posInLine; //������� � ������ ��� ������ ������

public:
    Scaner(const std::string& filename); //�����������
    bool readFile(const std::string& filename); //������ ��������� �����
    size_t getUK() const; //����������� ������� �������
    void setUK(size_t newUK); //�������������� �������
    int nextLexeme(TypeLex& l); //������� ��������� ��������� �������
    void error(const std::string& msg) const; //������� ������ ��������� �� ������
    bool isEnd() const; //�������� ����� �����
    int getLine();
    void setLine(int line);
    int getPosInLine();
    void setPosInLine(int posInline);
};

