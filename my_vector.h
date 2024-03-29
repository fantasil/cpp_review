#pragma once
#include"my_vector_iterator.h"
#include"my_exception.h"
#include<xmemory>


template<typename T,typename Alloc=std::allocator<T>>
class my_vector {
public:
	using iterator = my_vector_iterator<T>;
	using value_type =typename std::iterator_traits<iterator>::value_type;
	using pointer = value_type*;
	using reference = value_type&;
	using const_iterator = const iterator;
	using const_reference = const reference;
	using const_pointer = const pointer;
	using allocator_type = Alloc;
	using size_type = size_t;
	using difference_type = typename std::iterator_traits<iterator>::difference_type;
private:
	iterator _begin;
	iterator _end;
	iterator _end_of_storage;
	allocator_type alloc;
public:
	//constructor
	my_vector(size_type sz = 16);
	my_vector(std::initializer_list<value_type> ilist);
	template<typename InputIter>
	my_vector(InputIter begin, InputIter end);
	~my_vector();
public:
	//element access
	reference at(size_type pos);
	const_reference at(size_type pos) const;
	reference operator[](size_type pos) { return at(pos); }
	const_reference operator[](size_type pos) const { return at(pos); }
	reference front();
	const_reference front() const;
	reference back();
	const_reference back() const;
	pointer data() { return _begin->operator->(); }
	const_pointer data() const { return _begin->operator->(); }
public:
	//iterator
	iterator begin() { return _begin; }
	const_iterator begin() const { return _begin; }
	iterator end() { return _end; }
	const_iterator end() const { return _end; }
public:
	//
	inline bool empty() const { return _begin == _end; }
	inline bool full() const { return _end == _end_of_storage; }
	//inline size_type capacity() const { return _end_of_storage - _begin; }
	inline size_type size() const { return _end - _begin; }
	//void reserve(size_type new_cap);
	inline size_type capacity() const { return _end_of_storage - _begin; }
	//void shrink_to_fit();
public:
	//void clear();
	void insert(const_iterator pos, const T& value);
	void insert(const_iterator pos, T&& value);
	void insert(const_iterator pos, size_type count, const_reference value);
	void insert(const_iterator pos, std::initializer_list<T> ilist);
	template<typename InputIter>
	void insert(const_iterator pos, InputIter beign, InputIter end);

	void push_back(const_reference value);

	iterator erase(iterator pos);
	//iterator erase(const_iterator pos);
	iterator erase(iterator begin, iterator end);
	//iterator erase(const_iterator begin, const_iterator end);

private:
	void re_alloc(size_type capacity);
	inline size_type new_cap(size_type base, size_type factor = 2) { return base* factor; }

};

template<typename T, typename Alloc>
inline my_vector<T, Alloc>::my_vector(size_type sz)
{
	auto tmp = alloc.allocate(sz);
	_begin = iterator{ tmp };
	_end = _begin;
	_end_of_storage = _begin + sz;
}

template<typename T, typename Alloc>
inline my_vector<T, Alloc>::my_vector(std::initializer_list<value_type> ilist)
{
	size_t size = ilist.size();
	size_t factor = 2;
	size_t capacity = size * factor;
	pointer tmp = alloc.allocate(capacity);
	_begin = iterator{ tmp };
	std::copy(ilist.begin(), ilist.end(), _begin);
	_end = _begin + size;
	_end_of_storage = _begin + capacity;
}

template<typename T, typename Alloc>
inline my_vector<T, Alloc>::~my_vector()
{
	size_t capacity = _end_of_storage - _begin;
	pointer ptr = _begin.operator->();
	alloc.deallocate(ptr, capacity);
}

template<typename T, typename Alloc>
inline typename my_vector<T,Alloc>::reference my_vector<T, Alloc>::at(size_type pos)
{
	size_type top = _end - _begin;
	if (pos > top)
		throw std::out_of_range{};
	return _begin + pos;
}

template<typename T, typename Alloc>
inline typename my_vector<T, Alloc>::const_reference my_vector<T, Alloc>::at(size_type pos) const
{
	size_type top = _end - _begin;
	if (pos > top)
		throw std::out_of_range{};
	const_reference tmp = _begin + pos;
	return tmp;
}



template<typename T, typename Alloc>
inline typename my_vector<T, Alloc>::reference my_vector<T, Alloc>::front()
{
	if (empty())
		throw empty_container{};
	return *_begin;
}

template<typename T, typename Alloc>
inline typename my_vector<T, Alloc>::const_reference my_vector<T, Alloc>::front() const
{
	if (empty())
		throw empty_container{};
	return *_begin;
}

template<typename T, typename Alloc>
inline typename my_vector<T, Alloc>::reference my_vector<T, Alloc>::back()
{
	if (empty())
		throw empty_container{};
	return *(_end-1);
}

