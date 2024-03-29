#pragma once
#include"rb_tree_node.h"


namespace fantasil
{

	template<rb_tree_node_type Node>
	class rb_tree_iterator {
	public:
		using node_type = Node;
		using node_ptr = Node*;
		using key_type = typename Node::key_type;
		using value_type = typename node_traits<Node>::value_type;
		using reference = value_type&;
		using pointer = value_type*;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::bidirectional_iterator_tag;

		using self = rb_tree_iterator<node_type>;
		using self_ref = self&;

	public:
		rb_tree_iterator(node_ptr ptr=nullptr) :_data{ ptr } {}
		~rb_tree_iterator() { _data = nullptr; }
	public:
		inline reference operator*() { return _data->_val; }
		inline node_ptr operator->() { return _data; }

		inline self_ref operator++()
		{
			if(_data)
				_data = binary_tree_node_helper::successor(_data);
			return *this;
		}
		inline self_ref operator--()
		{
			if (_data)
				_data = binary_tree_node_helper::precursor(_data);
			return *this;
		}

		inline self operator++(int)
		{
			self tmp = *this;
			if (_data)
				_data = binary_tree_node_helper::successor(_data);
			return tmp;
		}

		inline self operator--(int)
		{
			self tmp = *this;
			if (_data)
				_data = binary_tree_node_helper::precursor(_data);
			return tmp;
		}

		inline bool operator==(const self_ref rhs) const
		{
			return _data == rhs._data;
		}

		inline bool operator!=(const self_ref rhs) const
		{
			return _data != rhs._data;
		}

		inline explicit operator bool()
		{
			return _data != nullptr;
		}

		inline node_ptr data() const { return _data; }
		inline void data(node_ptr data) { _data = data; }
		 
	private:
		node_ptr _data;
	};


}