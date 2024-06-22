#pragma once

#include"headers.h"
#include"node_traits.h"

#define DEBUG

namespace fantasil
{
	/*---------------------------------------------�ڵ�--------------------------------------------------------*/
	template<typename T>
	struct bi_node {
	public:
		//typedef...
		using value_type = T;
		using reference = value_type&;
		using node_type = bi_node<T>;
		using node_ptr = node_type*;
	public:
		//data member
		value_type _val;
		node_ptr _prev;
		node_ptr _next;
	public:

		
	public:
		//construct 
		bi_node(const reference val, node_ptr prev = nullptr, node_ptr next = nullptr) :_val{ val }, _prev{ prev }, _next{ next } {}

		~bi_node() { _val.~T(); _prev = _next = nullptr; }
	};


	/*---------------------------------------------------------------------------------------------------------*/
	
	namespace bi_node_helper {
		
#pragma region bi_node_helper_declare

		template<bi_node_type Node>
		inline void contact(Node* prev, Node* next);

		//������ָ���ڵ�λ��֮ǰ
		template<bi_node_type Node>
		inline Node* insert_before(Node* head, Node* pos, Node* node);

		//������pos֮ǰ,
		template<bi_node_type Node,typename Alloc=std::allocator<Node>>
		inline Node* insert_before(Node* head, Node* pos, const typename node_traits<Node>::value_type& val, Alloc& alloc);

		//������ָ���ڵ�λ��֮��
		template<bi_node_type Node>
		inline void insert_after(Node* pos, Node* node);

		//������pos֮��,
		template<bi_node_type Node, typename Alloc = std::allocator<Node>>
		inline void insert_after(Node* pos, const typename node_traits<Node>::value_type& val, Alloc& alloc);
		

		//ɾ��ָ��λ�õĽڵ�
		template<bi_node_type Node>
		inline void erase(Node* pos);



#pragma endregion

	



		/*---------------------impl------------------------------*/
		//����ڵ�
		

		template<typename T, typename Alloc>
		typename std::allocator_traits<Alloc>::value_type* allocate_node(const T& val, Alloc alloc)
		{
			using node_type = typename std::allocator_traits<Alloc>::value_type;
			using node_ptr = node_type*;
			node_ptr tmp = alloc.allocate(1);
			init_bi_node_type(tmp, val);
			return tmp;
		}

		////��������˫��ڵ�
		//template<typename T>
		//inline void contact(bi_node<T>* lhs, bi_node<T>* rhs)
		//{
		//	if (lhs)
		//		lhs->_next = rhs;
		//	if (rhs)
		//		rhs->_prev = lhs;
		//}
		template<bi_node_type Node>
		inline void contact(Node* prev, Node* next)
		{
			if (prev)
				prev->_next = next;
			if (next)
				next->_prev = prev;
		}


		template<bi_node_type Node>
		Node* insert_before(Node* head, Node* pos, Node* node)
		{
			using node_ptr = Node*;
			if (!pos || !node)
				return head;
			node_ptr prev = pos->_prev;
			contact(prev, node);
			contact(node, pos);
			if (pos == head)
				return node;
			return head;
		}

		template<bi_node_type Node, typename Alloc>
		Node* insert_before(Node* head, Node* pos, const typename node_traits<Node>::value_type& val, Alloc& alloc)
		{
			using node_ptr = Node*;
			node_ptr tmp = allocate_bi_node(val, alloc);
			return insert_before(head, pos, alloc);
		}

		template<bi_node_type Node>
		void insert_after(Node* pos, Node* node)
		{
			using node_ptr = Node*;
			if (!pos || !node)
				return;
			node_ptr next = pos->_next;
			contact(pos, node);
			contact(node, next);
		}


		template<bi_node_type Node>
		void erase(Node* pos)
		{
			using node_ptr = Node*;
			

		}

		////������pos֮ǰ
		//template<typename T>
		//inline void insert_before(bi_node<T>* pos, bi_node<T>* to_insert)
		//{
		//	using node_type = bi_node<T>;
		//	using node_ptr = node_type*;

		//	if (!pos || !to_insert)
		//		return;
		//	node_ptr prev = pos->_prev;
		//	contact(prev, to_insert);
		//	contact(to_insert, pos);
		//}

		template<typename T, typename Alloc>
		void insert_before(typename std::allocator_traits<Alloc>::value_type* pos, const T& val,Alloc& alloc)
		{
			using node_type = std::allocator_traits<Alloc>::value_type;
			using node_ptr = node_type*;

			node_ptr tmp = allocate_node(val, alloc);
			insert_before(pos, tmp);
		}

		

		

