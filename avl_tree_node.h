#pragma once

#ifndef fantasil_avl_tree_node_h
#define fantasil_avl_tree_node_h

#include"binary_search_tree_node.h"
namespace fantasil {

	//一个avl节点就是二叉搜索树节点
	template<typename K, typename V>
	using avl_tree_node = binary_search_tree_node<K, V>;


	namespace avl_tree_node_helper {

		using namespace binary_search_tree_node_helper;
		//--------------------------------------------------------------------------declare

		//平衡因子
		template<binary_search_tree_node_type Node>
		int balance_factor(Node* node);
		//查找关键字为key的节点
		template<binary_search_tree_node_type Node,typename Cmp>
		Node* find(Node* root, const typename Node::key_type& key,Cmp cmp);
		//不平衡时候的修补函数，使其保持平衡
		template<binary_search_tree_node_type Node>
		Node* fixup(Node* root, Node* node);
		//插入一个节点
		template<binary_search_tree_node_type Node,typename Cmp>
		Node* insert(Node* root, Node* node,Cmp cmp);
		//删除节点的内部实现
		template<binary_search_tree_node_type Node,typename Alloc>
		Node* erase_impl(Node* root,Node* node,Node* del,Alloc& alloc);
		//删除一个节点
		template<binary_search_tree_node_type Node,typename Alloc>
		Node* erase(Node* root, Node* node, Alloc& alloc);
		//构造avl平衡树
		template<binary_search_tree_node_type Node,input_iterator_of_pair<typename Node::key_type,typename node_traits<Node>::value_type> Iter,typename Alloc,typename Cmp>
		Node* make_avl_tree(Iter begin, Iter end, Alloc& alloc, Cmp cmp);
		//销毁avl平衡树
		template<binary_search_tree_node_type Node,typename Alloc>
		void destory_avl_tree(Node* root, Alloc& alloc);
		
		//---------------------------------------------------------------------------impl

		template<binary_search_tree_node_type Node>
		int balance_factor(Node* node)
		{
			using namespace fantasil::binary_tree_node_helper;
			return static_cast<int>(height(node->_left)) - static_cast<int>(height(node->_right));
		}

		template<binary_search_tree_node_type Node,typename Cmp>
		Node* find(Node* root, const typename Node::key_type& key,Cmp cmp)
		{
			return binary_search_tree_node_helper::find(root, key, cmp);
		}

		template<binary_search_tree_node_type Node>
		Node* fixup(Node* root, Node* node)
		{
			using namespace fantasil::binary_tree_node_helper;
			using node_ptr = Node*;
			if (!root)
				return nullptr;
			node_ptr p = nullptr;
			node_ptr c = nullptr;
			node_ptr tmp = nullptr;
			while (node)
			{
				if (balance_factor(node) == 2)
				{
					p = node->_left;
					if (balance_factor(p) == -1)
					{
						c = p->_right;
						turn_left(c);
						tmp = c;
						c = p;
						p = tmp;
						tmp = nullptr;
					}
					turn_right(p);
					if (node == root)
					{
						root = p;
						return root;
					}
					node = p;
				}
				else if (balance_factor(node) == -2)
				{
					p = node->_right;
					if (balance_factor(p) == 1)
					{
						c = p->_left;
						turn_right(c);
						tmp = c;
						c = p;
						p = tmp;
						tmp = nullptr;
					}
					turn_left(p);
					if (node == root)
					{
						root = p;
						return root;
					}
					node = p;
				}
				node = node->_parent;
			}
			return root;
		}

		template<binary_search_tree_node_type Node, typename Cmp>
		Node* insert(Node* root, Node* node,Cmp cmp)
		{
			
			using namespace fantasil::binary_search_tree_node_helper;
			using node_ptr = Node*;
			if (!node)
				return nullptr;
			if (!root)
				return node;
			node_ptr parent = locate_position_to_insert(root, node, cmp);
			if (cmp(node->_key, parent->_key))
				binary_tree_node_helper::as_left(parent, node);
			else
				binary_tree_node_helper::as_right(parent, node);
			return fixup(root, parent);
		}

		template<binary_search_tree_node_type Node, typename Alloc>
		Node* erase_impl(Node* root, Node* node, Node* del, Alloc& alloc)
		{
			using node_ptr = Node*;
			using namespace binary_tree_node_helper;
			static_assert(std::is_same_v<Node, typename std::allocator_traits<Alloc>::value_type>);
			node_ptr p = del->_parent;
			if (del != node)
			{
				node->_key = del->_key;
				node->_val = del->_val;
			}
			node_ptr child = del->_left ? del->_left : del->_right;
			if (del == root)
				root=child;
			else {
				if (is_left(del))
					as_left(p, child);
				else
					as_right(p, child);
			}
			std::destroy_at(del);
			alloc.deallocate(del, 1);
			return fixup(root, p);
		}

		template<binary_search_tree_node_type Node,typename Alloc>
		Node* erase(Node* root, Node* node,Alloc& alloc)
		{
			using node_ptr = Node*;
			node_ptr del = binary_search_tree_node_helper::locate_postion_to_delete(root, node);
			return erase_impl(root, node, del, alloc);
		}

		template<binary_search_tree_node_type Node, input_iterator_of_pair<typename Node::key_type, typename node_traits<Node>::value_type> Iter, typename Alloc, typename Cmp>
		Node* make_avl_tree(Iter begin, Iter end, Alloc& alloc, Cmp cmp)
		{
			using node_ptr = Node*;
			static_assert(std::is_same_v<binary_search_tree_node_type, std::allocator_traits<Alloc>::value_type>);
			if (begin == end)
				return nullptr;
			node_ptr root = alloc.allocate(1);
			init_binary_search_tree_node_type(root, begin->first, begin->second);
			node_ptr tmp = nullptr;
			for (auto p = ++begin; p != end; ++p)
			{
				tmp = alloc.allocate(1);
				init_binary_search_tree_node_type(tmp, p->first, p->second);
				root = insert(root, tmp, cmp);
			}
			return root;
		}

		template<binary_search_tree_node_type Node, typename Alloc>
		void destory_avl_tree(Node* root, Alloc& alloc)
		{
			if (root)
			{
				destory_avl_tree(root->_left, alloc);
				destory_avl_tree(root->_right, alloc);
				std::destroy_at(root);
				alloc.deallocate(root, 1);
			}
		}

		

	}

}

#endif