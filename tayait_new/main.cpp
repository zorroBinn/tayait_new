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
		if (type == TEnd) cout << "Синтаксических ошибок не обнаружено." << endl;
		else scaner->error("Лишний текст в конце программы");

		cout << "Конец файла" << endl;

		//syntax->getRoot()->printTree();
		delete scaner;
		//syntax->~Syntax();
	}
	catch (const runtime_error& e)
	{
		cerr << "Программа завершена из-за ошибки: " << e.what() << endl;
		delete scaner;
		//syntax->~Syntax();
	}
}