#pragma once
#include<type_traits>
#include<memory>
#include"node_traits.h"

namespace fantasil {



	//前向节点
	template<typename T>
	struct forward_node {
		using value_type = T;
		using reference = value_type&;
		using pointer = value_type*;
		using node_type = forward_node<T>;
		using node_ptr = node_type*;
		using node_category = forward_node_tag;

		template<typename U, typename ostream>
		friend ostream& operator<<(ostream& os, const forward_node<U>& node);

	public:
		forward_node(const value_type& val, node_ptr next = nullptr) :_val{ val }, _next{ next } {}
		~forward_node() { _val.~T(); _next = nullptr; }
	public:
		value_type _val;
		node_ptr _next;
	};

	template<typename T, typename ostream>
	ostream& operator<<(ostream& os, const forward_node<T>& node)
	{
		os << node._val;
		return os;
	}


	//以下是节点的辅助函数
	namespace forward_node_helper {

		//declare

		/*
		* 连接节点
		*/
		template<forward_node_type Node>
		inline void as_next(Node* prev, Node* next);

		/*
		* 链表容量
		*/
		template<forward_node_type Node>
		size_t size(Node* root);

		/*
		* 获取链表尾元素
		*/
		template<forward_node_type Node>
		Node* back(Node* root);

		//前进n
		template<forward_node_type Node>
		Node* step(Node* pos,size_t n);

		/*
		* 指定pos位置后插入节点,节点可以代表一个节点，也可以是链表
		*/
		template<forward_node_type Node>
		void insert_after(Node* pos, Node* to_insert);

		/*
		* 指定位置后插入元素
		*/
		template<forward_node_type Node,typename Alloc=std::allocator<Node>>
		void insert_after(Node* pos, const typename node_traits<Node>::reference val,Alloc& alloc);

		/*
		* 指定位置后插入序列
		*/
		template<forward_node_type Node,typename Iter,typename Alloc=std::allocator<Node>>
		void insert_after(Node* pos, Iter begin, Iter end, Alloc& alloc);

		//插入count个值为val的节点
		template<forward_node_type Node, typename Alloc = std::allocator<Node>>
		void insert_after(Node* pos, size_t count, const typename node_traits<Node>::reference val, Alloc& alloc);

		/*
		* 构造普通的单链表
		*/
		template<forward_node_type Node, typename Iter, typename Alloc = std::allocator<Node>>
		Node* make_basic_forward_list(Iter begin, Iter end, Alloc& alloc);

		template<forward_node_type Node, typename Alloc = std::allocator<Node>>
		Node* make_basic_forward_list(size_t count,const typename node_traits<Node>::reference val, Alloc& alloc);

		/*
		* 删除节点
		*/
		template<forward_node_type Node, typename Alloc = std::allocator<Node>>
		void erase_after(Node* pos, Alloc& alloc);

		/*
		* 删除子链表(pos,back]
		*/
		template<forward_node_type Node, typename Alloc = std::allocator<Node>>
		void erase_sublist_after(Node* pos, Node* back, Alloc& alloc);

		template<forward_node_type Node, typename Alloc = std::allocator<Node>>
		void erase_sublist_after(Node* pos, size_t count, Alloc& alloc);

		/*
		* 插入到表尾
		*/
		template<forward_node_type Node>
		void push_back(Node* root, Node* to_insert);

		template<forward_node_type Node, typename Alloc = std::allocator<Node>>
		void push_back(Node* root, const typename node_traits<Node>::value_type& val, Alloc& alloc);

		/*
		* 插入到表首
		*/
		template<forward_node_type Node>
		Node* push_front(Node* root, Node* to_insert);

		template<forward_node_type Node, typename Alloc = std::allocator<Node>>
		Node* push_front(Node* root, const typename node_traits<Node>::value_type& val, Alloc& alloc);

		//删除表首
		template<forward_node_type Node, typename Alloc = std::allocator<Node>>
		Node* pop_front(Node* root, Alloc& alloc);

		//合并链表
		template<forward_node_type Node>
		void merge_forward_list(Node* lhs, Node* rhs);

		//打印链表
		template<forward_node_type Node, typename ostream>
		void print_forward_list(Node* head, ostream& os);

