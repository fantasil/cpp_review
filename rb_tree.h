#pragma once
#include"rb_tree_iterator.h"


namespace fantasil {



	template<typename K,typename V,typename Alloc=std::allocator<rb_tree_node<K,V>>,typename Cmp=std::less<K>>
	class rb_tree {
	public:
		using node_type = rb_tree_node<K, V>;
		using node_ptr = node_type*;
		using allocator_type = Alloc;
		using compare_type = Cmp;
		
		using iterator = rb_tree_iterator<node_type>;
		using const_iterator = const iterator;

		using key_type = typename node_type::key_type;
		using value_type = typename std::iterator_traits<iterator>::value_type;
		using reference = value_type&;
		using pointer = value_type*;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::bidirectional_iterator_tag;

	public:
		rb_tree() :_root{ nullptr } {}
		rb_tree(std::initializer_list<std::pair<key_type, value_type>> ilist)
		{
			node_ptr root=rb_tree_node_helper::make_rb_tree<node_type>(ilist, _alloc, _cmp);
			_root = iterator{ root };
		}
		template<input_iterator_of_pair<key_type,value_type> Iter>
		rb_tree(Iter begin, Iter end)
		{
			node_ptr root = rb_tree_node_helper::make_rb_tree<node_type>(begin, end, _alloc, _cmp);
			_root = iterator{ root };
		}
		~rb_tree()
		{
			if (_root)
				rb_tree_node_helper::destory_rb_tree(_root.data(), _alloc);
		}

	public:
		iterator begin() { return binary_tree_node_helper::extreame_left_node(_root.data()); }
		const_iterator begin() const { return binary_tree_node_helper::extreame_left_node(_root.data()); }
		iterator end() { return iterator{ nullptr }; }
		const_iterator end() const { return iterator{ nullptr }; }
		iterator back() { return binary_tree_node_helper::extreame_right_node(_root.data()); }
		const_iterator back() const { return binary_tree_node_helper::extreame_right_node(_root.data()); }
	public:
		void insert(const key_type& key, const value_type& val);
		void insert(std::pair<key_type, value_type>& pr);
		void insert(size_t count, const key_type& key, const value_type& val);
		void insert(size_t count, std::pair<key_type, value_type>& pr);
		void insert(std::initializer_list<std::pair<key_type, value_type>> ilist);

		template<input_iterator_of_pair<key_type,value_type> Iter>
		void insert(Iter begin, Iter end)
		{
			node_ptr root = rb_tree_node_helper::rb_tree_insert(_root.data(), begin, end, _alloc, _cmp);
			_root = iterator{ root };
		}

		void erase(iterator pos);

		

	private:
		iterator _root;
		allocator_type _alloc;
		compare_type _cmp;
	};


	template<typename K,typename V,typename Alloc,typename Cmp,typename ostream>
	ostream& operator<<(ostream& os, const rb_tree<K, V, Alloc, Cmp>& tree)
	{
		for (auto p = tree.begin(); p != tree.end(); ++p)
		{
			os << *(p.data()) << " ";
		}
		return os;
	}




	

	template<typename K, typename V, typename Alloc, typename Cmp>
	void rb_tree<K, V, Alloc, Cmp>::insert(const key_type& key, const value_type& val)
	{
		node_ptr root = rb_tree_node_helper::rb_tree_insert(_root.data(), key, val, _alloc, _cmp);
		_root = iterator{ root };
	}

	template<typename K, typename V, typename Alloc, typename Cmp>
	void rb_tree<K, V, Alloc, Cmp>::insert(std::pair<key_type, value_type>& pr)
	{
		insert(pr.first, pr.second);
	}

	template<typename K, typename V, typename Alloc, typename Cmp>
	void rb_tree<K, V, Alloc, Cmp>::insert(size_t count, const key_type& key, const value_type& val)
	{
		node_ptr root=rb_tree_node_helper::rb_tree_insert(_root.data(), count, key, val, _alloc, _cmp);
		_root = iterator{ root };
	}

	template<typename K, typename V, typename Alloc, typename Cmp>
	void rb_tree<K, V, Alloc, Cmp>::insert(size_t count, std::pair<key_type, value_type>& pr)
	{
		insert(count, pr.first, pr.second);
	}

	template<typename K, typename V, typename Alloc, typename Cmp>
	void rb_tree<K, V, Alloc, Cmp>::insert(std::initializer_list<std::pair<key_type, value_type>> ilist)
	{
		insert(ilist.begin(), ilist.end());
	}

	template<typename K, typename V, typename Alloc, typename Cmp>
	void rb_tree<K, V, Alloc, Cmp>::erase(iterator pos)
	{
		node_ptr root = rb_tree_node_helper::rb_tree_erase(_root.data(), pos.data(), _alloc);
		_root = iterator{ root };
	}





}