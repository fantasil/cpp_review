#pragma once
#ifndef fantasil_binary_search_tree_node
#define fantasil_binary_search_tree_node
#include"binary_tree_node.h"

namespace fantasil {

	template<typename K, typename V>
	struct binary_search_tree_node {
		using key_type = K;
		using value_type = V;
		using reference = V&;
		using pointer = V*;
		using node_type = binary_search_tree_node<K, V>;
		using node_category = binary_search_tree_node_tag;
		using node_ptr = node_type*;

		key_type _key;
		value_type _val;
		node_ptr _parent;
		node_ptr _left;
		node_ptr _right;
	};

	template<typename K, typename V, typename ostream>
	ostream& operator<<(ostream& os, const binary_search_tree_node<K, V>& node)
	{
		os << "{" << node._key << "," << node._val << "} ";
		return os;
	}

	/*
	* binary_search_treee_node�洢����һ����ֵ�ԣ���binary_tree_node�洢����ֵ��
	* ���漰cmp������ʱ����������һЩ���֣����漰cmp�ĺ�������ͨ�õ�
	*/

	namespace binary_search_tree_node_helper
	{
		/*---------------------------------------------------------------------------declare----------------------------------------------------------------------*/
		//��λ�ڵ�Ĳ���λ��
		template<binary_search_tree_node_type Node, typename Cmp>
		Node* locate_position_to_insert(Node* root, Node* node, Cmp cmp);
		//�����������Ĳ���
		template<binary_search_tree_node_type Node, typename Cmp>
		Node* insert(Node* root, Node* node, Cmp cmp);
		//����
		template<binary_search_tree_node_type Node,typename Cmp>
		Node* find(Node* root, const typename node_traits<Node>::key_type& key,Cmp cmp);
		//��λɾ���ڵ��λ��
		template<binary_search_tree_node_type Node>
		Node* locate_position_to_delete(Node* root, Node* node);
		//ɾ��ָ���ڵ���ڲ�ʵ��
		template<binary_search_tree_node_type Node, typename Alloc>
		Node* erase_impl(Node* root, Node* node, Node* del, Alloc& alloc);
		//������������ɾ��
		template<binary_search_tree_node_type Node,typename Alloc>
		Node* erase(Node* root, Node* del, Alloc& alloc);
		//���ɶ���������
		template<binary_search_tree_node_type Node,input_iterator_of_pair<typename node_traits<Node>::key_type,typename node_traits<Node>::value_type> Iter,typename Alloc,typename Cmp>
		Node* make_binary_search_tree(const Iter begin, const Iter end, Alloc& alloc, Cmp cmp);
		//���ٶ���������
		template<binary_search_tree_node_type Node,typename Alloc>
		void destory_bianry_search_tree(Node* root,Alloc& alloc);
		/*---------------------------------------------------------------------------impl----------------------------------------------------------------------*/
		template<binary_search_tree_node_type Node, typename Cmp>
		Node* locate_position_to_insert(Node* root, Node* node, Cmp cmp)
		{
			using node_ptr = typename node_traits<Node>::node_ptr;
			if (!root)
				return nullptr;
			//�Ӹ��ڵ㿪ʼѰ������������Ҷ�ӽڵ�
			node_ptr cur = root;

			while (cur)
			{
				if (cmp(node->_key, cur->_key))
				{
					if (cur->_left)
						cur = cur->_left;
					else
						break;
				}
				else
				{
					if (cur->_right)
						cur = cur->_right;
					else
						break;
				}
			}
			return cur;
		}

		template<binary_search_tree_node_type Node, typename Cmp>
		Node* insert(Node* root, Node* node, Cmp cmp)
		{
			using node_ptr = typename node_traits<Node>::node_ptr;
			node_ptr parent = locate_position_to_insert(root, node, cmp);
			if (!parent)
				return node;
			if (cmp(node->_key, parent->_key))
				binary_tree_node_helper::as_left(parent, node);
			else
				binary_tree_node_helper::as_right(parent, node);
			return root;
		}

		template<binary_search_tree_node_type Node, typename Cmp>
		Node* find(Node* root, const typename Node::key_type& key,Cmp cmp)
		{
			using node_ptr = Node*;
			if (!root)
				return nullptr;
			node_ptr cur = root;
			while (cur)
			{
				if (key == cur->_key)
					return cur;
				else if (cmp(key, cur->_key))
					cur = cur->_left;
				else
					cur = cur->_right;
			}
			return cur;
		}

		template<binary_search_tree_node_type Node>
		Node* locate_position_to_delete(Node* root, Node* node)
		{
			return binary_tree_node_helper::binary_tree_location_real_erase_node(node);
		}

		template<binary_search_tree_node_type Node, typename Alloc>
		Node* erase_impl(Node* root, Node* node, Node* del, Alloc& alloc)
		{
			using namespace binary_tree_node_helper;
			using node_ptr = Node*;
			static_assert(std::is_same_v<Node, typename std::allocator_traits<Alloc>::value_type>);
			//��ʱnode��һ��Ҷ�ӽڵ���߽���һ�����ӽڵ�

			//��ͬ�Ļ��ͽ����ڵ��ֵ��
			if (del != node)
			{
				node->_key = del->_key;
				node->_val = del->_val;
			}
			//child��node�ĺ��ӽڵ�
			node_ptr child = del->_left ? del->_left : del->_right;
			if (del == root)
			{
				root = child;
				if(root)
					root->_parent = nullptr;
			}
			else {
				if (is_left(del))
					as_left<Node>(parent<Node>(del), child);
				else
					as_right<Node>(parent<Node>(del), child);
			}


			std::destroy_at(del);
			alloc.deallocate(del, 1);
			return root;
		}

		template<binary_search_tree_node_type Node, typename Alloc>
		Node* erase(Node* root, Node* del, Alloc& alloc)
		{
			using node_ptr = Node*;
			node_ptr real_del = locate_position_to_delete(root, del);
			return erase_impl(root, del, real_del, alloc);
		}

		template<binary_search_tree_node_type Node, input_iterator_of_pair<typename node_traits<Node>::key_type, typename node_traits<Node>::value_type> Iter, typename Alloc,typename Cmp>
		Node* make_binary_search_tree(Iter begin, Iter end, Alloc& alloc,Cmp cmp)
		{
			using node_ptr = Node*;
			if(begin==end)
				return nullptr;
			node_ptr root = alloc.allocate(1);
			init_binary_search_tree_node_type(root, (*begin).first, (*begin).second);
			node_ptr tmp = nullptr;
			for (auto p = begin; p != end; ++p)
			{
				tmp = alloc.allocate(1);
				init_binary_search_tree_node_type(root, (*p).first, (*p).second);
				root = insert(root,tmp, cmp);
			}
			return root;
		}

		template<binary_search_tree_node_type Node,typename Alloc>
		void destory_bianry_search_tree(Node* root,Alloc& alloc)
		{
			if (root) {
				destory_bianry_search_tree(root->_left,alloc);
				destory_bianry_search_tree(root->_right,alloc);
				std::destroy_at(root);
				alloc.deallocate(root, 1);
			}
		}



	

	}


}


#endif