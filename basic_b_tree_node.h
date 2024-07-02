#pragma once
#ifndef fantasil_basic_b_tree_node_h
#define fantasil_basic_b_tree_node_h
#include"headers.h"

namespace fantasil
{
	/*
	* һ��B���ڵ㣬����Degree�Ǵ������½�,��Ϊt
	* �ڵ�����Ԫ��������2t-1;��С��Ԫ��������t-1
	* ��Ӧ�Ľڵ����ĺ��ӿ�����2t��������t
	* ������������ڵ���������Ҫ����
	* ɾ����������ڵ�Ԫ��Ϊt-1����Ҫ��ȡ���ߺϲ�
	*/



	template<typename K,typename V,size_t Degree>
	struct basic_b_tree_node {
		using node_type = basic_b_tree_node<K, V, Degree>;
		using node_ptr = node_type*;

		basic_b_tree_node(node_ptr parent = nullptr)
			:_elem(max_elem_num),_children(max_cld_num),_parent{parent}
		{
			_elem.resize(0);
			_children.resize(0);
		}

		constexpr static size_t max_elem_num = 2 * Degree - 1;	//���������ɵ�Ԫ������
		constexpr static size_t max_cld_num = 2 * Degree;	//�����Խ������ӽڵ�����
		constexpr static size_t min_elem_num = Degree - 1;
		constexpr static size_t min_cld_num = Degree;
		
		node_ptr _parent;
		std::vector<std::pair<K, V>> _elem;			//����Ԫ��
		std::vector<node_ptr> _children;			//�����ӽڵ�
	};



	namespace basic_b_tree_node_helper {

		template<typename K,typename V,size_t Degree>
		struct category {
			using node_type = basic_b_tree_node<K, V, Degree>;
			using node_ptr = node_type*;
			using elem_iterator = typename decltype(node_type{}._elem.begin());
			using child_iterator = typename decltype(node_type{}._children.begin());
			using elem_type = typename std::iterator_traits<elem_iterator>::value_type;
			using child_type = typename std::iterator_traits<child_iterator>::value_type;
		};

		//һ���򵥵����ݽṹ�����ڴ��ĳ���ڵ������ھ�
		template<typename K,typename V,size_t Degree>
		struct neighbor {
			using node_type = basic_b_tree_node<K, V, Degree>;
			using node_ptr = node_type*;
			node_ptr _left;
			node_ptr _right;
			neighbor(node_ptr left = nullptr, node_ptr right = nullptr) :_left{ left }, _right{ right } {}
			~neighbor() {}
		};

		//һ���򵥵����ݽṹ�����ڴ��ĳ��Ԫ�ص���������
		template<typename K,typename V,size_t Degree>
		struct subtrees {
			using iterator = typename category<K, V, Degree>::child_iterator;
			iterator _left;
			iterator _right;
			subtrees(iterator left = nullptr, iterator right = nullptr) :_left{ left }, _right{ right } {}
			~subtrees() {}
		};

		template<typename K,typename V,size_t Degree>
		struct parent_elems {
			using node_type = basic_b_tree_node<K, V, Degree>;
			using node_ptr = node_type*;
			using iterator = typename decltype(node_type{}._elem.begin());
			iterator _left;
			iterator _right;
			parent_elems(iterator left, iterator right) :_left{ left }, _right{ right } {}
			~parent_elems() {}
		};
		
		/*---------------------------------------------------------------------declare---------------------------------------------------------------*/
		/*---------------��Ե����ڵ��ڵĲ���------------------------*/
		template<typename K, typename V, size_t Degree,typename ostream>
		void level_order(basic_b_tree_node<K, V, Degree>* root, ostream& os);

