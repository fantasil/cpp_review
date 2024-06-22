#pragma once
#include"my_forward_list.h"
namespace fantasil {



	template<typename T>
	struct bi_node {
		using value_type = T;
		using reference = T&;
		using pointer = T*;
		using node_type = bi_node<T>;
		using node_ptr = node_type*;
		using node_category = bidirectional_node_tag;

		bi_node(const T& val, node_ptr prev = nullptr, node_ptr next = nullptr) :_val{ val }, _prev{ prev }, _next{ next } {}
		~bi_node() { _val.~T(); _prev = _next = nullptr; }

		value_type _val;
		node_ptr _prev;
		node_ptr _next;
	};

	//值相等?
	template<typename T>
	bool operator==(const bi_node<T>& lhs, const bi_node<T>& rhs)
	{
		return lhs._val == rhs._val;
	}

	//值不等?
	template<typename T>
	bool operator!=(const bi_node<T>& lhs, const bi_node<T>& rhs)
	{
		return lhs._val != rhs._val;
	}

	//输出打印
	template<typename T,typename ostream>
	ostream& operator<<(ostream& os, const bi_node<T>& node)
	{
		os << node._val;
		return os;
	}


	namespace bi_node_helper {


		//
		template<bi_node_type Node>
		inline void as_prev(Node* prev, Node* next);
		
		//连接两个节点
		template<bi_node_type Node>
		inline void contact(Node* prev, Node* next);

		//连接两个双向链表
		template<bi_node_type Node>
		inline void merge_bi_list(Node* lhs_head, Node* rhs_head);

		//给定节点，返回头节点
		template<bi_node_type Node>
		Node* begin(Node* node);

		//给定节点，返回尾节点(可解引用访问)
		template<bi_node_type Node>
		Node* back(Node* node);

		//给定节点，返回前进n步的节点
		template<bi_node_type Node>
		Node* step_forward(Node* pos,size_t n);

		//给定节点，返回后退n步的节点
		template<bi_node_type Node>
		Node* step_back(Node* pos,size_t n);

		//构造双向链表
		template<bi_node_type Node,input_iterator_type Iter,typename Alloc>
		Node* make_basic_bidirectional_list(Iter begin, Iter end, Alloc& alloc);

		//使用val初始化具有count个节点的双向链表
		template<bi_node_type Node, typename Alloc>
		Node* make_basic_bidirectional_list(size_t count,const typename node_traits<Node>::reference val, Alloc& alloc);

		//使用initializer_list初始化双向链表
		template<bi_node_type Node,typename Alloc>
		Node* make_basic_bidirectional_list(std::initializer_list<typename node_traits<Node>::value_type> ilist,Alloc& alloc);


		//销毁双向链表并回收空间
		template<bi_node_type Node,typename Alloc>
		void destroy_basic_bidirectional_list(Node* head, Alloc& alloc);

		/*
		* 插入元素
		* insert_after不会影响head节点
		* insert_before操作会影响head节点
		*/

		//指定位置之后插入序列
		template<bi_node_type Node,input_iterator_type Iter,typename Alloc>
		void insert_after(Node* pos, Iter begin, Iter end, Alloc& alloc);

		//指定位置之前插入序列
		template<bi_node_type Node,input_iterator_type Iter,typename Alloc>
		Node* insert_before(Node* pos, Iter begin, Iter end, Alloc& alloc);

		//指定位置之后连续构造count个节点
		template<bi_node_type Node, typename Alloc>
		void insert_after(Node* pos, size_t count, const typename node_traits<Node>::reference val, Alloc& alloc);

		//指定位置之后连续构造count个节点
		template<bi_node_type Node, typename Alloc>
		Node* insert_before(Node* pos, size_t count, const typename node_traits<Node>::reference val, Alloc& alloc);

		//指定位置之后插入节点
		template<bi_node_type Node,typename Alloc>
		void insert_after(Node* pos, const typename node_traits<Node>::reference val, Alloc& alloc);

		//指定位置之前插入节点
		template<bi_node_type Node, typename Alloc>
		Node* insert_before(Node* pos, const typename node_traits<Node>::reference val, Alloc& alloc);

		/*
		* 删除元素
		* erase_after操作不会影响head节点，故返回值为void
		* erase可能会影响head节点，故返回连接后的右边头节点
		* erase_after其实和erase逻辑相同,知识指代的iterator不同而已，故删除erase_after系列
		*/

		////删除pos之后的节点
		//template<bi_node_type Node,typename Alloc>
		//void erase_after(Node* pos,Alloc& alloc);

		//删除pos节点
		template<bi_node_type Node,typename Alloc>
		Node* erase(Node* pos,Alloc& alloc);

