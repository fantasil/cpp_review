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


// ǰ��ڵ�ı�ǩ
struct forward_node_tag {};
//˫��ڵ��ǩ
struct bidirectional_node_tag :forward_node_tag {};

//��ͨ�������ڵ��ǩ
struct binary_tree_node_tag {};
//�����������Ľڵ��ǩ
struct binary_search_tree_node_tag :binary_tree_node_tag {};
//������ڵ��ǩ
struct rb_tree_node_tag :binary_tree_node_tag {};	


/*
* Ĭ�ϰ���ֵ��_val��ָ����_next
*/
template<typename Node>
concept forward_node_type = std::is_base_of_v<forward_node_tag, typename node_traits<Node>::node_category >		//��ǩ�̳���forward_node_tag
							|| std::is_same_v<forward_node_tag, typename node_traits<Node>::node_category>;		//��ǩ��fowrad_node_tag

template<forward_node_type Node>
inline void init_forward_node_type(Node* node,const typename node_traits<Node>::value_type& val, Node* next=nullptr)
{
	if (node) {
		node->_val = val;
		node->_next = next;
	}
}

/*
* Ĭ�ϰ���ֵ��_val��ָ����_prev,_next
*/
template<typename Node>
concept bi_node_type = std::is_base_of_v<bidirectional_node_tag, typename node_traits<Node>::node_category>		//��ǩ�̳���bidirectional_node_tag
						|| std::is_same_v <bidirectional_node_tag, typename node_traits<Node>::node_category>;	//��ǩ��bidirectional_node_tag

template<bi_node_type Node>
inline void init_bi_node_type(Node* node, const typename node_traits<Node>::value_type& val, Node* prev=nullptr, Node* next=nullptr)
{
	if (node) {
		node->_val = val;
		node->_prev = prev;
		node->_next = next;
	}
}


//һ���������������
template<typename Iter>
concept input_iterator_type = std::is_base_of_v<std::input_iterator_tag, typename std::iterator_traits<Iter>::iterator_category>	//�̳���std::input_iterator_tag
					|| std::is_same_v<std::input_iterator_tag, typename std::iterator_traits<Iter>::iterator_category>;		//��std::input_iterator_tag

//һ���������������
template<typename Iter>
concept output_iterator_type= std::is_base_of_v<std::output_iterator_tag, typename std::iterator_traits<Iter>::iterator_category>	//�̳���std::output_iterator_tag
					|| std::is_same_v<std::output_iterator_tag, typename std::iterator_traits<Iter>::iterator_category>;		//��std::output_iterator_tag

//һ������������
template<typename Iter>
concept iterator_type = input_iterator_type<Iter> || output_iterator_type<Iter>;


/*
* Ĭ�ϰ���ֵ��_val��ָ����_parent,_left,_right
*/
template<typename Node>
concept binary_tree_node_type = std::is_base_of_v<binary_tree_node_tag, typename node_traits<Node>::node_category>	//�̳���binary_tree_node_tag
							|| std::is_same_v<binary_tree_node_tag, typename node_traits<Node>::node_category>;		//��binary_tree_node_tag

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
* Ĭ�ϰ�������_key,ֵ��_val��ָ����_parent,_left,_right
*/
template<typename Node>
concept binary_search_tree_node_type = std::is_base_of_v<binary_search_tree_node_tag, typename node_traits<Node>::node_category>	//�̳���binary_search_tree_node_tag
									   || std::is_same_v<binary_search_tree_node_tag, typename node_traits<Node>::node_category>;		//��binary_search_tree_node_tag


//��ʼ��һ�������������ڵ�
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
* Ĭ�ϰ����ؼ�����_key,ֵ��_val,ɫ��_clr,ָ����_parent,_left,_right
*/
template<typename Node>
concept rb_tree_node_type = std::is_base_of_v<rb_tree_node_tag, typename node_traits<Node>::node_category>	//�̳���binary_tree_node_tag
							|| std::is_same_v<rb_tree_node_tag, typename node_traits<Node>::node_category>;		//��binary_tree_node_tag

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


//Iter��һ��pair<K,V>������
template<typename Iter,typename K,typename V>
concept input_iterator_of_pair = input_iterator_type<Iter>&& std::is_same_v<std::pair<K, V>, typename std::decay_t<typename std::iterator_traits<Iter>::value_type>>;