		//Ԫ���ڸýڵ�����
		template<typename K, typename V, size_t Degree>
		inline bool elem_in_node(basic_b_tree_node < K, V, Degree>* node, const K& key);
		//����һ���ڵ�
		template<typename K, typename V, size_t Degree>
		inline basic_b_tree_node<K, V, Degree>* allocate_node();
		//ɾ��һ���ڵ�
		template<typename K, typename V, size_t Degree>
		inline void destroy_node(basic_b_tree_node<K, V, Degree>* node);
		//�ڵ��ڵ�Ԫ��������
		template<typename K, typename V, size_t Degree>
		inline bool elem_full(basic_b_tree_node<K, V, Degree>* node);
		//�Ƿ���Ҷ�ӽڵ�
		template<typename K, typename V, size_t Degree>
		inline bool is_leaf(basic_b_tree_node<K, V, Degree>* node);
		//��ȡԪ�ص���������Ĭ��Ԫ���ڽڵ���
		/*template<typename K, typename V, size_t Degree>
		inline typename category<K, V, Degree>::child_iterator subtree_left(basic_b_tree_node<K, V, Degree>* node, const K& key);*/
		template<typename K, typename V, size_t Degree>
		inline typename category<K, V, Degree>::child_iterator subtree_left(basic_b_tree_node<K, V, Degree>* node, typename category<K,V,Degree>::elem_iterator it);
		////��ȡԪ�ص���������Ĭ��Ԫ���ڽڵ���
		//template<typename K, typename V, size_t Degree>
		//inline typename category<K, V, Degree>::child_iterator subtree_right(basic_b_tree_node<K, V, Degree>* node, const K& key);
		template<typename K, typename V, size_t Degree>
		inline typename category<K, V, Degree>::child_iterator subtree_right(basic_b_tree_node<K, V, Degree>* node, typename category<K, V, Degree>::elem_iterator it);
		//��ȡ��ǰ�ڵ�ĸ�Ԫ�أ��Ҹýڵ��Ǹ�Ԫ�ص�����
		template<typename K, typename V, size_t Degree>
		inline typename parent_elems<K,V,Degree>::iterator parent_elem_left(basic_b_tree_node<K, V, Degree>* node);
		//��ȡ��ǰ�ڵ�ĸ�Ԫ�أ��Ҹýڵ��Ǹ�Ԫ�ص��Һ���
		template<typename K, typename V, size_t Degree>
		inline typename parent_elems<K,V,Degree>::iterator parent_elem_right(basic_b_tree_node<K, V, Degree>* node);
		//��ȡ��ǰ�ڵ��������Ԫ�أ��ڵ�λ��������Ԫ��֮��
		template<typename K, typename V, size_t Degree>
		inline parent_elems<K, V, Degree> get_parent_elems(basic_b_tree_node<K, V, Degree>* node);
		////��ȡһ��Ԫ�ص���������
		//template<typename K, typename V, size_t Degree>
		//subtrees<K, V, Degree> get_subtrees(basic_b_tree_node<K, V, Degree>* node, const K& key);
		template<typename K, typename V, size_t Degree>
		subtrees<K, V, Degree> get_subtrees(basic_b_tree_node<K, V, Degree>* node, typename category<K, V, Degree>::elem_iterator it);
		//��ȡ��ǰ�ڵ��ڸ��ڵ��е�λ��
		template<typename K, typename V, size_t Degree>
		inline typename category<K, V, Degree>::child_iterator node_position_in_parent(basic_b_tree_node<K, V, Degree>*);
		//��ȡelem�ڽڵ�Ԫ���е�λ��
		template<typename K, typename V, size_t Degree>
		inline typename category<K, V, Degree>::elem_iterator elem_position_in_node(basic_b_tree_node<K, V, Degree>* node, const K& key);
		//�м�Ԫ��
		template<typename K, typename V, size_t Degree>
		typename category<K, V, Degree>::elem_iterator elem_mid_in_node(basic_b_tree_node<K, V, Degree>* node);
		//
		/*---------------------B���Ĳ���---------------------------*/
		//��ȡ����
		template<typename K, typename V, size_t Degree>
		inline basic_b_tree_node<K, V, Degree>* neighbor_left(basic_b_tree_node<K, V, Degree>* node);
		//��ȡ����
		template<typename K, typename V, size_t Degree>
		inline basic_b_tree_node<K, V, Degree>* neighbor_right(basic_b_tree_node<K, V, Degree>* node);
		//��ȡ��������
		template<typename K, typename V, size_t Degree>
		inline neighbor<K, V, Degree> get_neighbor(basic_b_tree_node<K, V, Degree>* node);

		//���ھӽڵ�ϲ�
		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* merge(basic_b_tree_node<K, V, Degree>*& root, basic_b_tree_node<K, V, Degree>* node);
		//�����ںϲ�
		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* merge_with_neighbor_left(basic_b_tree_node<K, V, Degree>*& root, basic_b_tree_node<K, V, Degree>* node);
		//������ϲ�
		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* merge_with_neighbor_right(basic_b_tree_node<K, V, Degree>*& root, basic_b_tree_node<K, V, Degree>* node);

		//���ھӽ�Ԫ��
		template<typename K, typename V, size_t Degree>
		void borrow_from_neightbor(basic_b_tree_node<K, V, Degree>* node);
		//�����ڽ�Ԫ��
		template<typename K, typename V, size_t Degree>
		void borrow_from_neightbor_left(basic_b_tree_node<K, V, Degree>* node);
		//�������Ԫ��
		template<typename K, typename V, size_t Degree>
		void borrow_from_neightbor_right(basic_b_tree_node<K, V, Degree>* node);
		//�����ܽ�Ԫ����
		template<typename K, typename V, size_t Degree>
		bool could_borrow_from_neightbor_left(basic_b_tree_node<K, V, Degree>* node);
		//�����ܽ�Ԫ����
		template<typename K, typename V, size_t Degree>
		bool could_borrow_from_neightbor_right(basic_b_tree_node<K, V, Degree>* node);
		//�ھ��ܽ�Ԫ����
		template<typename K, typename V, size_t Degree>
		bool could_borrow_from_neightbor(basic_b_tree_node<K, V, Degree>* node);