		//深拷贝一棵forward_list
		template<forward_node_type Node,typename Alloc=std::allocator<Node>>
		Node* deep_copy_forward_list(Node* rhs_head, Alloc& alloc);
	

		//impl
		template<forward_node_type Node>
		inline void as_next(Node* prev, Node* next)
		{
			if (prev)
				prev->_next = next;
		}


		template<forward_node_type Node>
		size_t size(Node* root)
		{
			size_t sz{};
			while (root)
			{
				root = root->_next;
				++sz;
			}
			return sz;
		}


		template<forward_node_type Node>
		Node* step(Node* pos,size_t n)
		{
			--n;
			while (n)
			{
				pos = pos->_next;
				--n;
			}
			return pos;
		

		}
		
		template<forward_node_type Node>
		void insert_after(Node* pos, Node* to_insert)
		{
			using node_ptr = typename node_traits<Node>::node_ptr;
			using node_ptr = typename node_traits<Node>::node_ptr;
			if (!pos || !to_insert)
				return;
			node_ptr next = pos->_next;
			node_ptr rhs_back = back(to_insert);
			as_next(pos, to_insert);
			as_next(rhs_back, next);
		}

		template<forward_node_type Node, typename Alloc >
		void insert_after(Node* pos, const typename node_traits<Node>::reference val,Alloc& alloc)
		{
			using node_ptr = Node*;
			node_ptr tmp = alloc.allocate(1);
			tmp->_val = val;
			insert_after(pos, tmp);
		}

		template<forward_node_type Node, typename Iter, typename Alloc>
		void insert_after(Node* pos, Iter begin, Iter end, Alloc& alloc)
		{
			using node_ptr = Node*;
			node_ptr rhs_head=make_basic_forward_list<Node,Iter,Alloc>(begin, end, alloc);
			insert_after(pos, rhs_head);
		}

		template<forward_node_type Node, typename Alloc>
		void insert_after(Node* pos, size_t count, const typename node_traits<Node>::reference val, Alloc& alloc)
		{
			using node_ptr = Node*;
			//构造链表
			node_ptr rhs_head=make_basic_forward_list<Node,Alloc>(count, val, alloc);
			//插入
			insert_after(pos, rhs_head);

		}


		/*
		* 删除节点
		*/
		template<forward_node_type Node, typename Alloc >
		void erase_after(Node* pos, Alloc& alloc)
		{
			using node_ptr = typename node_traits<Node>::node_ptr;

			if (!pos)
				return;
			node_ptr to_delete = pos->_next;
			if (!to_delete)
				return;
			node_ptr next = to_delete->_next;
			as_next(pos, next);

			std::destroy_at(to_delete);	//销毁对象
			alloc.deallocate(to_delete, 1);	//回收内存
		}


		

		/*
		* 获取链表尾元素
		*/
		template<forward_node_type Node>
		Node* back(Node* root)
		{
			if (!root)
				return nullptr;
			while (root->_next)
			{
				root = root->_next;
			}
			return root;
		}

		/*
		* 插入到表尾
		*/
		template<forward_node_type Node>
		void push_back(Node* root, Node* to_insert)
		{
			if (!root || !to_insert)
				return;
			back(root)->_next = to_insert;
		}

		template<forward_node_type Node, typename Alloc>
		void push_back(Node* root, const typename node_traits<Node>::value_type& val, Alloc& alloc)
		{
			using node_ptr = Node*;
			node_ptr tmp = alloc.allocate(1);
			{
				tmp->_val = val;
				tmp->_next = nullptr;
			}
			push_back(root, tmp);
		}

		/*
		* 插入到表首
		* 返回root
		*/
		template<forward_node_type Node>
		Node* push_front(Node* root, Node* to_insert)
		{
			if (!root || !to_insert)
				return nullptr;
			to_insert->_next = root;
			return to_insert;
		}


		template<forward_node_type Node, typename Alloc>
		Node* push_front(Node* root, const typename node_traits<Node>::value_type& val, Alloc& alloc)
		{
			using node_ptr = Node*;
			node_ptr tmp = alloc.allocate(1);
			{
				tmp->_val = val;
				tmp->_next = nullptr;
			}
			return push_front(root, tmp);
		}

