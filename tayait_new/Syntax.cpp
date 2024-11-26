#include "Syntax.h"

Syntax::~Syntax()
{
	delete root;
}

void Syntax::S()
{
	TypeLex l;
	int t = lookForward(1);
	while (t == TClass || t == TBool || t == TInt || t == TDouble) {
		W();
		t = lookForward(1);
	}
}

Tree* Syntax::getRoot()
{
	return this->root;
}

int Syntax::lookForward(int k)
{
	if (k > 0) {
		TypeLex l;
		int savedPointer = sc->getUK();
		int savedLine = sc->getLine();
		int savedPosInLine = sc->getPosInLine();

		int t;
		for (int i = 0; i < k; i++)
		{
			t = sc->nextLexeme(l);
		}
		sc->setUK(savedPointer);
		sc->setLine(savedLine);
		sc->setPosInLine(savedPosInLine);

		return t;
	}
}

void Syntax::W()
{
	TypeLex l;
	int t = lookForward(1);
	if ((t == TBool || t == TInt || t == TDouble || t == TIdent) && lookForward(2) != TMain) {
		Z();
		t = sc->nextLexeme(l);
		if (t != TSemicolon) sc->error("�������� ';'");
	}
	else if (t == TClass) {
		t = sc->nextLexeme(l);
		t = sc->nextLexeme(l);
		if (t != TIdent) sc->error("�������� ������������� ������");
		root->semInclude(l, ObjClass, UndefinedType);
		t = sc->nextLexeme(l);
		if (t != TBraceOpen) sc->error("�������� '{'");
		Tree* chPntClass = root->createNewScope();
		t = lookForward(1);
		while (t == TBool || t == TInt || t == TDouble || t == TIdent) {
			if (lookForward(2) == TIdent && lookForward(3) == TParenOpen) {
				t = sc->nextLexeme(l);
				DataType type = root->getDatatypeFromTypeLex(l);
				t = sc->nextLexeme(l);
				root->semInclude(l, ObjClassMethod, type);
				t = sc->nextLexeme(l);
				t = sc->nextLexeme(l);
				if (t != TParenClose) sc->error("�������� ')'");
				t = sc->nextLexeme(l);
				if (t != TBraceOpen) sc->error("�������� '{'");
				Tree* chPnt = root->createNewScope();
				K();
				t = sc->nextLexeme(l);
				if (t != TReturn) sc->error("�������� 'return'");
				V();
				t = sc->nextLexeme(l);
				if (t != TSemicolon) sc->error("�������� ';'");
				t = sc->nextLexeme(l);
				root->setCurrent(chPnt);
				std::cout << std::endl; root->printTree(root); //������ ��
				root->removeSubtree(chPnt);
				//root->printTree(root); //������ �����
				if (t != TBraceClose) sc->error("�������� '}'");
			}
			else {
				Z();
				t = sc->nextLexeme(l);
				if (t != TSemicolon) sc->error("�������� ';'");
			}
			t = lookForward(1);
		}
		root->setCurrent(chPntClass);
		t = sc->nextLexeme(l);
		if (t != TBraceClose) sc->error("�������� '}'");
	}
	else if (t == TInt && lookForward(2) == TMain) {
		t = sc->nextLexeme(l);
		t = sc->nextLexeme(l);
		root->semInclude(l, ObjMain, IntType);
		t = sc->nextLexeme(l);
		if (t != TParenOpen) sc->error("�������� '('");
		t = sc->nextLexeme(l);
		if (t != TParenClose) sc->error("�������� ')'");
		t = sc->nextLexeme(l);
		if (t != TBraceOpen) sc->error("�������� '{'");
		Tree* chPnt = root->createNewScope();
		K();
		root->setCurrent(chPnt);
		std::cout << std::endl; root->printTree(root); //������ ��
		root->removeSubtree(chPnt);
		//root->printTree(root); //������ �����
		t = sc->nextLexeme(l);
		if (t != TBraceClose) sc->error("�������� '}'");
	}
}

