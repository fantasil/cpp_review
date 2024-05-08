#pragma once
#include<vector>
#include<cmath>
namespace fantasil
{
	
	//һ��M���������Ľڵ�
	template<typename K,typename V,size_t M>
	struct m_way_search_tree_node {
		using node_type = m_way_search_tree_node<K, V, M>;
		using node_ptr = node_type*;
		using key_type = K;
		using value_type = V;
		std::vector<std::pair<K, V>> _elem;
		std::vector<node_ptr> _children;
		node_ptr _parent;
	};


	namespace m_way_search_tree_node_helper {

		/*------------------------------------------------------------------declare--------------------------------------------------------------*/

		//�ڵ�Ԫ��������
		template<typename K,typename V,size_t M>
		inline bool elem_full(m_way_search_tree_node<K, V, M>* node);
		//��Ҷ�ڵ���
		template<typename K,typename V,size_t M>
		inline bool is_leaf(m_way_search_tree_node<K, V, M>* node);

	
	


		//��λ��ֵΪkeyԪ�����ڵĽڵ�
		template<typename K,typename V,size_t M,typename Cmp>
		m_way_search_tree_node<K, V, M>* locate(m_way_search_tree_node<K, V, M>* root, const K& key, Cmp cmp);
		//��λӦ�ò���Ԫ�صĽڵ�
		template<typename K,typename V,size_t M,typename Cmp>
		m_way_search_tree_node<K, V, M>* locate_to_insert(m_way_search_tree_node<K, V, M>* root, const K& key, Cmp cmp);
		//���ؼ�ֵΪkey��Ԫ��ֵ
		template<typename K,typename V,size_t M,typename Cmp>
		V& find(m_way_search_tree_node<K, V, M>* root, const K& key, Cmp cmp);
		//
		template<typename K,typename V,size_t M,typename Alloc,typename Cmp>
		m_way_search_tree_node<K,V,M>* node_splay(m_way_search_tree_node<K, V, M>* node,Alloc& alloc,Cmp cmp);

		template<typename K,typename V,size_t M,typename Cmp>
		void elem_into_node(m_way_search_tree_node<K, V, M>* node, const K& key, const V& value,Cmp cmp);

		template<typename K,typename V,size_t M,typename Alloc,typename Cmp>
		m_way_search_tree_node<K, V, M>* insert(m_way_search_tree_node<K, V, M>* root, const K& key, const V& value,Alloc& alloc,Cmp cmp);

		//ʹ�÷��ѵķ�ʽ����Ԫ��
		template<typename K, typename V, size_t M, typename Alloc, typename Cmp>
		m_way_search_tree_node<K, V, M>* insert_with_splay(m_way_search_tree_node<K, V, M>* root, const K& key, const V& value, Alloc& alloc, Cmp cmp);

		template<typename K,typename V,size_t M,typename Alloc>
		m_way_search_tree_node<K, V, M>* erase(m_way_search_tree_node<K, V, M>* root, const K& key, Alloc& alloc);


		/*------------------------------------------------------------------impl-------------------------------------------------------------------*/

		template<typename K, typename V, size_t M>
		bool elem_full(m_way_search_tree_node<K, V, M>* node)
		{
			constexpr full = M - 1;
			return full == node->_elem.size();
		}

		template<typename K, typename V, size_t M>
		bool is_leaf(m_way_search_tree_node<K, V, M>* node)
		{

			return node->_children.size() == 0;
		}

		template<typename K, typename V, size_t M, typename Cmp>
		m_way_search_tree_node<K, V, M>* locate(m_way_search_tree_node<K, V, M>* root, const K& key, Cmp cmp)
		{
			using node_type = m_way_search_tree_node<K, V, M>;
			using node_ptr = node_type*;

			node_ptr cur = root;
			while (cur)
			{
				auto p = cur->_elem.begin();
				while (p != cur->_elem.end())
				{
					//�ҵ��ڵ�
					if (key == p->first)
						return cur;
					if (cmp(key, p->first))
						break;
					++p;
				}
				if (p == cur->_elem.end())
				{
					//����βԪ�ص�������������̽
					cur=cur->_children.back()��
				}
				else {
					//��������cmpԪ�ص�������������̽
					auto distance = p - cur->_elem.begin();
					node_ptr elem_left_subtree = cur->_children[distance];
					cur = elem_left_subtree;
				}
			}
			return cur;
		}