		/*
		* 删除表首
		* 返回root
		*/

		template<forward_node_type Node, typename Alloc>
		Node* pop_front(Node* root, Alloc& alloc)
		{
			if (!root)
				return nullptr;
			auto next = root->_next;

			std::destroy_at(root);
			alloc.deallocate(root, 1);
			return next;

		}


		//构造普通单向链表
		template<forward_node_type Node, typename Iter, typename Alloc>
		Node* make_basic_forward_list(Iter begin, Iter end, Alloc& alloc)
		{
			using node_ptr = typename node_traits<Node>::node_ptr;
			if (begin == end)
				return nullptr;
			node_ptr head = alloc.allocate(1);
			{
				head->_val = *begin;
				head->_next = nullptr;
			}
			node_ptr tmp = nullptr;
			node_ptr cur = head;
			for (auto p = ++begin; p != end; ++p)
			{
				tmp = alloc.allocate(1);
				{
					tmp->_val = *p;
					tmp->_next = nullptr;
				}
				insert_after(cur, tmp);
				cur = tmp;
			}
			return head;
		}

		//单向链表的节点值皆为val,数量有count个
		template<forward_node_type Node, typename Alloc>
		Node* make_basic_forward_list(size_t count, const typename node_traits<Node>::reference val, Alloc& alloc)
		{
			using node_ptr = Node*;
			node_ptr head = alloc.allocate(1);
			head->_val = val;
			node_ptr cur = head, next = nullptr;
			for (size_t left = --count; left != 0; --left)
			{
				next = alloc.allocate(1);
				next->_val = val;
				cur->_next = next;
				cur = next;
			}
			return head;
		}


		//销毁普通单向链表
		template<forward_node_type Node, typename Alloc>
		void destroy_basic_forward_list(Node* root, Alloc& alloc)
		{
			using node_ptr = typename node_traits<Node>::node_ptr;
			if (!root)
				return;
			node_ptr cur = root;
			node_ptr next{};
			while (cur)
			{
				next = cur->_next;
				std::destroy_at(cur);
				alloc.deallocate(cur, 1);
				cur = next;
			}

		}

		
		template<forward_node_type Node, typename Alloc>
		void erase_sublist_after(Node* pos, Node* back, Alloc& alloc)
		{
			using node_ptr = Node*;
			//提取[begin,back]为子表
			node_ptr next = back->_next;
			node_ptr begin = pos->_next;
			as_next(pos, next);
			back->_next = nullptr;

			destroy_basic_forward_list(begin, alloc);

		}

		template<forward_node_type Node, typename Alloc>
		void erase_sublist_after(Node* pos, size_t count, Alloc& alloc)
		{
			using node_ptr = Node*;
			node_ptr to_delete_head = pos->_next;
			node_ptr to_delete_back = to_delete_head;
			to_delete_back=step(to_delete_back, count);
			erase_sublist_after(pos, to_delete_back, alloc);
		}
	

		//合并链表
		template<forward_node_type Node>
		void merge_forward_list(Node* lhs, Node* rhs)
		{
			Node* _back = back(lhs);
			as_next(_back, rhs);
		}


		//打印链表
		template<forward_node_type Node, typename ostream>
		void print_forward_list(const Node* const head, ostream& os)
		{
			while (head)
			{
				os << *head << " ";
				head = head->_next;
			}
		}

		template<forward_node_type Node, typename Alloc>
		Node* deep_copy_forward_list(Node* rhs_head, Alloc& alloc)
		{
			using node_ptr = Node*;
			//初始化头节点
			node_ptr lhs_head = alloc.allocate(1);
			lhs_head->_val = rhs_head->_val;
			lhs_head->_next = nullptr;
			//遍历
			rhs_head = rhs_head->_next;
			node_ptr cur = lhs_head, next = nullptr;
			while (rhs_head)
			{
				next = alloc.allocate(1);
				next->_val = rhs_head->_val;
				next->_next = nullptr;
				cur->_next = next;
				cur = next;
			}
			return lhs_head;
		}
	}

}