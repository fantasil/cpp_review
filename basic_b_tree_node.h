#pragma once
#ifndef fantasil_basic_b_tree_node_h
#define fantasil_basic_b_tree_node_h
#include"headers.h"

namespace fantasil
{
	/*
	* 一个B树节点，其中Degree是代表着下界,令为t
	* 节点最大的元素数量是2t-1;最小的元素数量是t-1
	* 对应的节点最多的孩子可以有2t，最少是t
	* 插入下溯如果节点已满就需要分裂
	* 删除下溯如果节点元素为t-1就需要借取或者合并
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

		constexpr static size_t max_elem_num = 2 * Degree - 1;	//最多可以容纳的元素数量
		constexpr static size_t max_cld_num = 2 * Degree;	//最多可以交互的子节点数量
		constexpr static size_t min_elem_num = Degree - 1;
		constexpr static size_t min_cld_num = Degree;
		
		node_ptr _parent;
		std::vector<std::pair<K, V>> _elem;			//容纳元素
		std::vector<node_ptr> _children;			//容纳子节点
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

		//一个简单的数据结构，用于存放某个节点左右邻居
		template<typename K,typename V,size_t Degree>
		struct neighbor {
			using node_type = basic_b_tree_node<K, V, Degree>;
			using node_ptr = node_type*;
			node_ptr _left;
			node_ptr _right;
			neighbor(node_ptr left = nullptr, node_ptr right = nullptr) :_left{ left }, _right{ right } {}
			~neighbor() {}
		};

		//一个简单的数据结构，用于存放某个元素的左右子树
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
		/*---------------针对单个节点内的操作------------------------*/
		template<typename K, typename V, size_t Degree,typename ostream>
		void level_order(basic_b_tree_node<K, V, Degree>* root, ostream& os);

		//元素在该节点内吗？
		template<typename K, typename V, size_t Degree>
		inline bool elem_in_node(basic_b_tree_node < K, V, Degree>* node, const K& key);
		//分配一个节点
		template<typename K, typename V, size_t Degree>
		inline basic_b_tree_node<K, V, Degree>* allocate_node();
		//删除一个节点
		template<typename K, typename V, size_t Degree>
		inline void destroy_node(basic_b_tree_node<K, V, Degree>* node);
		//节点内的元素已满？
		template<typename K, typename V, size_t Degree>
		inline bool elem_full(basic_b_tree_node<K, V, Degree>* node);
		//是否是叶子节点
		template<typename K, typename V, size_t Degree>
		inline bool is_leaf(basic_b_tree_node<K, V, Degree>* node);
		//获取元素的左子树，默认元素在节点内
		/*template<typename K, typename V, size_t Degree>
		inline typename category<K, V, Degree>::child_iterator subtree_left(basic_b_tree_node<K, V, Degree>* node, const K& key);*/
		template<typename K, typename V, size_t Degree>
		inline typename category<K, V, Degree>::child_iterator subtree_left(basic_b_tree_node<K, V, Degree>* node, typename category<K,V,Degree>::elem_iterator it);
		////获取元素的右子树，默认元素在节点内
		//template<typename K, typename V, size_t Degree>
		//inline typename category<K, V, Degree>::child_iterator subtree_right(basic_b_tree_node<K, V, Degree>* node, const K& key);
		template<typename K, typename V, size_t Degree>
		inline typename category<K, V, Degree>::child_iterator subtree_right(basic_b_tree_node<K, V, Degree>* node, typename category<K, V, Degree>::elem_iterator it);
		//获取当前节点的父元素，且该节点是父元素的左孩子
		template<typename K, typename V, size_t Degree>
		inline typename parent_elems<K,V,Degree>::iterator parent_elem_left(basic_b_tree_node<K, V, Degree>* node);
		//获取当前节点的父元素，且该节点是父元素的右孩子
		template<typename K, typename V, size_t Degree>
		inline typename parent_elems<K,V,Degree>::iterator parent_elem_right(basic_b_tree_node<K, V, Degree>* node);
		//获取当前节点的两个父元素，节点位于两个父元素之间
		template<typename K, typename V, size_t Degree>
		inline parent_elems<K, V, Degree> get_parent_elems(basic_b_tree_node<K, V, Degree>* node);
		////获取一个元素的左右子树
		//template<typename K, typename V, size_t Degree>
		//subtrees<K, V, Degree> get_subtrees(basic_b_tree_node<K, V, Degree>* node, const K& key);
		template<typename K, typename V, size_t Degree>
		subtrees<K, V, Degree> get_subtrees(basic_b_tree_node<K, V, Degree>* node, typename category<K, V, Degree>::elem_iterator it);
		//获取当前节点在父节点中的位置
		template<typename K, typename V, size_t Degree>
		inline typename category<K, V, Degree>::child_iterator node_position_in_parent(basic_b_tree_node<K, V, Degree>*);
		//获取elem在节点元素中的位置
		template<typename K, typename V, size_t Degree>
		inline typename category<K, V, Degree>::elem_iterator elem_position_in_node(basic_b_tree_node<K, V, Degree>* node, const K& key);
		//中间元素
		template<typename K, typename V, size_t Degree>
		typename category<K, V, Degree>::elem_iterator elem_mid_in_node(basic_b_tree_node<K, V, Degree>* node);
		//
		/*---------------------B树的操作---------------------------*/
		//获取左邻
		template<typename K, typename V, size_t Degree>
		inline basic_b_tree_node<K, V, Degree>* neighbor_left(basic_b_tree_node<K, V, Degree>* node);
		//获取右舍
		template<typename K, typename V, size_t Degree>
		inline basic_b_tree_node<K, V, Degree>* neighbor_right(basic_b_tree_node<K, V, Degree>* node);
		//获取左邻右舍
		template<typename K, typename V, size_t Degree>
		inline neighbor<K, V, Degree> get_neighbor(basic_b_tree_node<K, V, Degree>* node);

