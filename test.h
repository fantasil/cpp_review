
#include"my_bidirectional_list.h"

#include"my_vector.h"
#include"binary_tree_node.h"
#include"rb_tree_node.h"

#include"basic_binary_tree.h"
#include"rb_tree.h"

#include"my_forward_list.h"
#include"rb_tree_node.h"
#include"binary_search_tree.h"
#include"avl_tree.h"
#include"splay_tree.h"
#include"basic_b_tree_node.h"
#include"bidirectional_circular_list.h"

#include"fib_heap_node.h"
#include"fib_heap_tree.h"

#include"proto_vEB.h"
#include"vEB.h"

#include<bitset>
#define DEBUG

namespace fantasil
{



	template<typename Iter ,typename ostream>
	void print_seq(Iter begin, Iter end, ostream& os)
	{
		while (begin != end)
		{
			os << *begin << " ";
			++begin;
		}
		os << "\n";
	}


	void my_vector_test()
	{
		my_vector<int> vec{ 1,2,3,4,5,6 };
		print_seq(vec.begin(), vec.end(), std::cout);
		
		std::vector<int> vec2{};
		for (int i = 0; i != 100; ++i)
		{
			vec2.push_back(i * 100);
		}

		vec.insert(vec.end(), vec2.begin(), vec2.end());

		print_seq(vec.begin(), vec.end(), std::cout);

		auto p = vec.begin();
		auto b = p + 50;
		auto e = b + 50;
		vec.erase(b, e);

		print_seq(vec.begin(), vec.end(), std::cout);
	}

	void my_binary_tree_node_test()
	{
		using namespace fantasil;
		using namespace fantasil::binary_tree_node_helper;
		using node_type = typename node_traits<binary_tree_node<int>>::node_type;
		using node_ptr = node_type*;
		using allocator_type = std::allocator<node_type>;
		
		allocator_type alloc;
		
		std::less<int> cmp{};

		
		std::initializer_list<int> ilist{ 5,2,4,1,3,7,8 };
		using iterator = decltype(ilist.begin());
		//auto root = make_basic_bianry_tree<node_type, iterator, allocator_type, std::less<int>>(ilist.begin(), ilist.end(), alloc, cmp);
		auto root = make_avl_tree<node_type>(ilist.begin(), ilist.end(), alloc, cmp);

		std::cout << "pre_order:\n";
		pre_order(root, std::cout);
		std::cout << "\nin_order:\n";
		in_order(root, std::cout);
		std::cout << "\npost_order:\n";
		post_order(root, std::cout);
		
		/*auto node = binary_tree_find(root, 2, std::less<int>{});
		turn_right(node);
		std::cout << "\n";
		pre_order(root, std::cout);*/
		

	}

	//���Ե������ڵ�
	void my_forward_node_test()
	{
		using namespace fantasil;
		using namespace fantasil::forward_node_helper;

		using node_type = forward_node<int>;
		using node_ptr = node_type*;
		using iterator = forward_iterator<node_type>;
		std::allocator<node_type> alloc;
		std::initializer_list<int> ilist{ 1,2,3,4,5 };
		node_ptr root = make_basic_forward_list<node_type>(ilist.begin(), ilist.end(), alloc);
		std::cout << "list:\n";
		print_seq(iterator{ root }, iterator{ nullptr }, std::cout);
		//print_forward_list(root, std::cout);
		std::cout << "\nsize:\n";
		std::cout << size(root) << std::endl;
		
		node_ptr pos = root->_next->_next;
		node_ptr tmp = alloc.allocate(1);
		std::construct_at(tmp, 6);
		insert_after(pos, tmp);
		std::cout << "\nafter insert list:\n";
		//print_forward_list(root, std::cout);
		print_seq(iterator{ root }, iterator{ nullptr }, std::cout);

		std::cout << "\nafter erase list:\n";
		erase_after(root, alloc);
		//print_forward_list(root, std::cout);
		print_seq(iterator{ root }, iterator{ nullptr }, std::cout);

		std::cout << "\nafter push back:\n";
		push_back(root, 6, alloc);
		//print_forward_list(root, std::cout);
		print_seq(iterator{ root }, iterator{ nullptr }, std::cout);

		std::cout << "\nafter pop front:\n";
		root=pop_front(root, alloc);
		//print_forward_list(root, std::cout);
		print_seq(iterator{ root }, iterator{ nullptr }, std::cout);

		std::cout << "\n after push front:\n";
		root = push_front(root, 100, alloc);
		//print_forward_list(root, std::cout);
		print_seq(iterator{ root }, iterator{ nullptr }, std::cout);

		
		std::cout << "\nafter destroy list:\n";
		destroy_basic_forward_list(root, alloc);
		//print_forward_list(root, std::cout);


	}

