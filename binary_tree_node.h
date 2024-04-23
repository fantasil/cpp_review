#pragma once
#include<memory>
#include"node_traits.h"
#include<type_traits>
#include<memory>
#include<stack>
#include<queue>

namespace fantasil
{

	//一个二叉树节点
	template<typename T>
	struct binary_tree_node {
		using value_type = T;
		using reference = value_type&;
		using pointer = value_type*;
		using node_category = binary_tree_node_tag;
		using node_type = binary_tree_node<T>;
		using node_ptr = node_type*;
		template<typename U, typename ostream>
		friend ostream& operator<<(ostream& os, const binary_tree_node<U>& node);


	public:
		value_type _val;
		node_ptr _parent;
		node_ptr _left;
		node_ptr _right;
	public:
		binary_tree_node(const value_type& val, node_ptr parent = nullptr, node_ptr left = nullptr, node_ptr right = nullptr) :_val{ val }, _parent{ parent }, _left{ left }, _right{ right } {}
		~binary_tree_node() { _val.~T(); _parent = _left = _right = nullptr; }
	};

	//输出节点
	template<typename T, typename ostream>
	ostream& operator<<(ostream& os, const binary_tree_node<T>& node)
	{
		os << node._val;
		return os;
	}



	namespace binary_tree_node_helper {

		/*------------------------------------------------------------------------------declare-------------------------------------------------------------------------------------------*/
		//节点的父节点
		template<binary_tree_node_type Node>
		inline Node* parent(Node* node);
		//节点的左孩子
		template<binary_tree_node_type Node>
		inline Node* left(Node* node);
		//节点的右孩子
		template<binary_tree_node_type Node>
		inline Node* right(Node* node);
		//节点的兄弟节点
		template<binary_tree_node_type Node>
		inline Node* brother(Node* node);
		//节点的叔节点
		template<binary_tree_node_type Node>
		inline Node* uncle(Node* node);
		//节点的祖父节点
		template<binary_tree_node_type Node>
		inline Node* grandparent(Node* node);
		//节点高度
		template<binary_tree_node_type Node>
		size_t height(Node* node);
		//位于最左的节点
		template<binary_tree_node_type Node>
		Node* extreame_left_node(Node* root);
		//位于最右的节点
		template<binary_tree_node_type Node>
		Node* extreame_right_node(Node* root);

		//连接左孩子和父节点
		template<binary_tree_node_type Node>
		inline void as_left(Node* parent, Node* left);
		//连接右孩子和父节点
		template<binary_tree_node_type Node>
		inline void as_right(Node* parent, Node* right);
		//是否是树根
		template<binary_tree_node_type Node>
		inline bool is_root(Node* node);
		//当前节点是左孩子?
		template<binary_tree_node_type Node>
		inline bool is_left(Node* node);
		//当前节点是右孩子?
		template<binary_tree_node_type Node>
		inline bool is_right(Node* node);

		//前序遍历
		template<binary_tree_node_type Node, typename ostream>
		void pre_order(Node* root, ostream& os);
		//中序遍历
		template<binary_tree_node_type Node, typename ostream>
		void in_order(Node* root, ostream& os);
		//后序遍历
		template<binary_tree_node_type Node, typename ostream>
		void post_order(Node* root, ostream& os);
		template<binary_tree_node_type Node,typename ostream>
		void level_order(Node* root, ostream& os);
		//前驱节点
		// template<binary_tree_node_type Node>
		template<binary_tree_node_type Node>
		Node* precursor(Node* node);
		//后继节点
		template<binary_tree_node_type Node>
		Node* successor(Node* node);
		//左旋转
		template<binary_tree_node_type Node>
		void turn_left(Node* node);
		//右旋转
		template<binary_tree_node_type Node>
		void turn_right(Node* node);

