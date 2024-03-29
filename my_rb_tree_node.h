#pragma once
#include<chrono>


enum class rb_clr {
	red, black
};

template<typename K,typename V>
struct my_rb_tree_node {

	using key_type = K;
	using value_type = V;
	using node_type = my_rb_tree_node<K, V>;
	using node_ptr = node_type*;

	key_type _key;
	value_type _val;
	node_ptr _left;
	node_ptr _right;
	node_ptr _parent;
	rb_clr _clr;
};


//连接两个节点(父-左孩子)
template<typename K,typename V>
inline void as_left(my_rb_tree_node<K, V>* parent, my_rb_tree_node<K, V>* left)
{
	if (parent)
		parent->_left = left;
	if (left)
		left->_parent = parent;
}

//连接两个节点(父-右孩子)
template<typename K, typename V>
inline void as_right(my_rb_tree_node<K, V>* parent, my_rb_tree_node<K, V>* right)
{
	if (parent)
		parent->_right = right;
	if (right)
		right->_parent = parent;
}

template<typename K,typename V>
inline bool is_left_child(my_rb_tree_node<K,V>* node)
{
	my_rb_tree_node<K, V>* pr = node->_parent;
	if (pr)
	{
		if (pr->_left == node)
			return true;
		else
			return false;
	}
	return false;
}

template<typename K,typename V>
inline bool is_right_child(my_rb_tree_node<K, V>* node)
{
	my_rb_tree_node<K, V>* pr = node->_parent;
	if (pr)
	{
		if (pr->_right == node)
			return true;
		else
			return false;
	}
	return fasle;
}

//以节点为基左转
//节点必须是其父节点的右孩子
template<typename K,typename V>
void turn_left(my_rb_tree_node<K, V>* node)
{
	using node_type = my_rb_tree_node<K, V>;
	using node_ptr = node_type*;

	node_ptr parent = node->_parent;
	if (!is_right_child(node))
		return;
	node_ptr grandparent = parent->_parent;
	node_ptr left_child = node->_left;

	if (grandparent)
	{
		if (is_left_child(parent))
			as_left(grandparent, node);
		else
			as_right(grandparent, node);
	}

	as_left(node, parent);
	as_right(parent, left_child);


}

//已节点为基右转
//节点必须是左孩子
template<typename K, typename V>
void turn_right(my_rb_tree_node<K, V>* node)
{
	using node_type = my_rb_tree_node<K, V>;
	using node_ptr = node_type*;

	if (!is_left_child(node))
		return;
	node_ptr parent = node->_parent;
	if (!parent)
		return;
	node_ptr grandparent = parent->_parent;
	node_ptr right_child = node->_right;

	if (grandparent)
	{
		if (is_left_child(parent))
			as_left(grandparent, node);
		else
			as_right(grandparent, node);
	}

	as_right(node, parent);
	as_left(parent, right_child);
	
}

//获取叔父节点
template<typename K,typename V>
inline my_rb_tree_node<K, V>* get_uncle_node(my_rb_tree_node<K, V>* base)
{
	using node_type = my_rb_tree_node<K, V>;
	using node_ptr = node_type*;
	
	node_ptr uncle = nullptr;

	if (base)
	{
		node_ptr parent = base->_parent;
		if (parent)	//必须得有父节点
		{
			node_ptr grandparent = parent->_parent;
			if (grandparent)	//必须得有祖父节点
			{
				if (is_left_child(parent))
					uncle = grandparent->_right;
				else
					uncle = grandparent->_left;
			}
		}
	}
	return uncle;
}

template<typename K,typename V>
inline my_rb_tree_node<K, V>* get_bro_node(my_rb_tree_node<K, V>* base)
{
	using node_type = my_rb_tree_node<K, V>;
	using node_ptr = node_type*;
	node_ptr bro = nullptr;
	if (base)
	{
		node_ptr parent = base->_parent;
		if (parent)
		{
			if (is_left_child(base))
				bro = parent->_right;
			else
				bro = parent->_left;
		}
	}
	return bro;

}