		//ɾ�����ݵ�ʱ��ýڵ���Ҫ��ȡԪ�ػ��ߺϲ���
		template<typename K, typename V, size_t Degree>
		bool need_borrow_or_merge(basic_b_tree_node<K, V, Degree>* node);
		//ɾ������ʱ��Ľ�ȡԪ�ػ��ߺϲ�
		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K,V,Degree>* borrow_or_merge(basic_b_tree_node<K,V,Degree>*& root,basic_b_tree_node<K, V, Degree>* node);

		//�������ݵ�ʱ��ýڵ���Ҫ������
		template<typename K, typename V, size_t Degree>
		bool need_split(basic_b_tree_node<K, V, Degree>* node);
		//��������ʱ��ķ���
		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* split(basic_b_tree_node<K, V, Degree>*& root, basic_b_tree_node<K, V, Degree>* node);
		//����һ��Ԫ��
		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* insert(basic_b_tree_node<K, V, Degree>* root, const K& key, const V& val);
		
		//�������ڵ�ǰ��Ԫ�����ڵĽڵ�
		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* precursor_node_from_subtree_left(basic_b_tree_node<K, V, Degree>* node, const K& key);
		//�������ڵ�ǰ��Ԫ��
		template<typename K, typename V, size_t Degree>
		typename category<K,V,Degree>::elem_iterator precursor_elem_from_subtree_left(basic_b_tree_node<K, V, Degree>* node, const K& key);
		//�������ڵĺ��Ԫ�����ڵĽڵ�
		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* successor_node_from_subtree_right(basic_b_tree_node<K, V, Degree>* node, const K& key);
		//�������ڵĺ��Ԫ��
		template<typename K, typename V, size_t Degree>
		typename category<K,V,Degree>::elem_iterator successor_elem_from_subtree_right(basic_b_tree_node<K, V, Degree>* node, const K& key);
		
		//��key���ڽڵ��ڣ������ݵ����ʵ��½ڵ�
		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* next_level_node(basic_b_tree_node<K, V, Degree>* node, const K& key);

		//��λԪ��elem���ڵĽڵ�
		template<typename K,typename V,size_t Degree>
		basic_b_tree_node<K, V, Degree>* locate(basic_b_tree_node<K, V, Degree>* node, const K& key);

		//ɾ��һ��Ԫ��
		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* erase(basic_b_tree_node<K, V, Degree>* root, const K& key);

		//
		template<typename K, typename V, size_t Degree>
		V& find(basic_b_tree_node<K, V, Degree>* node, const K& key);
		//��һ��δ���Ľڵ��в���Ԫ��
		template<typename K, typename V, size_t Degree>
		void elem_into_unfull_node(basic_b_tree_node<K, V, Degree>* node, const std::pair<K, V>& elem, basic_b_tree_node<K, V, Degree>* elem_right_subtree = nullptr);
		////����Ԫ��(����)
		//template<typename K,typename V,size_t Degree>
		//basic_b_tree_node<K,V,Degree>* insert(basic_b_tree_node<K, V, Degree>* root, const std::pair<K, V>& elem);


		/*----------------------------------------------------impl-------------------------------------------------------*/
		
		//����һ���ڵ�
		template<typename K,typename V,size_t Degree>
		inline basic_b_tree_node<K, V, Degree>* allocate_node()
		{
			return new basic_b_tree_node<K, V, Degree>{};
		}
	
		//ɾ��һ���ڵ�
		template<typename K,typename V,size_t Degree>
		inline void destroy_node(basic_b_tree_node<K,V,Degree>* node)
		{
			delete node;
		}
		//�ڵ�Ԫ��������
		template<typename K,typename V,size_t Degree>
		inline bool elem_full(basic_b_tree_node<K, V, Degree>* node)
		{
			using node_type = basic_b_tree_node<K, V, Degree>;
			return node_type::max_elem_num == node->_elem.size();
		}
		//�Ƿ���Ҷ�ӽڵ�
		template<typename K,typename V,size_t Degree>
		inline bool is_leaf(basic_b_tree_node<K, V, Degree>* node)
		{
			return node->_children.empty();
		}

		/*template<typename K, typename V, size_t Degree>
		typename category<K,V,Degree>::elem_iterator subtree_left(basic_b_tree_node<K, V, Degree>* node, const K& key)
		{
			subtrees<K, V, Degree> subtrees = get_subtrees(node, key);
			return subtrees._left;
		}*/

		template<typename K, typename V, size_t Degree>
		inline typename category<K, V, Degree>::child_iterator subtree_left(basic_b_tree_node<K, V, Degree>* node, typename category<K, V, Degree>::elem_iterator it)
		{
			subtrees<K, V, Degree> sbs = get_subtrees(node, it);
			return sbs._left;
		}

