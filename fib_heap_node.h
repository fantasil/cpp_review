#pragma once
#include<vector>
#include<cassert>
#include<numeric>

#pragma region describtion
/*
	斐波那契堆

	斐波那契堆可以视为斐波那契树的集合，


*/
#pragma endregion


#ifndef fantasil_fib_heap_h
#define fantasil_fib_heap_h

namespace fantasil
{


	template<typename K,typename V>
	struct fib_heap_node {
		using key_type = K;
		using value_type = V;
		using node_type = fib_heap_node<K, V>;
		using node_ptr = node_type*;

		fib_heap_node(const K& key, const V& val, size_t degree = 0, node_ptr left = nullptr, node_ptr right = nullptr, node_ptr parent = nullptr, node_ptr child = nullptr, bool marked = false) :
			_key{ key }, _val{ val }, _degree{ degree }, _left{ left }, _right{ right }, _parent{ parent }, _child{ child }, _marked{ marked }
		{

		}

		~fib_heap_node()
		{
		}

		key_type _key;
		value_type _val;
		size_t _degree;
		node_ptr _left;
		node_ptr _right;
		node_ptr _parent;
		node_ptr _child;
		bool _marked;
	};


	namespace fib_heap_node_helper {
		/*----------------------------------------------------------------declare-----------------------------------------------------------------*/
		
		//左邻右舍
		template<typename K, typename V>
		inline void as_neighbor(fib_heap_node<K, V>* left, fib_heap_node<K, V>* right);

		//插入在pos之前
		template<typename K, typename V>
		fib_heap_node<K,V>* insert_before(fib_heap_node<K, V>* pos, fib_heap_node<K, V>* node);

		//插入在pos之后
		template<typename K, typename V>
		void insert_after(fib_heap_node<K, V>* pos, fib_heap_node<K, V>* node);

		//链接成为孩子
		template<typename K,typename V>
		void link_as_child(fib_heap_node<K, V>* pos, fib_heap_node<K, V>* node);

		//向堆中插入一个节点
		template<typename K,typename V>
		fib_heap_node<K, V>* insert(fib_heap_node<K, V>* min, const std::pair<K, V>& pair);

		//抽取最小节点
		template<typename K,typename V>
		fib_heap_node<K, V>* extract_min(fib_heap_node<K, V>* min,size_t nums);


		//删除节点
		template<typename K, typename V>
		void erase_node(fib_heap_node<K, V>* node);

		//将x的孩子放入y所在的链表中
		template<typename K, typename V>
		void put_x_child_into_y_list(fib_heap_node<K, V>* x, fib_heap_node<K, V>* y);

		//将x的孩子放入x所在的链表中
		template<typename K, typename V>
		void put_x_child_into_parent_list(fib_heap_node<K, V>* x);

		//合并两个度数相同的节点
		template<typename K, typename V>
		fib_heap_node<K, V>* merge_two_same_degree_node(fib_heap_node<K, V>* x, fib_heap_node<K, V>* y);

		//如果能够合并就合并
		template<typename K, typename V>
		fib_heap_node<K, V>* merge_if_could(fib_heap_node<K, V>* x, std::vector<fib_heap_node<K, V>*>& vec);

		//合并相同度数的节点
		template<typename K, typename V>
		fib_heap_node<K, V>* merge_same_degree_node(fib_heap_node<K, V>* x, size_t n);

		//减小某个节点的key值
		template<typename K,typename V>
		fib_heap_node<K, V>* decrease_key(fib_heap_node<K, V>* min, fib_heap_node<K, V>* node, const K& key);

		//剪切
		template<typename K,typename V>
		fib_heap_node<K, V>* cut_and_move_to_root_list(fib_heap_node<K, V>* min, fib_heap_node<K, V>* node);

		//次级剪切
		template<typename K,typename V>
		fib_heap_node<K, V>* cascading_cut(fib_heap_node<K, V>* min, fib_heap_node<K, V>* node);

		//剥离节点，但保留孩子
		template<typename K,typename V>
		fib_heap_node<K, V>* strip_node(fib_heap_node<K, V>* node);




		/*----------------------------------------------------------------impl-----------------------------------------------------------------*/

		//左邻,右舍
		/*
		* @left 左邻
		* @right 右舍
		*/
		template<typename K,typename V>
		void as_neighbor(fib_heap_node<K, V>* left, fib_heap_node<K, V>* right)
		{
			if (left)
				left->_right = right;
			if (right)
				right->_left = left;
		}