	void my_forward_list_test()
	{
		using namespace fantasil;
		my_forward_list<int> fl{ 1,2,3,4,5,6 };
		std::cout << fl << std::endl;

		fl.push_back(6);
		fl.push_back(7);
		fl.pop_front();
		fl.pop_front();
		std::cout << fl << std::endl;
		
		fl.push_front(100);
		fl.push_front(2909);
		std::cout << fl << std::endl;
		fl.insert_after(fl.begin(), { 10,20,30,40 });
		std::cout << fl << std::endl;
		std::vector<int> vec{ 500,1000,1500 };
		fl.insert_after(fl.begin(), vec.begin(), vec.end());
		std::cout << fl << std::endl;

		fl.erase_after(fl.begin());
		fl.erase_sublist_after(fl.begin(), 5);
		std::cout << fl << std::endl;
	}

	void my_bi_node_test()
	{
	/*	using namespace fantasil;
		using namespace fantasil::bi_node_helper;
		using node_type = bi_node<int>;
		using node_ptr = node_type*;
		using allocator_type= std::allocator<node_type>;

		allocator_type alloc;
		std::initializer_list<int> ilist{ 1,2,3,4,5 };
		
		node_ptr head = make_basic_bidirectional_list<node_type, allocator_type>(ilist, alloc);
		print_bidirectional_list(head, std::cout);

		bi_node_helper::insert_after(head, 6, alloc);
		bi_node_helper::insert_after(head, 7, alloc);

		head=bi_node_helper::insert_before(head, 8, alloc);
		print_bidirectional_list(head, std::cout);

		bi_node_helper::insert_after<node_type,allocator_type>(head, 4, 5, alloc);
		print_bidirectional_list(head, std::cout);
		bi_node_helper::insert_after<node_type,decltype(ilist.begin()),allocator_type>(head, ilist.begin(), ilist.end(), alloc);
		print_bidirectional_list(head, std::cout);

		
		head = bi_node_helper::erase(head, alloc);
		print_bidirectional_list(head, std::cout);
	
		head=bi_node_helper::erase_sublist(head, step_forward(head, 3), alloc);
		print_bidirectional_list(head, std::cout);*/

		
	}

	//���Զ���������
	void my_binary_search_tree_test()
	{
		using namespace fantasil;
		
		using node_type = binary_search_tree_node<int, int>;
		using tree_type = binary_search_tree<int, int>;
		std::initializer_list<std::pair<int, int>> ilist{ {5,5,},{11,11}, { 2,2 },{4,4},{6,6},{7,7},{8,8} };
		tree_type tree{};
		for (auto p = ilist.begin(); p != ilist.end(); ++p)
		{
			tree.insert(p->first, p->second);
		}
		std::cout << tree << std::endl;
		for (auto p = ilist.begin(); p != ilist.end(); ++p)
		{
			tree.erase(p->first);
			std::cout << tree << std::endl;
		}
	}