		//构造一棵二叉树
		template<binary_tree_node_type Node,input_iterator_type Iter,typename Alloc,typename Cmp>
		Node* make_basic_bianry_tree(Iter begin, Iter end, Alloc& alloc, Cmp cmp);
		//销毁一棵二叉树
		template<binary_tree_node_type Node,typename Alloc>
		void destory_basic_binary_tree(Node* root, Alloc& alloc);
		//插入节点1)
		template<binary_tree_node_type Node,typename Cmp>
		Node* binary_tree_insert(Node* root, Node* to_insert, Cmp cmp);
		//2)
		template<binary_tree_node_type Node,typename Alloc,typename Cmp>
		Node* binary_tree_insert(Node* root, const typename node_traits<Node>::value_type& val, Alloc& alloc,Cmp cmp);
		//3)
		template<binary_tree_node_type Node,input_iterator_type Iter,typename Alloc,typename Cmp>
		Node* binary_tree_insert(Node* root, Iter begin, Iter end, Alloc& alloc, Cmp cmp);
		//4)
		template<binary_tree_node_type Node,typename Alloc,typename Cmp>
		Node* binary_tree_insert(Node* root, size_t count, const typename node_traits<Node>::reference val, Alloc& alloc, Cmp cmp);
		////插入分发函数
		//template<binary_tree_node_type Node, typename Cmp>
		//void binary_tree_insert_aux(Node* root, Node* to_insert, Cmp cmp, binary_tree_node_tag);
		//删除节点1),删除一个节点可能会改变root节点
		template<binary_tree_node_type Node,typename Alloc>
		Node* binary_tree_erase(Node* root, Node* to_delete,Alloc& alloc);
		//删除分发
		template<binary_tree_node_type Node,typename Alloc>
		void binary_tree_erase_aux(Node* root, Node* to_delete,Alloc& alloc, binary_tree_node_tag);
	
		//返回第一个值为val的节点
		template<binary_tree_node_type Node,typename Cmp>
		Node* binary_tree_find(Node* root,const typename node_traits<Node>::reference val, Cmp cmp);
		//定位实际删除的节点位置
		template<binary_tree_node_type Node>
		Node* binary_tree_location_real_erase_node(Node* node);
		//删除"实际被删除"的节点
		template<binary_tree_node_type Node,typename Alloc>
		Node* binary_tree_erase_impl(Node* root, Node* to_delete, Node* real_node_to_erase, Alloc& alloc);

		//
		//当前节点的平衡因子
		template<binary_tree_node_type Node>
		int balance_factor(Node* node);

		template<binary_tree_node_type Node>
		Node* avl_tree_insert_fixup(Node* root, Node* node);
		template<binary_tree_node_type Node>
		Node* avl_tree_erase_fixup(Node* root, Node* node);

		template<binary_tree_node_type Node>
		Node* avl_tree_fixup(Node* root, Node* node);

		template<binary_tree_node_type Node,input_iterator_type Iter,typename Alloc,typename Cmp>
		Node* make_avl_tree(Iter begin, Iter end, Alloc& alloc, Cmp cmp);


		



		

		/*-------------------------------------------------------------------------------impl---------------------------------------------------------------------------------------------*/
		template<binary_tree_node_type Node>
		inline Node* parent(Node* node)
		{
			if (node)
				return node->_parent;
			return nullptr;
		}
		template<binary_tree_node_type Node>
		inline Node* left(Node* node)
		{
			if (node)
				return node->_left;
			return nullptr;
		}
		template<binary_tree_node_type Node>
		inline Node* right(Node* node)
		{
			if (node)
				return node->_right;
			return nullptr;
		}

		template<binary_tree_node_type Node>
		Node* brother(Node* node)
		{
			using node_ptr = Node*;
			if (!node)
				return nullptr;
			node_ptr p = node->_parent;
			if (!p)
				return nullptr;
			if (is_left(node))
				return p->_right;
			else
				return p->_left;
		}

		template<binary_tree_node_type Node>
		Node* uncle(Node* node)
		{
			using node_ptr = Node*;
			if (!node)
				return nullptr;
			node_ptr p = node->_parent;
			if (!p)
				return nullptr;
			node_ptr gp = p->_parent;
			if (!gp)
				return nullptr;
			if (is_left(p))
				return gp->_right;
			else
				return gp->_left;
		}

		template<binary_tree_node_type Node>
		Node* grandparent(Node* node)
		{
			using node_ptr = Node*;
			if (!node)
				return nullptr;
			node_ptr p = node->_parent;
			if (!p)
				return nullptr;
			return p->_parent;
		}

