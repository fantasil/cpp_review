#pragma once
#include<type_traits>


template<typename Node>
struct node_traits {
	using value_type = typename Node::value_type;
	using reference = typename Node::reference;
	using pointer = typename Node::pointer;
	using node_type = Node;
	using node_ptr = node_type*;
	using node_category = typename Node::node_category;
};


// 前向节点的标签
struct forward_node_tag {};
//双向节点标签
struct bidirectional_node_tag :forward_node_tag {};

//普通二叉树节点标签
struct binary_tree_node_tag {};
//二叉搜索树的节点标签
struct binary_search_tree_node_tag :binary_tree_node_tag {};
//红黑树节点标签
struct rb_tree_node_tag :binary_tree_node_tag {};	


/*
* 默认包含值域_val和指针域_next
*/
template<typename Node>
concept forward_node_type = std::is_base_of_v<forward_node_tag, typename node_traits<Node>::node_category >		//标签继承自forward_node_tag
							|| std::is_same_v<forward_node_tag, typename node_traits<Node>::node_category>;		//标签是fowrad_node_tag

template<forward_node_type Node>
inline void init_forward_node_type(Node* node,const typename node_traits<Node>::value_type& val, Node* next=nullptr)
{
	if (node) {
		node->_val = val;
		node->_next = next;
	}
}

/*
* 默认包含值域_val和指针域_prev,_next
*/
template<typename Node>
concept bi_node_type = std::is_base_of_v<bidirectional_node_tag, typename node_traits<Node>::node_category>		//标签继承自bidirectional_node_tag
						|| std::is_same_v <bidirectional_node_tag, typename node_traits<Node>::node_category>;	//标签是bidirectional_node_tag

template<bi_node_type Node>
inline void init_bi_node_type(Node* node, const typename node_traits<Node>::value_type& val, Node* prev=nullptr, Node* next=nullptr)
{
	if (node) {
		node->_val = val;
		node->_prev = prev;
		node->_next = next;
	}
}


//一个输入迭代器类型
template<typename Iter>
concept input_iterator_type = std::is_base_of_v<std::input_iterator_tag, typename std::iterator_traits<Iter>::iterator_category>	//继承自std::input_iterator_tag
					|| std::is_same_v<std::input_iterator_tag, typename std::iterator_traits<Iter>::iterator_category>;		//是std::input_iterator_tag

//一个输出迭代器类型
template<typename Iter>
concept output_iterator_type= std::is_base_of_v<std::output_iterator_tag, typename std::iterator_traits<Iter>::iterator_category>	//继承自std::output_iterator_tag
					|| std::is_same_v<std::output_iterator_tag, typename std::iterator_traits<Iter>::iterator_category>;		//是std::output_iterator_tag

//一个迭代器类型
template<typename Iter>
concept iterator_type = input_iterator_type<Iter> || output_iterator_type<Iter>;


/*
* 默认包含值域_val，指针域_parent,_left,_right
*/
template<typename Node>
concept binary_tree_node_type = std::is_base_of_v<binary_tree_node_tag, typename node_traits<Node>::node_category>	//继承自binary_tree_node_tag
							|| std::is_same_v<binary_tree_node_tag, typename node_traits<Node>::node_category>;		//是binary_tree_node_tag

template<binary_tree_node_type Node>
inline void init_bianry_tree_node_type(Node* node, const typename node_traits<Node>::value_type& val, Node* parent=nullptr, Node* left=nullptr, Node* right=nullptr)
{
	if (node) {
		node->_val = val;
		node->_parent = parent;
		node->_left = left;
		node->_right = right;
	}
}

/*
* 默认包含键域_key,值域_val，指针域_parent,_left,_right
*/
template<typename Node>
concept binary_search_tree_node_type = std::is_base_of_v<binary_search_tree_node_tag, typename node_traits<Node>::node_category>	//继承自binary_search_tree_node_tag
									   || std::is_same_v<binary_search_tree_node_tag, typename node_traits<Node>::node_category>;		//是binary_search_tree_node_tag


//初始化一个二叉搜索树节点
template<binary_search_tree_node_type Node>
inline void init_binary_search_tree_node_type(Node* node, const typename Node::key_type& key,
	const typename node_traits<Node>::value_type& val, Node* parent = nullptr, Node* left = nullptr, Node* right = nullptr)
{
	if (node) {
		node->_key = key;
		node->_val = val;
		node->_parent = parent;
		node->_left = left;
		node->_right = right;
	}
}

/*
* 默认包含关键词域_key,值域_val,色域_clr,指针域_parent,_left,_right
*/
template<typename Node>
concept rb_tree_node_type = std::is_base_of_v<rb_tree_node_tag, typename node_traits<Node>::node_category>	//继承自binary_tree_node_tag
							|| std::is_same_v<rb_tree_node_tag, typename node_traits<Node>::node_category>;		//是binary_tree_node_tag

template<rb_tree_node_type Node>
inline void init_rb_tree_node_type(Node* node, const typename Node::key_type& key,
	const typename node_traits<Node>::value_type& val,
	typename Node::color_type clr = Node::color_type::red,
	Node* parent = nullptr, Node* left = nullptr, Node* right = nullptr)
{
	if (node)
	{
		node->_key = key;
		node->_val = val;
		node->_clr = clr;
		node->_parent = parent;
		node->_left = left;
		node->_right = right;
	}
}


//Iter是一个pair<K,V>的序列
template<typename Iter,typename K,typename V>
concept input_iterator_of_pair = input_iterator_type<Iter>&& std::is_same_v<std::pair<K, V>, typename std::decay_t<typename std::iterator_traits<Iter>::value_type>>;