	//����avl��
	void my_avl_tree_test()
	{
		using namespace fantasil;

		using node_type = avl_tree_node<int, int>;
		using tree_type = avl_tree<int, int>;
		std::initializer_list<std::pair<int, int>> ilist{ {5,5,},{11,11}, { 2,2 },{4,4},{6,6},{7,7},{8,8} };
		tree_type tree{};
		for (auto p = ilist.begin(); p != ilist.end(); ++p)
		{
			tree.insert(p->first, p->second);
		}
		std::cout << tree << std::endl;
		for (auto p = ilist.begin(); p != ilist.end(); ++p)
		{
			tree.erase(p->first);
			std::cout << tree << std::endl;
		}
	}

	//������չ��
	void my_splay_tree_test()
	{
		using namespace fantasil;

		using node_type = splay_tree_node<int, int>;
		using tree_type = splay_tree<int, int>;
		std::initializer_list<std::pair<int, int>> ilist{ {5,5,},{11,11}, { 2,2 },{4,4},{6,6},{7,7},{8,8} };
		tree_type tree{};
		for (auto p = ilist.begin(); p != ilist.end(); ++p)
		{
			tree.insert(p->first, p->second);
			std::cout << tree << std::endl;
		}

		for (auto p = ilist.begin(); p != ilist.end(); ++p)
		{
			tree.erase(p->first);
			std::cout << tree << std::endl;
		}
	}

	void my_bi_iter_test()
	{
		using namespace fantasil;
		using namespace fantasil::bi_node_helper;
		using node_type = bi_node<int>;
		using node_ptr = node_type*;
		using allocator_type = std::allocator<node_type>;
		using iterator = my_bidirectional_iterator<node_type>;

	/*	allocator_type alloc;
		std::initializer_list<int> ilist{ 1,2,3,4,5,6,7,8,9,10};

		node_ptr head = make_basic_bidirectional_list<node_type, allocator_type>(ilist, alloc);
		iterator iter{ head };

		while (iter)
		{
			std::cout << *iter << " ";
			++iter;
		}

		std::cout << "\n";
		
		iter.data(head);
		while (iter)
		{
			std::cout << iter->_val << " ";
			try {
				iter += 2;
			}
			catch (std::logic_error& e)
			{
				std::cout << e.what() << std::endl;
				break;
			}
		}
		std::cout << "\n";
		iter.data(head);
		while (iter)
		{
			std::cout << *iter << " ";
			try {
				iter += 3;
			}
			catch (std::logic_error& e)
			{
				std::cout << e.what() << std::endl;
				break;
			}
		}

		iter.data(head);
		iterator sec{ head };
		if (iter == sec)
		{
			std::cout << "iter==sec";
		}
		else {
			std::cout << "iter !=sec";
		}

		++sec;
		if (iter == sec)
		{
			std::cout << "iter==sec";
		}
		else {
			std::cout << "iter !=sec";
		}

		iterator third = sec + 5;
		std::cout << *third << std::endl;*/


	}

	void my_bi_list_test()
	{
		using namespace fantasil;
		
		my_bidirectional_list<int> list{ {1,2,3,4,5,6,7,8,9,10} };
		std::cout << list;

		list.insert_after(list.begin(), 5, 100);
		std::cout << list;

		list.insert_before(list.begin(), 4, 200);
		std::cout << list;

		list.erase(list.begin());
		std::cout << list;

		list.erase(list.begin(), 5);
		std::cout << list;


	}

	//���Ժ�����ڵ�
	void my_rb_node_test()
	{
	/*	using node_type = rb_tree_node<int, int>;
		using node_ptr = node_type*;
		using allocator_type = std::allocator<node_type>;
		using cmp_type = std::less<int>;
		allocator_type alloc{};
		cmp_type cmp{};
		node_ptr root = alloc.allocate(1);
		init_rb_tree_node_type(root, 1, 1, rb_clr::black);
		std::vector<std::pair<int, int>> vec;
		for (size_t i = 2; i != 10; ++i)
		{
			std::pair<int, int> pr{ i,i };
			vec.push_back(pr);

		}

		int m = 5, n = 4;
		root = fantasil::rb_tree_node_helper::rb_tree_insert(root, m, n, alloc, cmp);

		root=fantasil::rb_tree_node_helper::rb_tree_insert(root, vec.begin(), vec.end(), alloc, cmp);
		binary_tree_node_helper::pre_order(root, std::cout);

		for (size_t i = 9; i != 0; --i)
		{
			std::cout << "\nrb tree erase " << i << " :\n";
			node_ptr node = binary_tree_node_helper::binary_tree_find(root, i, cmp);
			root = rb_tree_node_helper::rb_tree_erase(root, node, alloc);
			if (root)
				binary_tree_node_helper::pre_order(root, std::cout);
			else
				std::cout << "{empty}" << std::endl;
		}*/

		
	}