		////������pos֮��
		//template<typename T>
		//inline void insert_after(bi_node<T>* pos, bi_node<T>* to_insert)
		//{
		//	using node_type = bi_node<T>;
		//	using node_ptr = node_type*;
		//	if (!pos || !to_insert)
		//		return;
		//	node_ptr next = pos->_next;
		//	contact(pos, to_insert);
		//	contact(to_insert, next);
		//}

		//ɾ��ָ���ڵ�
		template<typename T>
		inline void erase(bi_node<T>* pos)
		{
			using node_type = bi_node<T>;
			using node_ptr = node_type*;

			if (!pos)
				return;
			node_ptr prev = pos->_prev;
			node_ptr next = pos->_next;
			contact(prev, next);
			delete pos;
		}

		//ʹ��[begin,end)����˫������
		template<typename Iter, typename Alloc = std::allocator<bi_node<typename std::iterator_traits<Iter>::value_type>>>
		bi_node<typename std::iterator_traits<Iter>::value_type>* make_bidirection_list(Iter begin, Iter end)
		{
			using value_type = typename std::iterator_traits<Iter>::value_type;
			using node_type = bi_node<value_type>;
			using node_ptr = node_type*;
			if (begin == end)
				return nullptr;
			Alloc alloc{};
			node_ptr head = alloc.allocate(1);
			head->_val = *begin;
			head->_prev = head->_next = nullptr;
			node_ptr cur = head;
			node_ptr tmp = nullptr;
			for (auto p = ++begin; p != end; ++p)
			{
				tmp = alloc.allocate(1);
				tmp->_val = *p;
				tmp->_prev = tmp->_next = nullptr;
				contact(cur, tmp);
				cur = tmp;
			}
			return head;
		}

		//��������
		template<typename T, typename Alloc = std::allocator<bi_node<T>>>
		void destroy_bidirection_list(bi_node<T>* head)
		{
			using node_type = bi_node<T>;
			using node_ptr = node_type*;
			Alloc alloc{};
			node_ptr cur = head;
			node_ptr next = nullptr;
			while (cur)
			{
				next = cur->_next;
				alloc.deallocate(cur, 1);
				cur = next;
			}
		}

		//�������һ���ɽ�����Ԫ��
		template<typename T>
		bi_node<T>* tail_of_bi_list(bi_node<T>* head)
		{
			while (head->_next)
				head = head->_next;
			return head;
		}

		//������������
		template<typename T>
		void contact_bi_list(bi_node<T>* left_head, bi_node<T>* right_head)
		{

			using node_type = bi_node<T>;
			using node_ptr = node_type*;

			if (!left_head || !right_head)
				return;
			node_ptr left_tail = tail_of_bi_list(left_head);
			contact(left_tail, right_head);
		}

		//��pos֮���������
		template<typename T>
		void insert_list_after(bi_node<T>* pos, bi_node<T>* right_head)
		{
			using node_type = bi_node<T>;
			using node_ptr = node_type*;
			if (!pos || !right_head)
				return;
			node_type right_tail = tail_of_bi_list(right_head);
			node_type next = pos->_next;
			contact(pos, right_head);
			contact(right_tail, next);
		}

		//��pos֮ǰ��������
		template<typename T>
		void insert_list_before(bi_node<T>* pos, bi_node<T>* right_head)
		{
			using node_type = bi_node<T>;
			using node_ptr = node_type*;
			if (!pos || !right_head)
				return;
			node_type right_tail = tail_of_bi_list(right_head);
			node_type prev = pos->_prev;
			contact(prev, right_head);
			contact(right_tail, pos);
		}


		//ɾ���ӱ�
		template<typename T>
		void erase_sublist(bi_node<T>* head, bi_node<T>* begin, bi_node<T>* end)
		{
			using node_type = bi_node<T>;
			using node_ptr = node_type*;
			if (begin == end)
				return;
			if (!head)
				return;
			node_ptr prev = begin->_prev;
			node_ptr next = end;
			node_ptr tail = nullptr;
			if (end)
			{
				tail = next->_prev;
				tail->_next = nullptr;
			}

			destroy_bidirection_list(begin);
			contact(prev, next);
		}

	}
	




#ifdef DEBUG
	template<typename T, typename ostream>
	void print_bidirection_list(const bi_node<T>* head, ostream& os)
	{
		if (!head)
			return;
		while (head)
		{
			os << head->_val;
			head = head->_next;
		}
		os << "\n";
	}
#endif
	
}