		/*
		* @pos:插入位置
		* @node:插入节点
		*/
		template<typename K, typename V>
		fib_heap_node<K, V>* insert_before(fib_heap_node<K, V>* pos, fib_heap_node<K, V>* node)
		{
			using node_type = fib_heap_node<K, V>;
			using node_ptr = node_type*;

			if (!node)
				return pos;
			if (!pos)
			{
				node->_left = node;
				node->_right = node;
				return node;
			}
				

			node_ptr left = pos->_left;
			node_ptr parent = pos->_parent;
			//维护左右信息
			as_neighbor(left, node);
			as_neighbor(node, pos);

			//维护上下信息
			node->_parent = parent;
			if (parent)
			{
				if (parent->_child == pos)
					parent->_child = node;
				++parent->_degree;
			}

			if (pos->_key < node->_key)
				return pos;
			else
				return node;
		}

		/*
		* @pos:插入位置
		* @node:插入节点
		*/
		template<typename K, typename V>
		fib_heap_node<K, V>* insert_after(fib_heap_node<K, V>* pos, fib_heap_node<K, V>* node)
		{
			using node_type = fib_heap_node<K, V>;
			using node_ptr = node_type*;

			if (!node)
				return pos;
			if (!pos)
			{
				node->_left = node;
				node->_right = node;
				return node;
			}
			node_ptr right = pos->_right;
			node_ptr parent = pos->_parent;
			//维护左右信息
			as_neighbor(pos, node);
			as_neighbor(node, right);
			//维护上下信息
			node->_parent = parent;
			if (parent)
				++parent->_degree;

			if (pos->_key < node->_key)
				return pos;
			else
				return node;
		}

		/*
		* @pos:插入的位置，pos亦是父节点
		* @node:待插入的节点
		*/
		template<typename K, typename V>
		void link_as_child(fib_heap_node<K, V>* pos, fib_heap_node<K, V>* node)
		{
			using node_type = fib_heap_node<K, V>;
			using node_ptr = node_type*;

			if (!pos || !node)
				return;
			node_ptr head = pos->_child;
			if (head)
			{
				insert_before(head, node);	
			}
			else {
				pos->_child = node;
				node->_left = node;
				node->_right = node;
				++pos->_degree;
			}
			
			node->_parent = pos;
			node->_marked = false;

			
			
		}

		template<typename K, typename V>
		fib_heap_node<K, V>* insert(fib_heap_node<K, V>* min, const std::pair<K, V>& pair)
		{
			using node_type = fib_heap_node<K, V>;
			using node_ptr = node_type*;

			node_ptr tmp = new node_type{ pair.first,pair.second };

			min = insert_before(min, tmp);
			return min;
		}

		template<typename K, typename V>
		fib_heap_node<K, V>* extract_min(fib_heap_node<K, V>* min,size_t nums)
		{
			using node_type = fib_heap_node<K, V>;
			using node_ptr = node_type*;

			node_ptr right = min->_right;
			erase_node(min);
			size_t capacity = std::ceil(std::log2(nums));
			min=merge_same_degree_node(right, capacity);
			return min;
		}

		

	

		//删除一个节点
		/*
		* @node 待删除的节点
		*/
		template<typename K,typename V>
		void erase_node(fib_heap_node<K, V>* node)
		{
			using node_type = fib_heap_node<K, V>;
			using node_ptr = node_type*;
			if (!node)
				return;
			node_ptr left = node->_left;
			node_ptr right = node->_right;
			node_ptr parent = node->_parent;
			as_neighbor(left, right);
			if (parent)
			{
				--parent->_degree;
				if (parent->_child == node)
					parent->_child = right;
			}
			
			delete node;
		}

		//将x的孩子置入y所在的链表中
		/*
		* @x x的子节点
		* @y y所在的链表
		*/
		template<typename K, typename V>
		void put_x_child_into_y_list(fib_heap_node<K,V>* x, fib_heap_node<K, V>* y)
		{
			using node_type = fib_heap_node<K, V>;
			using node_ptr = node_type*;

			if (!x || !x->_child || !y)
				return;
			size_t n = x->_degree;
			node_ptr y_left = y->_left;
			node_ptr child_head = x->_child;
			node_ptr child_back = child_head;
			for (size_t i = 0; i != n; ++i)
			{
				child_back->_parent = y->_parent;
				child_back = child_back->_right;
			}
			as_neighbor(y_left, child_head);
			as_neighbor(child_back, y);
			x->_degree = 0;
			if (y->_parent)
				y->_parent->_degree += n;

		}


		template<typename K, typename V>
		void put_x_child_into_parent_list(fib_heap_node<K, V>* x)
		{
			put_x_child_into_y_list(x, x);
		}

