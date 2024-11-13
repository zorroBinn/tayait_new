#pragma once
#include "Scaner.h"

enum TypeObject {
	ObjEmpty = 0,
	ObjVar,
	ObjClass,
	ObjClassObject,
	ObjClassMethod,
	ObjMain,
};

enum DataType {
	UndefinedType = 0,
	IntType,
	DoubleType,
	BoolType
};

struct Node {
	TypeLex id; //������������� �������
	TypeObject objType; //��� �������
	DataType dataType; //��� ������
};

class Tree {
private:
	Node* n;
	Tree* parent, * left, * right;
	Scaner* sc;
	static Tree* current; //������� ������� ������

public:
	Tree(Scaner* sc);
	Tree(Node* node, Tree* parent, Tree* left, Tree* right, Scaner* sc); //����������� ������� ���� � ��������� ������� � �������
	~Tree();

	void setLeft(Tree* from, Node* node); //������� ������ ������� �� ������� �������
	void setRight(Tree* from, Node* node); //������� ������� ������� �� ������� �������
	void setRight(Tree* from, Tree* r); //������� ������ �� ������� ������� �� ������� �������
	Tree* getLeft(Tree* from);
	Tree* getRight(Tree* from);

	Tree* findUp(Tree* From, TypeLex id); //����� ������ � ������ �� �������� ������� From �� ��� ����� ����� �� ������
	Tree* findUpOneLevel(Tree* From, TypeLex id); //����� �������� id ����� �� ������ �� ������� ������� From. ����� �������������� �� ����� ������ ����������� �� ����� ������
	Tree* findRightLeft(Tree* From, TypeLex id); //����� ������ �������� �������� ������� From

	void printTree(Tree* from);

	void setCurrent(Tree* cur); //���������� ������� ���� ������
	Tree* getCurrent(); //�������� �������� �������� ���� ������

	Tree* semInclude(TypeLex id, TypeObject obj, DataType data); //��������� �������������� � �������
	Tree* semIncludeClassObject(TypeLex id, TypeObject obj, DataType data, Tree* r);
	Tree* semGetVar(TypeLex a); //����� � ������� ���������� � ������ a � ������� ������ �� ��������������� ������� ������
	Tree* semGetClass(TypeLex a); //����� � ������� ����� � ������ a � ������� ������ �� ��������������� ������� ������
	Tree* semGetMethod(TypeLex a, Tree* from); //����� � ������� ����� ������ � ������ a � ������� ������ �� ��������������� ������� ������
	Tree* semGetClassObject(TypeLex a, Tree* from); //����� � ������� ������ ������ � ������ a � ������� ������ �� ��������������� ������� ������
	Tree* semGetVarOrCO(TypeLex a, Tree* from); //����� � ������� ���������� ��� ������ ������ � ������ a � ������� ������ �� ��������������� ������� ������
	void dupControl(Tree* Addr, TypeLex a); //�������� �������������� � �� ��������� �������� ������ �����
	Tree* createNewScope(); //������� ����� ������� ���������
	DataType getDatatypeFromTypeLex(TypeLex a); //�������� ��� ������ �������
};