		template<typename K, typename V, size_t M, typename Cmp>
		m_way_search_tree_node<K, V, M>* locate_to_insert(m_way_search_tree_node<K, V, M>* root, const K& key, Cmp cmp)
		{
			using node_type = m_way_search_tree_node<K, V, M>;
			using node_ptr = node_type*;

			node_ptr p = nullptr;
			node_ptr c = root;
			while (c)
			{
				auto p = c->_elem.begin();
				while (p != c->_elem.end())
				{
					//��ֵ�������ظ�
					if (key == p->first)
						throw std::exception{ "elem already exist" };
					if (cmp(key, p->first))
						break;
					++p;
				}
				p = c;
				if (p == c->_elem.end())
				{
					//����βԪ�ص�������������̽
					c = c->_children.back();
				}
				else {
					//��������������Ԫ�ص�������������̽
					auto distance = p - c->_elem.begin();
					node_ptr elem_left_subtree = c->_children[distance];
					c = elem_left_subtree;
				}
			}

			return p;
		}

		template<typename K, typename V, size_t M, typename Cmp>
		V& find(m_way_search_tree_node<K, V, M>* root, const K& key, Cmp cmp)
		{
			using node_type = m_way_search_tree_node<K, V, M>;
			using node_ptr = node_type*;
			node_ptr node = locate(root, key, cmp);
			//�����ڣ��׳��쳣
			if (!node)
				throw std::exception{ "no such element" };
			//��vector�в���Ԫ�أ�������ֵ��
			for (auto p = node->_elem.begin(); p != node->_elem.end(); ++p)
			{
				if (key == p->first)
					return p->second;
			}
		}

		template<typename K, typename V, size_t M,typename Alloc, typename Cmp>
		m_way_search_tree_node<K, V, M>* node_splay(m_way_search_tree_node<K, V, M>* node,Alloc& alloc,Cmp cmp)
		{
			using node_type = m_way_search_tree_node<K, V, M>;
			using node_ptr = node_type*;

			size_t p = std::round(M / 2);
			//�µ��ֵܽڵ�
			auto b = node->_elem.begin() + p;
			auto e = node->_elem.end();
			std::vector<std::pair<K, V>> elem_2{ b,e };
			auto cb = node->_children.begin() + p;
			auto ce = node->_children.end();
			std::vector<node_ptr> children_2{ cb,ce };
			node_ptr bro = alloc.allocate(1);
			bro->_parent = node->_parent;
			bro->_elem = std::move(elem_2);
			bro->_children = std::move(children_2);
			//����Ԫ�ز�������뵽���ڵ���
			std::pair<K, V> elem_overflow = node->_elem[p - 1];
			node_ptr parent = node->_parent;
			if (!parent)
				parent = alloc.allocate(1);
			auto p = parent->_elem.begin();
			while (p != parent->_elem.end())
			{
				if (cmp(elem_overflow.first, p->first))
					break;
				++p;
			}
			parent->_elem.insert(p, elem_overflow);
			parent->_children[p + 1] = bro;
			//�ü���ǰ�ڵ�
			node->_elem.erase(node->_elem.begin() + p - 1, node->_elem.end());
			node->_children.erase(node->_children.begin() + p, node->_children.end());
			return parent;
		}