		////删除pos之后的序列(pos,back]
		//template<bi_node_type Node,typename Alloc>
		//void erase_sublist_after(Node* pos, Node* back, Alloc& alloc);

		////删除pos之后的n个节点
		//template<bi_node_type Node,typename Alloc>
		//void erase_sublist_after(Node* pos, size_t count, Alloc& alloc);

		//删除[pos,back]
		template<bi_node_type Node,typename Alloc>
		Node* erase_sublist(Node* pos, Node* back, Alloc& alloc);

		//删除pos(包括pos在内的)n个节点
		template<bi_node_type Node,typename Alloc>
		Node* erase_sublist(Node* pos, size_t count, Alloc& alloc);

		//打印测试
		template<bi_node_type Node,typename ostream>
		void print_bidirectional_list(Node* head,ostream& os);



		//impl
		template<bi_node_type Node>
		void as_prev(Node* prev, Node* next)
		{
			if (next)
				next->_prev = prev;
		}

		template<bi_node_type Node>
		void contact(Node* prev, Node* next)
		{
			if (prev)
				prev->_next = next;
			if (next)
				next->_prev = prev;
		}

		template<bi_node_type Node>
		void merge_bi_list(Node* lhs_head, Node* rhs_head)
		{
			using node_ptr = Node*;
			node_ptr lhs_back = back(lhs_head);
			contact(lhs_back, rhs_head);
		}

		template<bi_node_type Node>
		Node* begin(Node* node)
		{
			if (!node)
				return nullptr;
			while (node)
				node = node->_prev;
			return node;
		}

		template<bi_node_type Node>
		Node* back(Node* node)
		{
			if (!node)
				return nullptr;
			while (node->_next)
				node = node->_next;
			return node;
		}

		template<bi_node_type Node>
		Node* step_forward(Node* pos,size_t n)
		{
			while (pos && n)
			{
				pos = pos->_next;
				--n;
			}
			if (n)
				throw std::out_of_range{ "out of range" };
			return pos;
		}

		template<bi_node_type Node>
		Node* step_back(Node* pos,size_t n)
		{
			while (pos && n)
			{
				pos = pos->_prev;
				--n;
			}
			if (n)
				throw std::out_of_range{ "out of range" };
			return pos;
		}

		template<bi_node_type Node, input_iterator_type Iter, typename Alloc>
		Node* make_basic_bidirectional_list(Iter begin, Iter end, Alloc& alloc)
		{
			using node_ptr = typename node_traits<Node>::node_ptr;
			
			if (begin == end)
				return nullptr;
			node_ptr head = alloc.allocate(1);
			head->_val = *begin;
			head->_prev = nullptr;
			head->_next = nullptr;
			node_ptr prev = head, next = nullptr;
			for (auto p = ++begin; p != end; ++p)
			{
				next = alloc.allocate(1);
				next->_val = *p;
				next->_prev = nullptr;
				next->_next = nullptr;
				contact(prev, next);
				prev = next;
			}
			return head;
		}

		template<bi_node_type Node, typename Alloc>
		Node* make_basic_bidirectional_list(size_t count, const typename node_traits<Node>::value_type& val, Alloc& alloc)
		{
			using node_ptr = Node*;
			//构造链表
			if (!count)
				return nullptr;
			node_ptr rhs_head = alloc.allocate(1);
			rhs_head->_val = val;
			rhs_head->_prev = rhs_head->_next = nullptr;
			node_ptr prev = rhs_head, next = nullptr;
			
			while (--count)
			{
				next = alloc.allocate(1);
				next->_val = val;
				next->_prev = next->_next = nullptr;
				contact(prev, next);
				prev = next;
				
			}
			return rhs_head;
		}

		template<bi_node_type Node, typename Alloc>
		Node* make_basic_bidirectional_list(std::initializer_list<typename node_traits<Node>::value_type> ilist,Alloc& alloc)
		{
			using iterator = decltype(ilist.begin());
			return make_basic_bidirectional_list<Node, iterator, Alloc>(ilist.begin(), ilist.end(),alloc);
		}

		template<bi_node_type Node, typename Alloc>
		void destroy_basic_bidirectional_list(Node* head, Alloc& alloc)
		{
			using node_ptr = Node*;
			if (!head)
				return;
			node_ptr prev = head, next = head->_next;
			while (next)
			{
				std::destroy_at(prev);
				alloc.deallocate(prev, 1);
				prev = next;
				next = prev->_next;
			}
		}

