#pragma once

#include"node_traits.h"
#include"binary_tree_node.h"


namespace fantasil {

	enum class rb_clr {
		black, red
	};



	template<typename ostream>
	ostream& operator<<(ostream& os, const rb_clr& clr)
	{
		const char* msg = clr == rb_clr::black ? "black" : "red";
		os << msg;
		return os;
	}

	template<typename K, typename V>
	struct rb_tree_node {
		using key_type = K;
		using value_type = V;
		using color_type = rb_clr;
		using refernce = value_type&;
		using pointer = value_type*;
		using node_type = rb_tree_node<K, V>;
		using node_ptr = node_type*;
		using node_category = rb_tree_node_tag;

		template<typename U, typename M, typename ostream>
		friend ostream& operator<<(ostream& os, const rb_tree_node<U, M>& node);
	public:
		key_type _key;
		value_type _val;
		node_ptr _parent;
		node_ptr _left;
		node_ptr _right;
		color_type _clr;
	public:
		rb_tree_node(const key_type& key, const value_type& val, node_ptr parent = nullptr, node_ptr left = nullptr, node_ptr right = nullptr, rb_clr clr = rb_clr::black)
			:_key{ key }, _val{ val }, _parent{ parent }, _left{ left }, _right{ right }, _clr{ clr } {}
		~rb_tree_node() { _key.~K(); _val.~V(); _parent = _left = _right = nullptr; }
	};

	template<typename K, typename V, typename ostream>
	ostream& operator<<(ostream& os, const rb_tree_node<K, V>& node)
	{
		os << "{" << node._key << "," << node._val << "," << node._clr << "} ";
		return os;
	}

	namespace rb_tree_node_helper {
		/*---------------------------------------------------declare---------------------------------------------------------------*/
		//���ýڵ���ɫ
		template<rb_tree_node_type Node>
		typename Node::color_type color(Node* node);
		template<rb_tree_node_type Node>
		void color(Node* node, typename Node::color_type clr);
		//���ýڵ����
		template<rb_tree_node_type Node>
		typename Node::key_type key(Node* node);
		template<rb_tree_node_type Node>
		void key(Node* node,const typename Node::key_type key);
		//�����ڵ����ɫ
		template<rb_tree_node_type Node>
		void exchange_color(Node* lhs, Node* rhs);
		//�鿴��ɫ
		template<rb_tree_node_type Node>
		bool is_black(Node* node);
		template<rb_tree_node_type Node>
		bool is_red(Node* node);

		/*----------����ɾ��ϵ�к���------------*/

		//���������1)	����һ���ڵ�
		template<rb_tree_node_type Node, typename Cmp>
		Node* rb_tree_insert(Node* root, Node* to_insert, Cmp cmp);
		//���������2)	����һ���ڵ����
		template<rb_tree_node_type Node, typename Alloc, typename Cmp>
		Node* rb_tree_insert(Node* root, const typename Node::key_type key, const typename Node::value_type& val, Alloc& alloc, Cmp cmp);
		//���������3)	��pair�й���ڵ����
		template<rb_tree_node_type Node, typename Alloc, typename Cmp>
		Node* rb_tree_insert(Node* root, const std::pair<typename Node::key_type, typename Node::value_type>& pr, Alloc& alloc, Cmp cmp);
		//���������5)	��pair�������й���ڵ����
		template<rb_tree_node_type Node, typename Alloc, typename Cmp, input_iterator_of_pair<typename Node::key_type,typename Node::value_type> Iter>
		Node* rb_tree_insert(Node* root, Iter begin, Iter end, Alloc& alloc, Cmp cmp);
		//ɾ��һ���ڵ�
		template<rb_tree_node_type Node,typename Alloc>
		Node* rb_tree_erase(Node* root, Node* node,Alloc& alloc);
		//���������ڵ��ʱ���ʹ�õ���ڽڵ������������key
		template<rb_tree_node_type Node,typename Alloc>
		Node* rb_tree_erase_impl(Node* root, Node* to_delete,Node* real_erase_node, Alloc& alloc);
		//�����뷢�����������ĺ�ɫ�ڵ�ʱ���ͻ�����insert_fixup
		template<rb_tree_node_type Node>
		Node* rb_tree_insert_fixup(Node* root, Node* node);
		//��ɾ��һ����ɫ�ڵ��ʱ�򣬾ͻ�����erase_fixup
		template<rb_tree_node_type Node>
		Node* rb_tree_erase_fixup(Node* root, Node* read_node_to_erase);
		//�ȽϽڵ��ʱ���ʹ�õ������������������key
		template<rb_tree_node_type Node,typename Cmp>
		Node* rb_tree_insert_prework(Node* root, Node* to_insert, Cmp cmp);


