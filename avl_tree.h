#pragma once

#ifndef fantasil_avl_tree_h
#define fantasil_avl_tree_h

#include"avl_tree_node.h"

namespace fantasil {

	template<typename K,typename V,typename Alloc=std::allocator<avl_tree_node<K,V>>,typename Cmp=std::less<K>>
	class avl_tree {
	public:
		using node_type = avl_tree_node<K, V>;
		using key_type = typename node_type::key_type;
		using value_type = typename node_traits<node_type>::value_type;
		using allocator_type = Alloc;
		using compare_type = Cmp;
		using node_ptr = node_type*;
	public:
		avl_tree() :_root{ nullptr }, _alloc{}, _cmp{} {}
		template<input_iterator_of_pair<key_type,value_type> Iter>
		avl_tree(Iter begin, Iter end) : _alloc{}, _cmp{}
		{
			_root=avl_tree_node_helper::make_avl_tree(begin, end, _alloc, _cmp);
		}
		~avl_tree() { avl_tree_node_helper::destory_avl_tree(_root, _alloc); }
	public:
		node_ptr find(const key_type& key);
		void insert(const key_type& key, const value_type& val);
		void erase(const key_type& key);

		template<typename ostream>
		friend ostream& operator<<(ostream& os, const avl_tree<K, V, Alloc, Cmp>& tree)
		{
			if (tree._root)
				binary_tree_node_helper::in_order(tree._root, os);
			else
				os << "empty avl tree\n";
			return os;
		}
	private:
		node_ptr _root;
		allocator_type _alloc;
		compare_type _cmp;
	};

	template<typename K, typename V, typename Alloc, typename Cmp>
	 typename avl_tree<K,V,Alloc,Cmp>::node_ptr avl_tree<K, V, Alloc, Cmp>::find(const key_type& key)
	{
		return avl_tree_node_helper::find(_root, key, _cmp);
	}

	template<typename K, typename V, typename Alloc, typename Cmp>
	 void avl_tree<K, V, Alloc, Cmp>::insert(const key_type& key, const value_type& val)
	{
		 node_ptr tmp = _alloc.allocate(1);
		 init_binary_search_tree_node_type(tmp, key, val);
		 _root = avl_tree_node_helper::insert(_root, tmp, _cmp);
	}

	template<typename K, typename V, typename Alloc, typename Cmp>
	 void avl_tree<K, V, Alloc, Cmp>::erase(const key_type& key)
	{
		node_ptr node = avl_tree_node_helper::find(_root,key, _cmp);
		_root = avl_tree_node_helper::erase(_root, node, _alloc);
	}

}

#endif