		template<binary_tree_node_type Node>
		size_t height(Node* node)
		{
			if (!node)
				return 0;
			size_t left = height(node->_left);
			size_t right = height(node->_right);
			return left < right ? ++right : ++left;
		}
		template<binary_tree_node_type Node>
		Node* extreame_left_node(Node* root)
		{
			using node_ptr = Node*;
			if (root)
			{
				while (left(root))
					root = root->_left;
			}
			return root;
		}
		template<binary_tree_node_type Node>
		Node* extreame_right_node(Node* root)
		{
			using node_ptr = Node*;
			if (root)
			{
				while (right(root))
					root = root->_right;
			}
			return root;
		}
		template<binary_tree_node_type Node>
		inline void as_left(Node* parent, Node* left)
		{
			if (parent)
				parent->_left = left;
			if (left)
				left->_parent = parent;
		}

		template<binary_tree_node_type Node>
		inline void as_right(Node* parent, Node* right)
		{
			if (parent)
				parent->_right = right;
			if (right)
				right->_parent = parent;
		}

		template<binary_tree_node_type Node>
		inline bool is_root(Node* node)
		{
			if (node)
				return nullptr == node->_parent;
			return false;
		}

		template<binary_tree_node_type Node>
		inline bool is_left(Node* node)
		{
			using node_ptr = typename node_traits<Node>::node_ptr;

			if (!node)
				return false;

			if (!node->_parent)
				return false;

			if (node->_parent->_left == node)
				return true;
			return false;
		}

		template<binary_tree_node_type Node>
		inline bool is_right(Node* node)
		{
			using node_ptr = typename node_traits<Node>::node_ptr;

			if (!node)
				return false;

			if (!node->_parent)
				return false;

			if (node->_parent->_right == node)
				return true;
			return false;
		}

		template<binary_tree_node_type Node, typename ostream>
		void pre_order(Node* root, ostream& os)
		{
			if (root) {
				os << *root << " ";
				pre_order(root->_left, os);
				pre_order(root->_right, os);
			}
		}

		template<binary_tree_node_type Node, typename ostream>
		void in_order(Node* root, ostream& os)
		{
			if (root) {
				in_order(root->_left, os);
				os << *root << " ";
				in_order(root->_right, os);
			}

		}

		template<binary_tree_node_type Node, typename ostream>
		void post_order(Node* root, ostream& os)
		{
			if (root) {
				post_order(root->_left, os);
				post_order(root->_right, os);
				os << *root << " ";
			}
		}

		template<binary_tree_node_type Node, typename ostream>
		void level_order(Node* root, ostream& os)
		{
			using node_ptr = Node*;
			std::queue<node_ptr> q;
			q.push(root);
			size_t level = 1;
			while (!q.empty())
			{
				std::cout << "level " << level<<":";
				size_t sz = q.size();

				for (size_t i = 0; i != sz; ++i)
				{
					node_ptr node = q.front();
					q.pop();
					os << *node;
					if (node->_left)
						q.push(node->_left);
					if (node->_right)
						q.push(node->_right);
				}
				
				std::cout << std::endl;
				++level;
				
			}
		}

		template<binary_tree_node_type Node>
		Node* precursor(Node* node)
		{
			if (!node)
				return nullptr;
			if (node->_left)
			{
				node = node->_left;
				while (node->_right)
				{
					node = node->_right;
				}
				return node;
			}
			else {
				Node* p = node->_parent;
				if (!p)
					return nullptr;
				while (is_left(node))
				{
					node = p;
					p = p->_parent;
				}
				return p;
			}
		}

		template<binary_tree_node_type Node>
		Node* successor(Node* node)
		{
			if (!node)
				return nullptr;
			if (node->_right)
			{
				node = node->_right;
				while (node->_left)
					node = node->_left;
				return node;
			}
			else {
				Node* p = node->_parent;
				if (!p)
					return nullptr;
				while (is_right(node))
				{
					node = p;
					p = node->_parent;
				}
				return p;
			}
		}

		//template<binary_tree_node_type Node, typename Cmp>
		//void binary_tree_insert_aux(Node* root, Node* to_insert, Cmp cmp,binary_tree_node_tag)
		//{
		//	using node_ptr = typename node_traits<Node>::node_ptr;
		//	if (!root)
		//		return;
		//	//从根节点开始寻找满足条件的叶子节点
		//	node_ptr cur = root;
		//	
		//	while (cur)
		//	{
		//		if (cmp(to_insert->_val, cur->_val))
		//			if (cur->_left)
		//				cur = cur->_left;
		//			else
		//				break;
		//		else
		//			if (cur->_right)
		//				cur = cur->_right;
		//			else
		//				break;
		//	}

