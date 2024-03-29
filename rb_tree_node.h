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
		//设置节点颜色
		template<rb_tree_node_type Node>
		typename Node::color_type color(Node* node);
		template<rb_tree_node_type Node>
		void color(Node* node, typename Node::color_type clr);
		//设置节点键域
		template<rb_tree_node_type Node>
		typename Node::key_type key(Node* node);
		template<rb_tree_node_type Node>
		void key(Node* node,const typename Node::key_type key);
		//交换节点的颜色
		template<rb_tree_node_type Node>
		void exchange_color(Node* lhs, Node* rhs);
		//查看颜色
		template<rb_tree_node_type Node>
		bool is_black(Node* node);
		template<rb_tree_node_type Node>
		bool is_red(Node* node);

		/*----------插入删除系列函数------------*/

		//红黑树插入1)	插入一个节点
		template<rb_tree_node_type Node, typename Cmp>
		Node* rb_tree_insert(Node* root, Node* to_insert, Cmp cmp);
		//红黑树插入2)	构造一个节点插入
		template<rb_tree_node_type Node, typename Alloc, typename Cmp>
		Node* rb_tree_insert(Node* root, const typename Node::key_type key, const typename Node::value_type& val, Alloc& alloc, Cmp cmp);
		//红黑树插入3)	从pair中构造节点插入
		template<rb_tree_node_type Node, typename Alloc, typename Cmp>
		Node* rb_tree_insert(Node* root, const std::pair<typename Node::key_type, typename Node::value_type>& pr, Alloc& alloc, Cmp cmp);
		//红黑树插入5)	从pair的序列中构造节点插入
		template<rb_tree_node_type Node, typename Alloc, typename Cmp, input_iterator_of_pair<typename Node::key_type,typename Node::value_type> Iter>
		Node* rb_tree_insert(Node* root, Iter begin, Iter end, Alloc& alloc, Cmp cmp);
		//删除一个节点
		template<rb_tree_node_type Node,typename Alloc>
		Node* rb_tree_erase(Node* root, Node* node,Alloc& alloc);
		//交换两个节点的时候会使用到红黑节点的特殊属性域key
		template<rb_tree_node_type Node,typename Alloc>
		Node* rb_tree_erase_impl(Node* root, Node* to_delete,Node* real_erase_node, Alloc& alloc);
		//当插入发现两个连续的红色节点时，就会启动insert_fixup
		template<rb_tree_node_type Node>
		Node* rb_tree_insert_fixup(Node* root, Node* node);
		//当删除一个黑色节点的时候，就会启动erase_fixup
		template<rb_tree_node_type Node>
		Node* rb_tree_erase_fixup(Node* root, Node* read_node_to_erase);
		//比较节点的时候会使用到红黑树的特殊属性域key
		template<rb_tree_node_type Node,typename Cmp>
		Node* rb_tree_insert_prework(Node* root, Node* to_insert, Cmp cmp);


		/*----------构造红黑树---------*/

		//通过initializer_list<pair<K,V>>构造红黑树
		template<rb_tree_node_type Node,typename Alloc,typename Cmp>
		Node* make_rb_tree(std::initializer_list<std::pair<typename node_traits<Node>::key_type, typename node_traits<Node>::value_type>> ilist, Alloc& alloc, Cmp cmp);
		

		//通过pair<K,V>的序列构造红黑树
		template<rb_tree_node_type Node,input_iterator_of_pair<typename node_traits<Node>::key_type,typename node_traits<Node>::value_type> Iter,typename Alloc,typename Cmp>
		Node* make_rb_tree(Iter begin, Iter end, Alloc& alloc, Cmp cmp);
		

		//销毁红黑树
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



		//当插入发现两个连续的红色节点的时候，就会启动insert_fixup
		template<rb_tree_node_type Node>
		Node* rb_tree_insert_fixup(Node* root, Node* node)
		{
			using node_ptr = Node*;
			using color_type = typename Node::color_type;
			node_ptr p = node->_parent;

			if (color(p) == color_type::red)
			{
				/*
				* 此时已知条件：
				* 1 node,p和gp一定存在，且node,p是红色节点,gp是黑色节点
				*/
				node_ptr gp = binary_tree_node_helper::grandparent(node);
				node_ptr uncle = binary_tree_node_helper::uncle(node);

				//case 1:父节点和叔父节点都是红色的,重新着色
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
					//case 2:node--p--gp是否是一条直线?即node是p的左孩子且p是gp的左孩子
					if (!binary_tree_node_helper::is_left(node))
					{
						binary_tree_node_helper::turn_left(node);
						std::swap(p, node);	//现在node是p的父节点了，为保持一致，交换使得p仍是node的父节点,节点仍是红色
					}

					//case 3: node--p--gp是一条直线了。经过一次旋转改变颜色即可
					binary_tree_node_helper::turn_right(p);
					p->_clr = color_type::black;
					gp->_clr = color_type::red;

					if (gp == root)
						return p;
					return root;
				}
				else {
					//上述的镜像改变
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

		//当删除一个黑色节点的时候，就会启动erase_fixup
		template<rb_tree_node_type Node>
		Node* rb_tree_erase_fixup(Node* root, Node* node)
		{
			using color_type = typename Node::color_type;
			using node_ptr = Node*;
			//如果删除的是黑色节点，就需要进行修补保证红黑树的性质

			if (!node || !root)
				return root;
			node_ptr p = nullptr;
			node_ptr bro = nullptr;

			
			while (!binary_tree_node_helper::is_root(node) && color(node) == color_type::black) {

				p = binary_tree_node_helper::parent(node);
				bro = binary_tree_node_helper::brother(node);

				if (binary_tree_node_helper::is_left(node))
				{
					//当节点是左孩子的时候
					if (color(bro) == color_type::red)
					{
						//case 1:兄弟节点是红色的,进行一次旋转和颜色交换,此时节点的兄弟节点必为黑色
						binary_tree_node_helper::turn_left(bro);
						exchange_color(p, bro);
						if (root == p)
							root = bro;
					}
					else {
						//case 2:兄弟节点是黑色的，且两个孩子都是黑色的
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
								//case 3:兄弟节点是黑色的，左孩子是红色的，右孩子是黑色的
								node_ptr left = bro->_left;
								binary_tree_node_helper::turn_right(bro->_left);
								exchange_color(left, bro);
								bro = left;
							}

							//case 4:兄弟节点是黑色的，右孩子是红色的
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
						//case 1:兄弟节点w是红色的
						binary_tree_node_helper::turn_right(bro);
						exchange_color(p, bro);
						if (root == p)
							root = bro;
					}
					else {
						if (is_black(bro->_left) && is_black(bro->_right))
						{
							//case 2:w孩子都是黑色的
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
								//case 3:wl是黑色的
								node_ptr wr = bro->_right;
								binary_tree_node_helper::turn_left(wr);
								exchange_color(bro, wr);
								bro = wr;
							}

							//case 4:wl是红色的,这是我们需要的
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
			//从根节点开始寻找满足条件的叶子节点
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
//* 获取node的兄弟节点,对node做检查
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
//* 获取node的祖父节点
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
//* 获取叔父节点
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
//* 对节点进行左旋转
//* 当前节点必须是右孩子
//*/
//template<binary_tree_node_type Node>
//void turn_left(Node* node)
//{
//	using node_ptr =  typename node_traits<Node>::node_ptr;
//	if (!node)
//		return;		//节点必须存在
//	node_ptr parent = node->_parent;
//	if (!parent)
//		return;		//父节点必须存在
//	if (!is_right(node))
//		return;		//当前节点必须是父节点的右孩子
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
//* 对节点进行右旋转
//*/
//template<binary_tree_node_type Node>
//void turn_right(Node* node)
//{
//	using node_ptr =  typename node_traits<Node>::node_ptr;
//	if (!node)
//		return;		//节点必须存在
//	node_ptr parent = node->_parent;
//	if (!parent)
//		return;		//父节点必须存在
//	if (!is_left(node))
//		return;		//当前节点必须是父节点的左孩子
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
		//	//构造节点并插入
		//	using node_ptr = Node*;
		//	static_assert(std::is_same_v<Node, typename   std::allocator_traits<Alloc>::value_type>);
		//	node_ptr tmp = alloc.allocate(1);
		//	init_rb_tree_node_type(tmp, key, val);
		//	return rb_tree_insert(root, tmp, cmp);
		//}

		//template<rb_tree_node_type Node, typename Alloc, typename Cmp>
		//Node* rb_tree_insert(Node* root, const std::pair<typename Node::key_type, typename node_traits<Node>::value_type>& pr, Alloc& alloc, Cmp cmp)
		//{	//构造节点并插入
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