		/*template<typename K, typename V, size_t Degree>
		typename category<K, V, Degree>::elem_iterator subtree_right(basic_b_tree_node<K, V, Degree>* node, const K& key)
		{
			subtrees<K, V, Degree> subtrees = get_subtrees(node, key);
			return subtrees._right;
		}*/

		template<typename K, typename V, size_t Degree>
		inline typename category<K, V, Degree>::child_iterator subtree_right(basic_b_tree_node<K, V, Degree>* node, typename category<K, V, Degree>::elem_iterator it)
		{
			subtrees<K, V, Degree> sbs = get_subtrees(node, it);
			return sbs._right;
		}

		template<typename K, typename V, size_t Degree>
		typename parent_elems<K, V, Degree>::iterator parent_elem_left(basic_b_tree_node<K, V, Degree>* node)
		{
			auto elems = get_parent_elems(node);
			return elems._left;
		}

		template<typename K, typename V, size_t Degree>
		typename parent_elems<K, V, Degree>::iterator parent_elem_right(basic_b_tree_node<K, V, Degree>* node)
		{
			auto elems = get_parent_elems(node);
			return elems._right;
		}


		template<typename K, typename V, size_t Degree>
		parent_elems<K, V, Degree> get_parent_elems(basic_b_tree_node<K, V, Degree>* node)
		{
			using node_type = basic_b_tree_node<K, V, Degree>;
			using node_ptr = node_type*;
			using iterator = typename parent_elems<K, V, Degree>::iterator;
			node_ptr parent = node->_parent;
			if (!parent)
				throw std::exception{ "root has no parent elems" };
			auto p = parent->_children.begin();
			for (; p != parent->_children.end(); ++p)
			{
				if (node == *p)
					break;
			}
			auto distance = p - parent->_children.begin();
			iterator left = distance ? parent->_elem.begin() + distance - 1 : iterator{};
			iterator right = (p == parent->_children.end()) ? iterator{} : parent->_elem.begin() + distance;
			return parent_elems<K, V, Degree>{left, right};
		}



		template<typename K, typename V, size_t Degree,typename ostream>
		inline void level_order(basic_b_tree_node<K, V, Degree>* root,ostream& os)
		{
			using node_type = basic_b_tree_node<K, V, Degree>;
			using node_ptr = node_type*;
			std::queue<node_ptr> q{};
			size_t level{ 0 };
			q.push(root);
			node_ptr tmp = nullptr;
			while (!q.empty())
			{
				os << "level " << level << ":\n";
				for (size_t sz = q.size(); sz != 0; --sz)
				{
					tmp = q.front();
					q.pop();
					os << "{ ";
					for (auto p = tmp->_elem.begin(); p != tmp->_elem.end(); ++p)
					{
						os << p->first << ",";
					}
					os<< "}   ";

					for (auto p = tmp->_children.begin(); p != tmp->_children.end(); ++p)
					{
						if(*p)
							q.push(*p);
					}
					
				}
				++level;
				os << "\n";
				
			}
		}

		template<typename K, typename V, size_t Degree>
		bool elem_in_node(basic_b_tree_node<K, V, Degree>* node, const K& key)
		{
			auto p = elem_position_in_node(node, key);
			if (p == node->_elem.end())
				return false;
			else
				return true;
		}

		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* neighbor_left(basic_b_tree_node<K, V, Degree>* node)
		{
			neighbor<K, V, Degree> nb = get_neighbor(node);
			return nb._left;
		}

		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* neighbor_right(basic_b_tree_node<K, V, Degree>* node)
		{
			neighbor<K, V, Degree> nb = get_neighbor(node);
			return nb._right;
		}

