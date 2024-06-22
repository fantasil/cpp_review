#pragma once
#include<exception>


//container exception

//����������Ϊ��
class empty_container :std::exception {
public:
	const char* what() const override {
		return "empty container";
	}
};

//��������������
class full_container :std::exception {
public:
	const char* what() const override {
		return "full container";
	}
};

//Ԫ�ز���������
class not_in_container :std::exception {
public:
	const char* what() const override {
		return "elem not in container";
	}
};

//���ɲ�����λ��
class non_operable_pos :std::exception {
public:
	const char* what() const override {
		return "non_operable position";
	}
};