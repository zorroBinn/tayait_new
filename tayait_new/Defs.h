#pragma once
#include <iostream>
//�������� �����
#define TBool 1
#define TInt 2
#define TDouble 3
#define TClass 4
#define TTrue 5
#define TFalse 6
#define TMain 7
#define TFor 8
#define TReturn 9
//��������������
#define TIdent 20
//���������
#define TConstint 30
//����������� �����
#define TSemicolon 40 // ;
#define TComma 41 // ,
#define TBraceOpen 42 // {
#define TBraceClose 43 // }
#define TParenOpen 44 // (
#define TParenClose 45 // )
#define TDot 46 // .
// ����� ��������
#define TAssign 50 // =
#define TEqual 51 // ==
#define TNotEqual 52 // !=
#define TGreaterEqual 53 // >=
#define TLessEqual 54 // <=
#define TGreater 55 // >
#define TLess 56 // <
#define TPlus 57 // +
#define TMinus 58 // -
#define TMultiply 59 // *
#define TDivide 60 // /
#define TMod 61 // %
#define TShiftLeft 62 // <<
#define TShiftRight 63 // >>
#define TIncrement 64 // ++
#define TDecrement 65 // --
#define TNot 66 // !
//��������� �������
#define TErr 100
#define TEnd 200
//������ ���������
#define KEYWORD_COUNT 9 //����� �������� ����
typedef std::string TypeLex;