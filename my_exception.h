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