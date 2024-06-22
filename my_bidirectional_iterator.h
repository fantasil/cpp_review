#pragma once
#include"headers.h"
#include"my_bidirection_node.h"


template<bi_node_type Node>
class my_bidirectional_iterator {
public:
	using node_type = Node;
	using node_ptr = node_type*;
	using value_type = typename node_traits<Node>::value_type;
	using reference = value_type&;
	using pointer = value_type*;
	using iter_type = my_bidirectional_iterator<node_type>;
	using iter_ref = iter_type&;
	using difference_type = std::ptrdiff_t;
	using iterator_category = std::bidirectional_iterator_tag;
public:
	my_bidirectional_iterator(node_ptr ptr=nullptr) :_data{ ptr } {}
	~my_bidirectional_iterator() { _data = nullptr; }

public:
	inline reference operator*() const  { return _data->_val; }
	inline node_ptr operator->()  const { return _data; }
	inline iter_ref operator++()
	{
		if (_data)
			_data = _data->_next;
		return *this;
	}
	
	inline iter_ref operator--()
	{
		if (_data)
			_data = _data->_prev;
		return *this;
	}

	inline iter_type operator++(int)
	{
		node_ptr tmp = _data;
		if (_data)
			_data = _data->_next;
		return iter_type{ tmp };
	}

	inline iter_type operator--(int)
	{
		node_ptr tmp = _data;
		if (_data)
			_data = _data->_prev;
		return iter_type{ tmp };
	}

	iter_type operator+(size_t n)
	{
		node_ptr tmp = _data;
		while (n && tmp)
		{
			tmp = tmp->_next;
			--n;
		}
		if (n)
			throw std::logic_error{ "out_of_range" };
		return iter_type{ tmp };

	}

	iter_type operator-(size_t n)
	{
		node_ptr tmp = _data;
		while (tmp && n)
		{
			tmp = tmp->_prev;
			--n;
		}
		if (n)
			throw std::logic_error{ "out_of_range" };
		return iter_type{ tmp };
	}

	iter_ref operator+=(size_t n)
	{
		while (_data && n)
		{
			_data = _data->_next;
			--n;
		}
		if (n)
			throw std::logic_error{ "out_of_range" };
		return *this;
	}

	iter_ref operator-=(size_t n)
	{
		while (_data && n)
		{
			_data = _data->_prev;
			--n;
		}
		if (n)
			throw std::logic_error{ "out_of_range" };
		return *this;
	}

	inline bool operator==(const  iter_type& rhs) const { return _data == rhs._data; }
	inline bool operator!=(const  iter_type& rhs) const { return _data != rhs._data; }

	explicit operator bool() { return nullptr != _data; }
	inline node_ptr data() { return _data; }
	inline void data(node_ptr ptr) { _data = ptr; }


private:
	node_ptr _data;
};








