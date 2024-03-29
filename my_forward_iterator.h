#pragma once

#include<iterator>
#include"my_forward_node.h"

namespace fantasil {

	template<forward_node_type Node>
	class forward_iterator {
	public:
		using value_type = typename node_traits<Node>::value_type;
		using reference = value_type&;
		using pointer = value_type*;
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;

		using node_type = Node;
		using node_ptr = Node*;

		using iter_type = forward_iterator<node_type>;
		using iter_ref = iter_type&;

	public:
		forward_iterator(node_ptr ptr = nullptr) :_data{ ptr } {}
		forward_iterator(const iter_ref rhs) :_data{ rhs._data } {}
		forward_iterator(iter_type&& rhs) :_data{ std::exchange(rhs._data,nullptr)} {  }
		forward_iterator& operator=(const iter_ref rhs)
		{
			_data = rhs._data;
			return *this;
		}
		forward_iterator& operator=(iter_type&& rhs)
		{
			_data = std::exchange(rhs._data, nullptr);
			return *this;
		}
		~forward_iterator() { _data = nullptr; }
	public:
		iter_ref operator++()
		{
			if (_data)
				_data = _data->_next;
			return *this;
		}
		iter_type operator++(int)
		{
			iter_type iter = *this;
			if (_data)
				_data = _data->_next;
			return iter;
		}
		inline reference operator*() { return _data->_val; }
		inline node_ptr operator->() { return _data; }

		bool operator==(const iter_ref rhs) const { return _data == rhs._data; }
		bool operator!=(const iter_ref rhs) const { return _data != rhs._data; }

		explicit operator bool() const { return _data != nullptr; }
	public:
		inline node_ptr data() const { return _data; }
		inline void data(node_ptr ptr) { _data = ptr; }
	private:
		node_ptr _data;
	};

}
