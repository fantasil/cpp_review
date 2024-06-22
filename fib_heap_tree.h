#pragma once
#include"fib_heap_node.h"


#ifndef fantasil_fib_heap_tree_h
#define fantasil_fib_heap_tree_h

namespace fantasil
{

	template<typename K,typename V>
	class fib_heap_tree {
	public:
		using node_type = fib_heap_node<K, V>;
		using node_ptr = node_type*;
		using key_type = K;
		using value_type = V;
		using reference = value_type&;
	public:
		fib_heap_tree();
		~fib_heap_tree();
	public:
		void insert(const std::pair<K, V>& p);
		value_type min();
		void extract_min();
		void decrease_key(node_ptr pos, const key_type& key);
		void erase(node_ptr pos);
	private:

		static key_type min_key;
	private:
		node_ptr _min;	//最小值所在节点
		size_t _nums;	//节点数量
	};

	template<typename K, typename V>
	inline fib_heap_tree<K, V>::fib_heap_tree()
		:_min{nullptr},_nums{0}
	{
	}

	template<typename K, typename V>
	inline fib_heap_tree<K, V>::~fib_heap_tree()
	{
		while (_min)
			extract_min();
	}

	template<typename K, typename V>
	inline void fib_heap_tree<K, V>::insert(const std::pair<K, V>& p)
	{
		_min=fib_heap_node_helper::insert(_min, p);
		++_nums;
	}

	template<typename K, typename V>
	inline typename fib_heap_tree<K,V>::value_type fib_heap_tree<K, V>::min()
	{
		return _min->_val;
	}

	template<typename K, typename V>
	void fib_heap_tree<K, V>::extract_min()
	{
		_min = fib_heap_node_helper::extract_min(_min, _nums);
		--_nums;
	}

	template<typename K, typename V>
	inline void fib_heap_tree<K, V>::decrease_key(node_ptr pos, const key_type& key)
	{
		_min = fib_heap_node_helper::decrease_key(_min, pos, key);
	}

	template<typename K, typename V>
	inline void fib_heap_tree<K, V>::erase(node_ptr pos)
	{
		decrease_key(pos, std::numeric_limits<K>::min());
		extract_min();
	}














}
#endif