		template<typename K, typename V, size_t Degree>
		neighbor<K, V, Degree> get_neighbor(basic_b_tree_node<K, V, Degree>* node)
		{
			using node_type = basic_b_tree_node<K, V, Degree>;
			using node_ptr = node_type*;
			//�����ڻ��Ǹ��ڵ�
			if (!node || !node->_parent)
				return neighbor<K,V,Degree>{};
			
			auto p = node->_parent->_children.begin();
			for (; p != node->_parent->_children.end(); ++p)
			{
				if (node == *p)
					break;
			}

			auto left = p == node->_parent->_children.begin() ? nullptr : *(p - 1);
			auto right = p == node->_parent->_children.end() ? nullptr : *(p + 1);
			
			return neighbor<K, V, Degree>{left, right};
		}

		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* merge(basic_b_tree_node<K, V, Degree>*& root, basic_b_tree_node<K, V, Degree>* node)
		{
			neighbor<K, V, Degree> nb = get_neighbor(node);
			if (nb._left)
				return merge_with_neighbor_left(root,node);
			else
				return merge_with_neighbor_right(root,node);
		}

		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* merge_with_neighbor_left(basic_b_tree_node<K, V, Degree>*& root, basic_b_tree_node<K, V, Degree>* node)
		{
			using node_type = basic_b_tree_node<K, V, Degree>;
			using node_ptr = node_type*;
			node_ptr nbs_left = neighbor_left(node);
			//��λ��Ԫ��
			auto elem = parent_elem_left(node);
			auto it = node_position_in_parent(node);
			//��Ԫ������
			nbs_left->_elem.push_back(*elem);
			//�ü����ڵ�
			node->_parent->_elem.erase(elem);
			node->_parent->_children.erase(it);
			if (node->_parent == root && root->_elem.empty())
			{
				//���ڵ�ı�
				destroy_node(root);
				root = nbs_left;
				root->_parent = nullptr;
			}
			//���ھ���Ǩ
			nbs_left->_elem.insert(nbs_left->_elem.end(), node->_elem.begin(), node->_elem.end());
			nbs_left->_children.insert(nbs_left->_children.end(), node->_children.begin(), node->_children.end());
			//ɾ���ұ߽ڵ�
			destroy_node(node);
			//�ϲ��Ľڵ�Ϊ��ǰ�ڵ�
			return nbs_left;
			
		}

		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* merge_with_neighbor_right(basic_b_tree_node<K, V, Degree>*& root, basic_b_tree_node<K, V, Degree>* node)
		{
			using node_type = basic_b_tree_node<K, V, Degree>;
			using node_ptr = node_type*;
			node_ptr nbs_right = neighbor_right(node);
			//��λ��Ԫ��
			auto elem = parent_elem_right(node);
			auto it = node_position_in_parent(nbs_right);
			//��Ԫ������
			node->_elem.push_back(*elem);
			//�ü����ڵ�
			node->_parent->_elem.erase(elem);
			node->_parent->_children.erase(it);
			if (node->_parent == root && root->_elem.empty())
			{
				destroy_node(root);
				root = node;
				root->_parent = nullptr;
			}
			//���ھ���Ǩ
			node->_elem.insert(node->_elem.end(), nbs_right->_elem.begin(), nbs_right->_elem.end());
			node->_children.insert(node->_children.end(), nbs_right->_children.begin(), nbs_right->_children.end());
			//ɾ���ұ߽ڵ�
			destroy_node(nbs_right);
			//�ϲ��Ľڵ�Ϊ��ǰ�ڵ�
			return node;
		}

		

		template<typename K, typename V, size_t Degree>
		subtrees<K, V, Degree> get_subtrees(basic_b_tree_node<K, V, Degree>* node, const K& key)
		{
			using node_type = basic_b_tree_node<K, V, Degree>;
			using node_ptr = node_type*;
			auto elem = elem_position_in_node(node, key);
			if (elem == node->_elem.end())
				throw std::exception{ "key not in node" };
			auto distance = elem - node->_elem.begin();
			auto left = node->_children.begin() + distance;
			auto right = node->_children.begin() + distance + 1;
			return subtrees<K,V,Degree>{ left,right };
		}

		template<typename K, typename V, size_t Degree>
		inline subtrees<K, V, Degree> get_subtrees(basic_b_tree_node<K, V, Degree>* node, typename category<K, V, Degree>::elem_iterator it)
		{
			auto distance = it - node->_elem.begin();
			auto left = node->_children.begin() + distance;
			auto right = node->_children.begin() + distance + 1;
			return subtrees<K, V, Degree>{left, right};
		}

		template<typename K, typename V, size_t Degree>
		inline typename category<K, V, Degree>::child_iterator node_position_in_parent(basic_b_tree_node<K, V, Degree>* node)
		{
			using child_iterator = typename category<K, V, Degree>::child_iterator;
			using node_type = basic_b_tree_node<K, V, Degree>;
			using node_ptr = node_type*;

			node_ptr parent = node->_parent;
			if (!parent)
				throw std::exception{ "root has no parent" };
			auto p = parent->_children.begin();
			for (; p != parent->_children.end(); ++p)
			{
				if (node == *p)
					return p;
			}
			return child_iterator{};
		}

		template<typename K, typename V, size_t Degree>
		inline typename category<K, V, Degree>::elem_iterator elem_position_in_node(basic_b_tree_node<K, V, Degree>* node, const K& key)
		{
			using node_type = basic_b_tree_node<K, V, Degree>;
			using node_ptr = node_type*;
			auto p = node->_elem.begin();
			for (; p != node->_elem.end(); ++p)
			{
				if (key == p->first)
					return p;
			}
			return node->_elem.end();
		}