void Syntax::Z()
{
	TypeLex l;
	int t;
	t = sc->nextLexeme(l);
	if (t != TBool && t != TInt && t != TDouble && (t != TIdent || lookForward(1) != TIdent)) sc->error("�������� ���");
	Tree* classPoint = nullptr;
	if (t == TIdent) {
		classPoint = root->semGetClass(l);
		classPoint = classPoint->getLeft(classPoint);
		classPoint = classPoint->getRight(classPoint);
	}
	DataType type = root->getDatatypeFromTypeLex(l);
	do
	{
		t = sc->nextLexeme(l);
		if (t != TIdent) sc->error("�������� �������������");
		if (type != UndefinedType)
			root->semInclude(l, ObjVar, type);
		else { //������ ������ �� ��������� ������
			if (classPoint != nullptr) root->semIncludeClassObject(l, ObjClassObject, type, classPoint);
		}
		t = lookForward(1);
		if (t == TAssign) {
			t = sc->nextLexeme(l);
			V();
		}
		t = lookForward(1);
		if (t == TComma) t = sc->nextLexeme(l);
	} while (t == TComma);
}

void Syntax::K()
{
	TypeLex l;
	int t = lookForward(1);
	while (t != TReturn && t != TBraceClose) {
		if ((t == TBool || t == TInt || t == TDouble || t == TIdent) && lookForward(2) == TIdent) {
			Z();
			t = sc->nextLexeme(l);
			if (t != TSemicolon) sc->error("�������� ';'");
		}
		else O();
		t = lookForward(1);
	}
}

void Syntax::O()
{
	TypeLex l;
	int t = lookForward(1);
	if (t == TFor) {
		U();
	}
	else if (t == TBraceOpen) {
		t = sc->nextLexeme(l);
		Tree* chPnt = root->createNewScope();
		K();
		root->setCurrent(chPnt);
		std::cout << std::endl; root->printTree(root); //������ ��
		root->removeSubtree(chPnt); //�������� ��������� ���������� ���������
		//root->printTree(root); //������ ����� 
		t = sc->nextLexeme(l);
		if (t != TBraceClose) sc->error("�������� '}'");
	}
	else if (t == TIdent || t == TSemicolon) {
		if (t == TIdent) {
			t = sc->nextLexeme(l);
			Tree* classObject = nullptr;
			if (lookForward(1) == TDot)
				classObject = root->semGetClassObject(l, root->getCurrent()); //�������� ����� findrightleft
			else classObject = root->semGetVarOrCO(l, root->getCurrent());
			while (lookForward(1) == TDot) {
				t = sc->nextLexeme(l);
				t = sc->nextLexeme(l);
				if (t != TIdent) sc->error("�������� �������������");
				if (lookForward(1) == TDot)
					classObject = root->semGetClassObject(l, classObject);
				else classObject = root->semGetVarOrCO(l, classObject);
			}
			t = sc->nextLexeme(l);
			if (t != TAssign) sc->error("�������� '='");
			V();
		}
		t = sc->nextLexeme(l);
		if (t != TSemicolon) sc->error("�������� ';'");
	}
}

