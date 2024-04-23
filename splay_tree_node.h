#pragma once
#ifndef fantasil_splay_tree_node_h
#define fantasil_splay_tree_node_h


#include"binary_search_tree_node.h"

namespace fantasil
{
	//一个伸展树的的节点就是二叉搜索树的节点
	template<typename K,typename V>
	using splay_tree_node = binary_search_tree_node<K, V>;

	enum class splay_tree_opertor {
		find,insert,erase
	};

	//
	namespace splay_tree_node_helper {
		//--------------------------------------------------------------declare-------------------------------------------------------
		//伸展树的查找
		template<binary_search_tree_node_type Node,typename Cmp>
		Node* find(Node* root, const typename Node::key_type& key,Cmp cmp);
		//伸展树的插入
		template<binary_search_tree_node_type Node,typename Cmp>
		Node* insert(Node* root, Node* node, Cmp cmp);
		//伸展树的删除
		template<binary_search_tree_node_type Node,typename Alloc,typename Cmp>
		Node* erase(Node* root, Node* node, Alloc& alloc,Cmp cmp);

		//节点操作过后的旋转上升操作
		template<binary_search_tree_node_type Node>
		Node* fixup(Node* root, Node* node);

		//构造伸展树
		template<binary_search_tree_node_type Node,input_iterator_of_pair<typename Node::key_type,typename node_traits<Node>::value_type> Iter,typename Alloc,typename Cmp>
		Node* make_splay_tree(Iter begin, Iter end, Alloc& alloc, Cmp cmp);

		//销毁伸展树
		template<binary_search_tree_node_type Node,typename Alloc>
		void destory_splay_tree(Node* root, Alloc& alloc);

		//--------------------------------------------------------------impl-------------------------------------------------------

		template<binary_search_tree_node_type Node,typename Cmp>
		Node* find(Node* root, const typename Node::key_type& key,Cmp cmp)
		{
			using node_ptr = Node*;
			if (!root)
				return nullptr;
			node_ptr cur = root;
			while (cur)
			{
				if (key == cur->_key)
				{
					break;	//分裂节点就是找到的当前节点
				}
				else if (cmp(key, cur->_key))
				{	
					//否则就是查找路径中最深的节点
					if (cur->_left)
						cur = cur->_left;
					else
						break;
				}
				else {
					if (cur->_right)
						cur = cur->_right;
					else 
						break;
				}
			}
			return fixup(root,cur);	//处理分裂节点
		}

		template<binary_search_tree_node_type Node, typename Cmp>
		Node* insert(Node* root, Node* node, Cmp cmp)
		{
			//不用分裂
			if (!node)
				return root;
			if (!root)
				return node;
			root = binary_search_tree_node_helper::insert(root, node, cmp);
			//插入节点node即为分裂节点
			return fixup(root, node);
		}

		template<binary_search_tree_node_type Node, typename Alloc,typename Cmp>
		Node* erase(Node* root, Node* node, Alloc& alloc,Cmp cmp)
		{
			using node_ptr = Node*;
			node_ptr del = binary_search_tree_node_helper::locate_position_to_delete(root,node);	//实际的删除节点
			node_ptr splay_node = del->_parent;	//删除节点的父节点即为分裂节点
			root = binary_search_tree_node_helper::erase_impl(root, node, del, alloc);
			return fixup(root, splay_node);
		}


		template<binary_search_tree_node_type Node>
		Node* fixup(Node* root, Node* node)
		{
			using node_ptr = Node*;
			if (!node)
				return root;//如果删除节点没有父节点，即删除的是根节点，那么保持不变
			if (root == node)
				return root;
			node_ptr gp = nullptr;
			node_ptr p = nullptr;
			node_ptr c = nullptr;
			while (node!=root&&node->_parent != root)
			{
				//当node不是根节点或者不处于第二层时，进行双层旋转
				c = node;
				p = c->_parent;
				gp = p->_parent;
				if (binary_tree_node_helper::is_left(p))
				{
					if (binary_tree_node_helper::is_left(node))
					{
						//LL型的双层旋转
						binary_tree_node_helper::turn_right(p);
						binary_tree_node_helper::turn_right(c);
					}
					else {
						//LR型的双层旋转
						binary_tree_node_helper::turn_left(c);
						binary_tree_node_helper::turn_right(c);
					}
				}
				else {
					if (binary_tree_node_helper::is_right(node))
					{
						//RR型的双层旋转
						binary_tree_node_helper::turn_left(p);
						binary_tree_node_helper::turn_left(c);
					}
					else {
						//RL型的双层旋转
						binary_tree_node_helper::turn_right(c);
						binary_tree_node_helper::turn_left(c);
					}
				}
				if (gp == root)
				{
					root = node;
					return root;
				}
			}
			
			//此时执行单层旋转
			if (binary_tree_node_helper::is_left(node))
				binary_tree_node_helper::turn_right(node);
			else
				binary_tree_node_helper::turn_left(node);
			root = node;
			return root;
		}
		template<binary_search_tree_node_type Node, input_iterator_of_pair<typename Node::key_type, typename node_traits<Node>::value_type> Iter, typename Alloc, typename Cmp>
		Node* make_splay_tree(Iter begin, Iter end, Alloc& alloc, Cmp cmp)
		{
			using node_ptr = Node*;
			static_assert(std::is_same_v<Node, std::allocator_traits<Alloc>::value_type>);
			if (begin == end)
				return nullptr;
			node_ptr root = alloc.allocate(1);
			init_binary_search_tree_node_type(root, begin->first, begin->second);
			node_ptr tmp = nullptr;
			for (auto p = ++begin; p != end; ++p)
			{
				tmp = alloc.allocate(1);
				init_binary_search_tree_node_type(tmp, p->first, p->second);
				root=insert(root, tmp, cmp);
			}
			return root;
		}
		template<binary_search_tree_node_type Node, typename Alloc>
		void destory_splay_tree(Node* root, Alloc& alloc)
		{
			if (root)
			{
				destory_splay_tree(root->_left, alloc);
				destory_splay_tree(root->_right, alloc);
				std::destroy_at(root);
				alloc.deallocate(root, 1);
				
			}
		}
	}
}

#endif