		//合并两个节点
		/*
		* @x 第一个节点
		* @y 第二个节点
		*/
		template<typename K,typename V>
		fib_heap_node<K,V>* merge_two_same_degree_node(fib_heap_node<K, V>* x, fib_heap_node<K, V>* y)
		{
			assert(x != y);
			assert(x->_degree == y->_degree);
			if (x->_key < y->_key)
			{
				strip_node(y);
				link_as_child(x, y);
				return x;
			}
			else {
				strip_node(x);
				link_as_child(y, x);
				return y;
			}
		}

		//辅助函数，
		template<typename K,typename V>
		fib_heap_node<K,V>* merge_if_could(fib_heap_node<K, V>* x, std::vector<fib_heap_node<K, V>*>& vec)
		{
			size_t n = x->_degree;
			while (true)
			{
				if (vec[n])
				{
					x = merge_two_same_degree_node(x, vec[n]);
					vec[n] = nullptr;
					++n;
				}
				else {
					vec[n] = x;
					return x;
				}
			}
		}
		
		/*合并节点所在链表具有相同度数的节点
		* @x x节点所在的链表
		* @n 链表的节点个数
		*/
		template<typename K,typename V>
		fib_heap_node<K,V>* merge_same_degree_node(fib_heap_node<K, V>* x, size_t n)
		{
			using node_type = fib_heap_node<K, V>;
			using node_ptr = node_type*;

			//辅助数组
			std::vector<node_ptr> vec(n);
			std::fill(vec.begin(), vec.end(), nullptr);

			//保存所有根节点
			std::vector<node_ptr> nodes{};
			node_ptr cur = x;
			do {
				nodes.push_back(cur);
				cur = cur->_right;
			} while (cur != x);

			//合并所有根节点
			for (auto node : nodes)
			{
				merge_if_could(node, vec);
			}
			
			//遍历查找最小节点
			node_ptr min = nullptr;
			for (auto ptr : vec)
			{
				if (ptr && (!min || ptr->_key < min->_key))
					min = ptr;
			}
			return min;
		}

		/*
		* @min：最小节点
		* @node:减小值的节点
		* @key:减小的值
		*/
		template<typename K, typename V>
		fib_heap_node<K, V>* decrease_key(fib_heap_node<K, V>* min, fib_heap_node<K, V>* node, const K& key)
		{
			using node_type = fib_heap_node<K, V>;
			using node_ptr = node_type*;

			assert(node->_key > key);	//key值必须比原有的小
			node_ptr parent = node->_parent;
			node->_key = key;
			if (parent && parent->_key > key)
			{
				cut_and_move_to_root_list(min, node);
				cascading_cut(min, parent);
			}

			if (node->_key < min->_key)
				min = node;
			return min;
		}

		/*
		* @min：最小节点
		* @node：剪切的节点
		*/
		template<typename K, typename V>
		fib_heap_node<K, V>* cut_and_move_to_root_list(fib_heap_node<K, V>* min, fib_heap_node<K, V>* node)
		{
			using node_type = fib_heap_node<K, V>;
			using node_ptr = node_type*;
			
			node=strip_node(node);		//剥离节点
			insert_before(min, node);	//插入到根链表中
			node->_marked = false;
			return min;
		}

		/*
		* @min:最小节点
		* @node:需要次级剪切的节点
		*/
		template<typename K, typename V>
		fib_heap_node<K, V>* cascading_cut(fib_heap_node<K, V>* min, fib_heap_node<K, V>* node)
		{
			using node_type = fib_heap_node<K, V>;
			using node_ptr = node_type*;
			node_ptr parent = node->_parent;
			if (parent)
			{
				if (!node->_marked)
					node->_marked = true;	//如果未被标记就标记
				else
				{
					cut_and_move_to_root_list(min, node);	//已标记就继续裁剪
					cascading_cut(min, parent);				//继续上溯
				}
			}
			return nullptr;
		}

		/*
		* @node:要剥离的节点
		*/
		template<typename K, typename V>
		fib_heap_node<K, V>* strip_node(fib_heap_node<K, V>* node)
		{
			using node_type = fib_heap_node<K, V>;
			using node_ptr = node_type*;

			node_ptr parent = node->_parent;
			node_ptr left = node->_left;
			node_ptr right = node->_right;

			if (parent)
			{
				--parent->_degree;
				if (parent->_child == node)
					parent->_child = right;
			}

			as_neighbor(left, right);

			node->_left = nullptr;
			node->_parent = nullptr;
			node->_right = nullptr;
			return node;
		}





		

	};
	

}




#endif