		/*----------��������---------*/

		//ͨ��initializer_list<pair<K,V>>��������
		template<rb_tree_node_type Node,typename Alloc,typename Cmp>
		Node* make_rb_tree(std::initializer_list<std::pair<typename node_traits<Node>::key_type, typename node_traits<Node>::value_type>> ilist, Alloc& alloc, Cmp cmp);
		

		//ͨ��pair<K,V>�����й�������
		template<rb_tree_node_type Node,input_iterator_of_pair<typename node_traits<Node>::key_type,typename node_traits<Node>::value_type> Iter,typename Alloc,typename Cmp>
		Node* make_rb_tree(Iter begin, Iter end, Alloc& alloc, Cmp cmp);
		

		//���ٺ����
		template<rb_tree_node_type Node,typename Alloc>
		void destory_rb_tree(Node* root, Alloc& alloc);


		/*---------------------------------------------------impl---------------------------------------------------------------*/
		template<rb_tree_node_type Node>
		typename Node::color_type color(Node* node)
		{
			using color_type = typename Node::color_type;
			if (node)
				return node->_clr;
			return color_type::black;
		}
		template<rb_tree_node_type Node>
		void color(Node* node, typename Node::color_type clr)
		{
			if (node)
				node->_clr = clr;
		}

		template<rb_tree_node_type Node>
		typename Node::key_type key(Node* node)
		{
			if (!node)
				throw std::logic_error{ "dereference nullptr" };
			return node->_key;
		}

		template<rb_tree_node_type Node>
		void key(Node* node, const typename Node::key_type key)
		{
			if (!node)
				throw std::logic_error{ "dereference nullptr" };
			node->_key = key;
		}

		template<rb_tree_node_type Node>
		void exchange_color(Node* lhs, Node* rhs)
		{
			using color_type = typename Node::color_type;
			color_type tmp = lhs->_clr;
			lhs->_clr = rhs->_clr;
			rhs->_clr = tmp;
		}

		template<rb_tree_node_type Node>
		bool is_black(Node* node)
		{
			using color_type = typename Node::color_type;
			if (!node)
				return true;
			return node->_clr == color_type::black;
		}
		template<rb_tree_node_type Node>
		bool is_red(Node* node)
		{
			using color_type = typename Node::color_type;
			if (!node)
				return false;
			return node->_clr == color_type::red;
		}

		template<rb_tree_node_type Node, typename Cmp>
		Node* rb_tree_insert(Node* root, Node* to_insert, Cmp cmp)
		{
			using color_type = typename Node::color_type;
			root= rb_tree_insert_prework(root, to_insert, cmp);
			if(color(root)!=color_type::black)
				root->_clr = color_type::black;
			return rb_tree_insert_fixup(root, to_insert);
		}
		//
		template<rb_tree_node_type Node, typename Alloc, typename Cmp>
		Node* rb_tree_insert(Node* root, const std::pair<typename Node::key_type, typename Node::value_type>& pr, Alloc& alloc, Cmp cmp)
		{
			using node_ptr = Node*;
			node_ptr tmp = alloc.allocate(1);
			init_rb_tree_node_type(tmp, pr.first, pr.second);
			return rb_tree_insert(root, tmp, cmp);
		}

		template<rb_tree_node_type Node, typename Alloc, typename Cmp>
		Node* rb_tree_insert(Node* root, const typename Node::key_type key, const typename Node::value_type& val, Alloc& alloc, Cmp cmp)
		{
			using node_ptr = Node*;
			node_ptr tmp = alloc.allocate(1);
			init_rb_tree_node_type(tmp, key, val);
			return rb_tree_insert(root, tmp, cmp);
		}

		template<rb_tree_node_type Node, typename Alloc, typename Cmp, input_iterator_of_pair<typename Node::key_type, typename Node::value_type> Iter>
		Node* rb_tree_insert(Node* root, Iter begin, Iter end, Alloc& alloc, Cmp cmp)
		{
			using node_ptr = Node*;
			node_ptr tmp = nullptr;
			for (auto p = begin; p != end; ++p)
			{
				tmp = alloc.allocate(1);
				init_rb_tree_node_type(tmp, p->first, p->second);
				root = rb_tree_insert(root, tmp, cmp);
			}
			return root;
		}