		template<bi_node_type Node, input_iterator_type Iter, typename Alloc>
		void insert_after(Node* pos, Iter begin, Iter end, Alloc& alloc)
		{
			using node_ptr = Node*;
			node_ptr rhs_head = make_basic_bidirectional_list<Node, Iter, Alloc>(begin, end, alloc);
			node_ptr rhs_back = back(rhs_head);
			node_ptr next = pos->_next;
			contact(pos, rhs_head);
			contact(rhs_back, next);
		}

		template<bi_node_type Node, typename Iter, typename Alloc>
		Node* insert_before(Node* pos, Iter begin, Iter end, Alloc& alloc)
		{
			using node_ptr = Node*;
			node_ptr rhs_head = make_basic_bidirectional_list<Node, Iter, Alloc>(begin, end, alloc);
			node_ptr rhs_back = back(rhs_head);
			node_ptr prev = pos->_prev;
			contact(prev, rhs_head);
			contact(rhs_back, pos);
			return rhs_head;
		}

		template<bi_node_type Node, typename Alloc>
		void insert_after(Node* pos, size_t count, const typename node_traits<Node>::value_type& val, Alloc& alloc)
		{
			using node_ptr = Node*;
			node_ptr rhs_head = make_basic_bidirectional_list<Node, Alloc>(count, val, alloc);
			node_ptr rhs_back = back(rhs_head);
			node_ptr next = pos->_next;
			contact(pos, rhs_head);
			contact(rhs_back, next);
		}

		template<bi_node_type Node, typename Alloc>
		Node* insert_before(Node* pos, size_t count, const typename node_traits<Node>::value_type& val, Alloc& alloc)
		{
			using node_ptr = Node*;
			node_ptr rhs_head = make_basic_bidirectional_list<Node, Alloc>(count, val, alloc);
			node_ptr rhs_back = back(rhs_head);
			node_ptr prev = pos->_prev;
			contact(prev, rhs_head);
			contact(rhs_back, pos);
			return rhs_head;
		}

		template<bi_node_type Node, typename Alloc>
		void insert_after(Node* pos, const typename node_traits<Node>::value_type& val, Alloc& alloc)
		{
			insert_after<Node, Alloc>(pos, 1, val, alloc);
		}

		template<bi_node_type Node, typename Alloc>
		Node* insert_before(Node* pos, const typename node_traits<Node>::reference val, Alloc& alloc)
		{
			return insert_before<Node, Alloc>(pos, 1, val, alloc);
		}

		/*template<bi_node_type Node, typename Alloc>
		void erase_after(Node* pos, Alloc& alloc)
		{
			using node_ptr = Node*;
			node_ptr to_delete = pos->_next;
			node_ptr next = to_delete->_next;
			contact(pos, next);
			std::destroy_at(to_delete);
			alloc.deallocate(to_delete, 1);
		}*/

		template<bi_node_type Node, typename Alloc>
		Node* erase(Node* pos, Alloc& alloc)
		{
			using node_ptr = Node*;
			node_ptr prev = pos->_prev;
			node_ptr next = pos->_next;
			contact(prev, next);
			std::destroy_at(pos);
			alloc.deallocate(pos, 1);
			return next;
		}

		//template<bi_node_type Node, typename Alloc>
		//void erase_sublist_after(Node* pos, Node* back, Alloc& alloc)
		//{
		//	using node_ptr = Node*;

		//	node_ptr rhs_head = pos->_next;
		//	node_ptr next = back->_next;
		//	contact(pos, next);
		//	back->_next = nullptr;
		//	destroy_basic_bidirectional_list(rhs_head, alloc);
		//}

		//template<bi_node_type Node, typename Alloc>
		//void erase_sublist_after(Node* pos, size_t count, Alloc& alloc)
		//{
		//	using node_ptr = Node*;
		//	node_ptr back = step_forward(pos, count);
		//	erase_after<Node, Alloc>(pos, back, alloc);
		//}

		template<bi_node_type Node, typename Alloc>
		Node* erase_sublist(Node* pos, Node* back, Alloc& alloc)
		{
			using node_ptr = Node*;
			node_ptr prev = pos->_prev;
			node_ptr next = back->_next;
			contact(prev, next);
			back->_next = nullptr;
			destroy_basic_bidirectional_list(pos, alloc);
			return next;
		}

		template<bi_node_type Node, typename Alloc>
		Node* erase_sublist(Node* pos, size_t count, Alloc& alloc)
		{
			using node_ptr = Node*;
			node_ptr back = step_forward(pos, --count);
			return erase_sublist<Node, Alloc>(pos, back, alloc);
		}

		template<bi_node_type Node, typename ostream>
		void print_bidirectional_list(Node* head, ostream& os)
		{
			while (head)
			{
				os << *head << " ";
				head = head->_next;
			}
			os << "\n";
		}





	}

	

}