	//����������������
	void my_basic_binary_tree_iter_test()
	{
		using namespace fantasil;
		using value_type = int;
		using node_type = binary_tree_node<value_type>;
		using node_ptr = node_type*;
		using iterator = basic_tree_iterator<node_type>;
		using allocator_type = std::allocator<node_type>;
		using compare_type = std::less<int>;
		compare_type cmp{};
		allocator_type alloc{};
		std::vector<value_type> vec{ 1,2,3,4,5,6 };
		
		node_ptr root = binary_tree_node_helper::make_basic_bianry_tree<node_type>(vec.begin(), vec.end(), alloc, cmp);
		iterator rot{ root };
		std::cout << *rot << std::endl;
		++rot;
		std::cout << *rot << std::endl;
		--rot;
		std::cout << *rot << std::endl;


	}

	//��������������
	void my_basic_binary_tree_test()
	{
		using namespace fantasil;
		using value_type = int;
		using node_type = fantasil::binary_tree_node<value_type>;
		using node_ptr = node_type*;
		using allocator_type = std::allocator<node_type>;
		using iterator = basic_tree_iterator<node_type>;

		std::initializer_list<int> ilist = { 5,11,2,4,6,7,8 };
		basic_binary_tree<int, allocator_type> tree{ ilist };
		std::cout << tree << std::endl;
		std::vector<int> vec{ 9,19,2,29,15 };
		basic_binary_tree<int> tree_2{ vec.begin(),vec.end() };
		std::cout << tree_2 << std::endl;
		tree.insert(70);

		tree.insert(5, 90);
		tree.insert(vec.begin(), vec.end());
		std::cout << tree << std::endl;

		iterator p = tree.find(70);
		if (p)
			tree.erase(p);
		std::cout << tree << std::endl;


	}

	//���������
	void my_rb_tree_test()
	{
		using namespace fantasil;
		using key_type = int;
		using value_type = int;
		using node_type = rb_tree_node<key_type, value_type>;
		using node_ptr = node_type*;
		using allocator_type = std::allocator<node_type>;
		using iterator = rb_tree_iterator<node_type>;
		
		std::array<int, 5> a1{ 1,2,3,4,5 };
		std::array<int, 5> a2{ 5,4,3,2,1 };
		std::vector<std::pair<int, int>> vec;
		for (size_t i = 0; i != 5; ++i)
		{
			std::pair<int, int> pr{ a1[i],a2[i] };
			vec.push_back(pr);
		}

		rb_tree<int, int> tree{ vec.begin(),vec.end() };
		std::cout << tree << std::endl;
		tree.insert(10, 9);
		std::cout << tree << std::endl;

		
	}