		template<typename K, typename V, size_t M, typename Cmp>
		void elem_into_node(m_way_search_tree_node<K, V, M>* node, const K& key, const V& value,Cmp cmp)
		{
			using node_type = m_way_search_tree_node<K, V, M>;
			using node_ptr = node_type*;
			if (node->_elem.empty())
			{
				//�ڵ�û��Ԫ��
				node->_elem.push_back({ key,value });
				node->_children.push_back(nullptr);
				node->_children.push_back(nullptr);
				return;
			}
			//p�ǵ�һ�����������ĵ�����λ��
			auto p = node->_elem.begin();
			while (p != node->_elem.end())
			{
				if (cmp(key, p->first))
					break;
				++p;
			}
			if (p == node->_elem.end())
			{
				//���뵽ĩβ
				node->_elem.push_back({ key,value });
				node->_children.push_back(nullptr);
			}
			else {
				//���뵽���ʵ�λ��
				auto distance = p - node->_elem.begin();
				std::copy_backward(p, node->_elem.end(), p + 1);
				*p = { key,value };
				std::copy_backward(node->_children.begin() + p, node->_children.end(), node->_children.begin() + p + 1);
				node->_children[p] = nullptr;
			}

		}

		template<typename K, typename V, size_t M,typename Alloc,typename Cmp>
		m_way_search_tree_node<K, V, M>* insert(m_way_search_tree_node<K, V, M>* root, const K& key, const V& value,Alloc& alloc,Cmp cmp)
		{
			using node_type = m_way_search_tree_node<K, V, M>;
			using node_ptr = node_type*;
			static_assert(std::is_same_v<node_type, typename std::allocator_traits<Alloc>::value_type>);
			std::pair<K, V> e{ key,value };
			if (!root)
			{
				//���û�и��ڵ㣬������һ���½ڵ���Ϊ���ڵ㲢����
				node_ptr tmp = alloc.allocate(1);
				tmp->_elem.push_back(e);
				tmp->_children.push_back(nullptr);
				tmp->_parent = nullptr;
				return tmp;
			}
			node_ptr cur = root;
			node_ptr parent = nullptr;
			size_t pos = 0;
			while (cur)
			{
				auto begin = cur->_elem.begin();
				auto end = cur->_elem.end();
				auto p = begin;
				for (; p != end; ++p)
				{
					if (cmp(key, p->first))
						break;
				}
				auto distance = p == end ? M : p - begin;
				if (elem_full(cur)) {
					//�ڵ����������¼�����ѯ
					parent = cur;
					pos = distance;
					cur = cur->_children[distance];
					
				}else {
					//���뵽�ڵ�ĺ���λ��
					std::copy_backward(begin + distance, end, begin + distance + 1);
					std::copy_backward(cur->_children[distance], cur->_children.end(), cur->_children[distance + 1]);
					cur->_elem[distance] = e;
					cur->_children[distance] = nullptr;
					break;
				}
			}

			if (!cur)
			{
				//�½�һ���ڵ�������Ԫ��
				node_ptr tmp = alloc.allocate(1);
				tmp->_elem.push_back(e);
				tmp->_children.push_back(nullptr);
				parent->_children[pos] = tmp;
				tmp->_parent = parent;
			}
			return root;
		}

		template<typename K, typename V, size_t M, typename Alloc, typename Cmp>
		m_way_search_tree_node<K, V, M>* insert_with_splay(m_way_search_tree_node<K, V, M>* root, const K& key, const V& value, Alloc& alloc, Cmp cmp)
		{
			using node_type = m_way_search_tree_node<K, V, M>;
			using node_ptr = node_type*;
			static_assert(std::is_same_v<node_type, typename std::allocator_traits<Alloc>::value_type>);
			node_ptr node = locate_to_insert(root, key, cmp);
			std::pair<K, V> pa{ key,value };
			if (!node)
			{
				//����
				node_ptr tmp = alloc.allocate(1);
				tmp->_elem.push_back(pa);
				tmp->_children.push_back(nullptr);
				tmp->_children.push_back(nullptr);
				return tmp;
			}

			node_ptr cur = node;
			while (elem_full(cur))
			{
				//���Ѳ���������
				size_t p = std::round(M / 2);
				for(auto p=cur->_elem.begin();p!=cur->_elem)
			}


			return nullptr;
		}

		template<typename K, typename V, size_t M, typename Alloc>
		m_way_search_tree_node<K, V, M>* erase(m_way_search_tree_node<K, V, M>* root, const K& key, Alloc& alloc)
		{
			return nullptr;
		}

	}



}