		//与邻居节点合并
		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* merge(basic_b_tree_node<K, V, Degree>*& root, basic_b_tree_node<K, V, Degree>* node);
		//与左邻合并
		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* merge_with_neighbor_left(basic_b_tree_node<K, V, Degree>*& root, basic_b_tree_node<K, V, Degree>* node);
		//与右舍合并
		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* merge_with_neighbor_right(basic_b_tree_node<K, V, Degree>*& root, basic_b_tree_node<K, V, Degree>* node);

		//向邻居借元素
		template<typename K, typename V, size_t Degree>
		void borrow_from_neightbor(basic_b_tree_node<K, V, Degree>* node);
		//向左邻借元素
		template<typename K, typename V, size_t Degree>
		void borrow_from_neightbor_left(basic_b_tree_node<K, V, Degree>* node);
		//向右舍借元素
		template<typename K, typename V, size_t Degree>
		void borrow_from_neightbor_right(basic_b_tree_node<K, V, Degree>* node);
		//左邻能借元素吗？
		template<typename K, typename V, size_t Degree>
		bool could_borrow_from_neightbor_left(basic_b_tree_node<K, V, Degree>* node);
		//右舍能借元素吗？
		template<typename K, typename V, size_t Degree>
		bool could_borrow_from_neightbor_right(basic_b_tree_node<K, V, Degree>* node);
		//邻居能借元素吗？
		template<typename K, typename V, size_t Degree>
		bool could_borrow_from_neightbor(basic_b_tree_node<K, V, Degree>* node);

		//删除下溯的时候该节点需要借取元素或者合并吗
		template<typename K, typename V, size_t Degree>
		bool need_borrow_or_merge(basic_b_tree_node<K, V, Degree>* node);
		//删除下溯时候的借取元素或者合并
		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K,V,Degree>* borrow_or_merge(basic_b_tree_node<K,V,Degree>*& root,basic_b_tree_node<K, V, Degree>* node);

		//插入下溯的时候该节点需要分裂吗？
		template<typename K, typename V, size_t Degree>
		bool need_split(basic_b_tree_node<K, V, Degree>* node);
		//插入下溯时候的分裂
		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* split(basic_b_tree_node<K, V, Degree>*& root, basic_b_tree_node<K, V, Degree>* node);
		//插入一个元素
		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* insert(basic_b_tree_node<K, V, Degree>* root, const K& key, const V& val);
		
		//左子树内的前驱元素所在的节点
		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* precursor_node_from_subtree_left(basic_b_tree_node<K, V, Degree>* node, const K& key);
		//左子树内的前驱元素
		template<typename K, typename V, size_t Degree>
		typename category<K,V,Degree>::elem_iterator precursor_elem_from_subtree_left(basic_b_tree_node<K, V, Degree>* node, const K& key);
		//右子树内的后继元素所在的节点
		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* successor_node_from_subtree_right(basic_b_tree_node<K, V, Degree>* node, const K& key);
		//右子树内的后继元素
		template<typename K, typename V, size_t Degree>
		typename category<K,V,Degree>::elem_iterator successor_elem_from_subtree_right(basic_b_tree_node<K, V, Degree>* node, const K& key);
		
		//若key不在节点内，就下溯到合适的新节点
		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* next_level_node(basic_b_tree_node<K, V, Degree>* node, const K& key);

		//定位元素elem所在的节点
		template<typename K,typename V,size_t Degree>
		basic_b_tree_node<K, V, Degree>* locate(basic_b_tree_node<K, V, Degree>* node, const K& key);

		//删除一个元素
		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* erase(basic_b_tree_node<K, V, Degree>* root, const K& key);