	void b_tree_node_test()
	{
		using namespace fantasil;
		using namespace fantasil::basic_b_tree_node_helper;
		/*
		* Degree=3,elem_max_num=5,cld_max_num=6
		*/
		using node_type = basic_b_tree_node<char, char, 3>;
		using node_ptr = node_type*;
		using allocator_type = std::allocator<node_type>;



		//allocator_type alloc{};
		//node_ptr node = new node_type{};

		//
		//std::vector<std::pair<int, int>> vec;
		//for (int i = 0; i != node_type::max_elem_num-1; ++i)
		//{
		//	std::pair<int, int> p{ i * 2,i * 2 };
		//	vec.push_back(p);
		//}
		//std::vector<node_ptr> vec2{};
		//for (size_t n = 0; n != node_type::max_cld_num-1; ++n)
		//{
		//	vec2.push_back(nullptr);
		//}
		//
		//node->_elem.insert(node->_elem.begin(), vec.begin(), vec.end());
		//node->_children.insert(node->_children.begin(), vec2.begin(), vec2.end());
	
		//node->_parent = nullptr;
		////�۲�0��node��״̬
		//

		////�۲�1��elem_into_unfull_node
		//std::pair<int, int> p{ 20,20 };
		//elem_into_unfull_node(node, p);
		//system("pause");

		////�۲�2��split
		//split(node);
		//system("pause");
		node_ptr root = nullptr;
		std::initializer_list<char> ilist = { 'F','S','Q','K','C','L','H','T','V','W','M','R','N','P','A','B','X','Y','D','Z','E' };
		for (auto p = ilist.begin(); p != ilist.end(); ++p)
		{
			
			root = insert(root, *p, *p);
		}

		level_order(root, std::cout);

		for (auto p = ilist.begin(); p != ilist.end(); ++p)
		{
			root = erase(root, *p);
			std::cout << "\n after erase " << *p << ":\n";
			level_order(root, std::cout);
		}

		int i = 5;

	}

	//˫��ѭ����������
	void bidirectional_circular_list_test()
	{
		using list = bidirectional_circular_list<int>;

		list li{};
		std::initializer_list<int> aa{ 1,2,3,4,5,6,7,8 };

		li.insert_after(li.end(), aa.begin(), aa.end());
		std::cout << li << std::endl;

		auto p = li.find(5);
		li.erase(p);
		std::cout << li << std::endl;
		p = li.find(1);
		li.erase(p);
		std::cout << li << std::endl;
		p = li.find(8);
		li.erase(p);
		std::cout << li << std::endl;

		p = li.find(6);
		auto q = li.find(3);
		li.erase(p, q);
		std::cout << li << std::endl;

	}

	void fib_heap_node_test()
	{
		using namespace std;
		using node_type = fib_heap_node<size_t, size_t>;
		using node_ptr = node_type*;

		node_ptr min = nullptr;
		for (size_t i = 0; i != 10; ++i)
		{
			std::pair<size_t, size_t> tmp{ i,i };
			min=fib_heap_node_helper::insert(min, tmp);
		}

		min=fib_heap_node_helper::extract_min(min, 10);

		node_ptr ll = min->_child->_child;
		node_ptr lr = min->_child->_child->_right;
		size_t j = 4;
		size_t k = 3;
		min = fib_heap_node_helper::decrease_key(min, lr, k);
		min = fib_heap_node_helper::decrease_key(min, ll, j);

		int i = 5;
	}

	void fib_heap_tree_test()
	{
		using namespace std;
		
		fib_heap_tree<int,int> fht{};
		for (int i = 0; i != 10; ++i)
		{
			std::pair<int, int> tmp{ i,i };
			fht.insert(tmp);
		}

		fht.extract_min();

		int i = 5;
	}

	void proto_veb_test()
	{
		using namespace std;

		proto_vEB<int, 256> pv{};

		

		auto min_and_max = [&pv]()
			{
				try {
					size_t min = pv.min();
					std::cout << "min:" << min << std::endl;
					size_t max = pv.max();
					std::cout << "max:" << max << std::endl;
				}
				catch (std::exception& e)
				{
					std::cout << e.what() << std::endl;
				}
			};

		auto pre_and_suc = [&pv](size_t i)
			{
				if (pv.exist(i))
				{
					std::cout << "[";
					try {
						size_t pre = pv.precursor(i);
						std::cout << pre;
					}
					catch (std::logic_error& e)
					{
						std::cout << "NIL";
					}
					std::cout << ",";
					std::cout << pv.member(i) << ",";
					try {
						size_t suc = pv.successor(i);
						std::cout << suc;
					}
					catch (std::logic_error& e)
					{
						std::cout << "NIL";
					}
					std::cout << "]";
				}
			};
		//min_and_max();
		vector<int> vec{ 2,3,4,5,7,14,15 };

		for (auto p = vec.begin(); p != vec.end(); ++p)
		{
			pv.insert(*p, *p);
		}
		
		for (size_t i = 0; i != 16; ++i)
		{
			pre_and_suc(i);
		}

		for (size_t i = 0; i != 16; ++i)
		{
			pv.erase(i);
		}
		//pv.erase(2);
		

		int i = 5;
	}




