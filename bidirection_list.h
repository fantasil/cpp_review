#include"headers.h"
#include"bi_node_iterator.h"

#define DEBUG

namespace fantasil
{
	
	template<typename T,typename Alloc=std::allocator<bi_node<T>>>
	class bidirection_list {
	public:
		using iterator = bi_node_iterator<T>;
		using value_type =typename  std::iterator_traits<iterator>::value_type;
		using reference = value_type&;
		using node_type = bi_node<T>;
		using node_ptr = node_type*;
	private:
		iterator _head;
		Alloc _alloc;
	public:
		template<typename Iter>
		bidirection_list(Iter begin, Iter end) { node_ptr tmp = make_bidirection_list(begin, end); _head = iterator{ tmp }; }

		~bidirection_list() { destroy_bidirection_list(_head.operator->()); }
	public:
		void insert_after(iterator pos, const reference val);
		void insert_before(iterator pos, const reference val);
		void erase(iterator pos);
		
		inline iterator begin() const { return _head; }
		inline iterator end() const { return iterator{ nullptr }; }

	};

	template<typename T, typename Alloc>
	void bidirection_list<T, Alloc>::insert_after(iterator pos, const reference val)
	{
		node_ptr tmp = _alloc.allocate(1);
		tmp->_val = val;
		tmp->_prev = tmp->_next = nullptr;
		insert_after(pos.operator->(), tmp);
	}

	template<typename T, typename Alloc>
	inline void bidirection_list<T, Alloc>::insert_before(iterator pos, const reference val)
	{
		node_ptr tmp = _alloc.allocate(1);
		tmp->_val = val;
		tmp->_prev = tmp->_next = nullptr;
		insert_before(pos.operator->(), tmp);
	}

	template<typename T, typename Alloc>
	inline void bidirection_list<T, Alloc>::erase(iterator pos)
	{
		erase(pos.operator->());
	}

}