		//�����뷢�����������ĺ�ɫ�ڵ��ʱ�򣬾ͻ�����insert_fixup
		template<rb_tree_node_type Node>
		Node* rb_tree_insert_fixup(Node* root, Node* node)
		{
			using node_ptr = Node*;
			using color_type = typename Node::color_type;
			node_ptr p = node->_parent;

			if (color(p) == color_type::red)
			{
				/*
				* ��ʱ��֪������
				* 1 node,p��gpһ�����ڣ���node,p�Ǻ�ɫ�ڵ�,gp�Ǻ�ɫ�ڵ�
				*/
				node_ptr gp = binary_tree_node_helper::grandparent(node);
				node_ptr uncle = binary_tree_node_helper::uncle(node);

				//case 1:���ڵ���常�ڵ㶼�Ǻ�ɫ��,������ɫ
				if (color(uncle) == color_type::red)
				{
					uncle->_clr = p->_clr = color_type::black;
					gp->_clr = color_type::red;
					if (gp == root)
					{
						gp->_clr = color_type::black;
						return gp;
					}
					return rb_tree_insert_fixup(root, gp);
				}

				if (binary_tree_node_helper::is_left(p))
				{
					//case 2:node--p--gp�Ƿ���һ��ֱ��?��node��p��������p��gp������
					if (!binary_tree_node_helper::is_left(node))
					{
						binary_tree_node_helper::turn_left(node);
						std::swap(p, node);	//����node��p�ĸ��ڵ��ˣ�Ϊ����һ�£�����ʹ��p����node�ĸ��ڵ�,�ڵ����Ǻ�ɫ
					}

					//case 3: node--p--gp��һ��ֱ���ˡ�����һ����ת�ı���ɫ����
					binary_tree_node_helper::turn_right(p);
					p->_clr = color_type::black;
					gp->_clr = color_type::red;

					if (gp == root)
						return p;
					return root;
				}
				else {
					//�����ľ���ı�
					if (!binary_tree_node_helper::is_right(node))
					{
						binary_tree_node_helper::turn_right(node);
						std::swap(p, node);
					}
					binary_tree_node_helper::turn_left(p);
					p->_clr = color_type::black;
					gp->_clr = color_type::red;
					           
					if (gp == root)
						return p;
					return root;
				}
			}

			return root;
                   
		}

		template<rb_tree_node_type Node,typename Alloc>
		Node* rb_tree_erase(Node* root, Node* node,Alloc& alloc)
		{
			using node_ptr = Node*;
			node_ptr real_erase_node = binary_tree_node_helper::binary_tree_location_real_erase_node(node);
			root=rb_tree_erase_fixup(root, real_erase_node);
			return rb_tree_erase_impl (root, node, real_erase_node, alloc);
		}

		template<rb_tree_node_type Node, typename Alloc>
		Node* rb_tree_erase_impl(Node* root, Node* to_delete, Node* real_erase_node, Alloc& alloc)
		{
			using node_ptr = Node*;
			using color_type = typename Node::color_type;

			if (to_delete != real_erase_node)
			{
				to_delete->_key = real_erase_node->_key;
				to_delete->_val = real_erase_node->_val;
			}
			
			node_ptr p = binary_tree_node_helper::parent(real_erase_node);
			node_ptr child = real_erase_node->_left!=nullptr ? real_erase_node->_left : real_erase_node->_right;

			if (binary_tree_node_helper::is_root(real_erase_node))
			{
				root = child;
				if (root)
				{
					root->_parent = nullptr;
					color(root, color_type::black);
				}
			}
			else {
				if (binary_tree_node_helper::is_left(real_erase_node))
					binary_tree_node_helper::as_left(p, child);
				else
					binary_tree_node_helper::as_right(p, child);
			}
			std::destroy_at(real_erase_node);
			alloc.deallocate(real_erase_node, 1);
			return root;
			

		}

