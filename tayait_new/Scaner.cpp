#include "Scaner.h"
#include <iostream>
#include <cctype>
using namespace std;

Scaner::Scaner(const string& filename) : uk(0), line(1), posInLine(1)
{
    readFile(filename);
}

bool Scaner::readFile(const string& filename)
{
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "������ �������� �����: " << filename << endl;
        return false;
    }

    text.assign((istreambuf_iterator<char>(file)), (istreambuf_iterator<char>()));
    file.close();
    return true;
}

TypeLex Keyword[KEYWORD_COUNT] = { "bool", "int", "double", "class", "true", "false", "main", "for", "return" };

int IndexKeyword[KEYWORD_COUNT] = { TBool, TInt, TDouble, TClass, TTrue, TFalse, TMain, TFor, TReturn };

size_t Scaner::getUK() const
{
    return uk;
}

void Scaner::setUK(size_t newUK)
{
    if (newUK <= text.length()) {
        uk = newUK;
    }
}

int Scaner::nextLexeme(TypeLex& l)
{
start:
    l.clear(); // ������� ������ ����� ������� ����� �������

    while (uk < text.length() && isspace(text[uk])) {
        if (text[uk] == '\n') { //���� ��� ����� ������, ����������� ������� �����
            ++line;
            posInLine = 0;
        }
        ++uk;
        ++posInLine;
    }
    //���� ��������� ����� �����
    if (uk >= text.length()) {
        return TEnd;
    }

    char current = text[uk]; //������� ������

    //��������� ������������ ������������ (//)
    if (current == '/' && text[uk + 1] == '/') {
        while (uk < text.length() && text[uk] != '\n') {
            ++uk; ++posInLine;
        }
        goto start;
    }

    //��������� ������������� ������������ (/**/)
    if (current == '/' && text[uk + 1] == '*') {
        uk += 2; //���������� ������� "/*"
        while (uk < text.length() && !(text[uk] == '*' && text[uk + 1] == '/')) {
            if (text[uk] == '\n') {
                ++line;
                posInLine = 0;
            }
            ++uk; ++posInLine;
        }
        uk += 2; posInLine += 2; //���������� ������� "*/"
        goto start;
    }

    //������ ��������� ��������� ����� ��� ��������������
    if (isalpha(current)) {
        string lexeme;
        while (isalnum(text[uk])) {
            lexeme += text[uk++]; ++posInLine;
        }

        l = lexeme;

        for (int i = 0; i <= KEYWORD_COUNT; i++) {
            if (lexeme == Keyword[i]) return IndexKeyword[i];
        }

        return TIdent; //���� �� �������� ����� � ��� �������������
    }

    //������ ��������� ���� (����� �����)
    if (isdigit(current)) {
        while (isdigit(text[uk])) {
            l += text[uk++];
            ++posInLine;
        }
        return TConstint;
    }

    //��������� ����������� �������� � ��������
    switch (current) {
    case ';': l = ";"; ++uk; ++posInLine; return TSemicolon;
    case ',': l = ","; ++uk; ++posInLine; return TComma;
    case '{': l = "{"; ++uk; ++posInLine; return TBraceOpen;
    case '}': l = "}"; ++uk; ++posInLine; return TBraceClose;
    case '(': l = "("; ++uk; ++posInLine; return TParenOpen;
    case ')': l = ")"; ++uk; ++posInLine; return TParenClose;
    case '.': l = "."; ++uk; ++posInLine; return TDot;
    case '*': l = "*"; ++uk; ++posInLine; return TMultiply;
    case '/': l = "/"; ++uk; ++posInLine; return TDivide;
    case '%': l = "%"; ++uk; ++posInLine; return TMod;
    case '=':
        l = "=";
        ++uk; ++posInLine;
        if (text[uk] == '=') {
            l += "=";
            ++uk; ++posInLine;
            return TEqual;
        }
        return TAssign;
    case '!':
        l = "!";
        ++uk; ++posInLine;
        if (text[uk] == '=') {
            l += "=";
            ++uk; ++posInLine;
            return TNotEqual;
        }
        return TNot;
    case '>':
        l = ">";
        ++uk; ++posInLine;
        if (text[uk] == '=') {
            l += "=";
            ++uk; ++posInLine;
            return TGreaterEqual;
        }
        else if (text[uk] == '>') {
            l += ">";
            ++uk; ++posInLine;
            return TShiftRight;
        }
        return TGreater;
    case '<':
        l = "<";
        ++uk; ++posInLine;
        if (text[uk] == '=') {
            l += "=";
            ++uk; ++posInLine;
            return TLessEqual;
        }
        else if (text[uk] == '<') {
            l += "<";
            ++uk; ++posInLine;
            return TShiftLeft;
        }
        return TLess;
    case '+':
        l = "+";
        ++uk; ++posInLine;
        if (text[uk] == '+') {
            l += "+";
            ++uk; ++posInLine;
            return TIncrement;
        }
        return TPlus;
    case '-':
        l = "-";
        ++uk; ++posInLine;
        if (text[uk] == '-') {
            l += "-";
            ++uk; ++posInLine;
            return TDecrement;
        }
        return TMinus;
    }


    l = text[uk];
    error("����������� ������ '" + l + "' ");
    ++uk; ++posInLine;
    return TErr;
}

void Scaner::error(const string& msg) const
{
    cerr << "������: " << msg << " � ������ " << line << ", ������� " << posInLine - 1 << endl;
    throw runtime_error("������ ��������������� �������.");
}

bool Scaner::isEnd() const
{
    return uk >= text.length();
}

int Scaner::getLine()
{
    return line;
}

void Scaner::setLine(int line)
{
    this->line = line;
}

int Scaner::getPosInLine()
{
    return posInLine;
}

void Scaner::setPosInLine(int posInline)
{
    this->posInLine = posInline;
}