		template<typename K, typename V, size_t Degree>
		inline typename category<K, V, Degree>::elem_iterator elem_mid_in_node(basic_b_tree_node<K, V, Degree>* node)
		{
			auto distance = static_cast<size_t>(node->_elem.size() / 2);
			return node->_elem.begin() + distance;
		}

		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* locate(basic_b_tree_node<K, V, Degree>* root, const K& key)
		{
			using node_type = basic_b_tree_node<K, V, Degree>;
			using node_ptr = node_type*;
			node_ptr cur = root;
			while (cur)
			{
				auto p = cur->_elem.begin();
				while (p != cur->_elem.end())
				{
					if (key == p->first)
						return cur;
					if (key < p->first)
						break;
				}
				auto distance = p - cur->_elem.begin();
				auto child = cur->_children[distance];
				cur = child;
			}
			return cur;
		}

		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* erase(basic_b_tree_node<K, V, Degree>* root, const K& key)
		{
			
			using node_type = basic_b_tree_node<K, V, Degree>;
			using node_ptr = node_type*;
			using elem_iterator = typename category<K, V, Degree>::elem_iterator;
			elem_iterator q;	//p��keyֵ����λ�ã�q��ʵ��ɾ��Ԫ������λ��
			constexpr size_t min = Degree;	//���ݹ�������Ҫ��֤��ǰ�ڵ��Ԫ��������СΪmin

			node_ptr cur = root;
			K cur_key = key;	//��ʹ��������ݵ�ʱ�������Ҫ�ı�key
			while (cur)
			{
				//��ȡ���ߺϲ�ʹ�ýڵ�������������
				if (cur != root && need_borrow_or_merge(cur))
				{
					cur = borrow_or_merge(root, cur);
				}

				auto it = elem_position_in_node(cur, cur_key);
				if (it != cur->_elem.end())
				{
					//Ԫ���ڵ�ǰ�ڵ���
					it;
					subtrees<K, V, Degree> subs = get_subtrees(cur, it);
					auto left = subs._left;
					auto right = subs._right;
					if (*left)
					{
						//ʹ��ǰ��Ԫ�������ǰԪ��
						q = precursor_elem_from_subtree_left(cur, cur_key);
						*it = *q;
						cur_key = q->first;
						cur = *left;
					}
					else if (*right)
					{
						//ʹ�ú��Ԫ�������ǰԪ��
						q = successor_elem_from_subtree_right(cur, cur_key);
						*it = *q;
						cur_key = q->first;
						cur = *right;
					}
					else {
						//ֱ��ɾ��
						cur->_elem.erase(it);
						cur->_children.erase(right);
						return root;
					}
				}else {
					//��������
					cur = next_level_node(cur, cur_key);
				}
			}
			return root;
		}

		template<typename K,typename V, size_t Degree>
		void borrow_from_neightbor(basic_b_tree_node<K, V, Degree>* node)
		{
			if (could_borrow_from_neightbor_left(node))
				borrow_from_neightbor_left(node);
			else if (could_borrow_from_neightbor_right(node))
				borrow_from_neightbor_right(node);
			
		}

		template<typename K, typename V, size_t Degree>
		void borrow_from_neightbor_left(basic_b_tree_node<K, V, Degree>* node)
		{
			using node_type = basic_b_tree_node<K, V, Degree>;
			using node_ptr = node_type*;
		
			node_ptr nbs_left = neighbor_left(node);	//����
			auto elem = parent_elem_left(node);			//�ƶ��ĸ�Ԫ��
			node->_elem.insert(node->_elem.begin(), *elem);	
			//���ھ�����
			*elem = nbs_left->_elem.back();
			//������������Ԫ�صĺ���
			node->_children.insert(node->_children.begin(), nbs_left->_children.back());
			node_ptr child = nbs_left->_children.back();
			if(child)
				child->_parent = node;
			//�ü����ھ�
			nbs_left->_elem.pop_back();
			nbs_left->_children.pop_back();
		}

		template<typename K, typename V, size_t Degree>
		void borrow_from_neightbor_right(basic_b_tree_node<K, V, Degree>* node)
		{
			using node_type = basic_b_tree_node<K, V, Degree>;
			using node_ptr = node_type*;
			node_ptr nbs_right = neighbor_right(node);	//����
			auto elem = parent_elem_right(node);	//�ƶ��ĸ�Ԫ��
			//��Ԫ������
			node->_elem.push_back(*elem);
			//���ھ�����
			*elem = *(nbs_right->_elem.begin());
			//������������Ԫ�صĺ���
			node_ptr child = *(nbs_right->_children.begin());
			node->_children.push_back(child);
			if(child)
				child->_parent = node;
			//�ü�����
			nbs_right->_elem.erase(nbs_right->_elem.begin());
			nbs_right->_children.erase(nbs_right->_children.begin());
		}

		template<typename K, typename V, size_t Degree>
		bool could_borrow_from_neightbor_left(basic_b_tree_node<K, V, Degree>* node)
		{
			using node_type = basic_b_tree_node<K, V, Degree>;
			using node_ptr = node_type*;
			node_ptr nbs_left = neighbor_left(node);
			if (!nbs_left)
				return false;
			if (nbs_left->_elem.size() > Degree-1)
				return true;
			return false;
		}

		template<typename K, typename V, size_t Degree>
		bool could_borrow_from_neightbor_right(basic_b_tree_node<K, V, Degree>* node)
		{
			using node_type = basic_b_tree_node<K, V, Degree>;
			using node_ptr = node_type*;
			node_ptr nbs_right = neighbor_right(node);
			if (!nbs_right)
				return false;
			if (nbs_right->_elem.size() > Degree-1)
				return true;
			return false;
		}

