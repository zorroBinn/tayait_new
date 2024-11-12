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

	Tree* findUp(Tree* From, TypeLex id); //����� ������ � ������ �� �������� ������� From �� ��� ����� ����� �� ������
	Tree* findUp(TypeLex id); //����� ������ � ������, ������� �� ������� ������� this �� ��� ����� ����� �� ������
	Tree* findUpOneLevel(Tree* From, TypeLex id); //����� �������� id ����� �� ������ �� ������� ������� From. ����� �������������� �� ����� ������ ����������� �� ����� ������

	void printTree(Tree* from);

	void setCurrent(Tree* cur); //���������� ������� ���� ������
	Tree* getCurrent(); //�������� �������� �������� ���� ������

	Tree* semInclude(TypeLex id, TypeObject obj, DataType data); //��������� �������������� � �������
	Tree* semGetVar(TypeLex a); //����� � ������� ���������� � ������ a � ������� ������ �� ��������������� ������� ������
	Tree* semGetClass(TypeLex a); //����� � ������� ����� � ������ a � ������� ������ �� ��������������� ������� ������
	Tree* semGetMethod(TypeLex a); //����� � ������� ����� ������ � ������ a � ������� ������ �� ��������������� ������� ������
	Tree* semGetClassObject(TypeLex a); //����� � ������� ������ ������ � ������ a � ������� ������ �� ��������������� ������� ������
	Tree* semGetVarOrCO(TypeLex a); //����� � ������� ���������� ��� ������ ������ � ������ a � ������� ������ �� ��������������� ������� ������
	void dupControl(Tree* Addr, TypeLex a); //�������� �������������� � �� ��������� �������� ������ �����
	Tree* createNewScope(); //������� ����� ������� ���������
	DataType getDatatypeFromTypeLex(TypeLex a); //�������� ��� ������ �������
};

