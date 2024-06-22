#pragma once
#include"my_bidirectional_iterator.h"
#include"my_exception.h"
#ifndef fantasil_bidirectional_circular_list_h
#define fantasil_bidirectional_circular_list_h


namespace fantasil {

	template<typename T,typename Alloc=std::allocator<bi_node<T>>>
	class bidirectional_circular_list {
	public:
		using node_type = bi_node<T>;
		using node_ptr = node_type*;
		using iterator = my_bidirectional_iterator<node_type>;
		using const_iterator = const iterator;
		using allocator_type = Alloc;
		using value_type = T;


	public:
		iterator begin() { return _sentinel + 1; };
		const_iterator begin() const { node_ptr head = _sentinel->_next; return iterator{ head }; }
		iterator end() { return _sentinel; }
		const_iterator end() const { return _sentinel; }

	public:
		bidirectional_circular_list();
		template<typename Iter>
		bidirectional_circular_list(Iter begin, Iter end);
		~bidirectional_circular_list();

	public:
		inline bool empty() const { return begin() == end(); }
		iterator find(const value_type& val);

		void insert_before(iterator pos,const value_type& val);
		template<input_iterator_type Iter>
		void insert_before(iterator pos, Iter begin, Iter end);
		void insert_after(iterator pos, const value_type& val);
		template<input_iterator_type Iter>
		void insert_after(iterator pos, Iter begin, Iter end);

		void erase(iterator pos);
		void erase(iterator begin, iterator back);

	private:
		iterator _sentinel;		//哨兵
		allocator_type _alloc;
	};

	template<typename T, typename Alloc>
	inline bidirectional_circular_list<T, Alloc>::bidirectional_circular_list()
	{
		
		node_ptr tmp = _alloc.allocate(1);
		init_bi_node_type(tmp, value_type{}, tmp, tmp);
		_sentinel = iterator{ tmp };
	}

	template<typename T, typename Alloc>
	inline bidirectional_circular_list<T, Alloc>::~bidirectional_circular_list()
	{
		erase(begin(), end());

		std::destroy_at(_sentinel.data());
		_alloc.deallocate(_sentinel.data(), 1);
	}

	template<typename T, typename Alloc>
	inline typename bidirectional_circular_list<T,Alloc>::iterator bidirectional_circular_list<T, Alloc>::find(const value_type& val)
	{
		auto p = begin();
		for (; p != end(); ++p)
		{
			if (p->_val == val)
				break;
		}
		return p;
	}

	template<typename T, typename Alloc>
	inline void bidirectional_circular_list<T, Alloc>::insert_before(iterator pos, const value_type& val)
	{
		bi_node_helper::insert_before(pos.data(), val, _alloc);
	}

	template<typename T, typename Alloc>
	inline void bidirectional_circular_list<T, Alloc>::insert_after(iterator pos, const value_type& val)
	{
		bi_node_helper::insert_after(pos.data(), val, _alloc);
	}

	template<typename T, typename Alloc>
	inline void bidirectional_circular_list<T, Alloc>::erase(iterator pos)
	{
		//循环链表的删除需要跳过岗哨节点
		if (end()==pos)
			return;
		bi_node_helper::erase(pos.data(), _alloc);
	}

	template<typename T, typename Alloc>
	inline void bidirectional_circular_list<T, Alloc>::erase(iterator begin, iterator end)
	{
		//循环链表的删除需要跳过岗哨节点
		for (auto p = begin; p != end;)
		{
			//删除过后迭代器失效，需要提前存储
			auto q = p + 1;
			erase(p);
			p = q;
		}
	}

	template<typename T, typename Alloc>
	template<typename Iter>
	inline bidirectional_circular_list<T, Alloc>::bidirectional_circular_list(Iter begin, Iter end)
	{
		static_assert(std::is_same_v<value_type, typename std::iterator_traits<Iter>::value_type>);
		node_ptr tmp = _alloc.allocate(1);
		init_bi_node_type(tmp, value_type{}, tmp, tmp);
		_sentinel = iterator{ tmp };
		insert_after(_sentinel, begin, end);
	}


	template<typename T, typename Alloc>
	template<input_iterator_type Iter>
	inline void bidirectional_circular_list<T, Alloc>::insert_before(iterator pos, Iter begin, Iter end)
	{
		static_assert(std::is_same_v<value_type, std::iterator_traits<Iter>::value_type>);
		for (auto p = begin; p != end; ++p)
		{
			insert_before(pos,*p);
			++pos;
		}
	}

	template<typename T, typename Alloc>
	template<input_iterator_type Iter>
	inline void bidirectional_circular_list<T, Alloc>::insert_after(iterator pos, Iter begin, Iter end)
	{
		static_assert(std::is_same_v<value_type, typename std::iterator_traits<Iter>::value_type>);
		for (auto p = begin; p != end; ++p)
		{
			insert_after(pos, *p);
			++pos;
		}
	}

	template<typename T,typename Ostream>
	Ostream& operator<<(Ostream& os, const bidirectional_circular_list<T>& list)
	{
		os << "{";
		for (auto p = list.begin(); p != list.end(); ++p)
		{
			os << *p << " ";
		}
		os << "}\n";
		return os;
	}




}

#endif 