template<typename T, typename Alloc>
inline typename my_vector<T, Alloc>::const_reference my_vector<T, Alloc>::back() const
{
	if (empty())
		throw empty_container{};
	return *(_end - 1);
}

template<typename T, typename Alloc>
inline void my_vector<T, Alloc>::insert(const_iterator pos, const T& value)
{
	if (full())
		re_alloc(_end-_begin,new_cap);

	std::copy_backward(pos, _end, pos + 1);
	*pos = value;
	++_end;
}

template<typename T, typename Alloc>
inline void my_vector<T, Alloc>::insert(const_iterator pos, T&& value)
{
	if (full())
		re_alloc(new_cap(_end - _begin));
	std::copy_backward(pos, _end, pos + 1);
	*pos = value;
	++_end;
}

template<typename T, typename Alloc>
inline void my_vector<T, Alloc>::insert(const_iterator pos, size_type count, const_reference value)
{
	size_type left = _end - _begin;
	if (count > left)
		realloc(new_cap(_end - _begin + count));
	std::copy_backward(pos, _end, pos + count);
	std::fill(pos, pos + count, value);
	_end += count;
}

template<typename T, typename Alloc>
inline void my_vector<T, Alloc>::insert(const_iterator pos, std::initializer_list<T> ilist)
{
	insert(pos, ilist.begin(), ilist.end());
}

template<typename T, typename Alloc>
inline void my_vector<T, Alloc>::push_back(const_reference value)
{
	if (full())
		re_alloc(new_cap(_end - _begin));
	*_end = value;
	++_end;
}

template<typename T, typename Alloc>
inline typename my_vector<T, Alloc>::iterator my_vector<T, Alloc>::erase(iterator pos)
{
	std::copy(pos + 1, _end, pos);
	--_end;
	return pos;
}



/*template<typename T, typename Alloc>
typename my_vector<T, Alloc>::iterator my_vector<T, Alloc>::erase(const_iterator pos)
{
	std::copy(pos + 1, _end, pos);
	--_end;
	return pos;
}*/

template<typename T, typename Alloc>
inline typename my_vector<T, Alloc>::iterator my_vector<T, Alloc>::erase(iterator begin, iterator end)
{
	size_type size = end - begin;
	iterator pos = begin;
	std::copy(end, _end, pos);
	_end -= size;
	return pos;
}



//template<typename T, typename Alloc>
//typename my_vector<T, Alloc>::iterator my_vector<T, Alloc>::erase(const_iterator begin, const_iterator end)
//{
//	size_type size = end - begin;
//	iterator pos = begin;
//	std::copy(end, _end, pos);
//	_end -= size;
//	return pos;
//}

template<typename T, typename Alloc>
inline void my_vector<T, Alloc>::re_alloc(size_type capacity)
{
	size_type size = _end - _begin;
	size_type old_capacity = _end_of_storage - _begin;
	//分配空间
	auto ptr=alloc.allocate(capacity);
	iterator tmp{ ptr };
	//拷贝数据
	std::copy(_begin, _end, tmp);
	//删除旧空间
	alloc.deallocate(_begin.operator->(), old_capacity);
	//保持不变量
	_begin = tmp;
	_end = _begin + size;
	_end_of_storage = _begin + capacity;
}

template<typename T, typename Alloc>
template<typename InputIter>
my_vector<T, Alloc>::my_vector(InputIter begin, InputIter end)
{

	//分配空间
	using category = typename std::iterator_traits<InputIter>::iterator_category;
	size_type factor = 2;
	size_type size{};
	if (std::is_same_v<category, std::random_access_iterator_tag>)
	{
		size = end - begin;
	}
	else {
		auto tmp = begin;
		while (tmp != end)
		{
			++tmp;
			++size;
		}
	}
	size_type capacity = size * factor;
	auto ptr = alloc.allocate(capacity);
	_begin = iterator{ ptr };
	_end = _begin;
	_end_of_storage = _begin + capacity;

	//拷贝
	std::copy(begin, end, _begin);

	//保持不变量
	_end = _begin + size;	
}

template<typename T, typename Alloc>
template<typename InputIter>
inline void my_vector<T, Alloc>::insert(const_iterator pos, InputIter begin, InputIter end)
{
	if (end == begin)
		return;
	using category = typename std::iterator_traits<InputIter>::iterator_category;
	//探查是否需要分配空间
	size_type space_left = _end_of_storage - _end;
	size_type space_need{};
	if (std::is_same_v<category, std::random_access_iterator_tag>)
	{
		space_need = end - begin;
	}
	else {
		for (auto p = begin; p != end; ++p)
		{
			++space_need;
		}
	}

	if (space_need > space_left)
	{
		size_t factor = 2;
		size_t capacity = (size() + space_need) * factor;
		re_alloc(capacity);
	}

	//拷贝
	std::copy(begin, end, _end);

	//保持不变量
	_end += space_need;
}
