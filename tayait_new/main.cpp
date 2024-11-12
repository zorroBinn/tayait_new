#include "Scaner.h"
#include <iostream>
#include <Windows.h>
using namespace std;

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	Scaner* scaner = new Scaner("source.txt");
	//Syntax* syntax = new Syntax(scaner);
	try
	{
		//syntax->S();

		int type;
		TypeLex l;

		type = scaner->nextLexeme(l);
		if (type == TEnd) cout << "�������������� ������ �� ����������." << endl;
		else scaner->error("������ ����� � ����� ���������");

		cout << "����� �����" << endl;

		//syntax->getRoot()->printTree();
		delete scaner;
		//syntax->~Syntax();
	}
	catch (const runtime_error& e)
	{
		cerr << "��������� ��������� ��-�� ������: " << e.what() << endl;
		delete scaner;
		//syntax->~Syntax();
	}
}