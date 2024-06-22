#pragma once
#include<vector>
#include<cassert>
#include<numeric>

#pragma region describtion
/*
	쳲�������

	쳲������ѿ�����Ϊ쳲��������ļ��ϣ�


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
		
		//��������
		template<typename K, typename V>
		inline void as_neighbor(fib_heap_node<K, V>* left, fib_heap_node<K, V>* right);

		//������pos֮ǰ
		template<typename K, typename V>
		fib_heap_node<K,V>* insert_before(fib_heap_node<K, V>* pos, fib_heap_node<K, V>* node);

		//������pos֮��
		template<typename K, typename V>
		void insert_after(fib_heap_node<K, V>* pos, fib_heap_node<K, V>* node);

		//���ӳ�Ϊ����
		template<typename K,typename V>
		void link_as_child(fib_heap_node<K, V>* pos, fib_heap_node<K, V>* node);

		//����в���һ���ڵ�
		template<typename K,typename V>
		fib_heap_node<K, V>* insert(fib_heap_node<K, V>* min, const std::pair<K, V>& pair);

		//��ȡ��С�ڵ�
		template<typename K,typename V>
		fib_heap_node<K, V>* extract_min(fib_heap_node<K, V>* min,size_t nums);


		//ɾ���ڵ�
		template<typename K, typename V>
		void erase_node(fib_heap_node<K, V>* node);

		//��x�ĺ��ӷ���y���ڵ�������
		template<typename K, typename V>
		void put_x_child_into_y_list(fib_heap_node<K, V>* x, fib_heap_node<K, V>* y);

		//��x�ĺ��ӷ���x���ڵ�������
		template<typename K, typename V>
		void put_x_child_into_parent_list(fib_heap_node<K, V>* x);

		//�ϲ�����������ͬ�Ľڵ�
		template<typename K, typename V>
		fib_heap_node<K, V>* merge_two_same_degree_node(fib_heap_node<K, V>* x, fib_heap_node<K, V>* y);

		//����ܹ��ϲ��ͺϲ�
		template<typename K, typename V>
		fib_heap_node<K, V>* merge_if_could(fib_heap_node<K, V>* x, std::vector<fib_heap_node<K, V>*>& vec);

		//�ϲ���ͬ�����Ľڵ�
		template<typename K, typename V>
		fib_heap_node<K, V>* merge_same_degree_node(fib_heap_node<K, V>* x, size_t n);

		//��Сĳ���ڵ��keyֵ
		template<typename K,typename V>
		fib_heap_node<K, V>* decrease_key(fib_heap_node<K, V>* min, fib_heap_node<K, V>* node, const K& key);

		//����
		template<typename K,typename V>
		fib_heap_node<K, V>* cut_and_move_to_root_list(fib_heap_node<K, V>* min, fib_heap_node<K, V>* node);

		//�μ�����
		template<typename K,typename V>
		fib_heap_node<K, V>* cascading_cut(fib_heap_node<K, V>* min, fib_heap_node<K, V>* node);

		//����ڵ㣬����������
		template<typename K,typename V>
		fib_heap_node<K, V>* strip_node(fib_heap_node<K, V>* node);




		/*----------------------------------------------------------------impl-----------------------------------------------------------------*/

		//����,����
		/*
		* @left ����
		* @right ����
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
		* @pos:����λ��
		* @node:����ڵ�
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
			//ά��������Ϣ
			as_neighbor(left, node);
			as_neighbor(node, pos);

			//ά��������Ϣ
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
		* @pos:����λ��
		* @node:����ڵ�
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
			//ά��������Ϣ
			as_neighbor(pos, node);
			as_neighbor(node, right);
			//ά��������Ϣ
			node->_parent = parent;
			if (parent)
				++parent->_degree;

			if (pos->_key < node->_key)
				return pos;
			else
				return node;
		}

		/*
		* @pos:�����λ�ã�pos���Ǹ��ڵ�
		* @node:������Ľڵ�
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

		

	

		//ɾ��һ���ڵ�
		/*
		* @node ��ɾ���Ľڵ�
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

		//��x�ĺ�������y���ڵ�������
		/*
		* @x x���ӽڵ�
		* @y y���ڵ�����
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

		//�ϲ������ڵ�
		/*
		* @x ��һ���ڵ�
		* @y �ڶ����ڵ�
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

		//����������
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
		
		/*�ϲ��ڵ��������������ͬ�����Ľڵ�
		* @x x�ڵ����ڵ�����
		* @n ����Ľڵ����
		*/
		template<typename K,typename V>
		fib_heap_node<K,V>* merge_same_degree_node(fib_heap_node<K, V>* x, size_t n)
		{
			using node_type = fib_heap_node<K, V>;
			using node_ptr = node_type*;

			//��������
			std::vector<node_ptr> vec(n);
			std::fill(vec.begin(), vec.end(), nullptr);

			//�������и��ڵ�
			std::vector<node_ptr> nodes{};
			node_ptr cur = x;
			do {
				nodes.push_back(cur);
				cur = cur->_right;
			} while (cur != x);

			//�ϲ����и��ڵ�
			for (auto node : nodes)
			{
				merge_if_could(node, vec);
			}
			
			//����������С�ڵ�
			node_ptr min = nullptr;
			for (auto ptr : vec)
			{
				if (ptr && (!min || ptr->_key < min->_key))
					min = ptr;
			}
			return min;
		}

		/*
		* @min����С�ڵ�
		* @node:��Сֵ�Ľڵ�
		* @key:��С��ֵ
		*/
		template<typename K, typename V>
		fib_heap_node<K, V>* decrease_key(fib_heap_node<K, V>* min, fib_heap_node<K, V>* node, const K& key)
		{
			using node_type = fib_heap_node<K, V>;
			using node_ptr = node_type*;

			assert(node->_key > key);	//keyֵ�����ԭ�е�С
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
		* @min����С�ڵ�
		* @node�����еĽڵ�
		*/
		template<typename K, typename V>
		fib_heap_node<K, V>* cut_and_move_to_root_list(fib_heap_node<K, V>* min, fib_heap_node<K, V>* node)
		{
			using node_type = fib_heap_node<K, V>;
			using node_ptr = node_type*;
			
			node=strip_node(node);		//����ڵ�
			insert_before(min, node);	//���뵽��������
			node->_marked = false;
			return min;
		}

		/*
		* @min:��С�ڵ�
		* @node:��Ҫ�μ����еĽڵ�
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
					node->_marked = true;	//���δ����Ǿͱ��
				else
				{
					cut_and_move_to_root_list(min, node);	//�ѱ�Ǿͼ����ü�
					cascading_cut(min, parent);				//��������
				}
			}
			return nullptr;
		}

		/*
		* @node:Ҫ����Ľڵ�
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