	void veb_test()
	{
		using namespace fantasil;
		using namespace fantasil::veb;

		van_emde_boas<int> veb{ 16 };

		auto pre = [&veb](size_t i)
			{
				std::cout << "[";
				try {
					
					int j = veb.precursor(i);
					std::cout << j;
				}
				catch (std::logic_error& e)
				{
					std::cout << "nil";
				}
				std::cout << ",";
			};

		auto suc = [&veb](size_t i)
			{
				std::cout << ",";
				try {
					std::cout << veb.successor(i);
				}
				catch (std::logic_error& e)
				{
					std::cout << "nil";
				}
				std::cout << "]" << std::endl;
			};

		auto pre_and_suc = [&veb,&pre,&suc]()
			{
				for (size_t i = 0; i != 16; ++i)
				{
					pre(i);
					std::cout << i;
					suc(i);
				}
			};

		auto print = [&veb]()
			{
				for (size_t i = 0; i != veb.capacity(); ++i)
				{
					std::cout << "[" << i << ",";
					if (veb.exist(i))
					{
						std::cout << veb.member(i);
					}
					else {
						std::cout << "nil";
					}
					std::cout << "]  ";
				}
			};

		for (size_t i = 0; i != 16; ++i)
		{
			std::cout << veb.exist(i) << ",";
			veb.insert(i, i);
			std::cout << veb.exist(i) << std::endl;
		}

		pre_and_suc();

		for (size_t i = 0; i != 16; ++i)
		{
			std::cout << "after erase " << i << ":";
			veb.erase(i);
			print();
			std::cout << std::endl;
		}

		

		int i = 5;
		
	}


	//void veb_2_test()
	//{
	//	using namespace fantasil::veb_2_space;
	//	
	//	veb_2<size_t, 16> pv{};
	//	auto pre_and_suc = [&pv](size_t pos)
	//		{
	//			std::cout << "[";
	//			try {
	//				size_t i = pv.precursor(pos);
	//				std::cout << i << ",";
	//			}
	//			catch (std::logic_error& e)
	//			{
	//				std::cout << "nil,";
	//			}
	//			std::cout << pos << ",";
	//			try {
	//				size_t i = pv.successor(pos);
	//				std::cout << i ;
	//			}
	//			catch (std::logic_error& e)
	//			{
	//				std::cout << "nil";
	//			}
	//			std::cout << "]\n";

	//		};

	//	auto print_if_exist = [&pv]()
	//		{
	//			if (pv.empty())
	//				std::cout << "pv is empty!\n";
	//			else {
	//				for (size_t i = 0; i != 16; ++i)
	//				{
	//					if (pv.exist(i))
	//						std::cout << "pv[" << i << "] exists" << std::endl;
	//				}
	//			};
	//		};
	//	//pv.insert(4, 4);
	//	std::vector<size_t> vec{};
	//	for (size_t i = 0; i != 16; ++i)
	//	{
	//		vec.push_back(i);
	//	}
	//	
	//	std::cout << "before insert:" << std::endl;
	//	print_if_exist();

	//	for (size_t i = 0; i != 16; ++i)
	//	{
	//		pv.insert(i, i);
	//	}
	//	std::cout << "after insert:" << std::endl;
	//	print_if_exist();

	//

	//	for (size_t i = 0; i != 16; ++i)
	//	{
	//		pre_and_suc(i);
	//	}

	//	for (auto i : vec)
	//	{
	//		std::cout << "try erase " << i << std::endl;
	//		pv.erase(i);
	//		int j = 4;
	//		
	//	}
	//	int j = 5;

	//}
	void test()
	{
		veb_test();
	}

	
	
}