		//
		template<typename K, typename V, size_t Degree>
		V& find(basic_b_tree_node<K, V, Degree>* node, const K& key);
		//在一个未满的节点中插入元素
		template<typename K, typename V, size_t Degree>
		void elem_into_unfull_node(basic_b_tree_node<K, V, Degree>* node, const std::pair<K, V>& elem, basic_b_tree_node<K, V, Degree>* elem_right_subtree = nullptr);
		////插入元素(废弃)
		//template<typename K,typename V,size_t Degree>
		//basic_b_tree_node<K,V,Degree>* insert(basic_b_tree_node<K, V, Degree>* root, const std::pair<K, V>& elem);


		/*----------------------------------------------------impl-------------------------------------------------------*/
		
		//分配一个节点
		template<typename K,typename V,size_t Degree>
		inline basic_b_tree_node<K, V, Degree>* allocate_node()
		{
			return new basic_b_tree_node<K, V, Degree>{};
		}
	
		//删除一个节点
		template<typename K,typename V,size_t Degree>
		inline void destroy_node(basic_b_tree_node<K,V,Degree>* node)
		{
			delete node;
		}
		//节点元素已满？
		template<typename K,typename V,size_t Degree>
		inline bool elem_full(basic_b_tree_node<K, V, Degree>* node)
		{
			using node_type = basic_b_tree_node<K, V, Degree>;
			return node_type::max_elem_num == node->_elem.size();
		}
		//是否是叶子节点
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
			//不存在或是根节点
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
			//定位父元素
			auto elem = parent_elem_left(node);
			auto it = node_position_in_parent(node);
			//父元素下移
			nbs_left->_elem.push_back(*elem);
			//裁剪父节点
			node->_parent->_elem.erase(elem);
			node->_parent->_children.erase(it);
			if (node->_parent == root && root->_elem.empty())
			{
				//根节点改变
				destroy_node(root);
				root = nbs_left;
				root->_parent = nullptr;
			}
			//右邻居左迁
			nbs_left->_elem.insert(nbs_left->_elem.end(), node->_elem.begin(), node->_elem.end());
			nbs_left->_children.insert(nbs_left->_children.end(), node->_children.begin(), node->_children.end());
			//删除右边节点
			destroy_node(node);
			//合并的节点为当前节点
			return nbs_left;
			
		}

		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* merge_with_neighbor_right(basic_b_tree_node<K, V, Degree>*& root, basic_b_tree_node<K, V, Degree>* node)
		{
			using node_type = basic_b_tree_node<K, V, Degree>;
			using node_ptr = node_type*;
			node_ptr nbs_right = neighbor_right(node);
			//定位父元素
			auto elem = parent_elem_right(node);
			auto it = node_position_in_parent(nbs_right);
			//父元素下移
			node->_elem.push_back(*elem);
			//裁剪父节点
			node->_parent->_elem.erase(elem);
			node->_parent->_children.erase(it);
			if (node->_parent == root && root->_elem.empty())
			{
				destroy_node(root);
				root = node;
				root->_parent = nullptr;
			}
			//右邻居左迁
			node->_elem.insert(node->_elem.end(), nbs_right->_elem.begin(), nbs_right->_elem.end());
			node->_children.insert(node->_children.end(), nbs_right->_children.begin(), nbs_right->_children.end());
			//删除右边节点
			destroy_node(nbs_right);
			//合并的节点为当前节点
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
			elem_iterator q;	//p是key值所在位置，q是实际删除元素所在位置
			constexpr size_t min = Degree;	//下溯过程中需要保证当前节点的元素数量最小为min

			node_ptr cur = root;
			K cur_key = key;	//在使用替代数据的时候可能需要改变key
			while (cur)
			{
				//借取或者合并使得节点满足下溯条件
				if (cur != root && need_borrow_or_merge(cur))
				{
					cur = borrow_or_merge(root, cur);
				}

				auto it = elem_position_in_node(cur, cur_key);
				if (it != cur->_elem.end())
				{
					//元素在当前节点中
					it;
					subtrees<K, V, Degree> subs = get_subtrees(cur, it);
					auto left = subs._left;
					auto right = subs._right;
					if (*left)
					{
						//使用前驱元素替代当前元素
						q = precursor_elem_from_subtree_left(cur, cur_key);
						*it = *q;
						cur_key = q->first;
						cur = *left;
					}
					else if (*right)
					{
						//使用后继元素替代当前元素
						q = successor_elem_from_subtree_right(cur, cur_key);
						*it = *q;
						cur_key = q->first;
						cur = *right;
					}
					else {
						//直接删除
						cur->_elem.erase(it);
						cur->_children.erase(right);
						return root;
					}
				}else {
					//继续下溯
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
		
			node_ptr nbs_left = neighbor_left(node);	//左邻
			auto elem = parent_elem_left(node);			//移动的父元素
			node->_elem.insert(node->_elem.begin(), *elem);	
			//左邻居上移
			*elem = nbs_left->_elem.back();
			//处理左邻上溢元素的孩子
			node->_children.insert(node->_children.begin(), nbs_left->_children.back());
			node_ptr child = nbs_left->_children.back();
			if(child)
				child->_parent = node;
			//裁剪左邻居
			nbs_left->_elem.pop_back();
			nbs_left->_children.pop_back();
		}

		template<typename K, typename V, size_t Degree>
		void borrow_from_neightbor_right(basic_b_tree_node<K, V, Degree>* node)
		{
			using node_type = basic_b_tree_node<K, V, Degree>;
			using node_ptr = node_type*;
			node_ptr nbs_right = neighbor_right(node);	//右舍
			auto elem = parent_elem_right(node);	//移动的父元素
			//父元素下移
			node->_elem.push_back(*elem);
			//右邻居上溢
			*elem = *(nbs_right->_elem.begin());
			//处理右舍上溢元素的孩子
			node_ptr child = *(nbs_right->_children.begin());
			node->_children.push_back(child);
			if(child)
				child->_parent = node;
			//裁剪右舍
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
			if (node->_elem.size() > node->min_elem_num)	//至少是min_elem_num+1,即为Degree
				return false;
			return true;
		}

		template<typename K, typename V, size_t Degree>
		basic_b_tree_node<K, V, Degree>* borrow_or_merge(basic_b_tree_node<K, V, Degree>*& root, basic_b_tree_node<K, V, Degree>* node)
		{
			//借取元素不改变当前节点
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
			
			//如果是根节点，就需要生成一个新的根节点
			if (node == root)
			{
				//仅在该情况下改变根节点
				node_ptr tmp = allocate_node<K,V,Degree>();
				node->_parent = tmp;
				tmp->_children.push_back(node);
				root = tmp;
			}
			//分裂出的兄弟节点
			node_ptr bro = allocate_node<K,V,Degree>();
			bro->_parent = node->_parent;
			//父元素插入位置
			auto pr = parent_elem_right(node);	
			auto cr = subtree_right(node->_parent, pr);
			//需要上溢的数据位置
			auto mid = elem_mid_in_node(node);
			//拷贝至兄弟节点的起始位置
			auto bro_elem_begin = mid + 1;
			auto bro_cld_begin = subtree_right(node, mid);
			//元素右迁
			bro->_elem.insert(bro->_elem.begin(), bro_elem_begin, node->_elem.end());
			bro->_children.insert(bro->_children.begin(), bro_cld_begin, node->_children.end());
			//元素上溢
			node->_parent->_elem.insert(pr, *mid);
			node->_parent->_children.insert(cr, bro);
			//裁剪当前元素
			node->_elem.erase(mid, node->_elem.end());
			node->_children.erase(bro_cld_begin, node->_children.end());
			//元素上溢
			
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
					root = split(root, cur);	//节点已满就分裂
					cur = cur->_parent;
				}
				else
				{
					cur = next_level_node(cur, key);	//未满就继续下溯
				}
			}

			elem_into_unfull_node(p, { key,val });	//插入节点
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
				//查询节点的末尾元素是否有右孩子
				
				auto child = subtree_right(cur, --cur->_elem.end());
				if (*child)
					cur = *child;	//继续下溯
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
				//查询节点的首元素是否有左孩子
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

		//分裂
		template<typename K,typename V,size_t Degree>
		void split(basic_b_tree_node<K, V, Degree>* node)
		{
			using node_type = basic_b_tree_node<K, V, Degree>;
			using node_ptr = node_type*;

			//static_assert(std::is_same_v<node_type, typename std::allocator_traits<Alloc>::value_type>);
			
			//节点必须已满
			if (node_type::max_elem_num != node->_elem.size())
				return;

			//生成新节点
			node_ptr bro = allocate_node<K,V,Degree>();
			std::move(node->_elem.begin() + Degree, node->_elem.end(), std::back_inserter(bro->_elem));
			std::move(node->_children.begin() + Degree, node->_children.end(), std::back_inserter(bro->_children));
			
			bro->_parent = node->_parent;

			//元素上升
			auto elem = node->_elem[Degree - 1];
			elem_into_unfull_node(node->_parent, elem, bro);

			//节点裁剪
			node->_elem.erase(node->_elem.begin() + Degree - 1, node->_elem.end());
			node->_children.erase(node->_children.begin() + Degree, node->_children.end());
		}

		//在一个未满的节点中插入元素
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
* B树的作用方式理解不是特别困难，但实现的时候就会发现细节会非常的多。这些细节也在考验着对于B树的了解。
* 这里的实现只是在内存中模拟了B树的工作方式，而实际的B树通常会用于磁盘数据中，而不仅在内存中，因而只有研究价值。重点只是B树的工作逻辑
*/
