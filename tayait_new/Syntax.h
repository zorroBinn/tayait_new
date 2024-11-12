#pragma once
#include "Semant.h"

class Syntax
{
private:
	Scaner* sc;
	Tree* root;
	int lookForward(int k);
	void W(); //Одно описание
	void Z(); //Список
	void K(); //Операторы и описания
	void O(); //Оператор
	void U(); //for
	void V(); //Выражение
	void Y(); //Сдвиги
	void A(); //Слагаемое
	void B(); //Множитель
	void R(); //Элементарное выражение
public:
	Syntax(Scaner* s) { sc = s; root = new Tree(s); root->setCurrent(root); }
	~Syntax();
	void S(); //Программа
	Tree* getRoot();
};

