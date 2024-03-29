#pragma once
#include"my_forward_iterator.h"

namespace fantasil {



	template<typename T, typename Alloc=std::allocator<forward_node<T>>>
	class my_forward_list {
	public:
		using node_type = forward_node<T>;
		using node_ptr = node_type*;
		using iterator = forward_iterator<node_type>;
		using const_iterator = const iterator;
		using value_type = T;
		using reference = value_type&;
		using pointer = value_type*;
		using self = my_forward_list<T>;
		using allocator_type = Alloc;
		using size_type = size_t;

	public:
		template<typename Iter>
		my_forward_list(Iter begin, Iter end);
		my_forward_list(std::initializer_list<T> ilist);
		my_forward_list(const my_forward_list<T, Alloc>& rhs);
		my_forward_list(my_forward_list<T, Alloc>&& rhs);
		my_forward_list<T,Alloc>& operator=(const my_forward_list<T, Alloc>& rhs);
		my_forward_list<T,Alloc>& operator=(my_forward_list<T, Alloc>&& rhs);
		~my_forward_list();

	public:
		inline bool empty() const { return _head; }
		void insert_after(iterator pos, const T& val);
		void insert_after(iterator pos, size_type sz, const T& val);
		template<typename Iter>
		void insert_after(iterator pos, Iter begin, Iter end);
		void insert_after(iterator pos, std::initializer_list<T> ilist);

		void erase_after(iterator pos);
		void erase_sublist_after(iterator pos, iterator back);
		void erase_sublist_after(iterator pos, size_type count);

		void push_back(const T& val);
		void push_front(const T& val);
		void pop_front();

		template<typename U,typename RAlloc,typename ostream>
		friend ostream& operator<<(ostream& os, const my_forward_list<U, RAlloc>& list);
		

	public:
		iterator begin()  { return _head; }
		const_iterator begin() const 
		{
			return iterator{ _head.data() };
		}
		iterator end()  { return iterator{}; }

		const_iterator end() const 
		{
			return iterator{};
		}

	private:
		iterator _head;
		allocator_type _alloc;
	};

	template<typename T, typename Alloc,typename ostream>
	ostream& operator<<(ostream& os, const my_forward_list<T, Alloc>& list)
	{
		for (auto p = list.begin(); p != list.end(); ++p)
			os << *p << " ";
		return os;
	}
	template<typename T, typename Alloc>
	my_forward_list<T, Alloc>::my_forward_list(std::initializer_list<T> ilist)
		:_alloc{}
	{
		node_ptr head= forward_node_helper::make_basic_forward_list<node_type,decltype(ilist.begin()),allocator_type>(ilist.begin(), ilist.end(), _alloc);
		_head = iterator{ head };
	}
	template<typename T, typename Alloc>
	my_forward_list<T, Alloc>::my_forward_list(const my_forward_list<T, Alloc>& rhs)
		:_alloc{}
	{
		node_ptr head=forward_node_helper::deep_copy_forward_list(rhs._head.operator->(), _alloc);
		_head = iterator{ head };
	}
	template<typename T, typename Alloc>
	inline my_forward_list<T, Alloc>::my_forward_list(my_forward_list<T, Alloc>&& rhs)
		:_head{ rhs._head }, _alloc{}
	{
		rhs._head = iterator{ nullptr };
	}
	template<typename T, typename Alloc>
	my_forward_list<T,Alloc>& my_forward_list<T, Alloc>::operator=(const my_forward_list<T, Alloc>& rhs)
	{
		_alloc = Alloc{};
		node_ptr head = forward_node_helper::deep_copy_forward_list(rhs._head.operator->(), _alloc);
		_head = iterator{ head };
		return *this;
	}
	template<typename T, typename Alloc>
	my_forward_list<T,Alloc>& my_forward_list<T, Alloc>::operator=(my_forward_list<T, Alloc>&& rhs)
	{
		_alloc = Alloc{};
		_head = rhs._head;
		rhs._head = iterator{ nullptr };
		return *this;
	}
	template<typename T, typename Alloc>
	inline my_forward_list<T, Alloc>::~my_forward_list()
	{
		forward_node_helper::destroy_basic_forward_list(_head.operator->(), _alloc);
	}
	template<typename T, typename Alloc>
	void my_forward_list<T, Alloc>::insert_after(iterator pos, const T& val)
	{
		forward_node_helper::insert_after<node_type,allocator_type>(pos.data(), val, _alloc);
	}

	template<typename T, typename Alloc>
	void my_forward_list<T, Alloc>::insert_after(iterator pos, size_type count, const T& val)
	{

		forward_node_helper::insert_after<node_type,allocator_type>(pos.data(), count, val, _alloc);
	}


	template<typename T, typename Alloc>
	template<typename Iter>
	inline my_forward_list<T, Alloc>::my_forward_list(Iter begin, Iter end)
		:_alloc{}
	{
		static_assert(std::is_same_v<T, std::iterator_traits<Iter>::value_type>);
		node_ptr head = forward_node_helper::make_basic_forward_list<node_type,Iter,Alloc>(begin, end, _alloc);
		_head = iterator{ head };
	}

	template<typename T, typename Alloc>
	template<typename Iter>
	void my_forward_list<T, Alloc>::insert_after(iterator pos, Iter begin, Iter end)
	{
		forward_node_helper::insert_after<node_type,Iter,allocator_type>(pos.data(), begin, end, _alloc);
	}

	template<typename T, typename Alloc>
	void my_forward_list<T, Alloc>::insert_after(iterator pos, std::initializer_list<T> ilist)
	{
		insert_after(pos, ilist.begin(), ilist.end());
	}

	template<typename T, typename Alloc>
	void my_forward_list<T, Alloc>::erase_after(iterator pos)
	{
		forward_node_helper::erase_after(pos.data(), _alloc);
	}

	template<typename T, typename Alloc>
	inline void my_forward_list<T, Alloc>::erase_sublist_after(iterator pos, iterator back)
	{
		forward_node_helper::erase_sublist_after<node_type,iterator,allocator_type>(pos.data(), back.operator->(),_alloc);
	}

	template<typename T, typename Alloc>
	inline void my_forward_list<T, Alloc>::erase_sublist_after(iterator pos, size_type count)
	{
		forward_node_helper::erase_sublist_after<node_type,allocator_type>(pos.data(), count, _alloc);
	}

	template<typename T, typename Alloc>
	inline void my_forward_list<T, Alloc>::push_back(const T& val)
	{
		forward_node_helper::push_back(_head.operator->(), val, _alloc);
	}

	template<typename T, typename Alloc>
	inline void my_forward_list<T, Alloc>::push_front(const T& val)
	{
		node_ptr tmp=forward_node_helper::push_front(_head.operator->(), val, _alloc);
		_head = iterator{ tmp };
	}

	template<typename T, typename Alloc>
	inline void my_forward_list<T, Alloc>::pop_front()
	{
		node_ptr tmp = forward_node_helper::pop_front(_head.operator->(), _alloc);
		_head = iterator{ tmp };
	}



}