		//	if (cmp(to_insert->_val, cur->_val))
		//		as_left(cur, to_insert);
		//	else
		//		as_right(cur, to_insert);

		//}

		template<binary_tree_node_type Node, typename Alloc, typename Cmp>
		Node* binary_tree_insert(Node* root, const typename node_traits<Node>::value_type& val,Alloc& alloc, Cmp cmp)
		{
			static_assert(std::is_same_v<Node, typename std::allocator_traits<Alloc>::value_type>);
			using node_ptr = Node*;
			
			node_ptr tmp = alloc.allocate(1);
			init_bianry_tree_node_type(tmp, val);
			//插入
			return binary_tree_insert<Node, Cmp>(root, tmp, cmp);

		}

		// 二叉树插入节点
		template<binary_tree_node_type Node, typename Cmp>
		Node* binary_tree_insert(Node* root, Node* to_insert,Cmp cmp)
		{
			using node_ptr = typename node_traits<Node>::node_ptr;
			if (!root)
				return to_insert;
			//从根节点开始寻找满足条件的叶子节点
			node_ptr cur = root;

			while (cur)
			{
				if (cmp(to_insert->_val, cur->_val))
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

			if (cmp(to_insert->_val, cur->_val))
				as_left(cur, to_insert);
			else
				as_right(cur, to_insert);

			return root;
		}

		
		
	

		//二叉树节点的删除
		template<binary_tree_node_type Node, typename Alloc>
		Node* binary_tree_erase(Node* root, Node* to_delete, Alloc& alloc)
		{
			using node_ptr = Node*;
			node_ptr read_node_to_erase = binary_tree_location_real_erase_node(to_delete);
			return binary_tree_erase_impl(root, to_delete, read_node_to_erase, alloc);
		}

		template<binary_tree_node_type Node, typename Alloc>
		void binary_tree_erase_aux(Node* root, Node* to_delete, Alloc& alloc, binary_tree_node_tag)
		{
			static_assert(std::is_same_v<Node, typename std::allocator_traits<Alloc>::value_type>);
			using node_ptr = Node*;

			if (!to_delete||!root)
				return;
			bool left_exist = to_delete->_left != nullptr;
			bool right_exist = to_delete->_right != nullptr;

			node_ptr tmp = nullptr;
			node_ptr nil = nullptr;
			if (!left_exist && !right_exist)	//左右孩子都不存在，是个叶子节点，直接返回
			{
				if (is_left(to_delete))
					as_left(to_delete->_parent, nil);
				else
					as_right(to_delete->_parent, nil);

				tmp = to_delete;
			}
			else if (left_exist && right_exist)	//拥有两个孩子节点,那么可以找到其前驱或者后继节点替代当前节点，然后删除替代节点
			{
				Node* suc = successor(to_delete);
				to_delete->_val = suc->_val;
				if (is_left(suc))
					as_left(suc->_parent, nil);
				else
					as_right(suc->_parent, nil);

				tmp = suc;
			}
			else {	//仅有一个孩子,连接父节点和子节点即可
				Node* child = nullptr;
				if (left_exist)
					child = to_delete->_left;
				else
					child = to_delete->_right;

				if (is_left(to_delete))
					as_left(to_delete->_parent, child);
				else
					as_right(to_delete->_parent, child);

				tmp = to_delete;
			}
			std::destroy_at(tmp);
			alloc.deallocate(tmp, 1);
		}

		template<binary_tree_node_type Node>
		void turn_left(Node* node)
		{
			using node_ptr = Node*;
			
			//node节点及其父节点必须存在
			if (!node)
				return;
			node_ptr p = node->_parent;
			if (!p)
				return;
			//node必须是其父节点的右孩子
			if (!is_right(node))
				throw std::logic_error{ "binary_node_turn_left::node must be parent's right child!" };

			//operator
			node_ptr gp = p->_parent;
			node_ptr lf = node->_left;

			if (is_left(p))
				as_left(gp, node);
			else
				as_right(gp, node);

			as_left(node, p);
			as_right(p, lf);
		}

		template<binary_tree_node_type Node>
		void turn_right(Node* node)
		{
			using node_ptr = Node*;

			//node节点及其父节点必须存在
			if (!node)
				return;
			node_ptr p = node->_parent;
			if (!p)
				return;
			//node必须是其父节点的左孩子
			if (!is_left(node))
				throw std::logic_error{ "binary_node_turn_right::node must be parent's left child!" };

			node_ptr gp = p->_parent;
			node_ptr rt = node->_right;

			if (is_left(p))
				as_left(gp, node);
			else
				as_right(gp, node);
			as_right(node, p);
			as_left(p, rt);
		}

		template<binary_tree_node_type Node, typename Cmp>
		Node* binary_tree_find(Node* root,const typename node_traits<Node>::value_type& val, Cmp cmp)
		{
			using node_ptr = Node*;
			node_ptr cur = root;
			while (cur)
			{
				if (cur->_val == val)
					break;
				if (cmp(val, cur->_val))
					cur = cur->_left;
				else
					cur = cur->_right;
			}
			return cur;
		}

		template<binary_tree_node_type Node, input_iterator_type Iter, typename Alloc, typename Cmp>
		Node* binary_tree_insert(Node* root, Iter begin, Iter end, Alloc& alloc, Cmp cmp)
		{
			static_assert(std::is_same_v<Node, typename std::allocator_traits<Alloc>::value_type>);
			static_assert(std::is_same_v<typename node_traits<Node>::value_type, typename std::iterator_traits<Iter>::value_type>);

			using node_ptr = Node*;

			if (begin == end)
				return root;
			//构造节点并插入
			node_ptr tmp = nullptr;
			for (auto p = begin; p != end; ++p)
			{
				tmp = alloc.allocate(1);
				init_bianry_tree_node_type(tmp, *p);
				root=binary_tree_insert<Node,Cmp>(root, tmp, cmp);
			}
			return root;
		}

		template<binary_tree_node_type Node, typename Alloc, typename Cmp>
		Node* binary_tree_insert(Node* root, size_t count, const typename node_traits<Node>::value_type& val, Alloc& alloc, Cmp cmp)
		{
			using node_ptr = Node*;
			node_ptr tmp = nullptr;
			while (count)
			{
				tmp = alloc.allocate(1);
				init_bianry_tree_node_type(tmp, val);
				root=binary_tree_insert(root, tmp, cmp);
				--count;
			}
			return root;
		}

		

		

		////构造一棵普通的二叉树
		//template<binary_tree_node_type Node, typename Iter,
		//	typename Alloc = std::allocator<Node>, typename Cmp = std::less<typename node_traits<Node>::value_type>
		//>
		//Node* make_basic_binary_tree(Iter begin, Iter end, Alloc& alloc, Cmp cmp = {})
		//{
		//	using node_ptr = typename node_traits<Node>::node_ptr;
		//	node_ptr root = alloc.allocate(1);
		//	{
		//		//由于我们对于binary_tree_node_type的了解仅有值域_val和三个指针域_parent,_left,_right,所以仅初始化这三项
		//		root->_val = *begin;
		//		root->_parent = root->_left = root->_right = nullptr;
		//	}
		//	node_ptr tmp = nullptr;
		//	for (auto p = ++begin; p != end; ++p)
		//	{
		//		tmp = alloc.allocate(1);
		//		{
		//			tmp->_val = *p;
		//			tmp->_parent = tmp->_left = tmp->_right = nullptr;
		//		}
		//		tree_insert(root, tmp);
		//	}
		//	return root;
		//}

		template<binary_tree_node_type Node, input_iterator_type Iter, typename Alloc, typename Cmp>
		Node* make_basic_bianry_tree(Iter begin, Iter end, Alloc& alloc, Cmp cmp)
		{
			static_assert(std::is_same_v<Node, typename std::allocator_traits<Alloc>::value_type>);
			using node_ptr = Node*;
			//
			if (begin == end)
				return nullptr;
			//初始化根
			node_ptr root = alloc.allocate(1);
			init_bianry_tree_node_type(root, *begin);
			//构造节点并插入到树中
			node_ptr tmp = nullptr;
			for (auto p = ++begin; p != end; ++p)
			{
				tmp = alloc.allocate(1);
				init_bianry_tree_node_type(tmp, *p);
                binary_tree_insert(root, tmp, cmp);
			}
			return root;
		}

		//销毁一棵普通的二叉树
		template<binary_tree_node_type Node, typename Alloc = std::allocator<Node>>
		void destory_basic_binary_tree(Node* root, Alloc& alloc)
		{
			if (root)
			{
				destory_basic_binary_tree(root->_left);
				destory_basic_binary_tree(root->_right);
				std::destroy_at(root);
				alloc.deallocate(root);
			}
		}

		
		template<binary_tree_node_type Node>
		Node* binary_tree_location_real_erase_node(Node* to_delete)
		{
			using node_ptr = Node*;

			if (!to_delete)
				return nullptr;
			
			if (left(to_delete) && right(to_delete))
				return successor(to_delete);
			else
				return to_delete;
		}

		template<binary_tree_node_type Node,typename Alloc>
		Node* binary_tree_erase_impl(Node* root, Node* to_delete, Node* real_node_to_erase, Alloc& alloc)
		{
			using node_ptr = Node*;
			static_assert(std::is_same_v<Node,typename std::allocator_traits<Alloc>::value_type>);
			//此时node是一个叶子节点或者仅有一个孩子节点
			
			//不同的话就交换节点的值域
			if (real_node_to_erase != to_delete)
			{
				to_delete->_val = real_node_to_erase->_val;
			}
			//child是node的孩子节点
			node_ptr child = real_node_to_erase->_left ? real_node_to_erase->_left : real_node_to_erase->_right;
			if (real_node_to_erase == root)
				root = child;
			else {
				if (is_left(real_node_to_erase))
					as_left(parent(real_node_to_erase), child);
				else
					as_right(parent(real_node_to_erase), child);
			}


			std::destroy_at(real_node_to_erase);
			alloc.deallocate(real_node_to_erase, 1);
			return root;

		}

		template<binary_tree_node_type Node>
		int balance_factor(Node* node)
		{
			if (!node)
				return 0;
			return static_cast<int>(height(node->_left)) -static_cast<int>( height(node->_right));
		}

		template<binary_tree_node_type Node>
		Node* avl_tree_insert_fixup(Node* root, Node* node)
		{
			return avl_tree_fixup(root, node);
		}

		template<binary_tree_node_type Node>
		Node* avl_tree_fixup(Node* root, Node* node)
		{
			using node_ptr = Node*;
			node_ptr gp = node;
			node_ptr tmp = nullptr;
			while (gp)
			{
				if (balance_factor(gp)==2)
				{
					node_ptr p = gp->_left;
					if (balance_factor(p) == -1)
					{
						node_ptr c = p->_right;
						turn_left(c);
						tmp = p;
						p = c;
						c = tmp;
						tmp = nullptr;
					}
					turn_right(p);
					if (gp == root)
						root = p;
					gp = p;
				}
				else if (balance_factor(gp) == -2)
				{
					node_ptr p = gp->_right;
					if (balance_factor(p) == 1)
					{
						node_ptr c = p->_left;
						turn_right(c);
						tmp = p;
						p = c;
						c = tmp;
						tmp = nullptr;
					}
					turn_left(p);
					if (gp == root)
						root = p;
					gp = p;
				}
				gp = gp->_parent;
			}
			return root;
		}

		template<binary_tree_node_type Node, input_iterator_type Iter, typename Alloc, typename Cmp>
		Node* make_avl_tree(Iter begin, Iter end, Alloc& alloc, Cmp cmp)
		{
			using node_ptr = Node*;
			static_assert(std::is_same_v<Node,typename std::allocator_traits<Alloc>::value_type>);
			if (begin == end)
				return nullptr;
			//初始化根
			node_ptr root = alloc.allocate(1);
			init_bianry_tree_node_type(root, *begin);
			//
			node_ptr tmp = nullptr;
			for (auto p = ++begin; p != end; ++p)
			{
				tmp = alloc.allocate(1);
				init_bianry_tree_node_type(tmp, *p);
				root=binary_tree_insert(root, tmp, cmp);
				root=avl_tree_insert_fixup(root, tmp);
			}
			return root;
		}

	}


}
