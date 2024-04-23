#pragma once
#ifndef fantasil_splay_tree_node_h
#define fantasil_splay_tree_node_h


#include"binary_search_tree_node.h"

namespace fantasil
{
	//һ����չ���ĵĽڵ���Ƕ����������Ľڵ�
	template<typename K,typename V>
	using splay_tree_node = binary_search_tree_node<K, V>;

	enum class splay_tree_opertor {
		find,insert,erase
	};

	//
	namespace splay_tree_node_helper {
		//--------------------------------------------------------------declare-------------------------------------------------------
		//��չ���Ĳ���
		template<binary_search_tree_node_type Node,typename Cmp>
		Node* find(Node* root, const typename Node::key_type& key,Cmp cmp);
		//��չ���Ĳ���
		template<binary_search_tree_node_type Node,typename Cmp>
		Node* insert(Node* root, Node* node, Cmp cmp);
		//��չ����ɾ��
		template<binary_search_tree_node_type Node,typename Alloc,typename Cmp>
		Node* erase(Node* root, Node* node, Alloc& alloc,Cmp cmp);

		//�ڵ�����������ת��������
		template<binary_search_tree_node_type Node>
		Node* fixup(Node* root, Node* node);

		//������չ��
		template<binary_search_tree_node_type Node,input_iterator_of_pair<typename Node::key_type,typename node_traits<Node>::value_type> Iter,typename Alloc,typename Cmp>
		Node* make_splay_tree(Iter begin, Iter end, Alloc& alloc, Cmp cmp);

		//������չ��
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
					break;	//���ѽڵ�����ҵ��ĵ�ǰ�ڵ�
				}
				else if (cmp(key, cur->_key))
				{	
					//������ǲ���·��������Ľڵ�
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
			return fixup(root,cur);	//������ѽڵ�
		}

		template<binary_search_tree_node_type Node, typename Cmp>
		Node* insert(Node* root, Node* node, Cmp cmp)
		{
			//���÷���
			if (!node)
				return root;
			if (!root)
				return node;
			root = binary_search_tree_node_helper::insert(root, node, cmp);
			//����ڵ�node��Ϊ���ѽڵ�
			return fixup(root, node);
		}

		template<binary_search_tree_node_type Node, typename Alloc,typename Cmp>
		Node* erase(Node* root, Node* node, Alloc& alloc,Cmp cmp)
		{
			using node_ptr = Node*;
			node_ptr del = binary_search_tree_node_helper::locate_position_to_delete(root,node);	//ʵ�ʵ�ɾ���ڵ�
			node_ptr splay_node = del->_parent;	//ɾ���ڵ�ĸ��ڵ㼴Ϊ���ѽڵ�
			root = binary_search_tree_node_helper::erase_impl(root, node, del, alloc);
			return fixup(root, splay_node);
		}


		template<binary_search_tree_node_type Node>
		Node* fixup(Node* root, Node* node)
		{
			using node_ptr = Node*;
			if (!node)
				return root;//���ɾ���ڵ�û�и��ڵ㣬��ɾ�����Ǹ��ڵ㣬��ô���ֲ���
			if (root == node)
				return root;
			node_ptr gp = nullptr;
			node_ptr p = nullptr;
			node_ptr c = nullptr;
			while (node!=root&&node->_parent != root)
			{
				//��node���Ǹ��ڵ���߲����ڵڶ���ʱ������˫����ת
				c = node;
				p = c->_parent;
				gp = p->_parent;
				if (binary_tree_node_helper::is_left(p))
				{
					if (binary_tree_node_helper::is_left(node))
					{
						//LL�͵�˫����ת
						binary_tree_node_helper::turn_right(p);
						binary_tree_node_helper::turn_right(c);
					}
					else {
						//LR�͵�˫����ת
						binary_tree_node_helper::turn_left(c);
						binary_tree_node_helper::turn_right(c);
					}
				}
				else {
					if (binary_tree_node_helper::is_right(node))
					{
						//RR�͵�˫����ת
						binary_tree_node_helper::turn_left(p);
						binary_tree_node_helper::turn_left(c);
					}
					else {
						//RL�͵�˫����ת
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
			
			//��ʱִ�е�����ת
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
