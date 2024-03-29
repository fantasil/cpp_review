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