void Syntax::U()
{
	TypeLex l;
	int t = sc->nextLexeme(l);
	if (t != TFor) sc->error("�������� 'for'");
	t = sc->nextLexeme(l);
	if (t != TParenOpen) sc->error("�������� '('");

	t = sc->nextLexeme(l);
	if (t != TIdent) sc->error("�������� �������������");
	Tree* classObject = nullptr;
	if (lookForward(1) == TDot)
		classObject = root->semGetClassObject(l, root->getCurrent());
	else classObject = root->semGetVarOrCO(l, root->getCurrent());
	while (lookForward(1) == TDot) {
		t = sc->nextLexeme(l);
		t = sc->nextLexeme(l);
		if (t != TIdent) sc->error("�������� �������������");
		if (lookForward(1) == TDot)
			classObject = root->semGetClassObject(l, classObject);
		else classObject = root->semGetVarOrCO(l, classObject);
	}
	t = sc->nextLexeme(l);
	if (t != TAssign) sc->error("�������� '='");
	V();

	t = sc->nextLexeme(l);
	if (t != TSemicolon) sc->error("�������� ';'");
	V();
	t = sc->nextLexeme(l);
	if (t != TSemicolon) sc->error("�������� ';'");

	t = sc->nextLexeme(l);
	if (t != TIdent) sc->error("�������� �������������");
	if (lookForward(1) == TDot)
		classObject = root->semGetClassObject(l, root->getCurrent());
	else classObject = root->semGetVarOrCO(l, root->getCurrent());
	while (lookForward(1) == TDot) {
		t = sc->nextLexeme(l);
		t = sc->nextLexeme(l);
		if (t != TIdent) sc->error("�������� �������������");
		if (lookForward(1) == TDot)
			classObject = root->semGetClassObject(l, classObject);
		else classObject = root->semGetVarOrCO(l, classObject);
	}
	t = sc->nextLexeme(l);
	if (t != TAssign) sc->error("�������� '='");
	V();

	t = sc->nextLexeme(l);
	if (t != TParenClose) sc->error("�������� ')'");
	O();
}

void Syntax::V()
{
	TypeLex l;
	int t = lookForward(1);
	if (t == TPlus || t == TMinus) {
		t = sc->nextLexeme(l);
	}
	Y();
	t = lookForward(1);
	while (t >= TAssign && t <= TLess) {
		t = sc->nextLexeme(l);
		Y();
		t = lookForward(1);
	}
}

void Syntax::Y()
{
	TypeLex l;
	int t;
	A();
	t = lookForward(1);
	while (t == TShiftLeft || t == TShiftRight) {
		t = sc->nextLexeme(l);
		A();
		t = lookForward(1);
	}
}

void Syntax::A()
{
	TypeLex l;
	int t;
	B();
	t = lookForward(1);
	while (t == TPlus || t == TMinus) {
		t = sc->nextLexeme(l);
		B();
		t = lookForward(1);
	}
}

void Syntax::B()
{
	TypeLex l;
	int t = lookForward(1);
	if (t == TIncrement || t == TDecrement) {
		t = sc->nextLexeme(l);
	}
	R();
	t = lookForward(1);
	if (t == TIncrement || t == TDecrement) {
		t = sc->nextLexeme(l);
	}
	t = lookForward(1);
	while (t >= TMultiply && t <= TMod) {
		t = sc->nextLexeme(l);
		R();
		t = lookForward(1);
	}
}

void Syntax::R()
{
	TypeLex l;
	int t = sc->nextLexeme(l);
	if (t == TFalse || t == TTrue || t == TConstint) {
		return;
	}
	if (t == TParenOpen) {
		V();
		t = sc->nextLexeme(l);
		if (t != TParenClose) sc->error("�������� ')'");
		return;
	}
	if (t == TIdent && lookForward(3) == TParenOpen && lookForward(4) == TParenClose) {
		Tree* classObject = root->semGetClassObject(l, root->getCurrent());
		t = sc->nextLexeme(l);
		if (t != TDot) sc->error("�������� '.'");
		t = sc->nextLexeme(l);
		if (t != TIdent) sc->error("�������� �������������");
		root->semGetMethod(l, classObject); //�������� ����� findrightleft
		t = sc->nextLexeme(l);
		t = sc->nextLexeme(l);
	}
	else {
		if (t != TIdent) sc->error("�������� �������������");
		Tree* classObject = nullptr;
		if (lookForward(1) == TDot)
			classObject = root->semGetClassObject(l, root->getCurrent()); //�������� ����� findrightleft
		else classObject = root->semGetVarOrCO(l, root->getCurrent());
		while (lookForward(1) == TDot) {
			t = sc->nextLexeme(l);
			t = sc->nextLexeme(l);
			if (t != TIdent) sc->error("�������� �������������");
			if (lookForward(1) == TDot)
				classObject = root->semGetClassObject(l, classObject);
			else classObject = root->semGetVarOrCO(l, classObject);
		}
	}
}
