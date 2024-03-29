#pragma once
#include"headers.h"
#include"bi_node.h"

namespace fantasil
{
	//一个双向链表的迭代器

	template<typename T>
	class bi_node_iterator {
	public:
		using value_type = T;
		using reference = value_type&;
		using pointer = value_type*;
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using node_type = bi_node<value_type>;
		using node_ptr = node_type*;
		using self = bi_node_iterator<T>;
		using self_ref = self&;
		using iterator = self;
	private:
		node_ptr node;
	public:
		bi_node_iterator(node_ptr ptr = nullptr) :node{ ptr } {}
		~bi_node_iterator() {}
	public:
		inline self_ref operator++();		//递增
		inline self_ref operator--();		//递减
		inline self operator++(int);
		inline self operator--(int);
		reference operator*() { return node->_val;}	//解引用
		node_ptr operator->() { return node; }		//
		inline bool operator==(const self_ref rhs) { return node == rhs.node; }
		inline bool operator!=(const self_ref rhs) { return node != rhs.node; }
		inline operator bool() { return node; }
		inline operator node_ptr() { return node; }
	};

	template<typename T>
	inline typename bi_node_iterator<T>::self_ref bi_node_iterator<T>::operator++()
	{
		if (node)
			node = node->_next;
		return *this;
	}

	template<typename T>
	inline typename bi_node_iterator<T>::self_ref bi_node_iterator<T>::operator--()
	{
		if (node)
			node = node->_prev;
		return *this;
	}

	template<typename T>
	inline typename bi_node_iterator<T>::self bi_node_iterator<T>::operator++(int)
	{
		iterator tmp{ nullptr };
		if (node)
		{
			tmp = *this;
			node = node->_next;
		}
		return tmp;
	}

	template<typename T>
	inline typename bi_node_iterator<T>::self bi_node_iterator<T>::operator--(int)
	{
		iterator tmp{ nullptr };
		if (node)
		{
			tmp = *this;
			node = node->_prev;
		}
		return tmp;
	}
}