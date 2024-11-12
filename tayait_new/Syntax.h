#pragma once
#include "Semant.h"

class Syntax
{
private:
	Scaner* sc;
	Tree* root;
	int lookForward(int k);
	void W(); //���� ��������
	void Z(); //������
	void K(); //��������� � ��������
	void O(); //��������
	void U(); //for
	void V(); //���������
	void Y(); //������
	void A(); //���������
	void B(); //���������
	void R(); //������������ ���������
public:
	Syntax(Scaner* s) { sc = s; root = new Tree(s); root->setCurrent(root); }
	~Syntax();
	void S(); //���������
	Tree* getRoot();
};

