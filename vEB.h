#pragma once
#include<utility>
#include<vector>
namespace fantasil
{

	inline size_t high(const size_t N)
	{
		return 0;
	}

	inline size_t low(const size_t N)
	{
		return 0;
	}

	inline size_t index(const size_t x, const size_t y)
	{
		return 0;
	}



	template<typename T>
	struct van_emde_boas_node {
	public:
		//typedef
		using value_type = T;
		using pointer = T*;
		using summary_type = van_emde_boas_node<bool>;
		using summary_ptr = summary_type*;
		using cluster_type = van_emde_boas_node<T>;
		using cluster_ptr = cluster_type*;
	public:
		van_emde_boas_node(size_t n);
		~van_emde_boas_node() {}
	public:
		T min() const;
		T max() const;
		void insert(size_t pos, const T& val);
	private:
		inline bool empty() const;
		inline bool only_one() const;
		inline bool more_than_one() const;
	public:
		//data member
		std::pair<size_t, pointer> _min;
		std::pair<size_t, pointer> _max;
		summary_ptr _summary;
		std::vector<cluster_ptr> _cluster;
	};


	template<typename T>
	van_emde_boas_node<T>::van_emde_boas_node(size_t n)
	{
		_min = { -1,nullptr };
		_max = { -1,nullptr };
		if (n > 2)
		{
			_summary = new summary_type{ high(n) };
			for (size_t i = 0; i != low(n); ++i)
			{
				cluster_ptr tmp = new cluster_type{ high(n) };
				_cluster.push_back(tmp);
			}
		}
		else {
			_summary = nullptr;
			for (size_t i = 0; i != 2; ++i)
			{
				_cluster.push_back(nullptr);
			}
		}
	}

	template<typename T>
	inline T van_emde_boas_node<T>::min() const
	{
		if (_min.second)
			return *_min.second;
		else
			throw std::exception{ "veb::min():no min" };
	}

	template<typename T>
	inline T van_emde_boas_node<T>::max() const
	{
		if (_max.second)
			return *_max.second;
		else
			throw std::exception{ "veb::max():no max" };
	}

	template<typename T>
	void van_emde_boas_node<T>::insert(size_t pos, const T& val)
	{
		//T* tmp = new T{ val };
		//std::pair<size_t, pointer> p{ pos,tmp };

		//if (empty())
		//{
		//	_min = _max = p;
		//}
		//else if (only_one())
		//{
		//	pos > _min.first ? _max = p : _min = p;
		//}
		//else {

		//}
	}

	template<typename T>
	inline bool van_emde_boas_node<T>::empty() const
	{
		return _min.second == nullptr;
	}

	template<typename T>
	inline bool van_emde_boas_node<T>::only_one() const
	{
		return _min.second != nullptr && _min.second == _max.second;
	}

	template<typename T>
	inline bool van_emde_boas_node<T>::more_than_one() const
	{
		return _min.second != nullptr && _min.second != _max.second;
	}

}
