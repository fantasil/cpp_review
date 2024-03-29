#pragma once
#include"my_bidirectional_iterator.h"

namespace fantasil {

	template<typename T, typename Alloc = std::allocator<bi_node<T>>>
	class my_bidirectional_list
	{
	public:
		using node_type = bi_node<T>;
		using node_ptr = node_type*;
		using iterator = my_bidirectional_iterator<node_type>;
		using const_iterator = const iterator;
		using value_type = typename std::iterator_traits<iterator>::value_type;
		using reference = value_type&;
		using pointer = value_type*;
		using difference_type = typename std::iterator_traits<iterator>::difference_type;
		using iterator_category = typename std::iterator_traits<iterator>::iterator_category;
		using allocator_type = Alloc;
		using size_type = size_t;
	public:
		template<input_iterator_type Iter>
		my_bidirectional_list(Iter begin, Iter end);
		my_bidirectional_list(std::initializer_list<T> ilist);
		~my_bidirectional_list();
	public:
		inline bool empty() const { return staic_cast<bool>(_begin); }
		inline iterator begin() { return _begin; }
		inline const_iterator begin() const { return _begin; }
		inline iterator end() { return iterator{ nullptr }; }
		inline const_iterator end() const { return iterator{ nullptr }; }

	public:
		//在节点之前插入
		void insert_before(iterator pos, const value_type& val);
		void insert_before(iterator pos, size_type count, const value_type& val);
		template<input_iterator_type Iter>
		void insert_before(iterator pos, Iter begin, Iter end);

		//节点之后插入
		void insert_after(iterator pos, const value_type& val);
		void insert_after(iterator pos, size_type count, const value_type& val);
		template<input_iterator_type Iter>
		void insert_after(iterator pos, Iter begin, Iter end);

		//删除节点
		void erase(iterator pos);
		void erase(iterator begin, iterator back);
		void erase(iterator beign, size_t count);



	private:
		iterator _begin;
		allocator_type _alloc;
	};

	template<typename T, typename Alloc, typename ostream >
	ostream& operator<<(ostream& os, const my_bidirectional_list<T, Alloc>& list)
	{
		for (auto p = list.begin(); p != list.end(); ++p)
		{
			os << *p << " ";
		}
		os << "\n";
		return os;
	}

	template<typename T, typename Alloc>
	template<input_iterator_type Iter>
	my_bidirectional_list<T, Alloc>::my_bidirectional_list(Iter begin, Iter end)
		:_alloc{}
	{
		node_ptr head=bi_node_helper::make_basic_bidirectional_list<node_type,Iter,allocator_type>(begin, end, _alloc);
		_begin = iterator{ head };
	}

	

	template<typename T, typename Alloc>
	my_bidirectional_list<T, Alloc>::my_bidirectional_list(std::initializer_list<T> ilist)
		:_alloc{}
	{
		node_ptr head = bi_node_helper::make_basic_bidirectional_list<node_type,allocator_type>(ilist, _alloc);
		_begin = iterator{ head };
	}

	template<typename T, typename Alloc>
	my_bidirectional_list<T, Alloc>::~my_bidirectional_list()
	{
		bi_node_helper::destroy_basic_bidirectional_list(_begin.data(), _alloc);
		_begin.data(nullptr);
	}

	template<typename T, typename Alloc>
	void my_bidirectional_list<T, Alloc>::insert_before(iterator pos, const value_type& val)
	{
		node_ptr node=bi_node_helper::insert_before(pos.data(), val, _alloc);
		if (pos == _begin)
			_begin = iterator{ node };
	}

	template<typename T, typename Alloc>
	void my_bidirectional_list<T, Alloc>::insert_before(iterator pos, size_type count, const value_type& val)
	{
		node_ptr node = bi_node_helper::insert_before(pos.data(), count, val, _alloc);
		if (pos == _begin)
			_begin = iterator{ node };
	}

	template<typename T, typename Alloc>
	inline void my_bidirectional_list<T, Alloc>::insert_after(iterator pos, const value_type& val)
	{
		bi_node_helper::insert_after(pos.data(), val, _alloc);
	}

	template<typename T, typename Alloc>
	inline void my_bidirectional_list<T, Alloc>::insert_after(iterator pos, size_type count, const value_type& val)
	{
		bi_node_helper::insert_after(pos.data(), count, val, _alloc);
	}

	template<typename T, typename Alloc>
	inline void my_bidirectional_list<T, Alloc>::erase(iterator pos)
	{
		node_ptr node=bi_node_helper::erase(pos.data(), _alloc);
		if (pos == _begin)
			_begin = iterator{ node };
	}

	template<typename T, typename Alloc>
	inline void my_bidirectional_list<T, Alloc>::erase(iterator begin, iterator back)
	{
		node_ptr node = bi_node_helper::erase_sublist(begin.data(), back.data(), _alloc);
		if (begin == _begin)
			_begin = iterator{ node };
	}

	template<typename T, typename Alloc>
	inline void my_bidirectional_list<T, Alloc>::erase(iterator begin, size_t count)
	{
		node_ptr node = bi_node_helper::erase_sublist(begin.data(), count, _alloc);
		if (begin == _begin)
			_begin = iterator{ node };
	}

	template<typename T, typename Alloc>
	template<input_iterator_type Iter>
	void my_bidirectional_list<T, Alloc>::insert_before(iterator pos, Iter begin, Iter end)
	{
		node_ptr node = bi_node_helper::insert_before(pos.data(), begin, end, _alloc);
		if (pos == _begin)
			_begin = iterator{ node };
	}

	template<typename T, typename Alloc>
	template<input_iterator_type Iter>
	inline void my_bidirectional_list<T, Alloc>::insert_after(iterator pos, Iter begin, Iter end)
	{
		bi_node_helper::insert_after(pos.data(), begin, end, _alloc);
	}



	





}