		template<typename K, typename V, size_t Degree>
		bool could_borrow_from_neightbor(basic_b_tree_node<K, V, Degree>* node)
		{
			return could_borrow_from_neightbor_left(node) || could_borrow_from_neightbor_right(node);
		}

		template<typename K, typename V, size_t Degree>
		bool need_borrow_or_merge(basic_b_tree_node<K, V, Degree>* node)
		{
			using node_type = basic_b_tree_node<K, V, Degree>;
			using node_ptr = node_type*;
			if (!node)
				return false;
			if (node->_elem.size() > node->min_elem_num)	//������min_elem_num+1,��ΪDegree
				return false;
			return true;
		}

		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* borrow_or_merge(basic_b_tree_node<K, V, Degree>*& root, basic_b_tree_node<K, V, Degree>* node)
		{
			//��ȡԪ�ز��ı䵱ǰ�ڵ�
			if (could_borrow_from_neightbor(node))
			{
				borrow_from_neightbor(node);
				return node;
			}
			else {
				return merge(root, node);
			}
			
		}

		template<typename K, typename V, size_t Degree>
		inline bool need_split(basic_b_tree_node<K, V, Degree>* node)
		{
			if (elem_full(node))
				return true;
			return false;
		}

		template<typename K, typename V, size_t Degree>
		inline basic_b_tree_node<K, V, Degree>* split(basic_b_tree_node<K, V, Degree>*& root, basic_b_tree_node<K, V, Degree>* node)
		{
			using node_type = basic_b_tree_node<K, V, Degree>;
			using node_ptr = node_type*;
			
			//����Ǹ��ڵ㣬����Ҫ����һ���µĸ��ڵ�
			if (node == root)
			{
				//���ڸ�����¸ı���ڵ�
				node_ptr tmp = allocate_node<K,V,Degree>();
				node->_parent = tmp;
				tmp->_children.push_back(node);
				root = tmp;
			}
			//���ѳ����ֵܽڵ�
			node_ptr bro = allocate_node<K,V,Degree>();
			bro->_parent = node->_parent;
			//��Ԫ�ز���λ��
			auto pr = parent_elem_right(node);	
			auto cr = subtree_right(node->_parent, pr);
			//��Ҫ���������λ��
			auto mid = elem_mid_in_node(node);
			//�������ֵܽڵ����ʼλ��
			auto bro_elem_begin = mid + 1;
			auto bro_cld_begin = subtree_right(node, mid);
			//Ԫ����Ǩ
			bro->_elem.insert(bro->_elem.begin(), bro_elem_begin, node->_elem.end());
			bro->_children.insert(bro->_children.begin(), bro_cld_begin, node->_children.end());
			//Ԫ������
			node->_parent->_elem.insert(pr, *mid);
			node->_parent->_children.insert(cr, bro);
			//�ü���ǰԪ��
			node->_elem.erase(mid, node->_elem.end());
			node->_children.erase(bro_cld_begin, node->_children.end());
			//Ԫ������
			
			return root;
		}

		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* insert(basic_b_tree_node<K, V, Degree>* root, const K& key, const V& val)
		{
			using node_type = basic_b_tree_node<K, V, Degree>;
			using node_ptr = node_type*;

			if (!root)
			{
				node_ptr tmp = allocate_node<K, V, Degree>();
				tmp->_children.push_back(nullptr);
				root = tmp;
			}
			node_ptr cur = root;
			node_ptr p = nullptr;
			while (cur)
			{
				p = cur;
				if (need_split(cur))
				{
					root = split(root, cur);	//�ڵ������ͷ���
					cur = cur->_parent;
				}
				else
				{
					cur = next_level_node(cur, key);	//δ���ͼ�������
				}
			}

			elem_into_unfull_node(p, { key,val });	//����ڵ�
			return root;
		}

		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* precursor_node_from_subtree_left(basic_b_tree_node<K, V, Degree>* node, const K& key)
		{
			using node_type = basic_b_tree_node<K, V, Degree>;
			using node_ptr = node_type*;
			
			auto elem_it = elem_position_in_node(node, key);
			auto it = subtree_left(node, elem_it);
			node_ptr cur = *it;
			while (cur)
			{
				//��ѯ�ڵ��ĩβԪ���Ƿ����Һ���
				
				auto child = subtree_right(cur, --cur->_elem.end());
				if (*child)
					cur = *child;	//��������
				else
					break;	
			}
			return cur;
		}

