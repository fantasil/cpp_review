#pragma once


#ifndef fantasil_basic_graph_node_h
#define fantasil_basic_graph_node_h
#include"my_forward_list.h"
#include"my_vector.h"
namespace fantasil {

	//普通的抽象节点
	template<typename T>
	struct basic_graph_node {
		using value_type = T;
		value_type _val;
	};

	
	namespace basic_graph_node_helper {
		namespace directed {
			namespace weight {

#pragma region directed_weight_graph_class_define
				//边是有向的，带权值的
				template<typename T, typename W>
				struct edge {
					using node_type = basic_graph_node<T>;
					using node_ptr = node_type*;
					using weight_type = W;
					node_ptr _from;
					node_ptr _to;
					weight_type _weight;
					edge(node_ptr from = nullptr, node_ptr to = nullptr, weight_type weight = {}) :_from{ from }, _to{ to }, _weight{ weight } {}
					~edge() {}
				};

				//一个邻接节点包括节点本身和它"连接至"的边
				template<typename T, typename W>
				struct adjacent_node {
					using node_type = basic_graph_node<T>;
					using node_ptr = node_type*;
					using weight_type = W;
					using elem_type = std::pair<node_ptr, weight_type>;

					node_ptr _node;
					my_forward_list<elem_type> _link_to;
				};
				
				//有向边
				template<typename T,typename W>
				struct edge {
					using node_type = basic_graph_node<T>;
					using node_ptr = node_type*;
					using weight_type = W;
					node_ptr _from;
					node_ptr _to;
					weight_type _weight;
				};

				//图是节点及其关系的集合
				template<typename T, typename W>
				struct graph {
					using elem_type = adjacent_node<T, W>;
					my_vector<elem_type*> _elems;
				};

				//萃取一些类型信息
				template<typename T, typename W>
				struct category {
					using node_type = basic_graph_node<T>;
					using adjacent_node_type = adjacent_node<T, W>;
					using graph_type = graph<T, W>;
					using fl_elem_type = typename adjacent_node_type::elem_type;
					using fl_elem_iter = decltype(adjacent_node_type{}._link_to.begin());
					using vc_elem_type = typename graph<T, W>::elem_type;
					using vc_elem_iter = decltype(graph_type{}._elems.begin());
				};

				
#pragma endregion

#pragma region directed_weight_graph_func_declare

				//节点再图中？
				template<typename T, typename W>
				bool is_in_graph(graph<T, W>* g, basic_graph_node<T>* node);
				//节点所在
				template<typename T, typename W>
				typename category<T, W>::vc_elem_iter node_pos_in_graph(graph<T, W>* g, basic_graph_node<T>* node);
				//插入一个邻接节点
				template<typename T,typename W>
				void insert(graph<T, W>* g, adjacent_node<T, W>* node);
				//插入一个节点，和它链接到的节点集合
				template<typename T,typename W,typename Alloc,typename Iter>
				void insert(graph<T, W>* g, basic_graph_node<T>* node, Alloc& alloc, Iter begin, Iter end);
				//查询边是否存在
				template<typename T,typename W>
				bool is_edge_exist(graph<T, W>* g, edge<T, W> e);
				//邻接节点中有这样的有向边吗？
				template<typename T,typename W>
				typename category<T, W>::fl_elem_iter edge_pos_in_adjacent_node(adjacent_node<T,W>* node, edge<T, W> e);

				
#pragma endregion

#pragma region directed_weight_graph_func_impl

				template<typename T, typename W>
				bool is_in_graph(graph<T, W>* g, basic_graph_node<T>* node)
				{
					auto p = node_pos_in_graph(g, node);
					if (p != g->_elems.end())
						return true;
					return false;
				}
				template<typename T, typename W>
				typename category<T, W>::vc_elem_iter node_pos_in_graph(graph<T, W>* g, basic_graph_node<T>* node)
				{
					auto p = g->_elems.begin();
					for (; p != g->_elems.end(); ++p)
					{
						if (p->_node == node)
							break;
					}
					return p;
				}

				template<typename T, typename W>
				void insert(graph<T, W>* g, adjacent_node<T, W>* node)
				{
					g->_elems.push_back(node);
				}

				template<typename T, typename W, typename Alloc,typename Iter>
				void insert(graph<T, W>* g, basic_graph_node<T>* node,Alloc& alloc, Iter begin, Iter end)
				{
					static_assert(std::is_same_v<typename std::allocator_traits<Alloc>::value_type, adjacent_node<T, , W>>);
					static_assert(std::is_same_v<typename std::iterator_traits<Iter>::value_type, typenme category<T, W>::fl_elem_type>);

					adjacent_node<T, W>* an = alloc.allocate(1);
					an->_link_to.insert_after(an->_link_to.end(), begin, end);
					an->_node = node;
					g->_elems.push_back(an);

				}

				template<typename T, typename W>
				bool is_edge_exist(graph<T, W>* g, edge<T, W> e)
				{
					auto p = node_pos_in_graph(g, e._from);
					if (p == g->_elems.end())
						return false;
					auto q = edge_pos_in_adjacent_node(*p, e);
					if (q == p->_link_to.end())
						return false;
					return true;
				}

				template<typename T, typename W>
				typename category<T, W>::fl_elem_iter edge_pos_in_adjacent_node(adjacent_node<T, W>* node, edge<T, W> e)
				{
					if (e._from != node)
						throw std::exception{ "edge from not match adjacent node" };
					auto p = node->_link_to.begin();
					for (; p != node->_link_to.end(); ++p)
					{
						if (e._to == p->first && e._weight == p->second)
							break;
					}

					return p;
				}

#pragma endregion
				
			}

			namespace unweight {

			}
		}

		namespace undirected {
			namespace weight {

			}

			namespace unweight {

			}
		}
	}









}


#endif 