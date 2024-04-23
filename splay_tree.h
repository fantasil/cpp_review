#pragma once
#ifndef fantasil_splay_tree_h
#define fantasil_splay_tree_h
#include"splay_tree_node.h"

namespace fantasil
{

	template<typename K,typename V,typename Alloc=std::allocator<splay_tree_node<K,V>>,typename Cmp=std::less<K>>
	class splay_tree {
	public:
		using node_type = splay_tree_node<K, V>;
		using node_ptr = node_type*;
		using key_type = K;
		using value_type = typename node_traits<node_type>::value_type;
		using allocator_type = Alloc;
		using compare_type = Cmp;
	public:
		splay_tree() :_root{ nullptr }, _alloc{}, _cmp{} {}
		template<input_iterator_of_pair<key_type,value_type> Iter>
		splay_tree(Iter begin, Iter end)
			: _alloc{}, _cmp{}
		{
			_root = splay_tree_node_helper::make_splay_tree<node_type>(begin, end, _alloc, _cmp);
		}
		~splay_tree()
		{
			splay_tree_node_helper::destory_splay_tree(_root, _alloc);
		}
	public:
		node_ptr find(const key_type& key);
		void insert(const key_type& key, const value_type& val);
		void erase(const key_type& key);

		template<typename ostream>
		friend ostream& operator<<(ostream& os, const splay_tree<K, V, Alloc, Cmp>& tree)
		{
			if (tree._root)
				binary_tree_node_helper::level_order(tree._root, os);
			else
				os << "empty splay tree!\n";
			return os;

		}
	private:
		node_ptr _root;
		allocator_type _alloc;
		compare_type _cmp;
	};

	template<typename K, typename V, typename Alloc, typename Cmp>
	typename splay_tree<K,V,Alloc,Cmp>::node_ptr splay_tree<K, V, Alloc, Cmp>::find(const key_type& key)
	{
		return splay_tree_node_helper::find(_root, key, _cmp);
	}

	template<typename K, typename V, typename Alloc, typename Cmp>
	void splay_tree<K, V, Alloc, Cmp>::insert(const key_type& key, const value_type& val)
	{
		node_ptr tmp = _alloc.allocate(1);
		init_binary_search_tree_node_type(tmp, key, val);
		_root = splay_tree_node_helper::insert(_root, tmp, _cmp);
	}

	template<typename K, typename V, typename Alloc, typename Cmp>
	inline void splay_tree<K, V, Alloc, Cmp>::erase(const key_type& key)
	{
		node_ptr node = binary_search_tree_node_helper::find(_root,key,_cmp);	//这里不能调用splay_tree的find,因为该find会有副作用导致5会被提到根节点
		if (node)
			_root = splay_tree_node_helper::erase(_root, node, _alloc, _cmp);
	}

}


#endif
