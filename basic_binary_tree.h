#pragma once
#include"basic_tree_iterator.h"

namespace fantasil {


	template<typename T,typename Alloc=std::allocator<binary_tree_node<T>>,typename Cmp=std::less<T>>
	class basic_binary_tree
	{
	public:
		using value_type = T;
		using reference = value_type&;
		using pointer = value_type*;
		using node_type = binary_tree_node<T>;
		using node_ptr = node_type*;
		using iterator = basic_tree_iterator<node_type>;
		using const_iterator = const iterator;
		using allocator_type = Alloc;
		using compare_type = Cmp;

	public:
		basic_binary_tree() :_root{ nullptr } {}
		basic_binary_tree(std::initializer_list<value_type> ilist);

		template<input_iterator_type Iter>
		basic_binary_tree(Iter begin, Iter end);
	public:
		iterator begin() { return binary_tree_node_helper::extreame_left_node(_root.data()); }
		const_iterator begin() const { return binary_tree_node_helper::extreame_left_node(_root.data()); }
		iterator end() { return iterator{ nullptr }; }
		const_iterator end() const { return iterator{ nullptr }; }
		iterator back() { return binary_tree_node_helper::extreame_right_node(_root.data()); }
		const_iterator back() const { return binary_tree_node_helper::extreame_right_node(_root.data()); }
		iterator find(const value_type& val)
		{
			node_ptr tmp = binary_tree_node_helper::binary_tree_find(_root.data(), val, _cmp);
			return iterator{ tmp };
		}
		const_iterator find(const value_type& val) const
		{
			node_ptr tmp = binary_tree_node_helper::binary_tree_find(_root.data(), val, _cmp);
			return iterator{ tmp };
		}
	public:

		void insert(const value_type& val);
		void insert(size_t count, const value_type& val);
		template<input_iterator_type Iter>
		void insert(Iter begin, Iter end);

		void erase(iterator pos);

	public:
		template<typename U,typename UAlloc,typename ostream>
		friend ostream& operator<<(ostream& os, const basic_binary_tree<U, UAlloc>& rhs);

	private:
		iterator _root;
		allocator_type _alloc;
		compare_type _cmp;

	};




	template<typename T, typename Alloc, typename Cmp>
	template<input_iterator_type Iter>
	inline basic_binary_tree<T, Alloc, Cmp>::basic_binary_tree(Iter begin, Iter end)
	{
		node_ptr root=binary_tree_node_helper::make_basic_bianry_tree<node_type>(begin, end, _alloc, _cmp);
		_root = iterator{ root };
	}

	template<typename T, typename Alloc, typename Cmp>
	template<input_iterator_type Iter>
	inline void basic_binary_tree<T, Alloc, Cmp>::insert(Iter begin, Iter end)
	{
		binary_tree_node_helper::binary_tree_insert(_root.data(), begin, end, _alloc, _cmp);
	}

	template<typename T, typename Alloc, typename Cmp>
	inline basic_binary_tree<T, Alloc, Cmp>::basic_binary_tree(std::initializer_list<value_type> ilist)
	{
		node_ptr root = binary_tree_node_helper::make_basic_bianry_tree<node_type>(ilist.begin(), ilist.end(), _alloc, _cmp);
		_root = iterator{ root };
	}



	template<typename T, typename Alloc, typename Cmp>
	inline void basic_binary_tree<T, Alloc, Cmp>::insert(const value_type& val)
	{
		binary_tree_node_helper::binary_tree_insert(_root.data(), val, _alloc, _cmp);
	}

	template<typename T, typename Alloc, typename Cmp>
	inline void basic_binary_tree<T, Alloc, Cmp>::insert(size_t count, const value_type& val)
	{
		binary_tree_node_helper::binary_tree_insert(_root.data(), count, val, _alloc, _cmp);
	}

	template<typename T, typename Alloc, typename Cmp>
	inline void basic_binary_tree<T, Alloc, Cmp>::erase(iterator pos)
	{
		node_ptr root=binary_tree_node_helper::binary_tree_erase(_root.data(), pos.data(), _alloc);
		_root = iterator{ root };
	}

	


	template<typename T, typename Alloc, typename ostream>
	ostream& operator<<(ostream& os, const basic_binary_tree<T, Alloc>& rhs)
	{
		for (auto p = rhs.begin(); p != rhs.end(); ++p)
		{
			os << *p << " ";
		}
		os << "\n";
		return os;
	}

}