		//��ɾ��һ����ɫ�ڵ��ʱ�򣬾ͻ�����erase_fixup
		template<rb_tree_node_type Node>
		Node* rb_tree_erase_fixup(Node* root, Node* node)
		{
			using color_type = typename Node::color_type;
			using node_ptr = Node*;
			//���ɾ�����Ǻ�ɫ�ڵ㣬����Ҫ�����޲���֤�����������

			if (!node || !root)
				return root;
			node_ptr p = nullptr;
			node_ptr bro = nullptr;

			
			while (!binary_tree_node_helper::is_root(node) && color(node) == color_type::black) {

				p = binary_tree_node_helper::parent(node);
				bro = binary_tree_node_helper::brother(node);

				if (binary_tree_node_helper::is_left(node))
				{
					//���ڵ������ӵ�ʱ��
					if (color(bro) == color_type::red)
					{
						//case 1:�ֵܽڵ��Ǻ�ɫ��,����һ����ת����ɫ����,��ʱ�ڵ���ֵܽڵ��Ϊ��ɫ
						binary_tree_node_helper::turn_left(bro);
						exchange_color(p, bro);
						if (root == p)
							root = bro;
					}
					else {
						//case 2:�ֵܽڵ��Ǻ�ɫ�ģ����������Ӷ��Ǻ�ɫ��
						if (is_black(bro->_left) && is_black(bro->_right))
						{
							color(bro, color_type::red);
							if (is_red(p))
							{
								color(p, color_type::black);
								return root;
							}
							else
							{
								node = p;
							}
							
						}
						else {
							if (is_black(bro->_right))
							{
								//case 3:�ֵܽڵ��Ǻ�ɫ�ģ������Ǻ�ɫ�ģ��Һ����Ǻ�ɫ��
								node_ptr left = bro->_left;
								binary_tree_node_helper::turn_right(bro->_left);
								exchange_color(left, bro);
								bro = left;
							}

							//case 4:�ֵܽڵ��Ǻ�ɫ�ģ��Һ����Ǻ�ɫ��
							binary_tree_node_helper::turn_left(bro);
							exchange_color(p, bro);
							color(bro->_right, color_type::black);
							color(node, color_type::red);
							if (p == root)
								root = bro;
						}

					}
				}
				else {
					if (color(bro) == color_type::red)
					{
						//case 1:�ֵܽڵ�w�Ǻ�ɫ��
						binary_tree_node_helper::turn_right(bro);
						exchange_color(p, bro);
						if (root == p)
							root = bro;
					}
					else {
						if (is_black(bro->_left) && is_black(bro->_right))
						{
							//case 2:w���Ӷ��Ǻ�ɫ��
							color(bro, color_type::red);
							if (is_red(p))
							{
								color(p, color_type::black);
								return root;
							}
							else {
								node = p;
							}
						}
						else {
							if (is_black(bro->_left))
							{
								//case 3:wl�Ǻ�ɫ��
								node_ptr wr = bro->_right;
								binary_tree_node_helper::turn_left(wr);
								exchange_color(bro, wr);
								bro = wr;
							}

							//case 4:wl�Ǻ�ɫ��,����������Ҫ��
							binary_tree_node_helper::turn_right(bro);
							exchange_color(bro, p);
							color(bro->_left, color_type::black);
							color(node, color_type::red);
							if (p == root)
								root = bro;
						}
					}
				}

			}

			color(root, color_type::black);
			return root;
		}

		
		template<rb_tree_node_type Node, typename Cmp>
		Node* rb_tree_insert_prework(Node* root, Node* to_insert, Cmp cmp)
		{
			using node_ptr = typename node_traits<Node>::node_ptr;
			if (!root)
				return to_insert;
			//�Ӹ��ڵ㿪ʼѰ������������Ҷ�ӽڵ�
			node_ptr cur = root;

			while (cur)
			{
				if (cmp(to_insert->_key, cur->_key))
					if (cur->_left)
						cur = cur->_left;
					else
						break;
				else
					if (cur->_right)
						cur = cur->_right;
					else
						break;
			}

			if (cmp(to_insert->_key, cur->_key))
				binary_tree_node_helper::as_left(cur, to_insert);
			else
				binary_tree_node_helper::as_right(cur, to_insert);

			return root;
		}

		template<rb_tree_node_type Node, typename Alloc, typename Cmp>
		Node* make_rb_tree(std::initializer_list<std::pair<typename node_traits<Node>::key_type, typename node_traits<Node>::value_type>> ilist, Alloc& alloc, Cmp cmp)
		{
			using iterator = decltype(ilist.begin());
			return make_rb_tree<Node, iterator>(ilist.begin(), ilist.end(), alloc, cmp);
		}

