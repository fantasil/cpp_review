#pragma once
#include"bi_node_iterator.h"
#include"node_traits.h"

#ifndef fantasil_bi_direction_cyclic_list_h
#define fantasil_bi_direction_cyclic_list_h

namespace fantasil
{


	template<typename T,typename Alloc=std::allocator<bi_node<T>>>
	class bi_direction_cyclic_list
	{
	public:
		using iterator = bi_node_iterator<T>;
		using value_type = typename std::iterator_traits<iterator>::value_type;
		using allocator_type = Alloc;
		using node_type = bi_node<T>;
		using node_ptr = node_type*;
	public:

	public:
		void insert(iterator pos,const value_type& val);
		template<typename Iter>
		void insert(iterator pos, Iter begin, Iter end);
		void erase(iterator pos);
	
	private:
		iterator _head;
		iterator _back;
		allocator_type _alloc;
	};

	template<typename T, typename Alloc>
	inline void bi_direction_cyclic_list<T, Alloc>::insert(iterator pos, const value_type& val)
	{
		using namespace bi_node_helper;
		node_ptr tmp = allocate_node(val, _alloc);
		insert_after(*pos, tmp);
	}



	template<typename T, typename Alloc>
	template<typename Iter>
	inline void bi_direction_cyclic_list<T, Alloc>::insert(iterator pos, Iter begin, Iter end)
	{
		static_assert(std::is_same_v(typename std::iterator_traits<Iter>::value_type, value_type));
		using namespace bi_node_helper;
		insert_after()
	}

}


#endif 