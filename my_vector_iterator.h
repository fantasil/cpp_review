#pragma once


#include"headers.h"



template<typename T>
class my_vector_iterator {
public:
	using value_type = T;
	using reference = value_type&;
	using pointer = value_type*;
	using difference_type = std::ptrdiff_t;
	using iterator_category = std::random_access_iterator_tag;

	using self = my_vector_iterator<T>;
	using self_ref = self&;
private:
	pointer ptr;
public:
	inline self_ref operator++();
	inline self_ref operator--();
	inline self operator++(int);
	inline self operator--(int);
	inline self operator+(size_t n);
	inline self operator-(size_t n);
	inline self_ref operator+=(size_t n);
	inline self_ref operator-=(size_t n);
	inline reference operator*() { return *ptr; }
	inline pointer operator->() { return ptr; }
	template<typename U>
	friend bool operator==(const my_vector_iterator<U>& lhs, const my_vector_iterator<U>& rhs)
	{
		return lhs.ptr == rhs.ptr;
	}

	template<typename U>
	friend bool operator != (const my_vector_iterator<U>&lhs, const my_vector_iterator<U>& rhs)
	{
		return lhs.ptr != rhs.ptr;
	}


	//inline bool operator==(const self_ref rhs) { return ptr == rhs.ptr; }
	//inline bool operator!=(const self_ref rhs) { return ptr != rhs.ptr; }

	template<typename U>
	friend difference_type operator-(const my_vector_iterator<U>& lhs, const my_vector_iterator<U>& rhs)
	{
		return lhs.ptr - rhs.ptr;
	}
	
public:
	my_vector_iterator(pointer pt=nullptr) :ptr{ pt } {}
	~my_vector_iterator() {
		if (ptr)
			ptr = nullptr;
	}

};

template<typename T>
inline typename my_vector_iterator<T>::self_ref my_vector_iterator<T>::operator++()
{
	if (ptr)
		++ptr;
	return *this;
}

template<typename T>
inline typename my_vector_iterator<T>::self_ref my_vector_iterator<T>::operator--()
{
	if (ptr)
		--ptr;
	return *this;
}

template<typename T>
inline typename my_vector_iterator<T>::self my_vector_iterator<T>::operator++(int)
{
	pointer tmp = ptr;
	if (ptr)
		++ptr;
	return tmp;
}

template<typename T>
inline typename my_vector_iterator<T>::self my_vector_iterator<T>::operator--(int)
{
	pointer tmp = ptr;
	if (ptr)
		--ptr;
	return tmp;
}

template<typename T>
inline typename my_vector_iterator<T>::self my_vector_iterator<T>::operator+(size_t n)
{
	pointer tmp = ptr;
	if (tmp)
		tmp += n;
	return tmp;
}

template<typename T>
inline typename my_vector_iterator<T>::self my_vector_iterator<T>::operator-(size_t n)
{
	pointer tmp = ptr;
	if (tmp)
		tmp -= n;
	return tmp;
}

template<typename T>
inline  typename my_vector_iterator<T>::self_ref my_vector_iterator<T>::operator+=(size_t n)
{
	if (ptr)
		ptr += n;
	return *this;
}

template<typename T>
inline  typename my_vector_iterator<T>::self_ref my_vector_iterator<T>::operator-=(size_t n)
{
	if (ptr)
		ptr -= n;
	return *this;
}
