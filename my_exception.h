#pragma once
#include<exception>


//container exception

//操作的容器为空
class empty_container :std::exception {
public:
	const char* what() const override {
		return "empty container";
	}
};

//操作的容器已满
class full_container :std::exception {
public:
	const char* what() const override {
		return "full container";
	}
};

//元素不在容器内
class not_in_container :std::exception {
public:
	const char* what() const override {
		return "elem not in container";
	}
};

//不可操作的位置
class non_operable_pos :std::exception {
public:
	const char* what() const override {
		return "non_operable position";
	}
};