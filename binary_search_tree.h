#pragma once
#ifndef fantasil_binary_search_tree_h
#define fantasil_binary_search_tree_h
#include"binary_search_tree_node.h"

namespace fantasil{
	template<typename K,typename V,typename Alloc=std::allocator<binary_search_tree_node<K,V>>,typename Cmp=std::less<K>>
	class binary_search_tree {
	public:
		using key_type = K;
		using value_type = V;
		using node_type = binary_search_tree_node<K, V>;
		using node_ptr = node_type*;
		using allocator_type = Alloc;
		using compare_type = Cmp;
	public:
		binary_search_tree() :_root{nullptr}, _alloc{}, _cmp{} {}
		~binary_search_tree();
	public:
		node_ptr find(const key_type& key);
		void insert(const key_type& key, const value_type& val);
		void erase(const key_type& key);

		template<typename ostream>
		friend ostream& operator<<(ostream& os, const binary_search_tree<K, V>& tree)
		{
			binary_tree_node_helper::in_order(tree._root, os);
			return os;
		}
	private:
		node_ptr _root;
		allocator_type _alloc;
		compare_type _cmp;
	};

	


	template<typename K, typename V, typename Alloc, typename Cmp>
	inline binary_search_tree<K, V, Alloc, Cmp>::~binary_search_tree()
	{
		binary_search_tree_node_helper::destory_bianry_search_tree(_root, _alloc);
	}
	template<typename K, typename V, typename Alloc, typename Cmp>
	binary_search_tree_node<K,V>* binary_search_tree<K, V, Alloc, Cmp>::find(const key_type& key)
	{
		return binary_search_tree_node_helper::find(_root, key, _cmp);
	}

	template<typename K, typename V, typename Alloc, typename Cmp>
	void binary_search_tree<K, V, Alloc, Cmp>::insert(const key_type& key, const value_type& val)
	{
		node_ptr tmp = _alloc.allocate(1);
		init_binary_search_tree_node_type<node_type>(tmp, key, val);
		_root = binary_search_tree_node_helper::insert(_root, tmp, _cmp);
	}

	template<typename K, typename V, typename Alloc, typename Cmp>
	void binary_search_tree<K, V, Alloc, Cmp>::erase(const key_type& key)
	{
		node_ptr del = find(key);
		if (del)
			_root = binary_search_tree_node_helper::erase(_root, del, _alloc);
	}

	
	

}


#endif