		template<rb_tree_node_type Node, input_iterator_of_pair<typename Node::key_type, typename node_traits<Node>::value_type> Iter, typename Alloc, typename Cmp>
		Node* make_rb_tree(Iter begin, Iter end, Alloc& alloc, Cmp cmp)
		{
			using node_ptr = Node*;

			node_ptr root = nullptr;
			node_ptr tmp = nullptr;
			for (auto p = begin; p != end; ++p)
			{
				tmp = alloc.allocate(1);
				init_rb_tree_node_type(tmp, p->first, p->second);
				root = rb_tree_insert(root, tmp, cmp);
			}
			return root;
		}

		template<rb_tree_node_type Node, typename Alloc>
		void destory_rb_tree(Node* root, Alloc& alloc)
		{
			using node_ptr = Node*;

			if (!root)
				return;
			
			auto p = binary_tree_node_helper::extreame_left_node(root);
			auto q = binary_tree_node_helper::successor(p);
			while (p)
			{
				std::destroy_at(p);
				alloc.deallocate(p, 1);
				p = q;
				q = binary_tree_node_helper::successor(p);
			}

		}

	}

}

///*
//* ��ȡnode���ֵܽڵ�,��node�����
//*/
//template<binary_tree_node_type Node>
//inline typename node_traits<Node>::node_ptr brother(Node* node)
//{
//	using node_ptr =  typename node_traits<Node>::node_ptr;
//
//	if (!node)
//		return nullptr;
//	
//	if (!node->_parent)
//		return nullptr;
//
//	if (is_left(node))
//		return node->_parent->_right;
//	else
//		return node->_parent->_left;
//		
//}
//
///*
//* ��ȡnode���游�ڵ�
//*/
//template<binary_tree_node_type Node>
//inline typename node_traits<Node>::node_ptr grandparent(Node* node)
//{
//	using node_ptr =  typename node_traits<Node>::node_ptr;
//	if (!node)
//		return nullptr;
//	if (node->_parent)
//		return node->_parent->_parent;
//	return nullptr;
//}
//
///*
//* ��ȡ�常�ڵ�
//*/
//template<binary_tree_node_type Node>
//inline typename node_traits<Node>::node_ptr uncle(Node* node)
//{
//	using node_ptr =  typename node_traits<Node>::node_ptr;
//	if (!node)
//		return nullptr;
//	if (node->_parent)
//		return brother(node->_parent);
//	return nullptr;
//	
//}
//
//
///*
//* �Խڵ��������ת
//* ��ǰ�ڵ�������Һ���
//*/
//template<binary_tree_node_type Node>
//void turn_left(Node* node)
//{
//	using node_ptr =  typename node_traits<Node>::node_ptr;
//	if (!node)
//		return;		//�ڵ�������
//	node_ptr parent = node->_parent;
//	if (!parent)
//		return;		//���ڵ�������
//	if (!is_right(node))
//		return;		//��ǰ�ڵ�����Ǹ��ڵ���Һ���
//	node_ptr grandparent = parent->_parent;
//
//	if (grandparent)
//	{
//		if (is_left(parent))
//			as_left(grandparent, node);
//		else
//			as_right(grandparent, node);
//	}
//
//	as_right(parent, node->_left);
//	as_left(node, parent);
//}
//
//
///*
//* �Խڵ��������ת
//*/
//template<binary_tree_node_type Node>
//void turn_right(Node* node)
//{
//	using node_ptr =  typename node_traits<Node>::node_ptr;
//	if (!node)
//		return;		//�ڵ�������
//	node_ptr parent = node->_parent;
//	if (!parent)
//		return;		//���ڵ�������
//	if (!is_left(node))
//		return;		//��ǰ�ڵ�����Ǹ��ڵ������
//	node_ptr grandparent = parent->_parent;
//
//	if (grandparent)
//	{
//		if (is_left(parent))
//			as_left(grandparent, node);
//		else
//			as_right(grandparent, node);
//	}
//
//	as_left(parent, node->_right);
//	as_right(node, parent);
//}
//

		//template<rb_tree_node_type Node, typename Alloc, typename Cmp>
		//Node* rb_tree_insert(Node* root, const typename Node::key_type key, const typename node_traits<Node>::value_type& val, Alloc& alloc, Cmp cmp)
		//{
		//	//����ڵ㲢����
		//	using node_ptr = Node*;
		//	static_assert(std::is_same_v<Node, typename   std::allocator_traits<Alloc>::value_type>);
		//	node_ptr tmp = alloc.allocate(1);
		//	init_rb_tree_node_type(tmp, key, val);
		//	return rb_tree_insert(root, tmp, cmp);
		//}

		//template<rb_tree_node_type Node, typename Alloc, typename Cmp>
		//Node* rb_tree_insert(Node* root, const std::pair<typename Node::key_type, typename node_traits<Node>::value_type>& pr, Alloc& alloc, Cmp cmp)
		//{	//����ڵ㲢����
		//	using node_ptr = Node*;
		//	static_assert(std::is_same_v<Node, typename std::allocator_traits<Alloc>::value_type>);
		//	node_ptr tmp = alloc.allocate(1);
		//	init_rb_tree_node_type(tmp, pr.first, pr.second);
		//	return rb_tree_insert(root, tmp, cmp);
		//}

		/*template<rb_tree_node_type Node, input_iterator_type key_Iter, input_iterator_type val_Iter, typename Alloc, typename Cmp>
		Node* rb_tree_insert(Node* root, key_Iter kbegin, key_Iter kend, val_Iter vbegin, val_Iter vend, Alloc& alloc, Cmp cmp)
		{
			using node_ptr = Node*;
			static_assert(std::is_same_v<Node, typename std::allocator_traits<Alloc>::value_type>);
			size_t key_sz{}, val_sz{};
			if (is_same_v<std::random_access_iterator_tag, std::iterator_traits<key_Iter>::iterator_category>)
			{
				key_sz = kend - kbegin;
			}
			else
			{
				key_Iter cur = kbegin;
				while (cur != kend)
				{
					++key_sz;
					++cur;
				}
			}

			if (is_same_v<std::random_access_iterator_tag, std::iterator_traits<val_Iter>::iterator_category>)
				val_sz = vend - vbegin;
			else
			{
				val_Iter cur = vbegin;
				while (cur != vend)
				{
					++val_sz;
					++cur;
				}
			}

			if (key_sz != val_sz)
				throw std::logic_error{ "key_iter size not equal val_iter size" };

			node_ptr tmp = nullptr;
			for (auto p = kbegin, q = vbegin; p != kend; ++p)
			{
				tmp = alloc.allocate(1);
				root = init_rb_tree_node_type(tmp, *p, *q);
				rb_tree_insert(root, tmp, cmp);
			}

			return root;
		}*/

		/*template<rb_tree_node_type Node, typename Alloc, typename Cmp, input_iterator_of_pair<typename Node::key_type, typename Node::value_type> Iter>
		Node* rb_tree_insert(Node* root, Iter begin, Iter end, Alloc& alloc, Cmp cmp)
		{
			using node_ptr = Node*;
			static_assert(std::is_same_v<Node, typename std::allocator_traits<Alloc>::value_type>);
			node_ptr tmp{ nullptr };
			for (auto p = begin; p != end; ++p)
			{
				tmp = alloc.allocate(1);
				init_rb_tree_node_type(tmp, p->first, p->second);
				root = rb_tree_insert(root, tmp, cmp);
			}
			return root;
		}*/

		/*template<rb_tree_node_type Node, typename Alloc, typename Cmp>
		Node* rb_tree_insert(Node* root, size_t count, const std::pair<typename node_traits<Node>::key_type, typename node_traits<Node>::value_type>& pr, Alloc& alloc, Cmp cmp)
		{
			using node_ptr = Node*;
			static_assert(std::is_same_v<Node, typename std::allocator_traits<Alloc>::value_type>);
			node_ptr tmp{ nullptr };
			for (; count; --count)
			{
				tmp = alloc.allocate(1);
				init_rb_tree_node_type(tmp, pr.first, pr.second);
				root = rb_tree_insert(root, tmp, cmp);
			}
			return root;
		}

		template<rb_tree_node_type Node, typename Alloc, typename Cmp>
		Node* rb_tree_insert(Node* root, size_t count, const typename node_traits<Node>::key_type key, const typename node_traits<Node>::value_type& val, Alloc& alloc, Cmp cmp)
		{
			using node_ptr = Node*;
			static_assert(std::is_same_v<Node, typename std::allocator_traits<Alloc>::value_type>);
			node_ptr tmp{ nullptr };
			for (; count; --count)
			{
				tmp = alloc.allocate(1);
				init_rb_tree_node_type(tmp, key, val);
				root = rb_tree_insert(root, tmp, cmp);
			}
			return root;
		}*/