		template<typename K, typename V, size_t Degree>
		typename category<K, V, Degree>::elem_iterator precursor_elem_from_subtree_left(basic_b_tree_node<K, V, Degree>* node, const K& key)
		{
			using node_type = basic_b_tree_node<K, V, Degree>;
			using node_ptr = node_type*;
			node_ptr pre_node = precursor_node_from_subtree_left(node, key);
			if (pre_node)
				return --pre_node->_elem.end();
			else
				throw std::exception{ "no prevcursor elem" };
		}

		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* successor_node_from_subtree_right(basic_b_tree_node<K, V, Degree>* node, const K& key)
		{
			using node_type = basic_b_tree_node<K, V, Degree>;
			using node_ptr = node_type*;

			auto elem_it = elem_position_in_node(node, key);
			auto it = subtree_right(node, elem_it);
			node_ptr cur = *it;
			while (cur)
			{
				//��ѯ�ڵ����Ԫ���Ƿ�������
				auto child = cur->_children.begin();
				if (*child)
				{
					cur = *child;
				}
				else
					break;
			}

			return cur;
		}

		template<typename K, typename V, size_t Degree>
		typename category<K, V, Degree>::elem_iterator successor_elem_from_subtree_right(basic_b_tree_node<K, V, Degree>* node, const K& key)
		{
			using node_type = basic_b_tree_node<K, V, Degree>;
			using node_ptr = node_type*;
			node_ptr suc_node = successor_node_from_subtree_right(node, key);
			if (suc_node)
				return suc_node->_elem.begin();
			else
				throw std::exception{ "no successor elem" };
		}

		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* next_level_node(basic_b_tree_node<K, V, Degree>* node, const K& key)
		{
			using node_type = basic_b_tree_node<K, V, Degree>;
			using node_ptr = node_type*;
			auto p = node->_elem.begin();
			for (; p != node->_elem.end(); ++p)
			{
				if (key < p->first)
					break;
			}
			auto distance = p - node->_elem.begin();
			return node->_children[distance];
		}

		template<typename K, typename V, size_t Degree>
		V& find(basic_b_tree_node<K, V, Degree>* root, const K& key)
		{
			using node_type = basic_b_tree_node<K, V, Degree>;
			using node_ptr = node_type*;
			node_ptr cur = root;
			while (cur)
			{
				auto it = elem_position_in_node(cur, key);
				if (it != cur->_elem.end())
				{
					return it->second;
				}
				else {
					cur = next_level_node(cur, key);
				}
			}
			
			throw std::exception{ "elem not in tree" };
		}

		//����
		template<typename K,typename V,size_t Degree>
		void split(basic_b_tree_node<K, V, Degree>* node)
		{
			using node_type = basic_b_tree_node<K, V, Degree>;
			using node_ptr = node_type*;

			//static_assert(std::is_same_v<node_type, typename std::allocator_traits<Alloc>::value_type>);
			
			//�ڵ��������
			if (node_type::max_elem_num != node->_elem.size())
				return;

			//�����½ڵ�
			node_ptr bro = allocate_node<K,V,Degree>();
			std::move(node->_elem.begin() + Degree, node->_elem.end(), std::back_inserter(bro->_elem));
			std::move(node->_children.begin() + Degree, node->_children.end(), std::back_inserter(bro->_children));
			
			bro->_parent = node->_parent;

			//Ԫ������
			auto elem = node->_elem[Degree - 1];
			elem_into_unfull_node(node->_parent, elem, bro);

			//�ڵ�ü�
			node->_elem.erase(node->_elem.begin() + Degree - 1, node->_elem.end());
			node->_children.erase(node->_children.begin() + Degree, node->_children.end());
		}

		//��һ��δ���Ľڵ��в���Ԫ��
		template<typename K,typename V,size_t Degree>
		void elem_into_unfull_node(basic_b_tree_node<K,V,Degree>* node,const std::pair<K, V>& elem,basic_b_tree_node<K,V,Degree>* elem_right_subtree)
		{
			auto p = node->_elem.begin();
			for (; p != node->_elem.end(); ++p)
			{
				if (elem.first < p->first)
					break;
			}
			auto distance = p - node->_elem.begin();
			node->_elem.insert(p, elem);
			auto it = node->_children.begin() + distance + 1;
			node->_children.insert(it, elem_right_subtree);
		}

		

		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* down_to(basic_b_tree_node<K, V, Degree>* node, const K& key)
		{
			for (auto p = node->_elem.begin(); p != node->_elem.end(); ++p)
			{
				if (key < p->first)
				{
					auto distance = p - node->_elem.begin();
					return node->_children[distance];
				}
			}
			return node->_children.back;
		}

	}
	
}

#endif



/*tips:
* B�������÷�ʽ��ⲻ���ر����ѣ���ʵ�ֵ�ʱ��ͻᷢ��ϸ�ڻ�ǳ��Ķࡣ��Щϸ��Ҳ�ڿ����Ŷ���B�����˽⡣
* �����ʵ��ֻ�����ڴ���ģ����B���Ĺ�����ʽ����ʵ�ʵ�B��ͨ�������ڴ��������У����������ڴ��У����ֻ���о���ֵ���ص�ֻ��B���Ĺ����߼�
*/
