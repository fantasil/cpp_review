#pragma once
#include<utility>
#include<vector>
#include<assert.h>
#include<cmath>
#include<limits>

namespace fantasil
{

	namespace veb {
		//向上取整最接近的2的幂次
		inline size_t next_power_of_2(size_t x)
		{
			x -= 1;			//假设现在最高位为n
			x |= x >> 1;	//次高位置n-1为1,(n和n-1均为1)
			x |= x >> 2;	//接下去的两位为1,(n到n-3均为1）
			x |= x >> 4;	//一次类推
			x |= x >> 8;
			x |= x >> 16;
			x |= x >> 32;	//(低于n的位置均为1）
			return ++x;
		}

		inline size_t upper_sqrt(size_t N)
		{
			size_t k = std::ceil(log2(N) / 2);
			return std::pow(2, k);
		}

		inline size_t lower_sqrt(size_t N)
		{
			size_t k = std::floor(log2(N) / 2);
			return std::pow(2, k);
		}

		inline size_t high(const size_t N,const size_t U)
		{
			return std::floor(N / U);
		}

		inline size_t low(const size_t N,const size_t U)
		{
			return N % U;
		}

		inline size_t index(const size_t x, const size_t y,const size_t U)
		{
			return x * U + y;
		}






		template<typename T>
		struct van_emde_boas{
		public:
			//typedef
			using value_type = T;
			using pointer = T*;
			using summary_type = van_emde_boas<size_t>;
			using summary_ptr = summary_type*;
			using cluster_type = van_emde_boas<T>;
			using cluster_ptr = cluster_type*;
			using record_type = std::pair<size_t, pointer>;
		public:
			van_emde_boas(size_t n);	//指定大小和无效对象
			~van_emde_boas() {}
		public:
			bool exist(size_t pos) const;	//pos位置的元素是否存在
			T min() const;			//最小元素
			T max() const;			//最大元素
			T precursor(size_t pos) const;	//前驱元素
			T successor(size_t pos) const;	//后继元素
			void insert(size_t pos, const T& val);	//指定位置插入元素
			void erase(size_t pos);	//删除指定位置元素
			T member(size_t pos) const;	//获取指定位置的元素
		public:
			inline size_t capacity() const;	//节点容量
			size_t min_pos() const;		//最小值的索引
			size_t max_pos() const;		//最大值的索引
			size_t precursor_pos(size_t pos) const;	//前驱的索引
			size_t successor_pos(size_t pos) const;	//后继的索引
			size_t cluster_index(size_t pos) const;		//pos所对应的簇索引
			size_t cluster_offset(size_t pos) const;	//pos在对应簇内的偏移
			size_t current_offset(size_t cl_index, size_t cl__offset) const;	//i簇内偏移为offset的位置在当前节点内的偏移
			void empty_insert(size_t offset,const T& val);
			inline bool empty() const;
		public:
			//data member
			size_t _size;			//节点大小N
			record_type _min;			//存储最小值的节点
			record_type _max;			//存储最大值的节点
			summary_ptr _summary;		//信息节点
			std::vector<cluster_ptr> _cluster;	//簇数组
			static T invalid_obj;
			static const size_t invalid_pos = std::numeric_limits<size_t>::max();
		};

		

		template<typename T>
		van_emde_boas<T>::van_emde_boas(size_t n)
		{

			if (n < 2)
				throw std::logic_error{ "size必须大于2" };	
			_size = next_power_of_2(n);
			_min = _max = { invalid_pos,nullptr };	//_min和_max是必须初始化的
			if (n > 2)
			{
				//需要初始化_summary和_cluster;
				size_t U = upper_sqrt(_size);
				size_t DU = lower_sqrt(_size);
				_summary = new summary_type{ U };
				_cluster = std::vector<cluster_ptr>(U);
				for (size_t i = 0; i != U; ++i)
				{
					_cluster[i] = new cluster_type{ DU };
				}
			}
			else {
				_summary = nullptr;
				_cluster = std::vector<cluster_ptr>(0);
			}
		}

		template<typename T>
		inline bool van_emde_boas<T>::exist(size_t pos) const
		{
			if (empty())
				return false;
			if (pos == _min.first || pos == _max.first)
				return true;
			if (_size == 2)
				return false;
			return _cluster[cluster_index(pos)]->exist(cluster_offset(pos));
		}

		template<typename T>
		inline T van_emde_boas<T>::min() const
		{
			if (_min.second)
				return *_min.second;
			throw std::logic_error{ "empty set" };
		}

		template<typename T>
		inline T van_emde_boas<T>::max() const
		{
			if (_max.second)
				return *_max.second;
			throw std::logic_error{ "empty set" };
		}

		template<typename T>
		inline T van_emde_boas<T>::precursor(size_t pos) const
		{
			if (_size == 2)
			{
				if (pos == 1 && _min.first == 0)
				{
					return *_min.second;	//基础结构
				}
				throw std::logic_error{ "precursor():no pre elem" };
			}
			else if(_max.second&&pos>_max.first){
				return *_max.second;	//max就是前驱
			}
			
			if (!_cluster[cluster_index(pos)]->empty())
			{
				//对应簇不为空
				size_t min_pos = _cluster[cluster_index(pos)]->min_pos();
				if (cluster_offset(pos) > min_pos)
				{
					//在簇内寻找前驱
					return _cluster[cluster_index(pos)]->precursor(cluster_offset(pos));
				}
			}

			//寻找后继簇的极大值作为前驱
			try {
				size_t pre_cluster = _summary->precursor_pos(cluster_index(pos));
				return _cluster[pre_cluster]->max();
			}
			catch (std::logic_error& e)
			{
				//没有后继簇，查看_min
				if (_min.second && _min.first < pos)
					return *(_min.second);
				throw e;
			}
				
				
			
		}

		template<typename T>
		inline T van_emde_boas<T>::successor(size_t pos) const
		{
			if (_size == 2)
			{
				if (pos == 0 && _max.first == 1)
					return *_max.second;
				throw std::logic_error{ "no suc elem" };	//基础结构
			}
			else if (_min.second&&pos < _min.first)
			{
				return *_min.second;						//_min就是后记
			}
			else {

				if (!_cluster[cluster_index(pos)]->empty())
				{
					//对应簇不为空
					size_t max_offset = _cluster[cluster_index(pos)]->max_pos();
					if (cluster_offset(pos) < max_offset)
					{
						//对应簇内有后继
						return _cluster[cluster_index(pos)]->successor(cluster_offset(pos));
					}
				}
				 
				//尝试使用后继簇的最小值
				try {
					//尝试使用后继簇的极小值作为后继
					size_t suc_cluster = _summary->successor_pos(cluster_index(pos));
					return _cluster[suc_cluster]->min();
				}
				catch (std::logic_error& e)
				{
					throw e;	//没有后继
				}
				
			}
		}

		template<typename T>
		inline void van_emde_boas<T>::insert(size_t pos, const T& val)
		{
			if (empty())
			{
				empty_insert(pos, val);	//空表插入
			}
			else
			{
				record_type p{ pos,new T{val} };
				if (pos < _min.first)
				{
					std::swap(_min, p);	//取代极小值
				}

				if (_size > 2)
				{
					if (_cluster[cluster_index(pos)]->empty())
					{
						//所在簇为空，那么需要summary插入
						_summary->insert(cluster_index(p.first), cluster_index(p.first));
						_cluster[cluster_index(p.first)]->empty_insert(cluster_offset(p.first), *(p.second));
					}
					else {
						//否则递归哈如即可
						_cluster[cluster_index(p.first)]->insert(cluster_offset(p.first), *(p.second));
					}
				}

				if (pos > _max.first)
					_max = p;
			}
		}

		template<typename T>
		inline void van_emde_boas<T>::erase(size_t pos)
		{
			if (_min.first == _max.first)
			{
				//仅有一个元素，删除即可
				delete _min.second;
				_min = _max = { invalid_pos,nullptr };
				return;
			}
			else if(_size==2){
				//基础结构
				if (pos == 0)	//删除_min
				{
					delete _min.second;
					_min = _max;
				}
				else {			//删除_max
					delete _max.second;
					_max = _min;
				}
				return;
			}
			if (pos == _min.first)
			{
				//当前节点下的首簇
				size_t first_cluster = _summary->min_pos();
				size_t offset = _cluster[first_cluster]->min_pos();
				size_t x = current_offset(first_cluster, offset);	//次小元素在当前节点的偏移位置
				//record_type second_min = _cluster[first_cluster]->_min;	//次小元素
				std::swap(_min.second,_cluster[first_cluster]->_min.second);		//交换元素
				_min.first = x;										//重置偏移
				pos = x;
			}
			_cluster[cluster_index(pos)]->erase(cluster_offset(pos));	//递归删除pos位置的元素，pos或已是最小元素

			if (_cluster[cluster_index(pos)]->empty())
			{	//如果对应簇为空，就删除簇号
				_summary->erase(cluster_index(pos));
			}
			//如果删除的是_max，还得更新_max
			else if(_max.first == pos)
			{
				if (_summary->empty())
				{
					//如果当前簇为空，那么
					_max = _min;
				}
				else {
					size_t max_cluster = _summary->max_pos();	//节点内的最大簇索引
					if (!_cluster[max_cluster]->empty())
					{
						//使用最大簇号内的极大值取代_max
						size_t off = _cluster[max_cluster]->max_pos();	//极大值在对应簇内的偏移
						size_t cur_off = current_offset(max_cluster, off);	//该值在当前节点内的偏移
						_max.first = cur_off;
						_max.second = _cluster[max_cluster]->_min.second;	//使用次大值取代_max
					}
				}
				
				
			
			}

		}

		template<typename T>
		inline T van_emde_boas<T>::member(size_t pos) const
		{
			if (!exist(pos))
				throw std::logic_error{ "no such elem" };
			if (_min.first == pos)
				return *_min.second;
			if (_max.first == pos)
				return *_max.second;
			return _cluster[cluster_index(pos)]->member(cluster_offset(pos));
		}

		template<typename T>
		inline size_t van_emde_boas<T>::capacity() const
		{
			return _size;
		}

		template<typename T>
		inline size_t van_emde_boas<T>::min_pos() const
		{
			if (_min.second)
				return _min.first;
			throw std::logic_error{ "no min elem" };
		}

		template<typename T>
		inline size_t van_emde_boas<T>::max_pos() const
		{
			if (_max.second)
				return _max.first;
			throw std::logic_error{ "no max elem" };
		}

		template<typename T>
		inline size_t van_emde_boas<T>::precursor_pos(size_t pos) const
		{
			if (_size == 2)
			{
				if (pos == 1 && _min.first == 0)
					return 0;
				throw std::logic_error{ "no pre elem" };	//基础结构(size=2)
			}
			else if (pos > _max.first)
			{
				return _max.first;							//前驱是max
			}
			else {

				if (!_cluster[cluster_index(pos)]->empty())
				{
					size_t min = _cluster[cluster_index(pos)]->min_pos();	//簇内最小偏移,如果簇不为空
					if (cluster_offset(pos) > min)
					{
						//簇内有前驱
						size_t offset = _cluster[cluster_index(pos)]->precursor_pos(cluster_offset(pos));
						return current_offset(cluster_index(pos), offset);
					}
				}
				
				//前驱簇内的极大值为前驱
				try {
					size_t pre_cluster = _summary->precursor(cluster_index(pos));
					size_t offset = _cluster[pre_cluster]->max_pos();
					return current_offset(pre_cluster, offset);
				}
				catch (std::logic_error& e)
				{
					//前驱簇不存在，但是极小值存在，那么极小值就是前驱
					if (_min.second && pos > _min.first)
						return _min.first;
					throw e;	//前驱簇不存在
				}
				

				//try {
				//	size_t min = _cluster[cluster_index(pos)]->min_pos();	//簇内最小偏移,如果簇不为空
				//	if (cluster_offset(pos) > min)
				//	{
				//		//簇内有前驱
				//		size_t offset = _cluster[cluster_index(pos)]->precursor_pos(cluster_offset(pos));
				//		return current_offset(cluster_index(pos), offset);
				//	}
				//	else {
				//		//前驱簇内的极大值为前驱
				//		try {
				//			size_t pre_cluster = _summary->precursor(cluster_index(pos));
				//			size_t offset = _cluster[pre_cluster]->max_pos();
				//			return current_offset(pre_cluster, offset);
				//		}
				//		catch (std::logic_error& e)
				//		{
				//			//前驱簇不存在，但是极小值存在，那么极小值就是前驱
				//			if (_min.second && pos > _min.first)	
				//				return _min.first;
				//			throw e;	//前驱簇不存在
				//		}
				//	}
				//}
				//catch (std::logic_error& e)
				//{
				//	//前驱簇内的极大值为前驱
				//	try {
				//		size_t pre_cluster = _summary->precursor(cluster_index(pos));
				//		size_t offset = _cluster[pre_cluster]->max_pos();
				//		return current_offset(pre_cluster, offset);
				//	}
				//	catch (std::logic_error& e)
				//	{
				//		//前驱簇不存在，但是极小值存在，那么极小值就是前驱
				//		if (_min.second && pos > _min.first)
				//			return _min.first;
				//		throw e;	//前驱簇不存在
				//	}
				//}
				
			}
		}

		template<typename T>
		inline size_t van_emde_boas<T>::successor_pos(size_t pos) const
		{
			if (_size == 2)
			{
				//基础结构
				if (pos == 0 && _max.first == 1)
					return 1;
				throw std::logic_error{ "no suc elem" };

			}
			else if (_min.second && pos < _min.first)
			{
				return _min.first;	//极小值就是后继
			}
			else {


				if (!_cluster[cluster_index(pos)]->empty())
				{
					//尝试在所在簇内寻找后继
					size_t max_pos = _cluster[cluster_index(pos)]->max_pos();
					if (cluster_offset(pos) < max_pos)
					{
						//对应簇内存在后继
						size_t offset = _cluster[cluster_index(pos)]->successor_pos(cluster_offset(pos));
						return current_offset(cluster_index(pos), offset);
					}
				}
				 
				//后继簇的极小值作为后继
				try {
					size_t suc_cluster = _summary->successor_pos(cluster_index(pos));
					size_t offset = _cluster[suc_cluster]->min_pos();
					return current_offset(suc_cluster, offset);
				}
				catch (std::logic_error& e)
				{
					throw e;	//没有后继
				}
				


				//try {
				//	//尝试在所在簇内寻找后继
				//	size_t max_pos = _cluster[cluster_index(pos)]->max_pos();
				//	if (cluster_offset(pos) < max_pos)
				//	{
				//		//对应簇内存在后继
				//		size_t offset = _cluster[cluster_index(pos)]->successor_pos(cluster_offset(pos));
				//		return current_offset(cluster_index(pos), offset);
				//	}
				//	else {
				//		//后继簇的极小值作为后继
				//		try {
				//			size_t suc_cluster = _summary->successor_pos(cluster_index(pos));
				//			size_t offset = _cluster[suc_cluster]->min_pos();
				//			return current_offset(suc_cluster, offset);
				//		}
				//		catch (std::logic_error& e)
				//		{
				//			throw e;	//没有后继
				//		}
				//	}
				//}
				//catch (std::logic_error& e)
				//{
				//	//所在簇为空
				//	//后继簇的极小值作为后继
				//	try {
				//		size_t suc_cluster = _summary->successor_pos(cluster_index(pos));
				//		size_t offset = _cluster[suc_cluster]->min_pos();
				//		return current_offset(suc_cluster, offset);
				//	}
				//	catch (std::logic_error& e)
				//	{
				//		throw e;	//没有后继
				//	}
				//}
			}
			
		}

		template<typename T>
		inline size_t van_emde_boas<T>::cluster_index(size_t pos) const
		{
			return high(pos, upper_sqrt(_size));
		}

		template<typename T>
		inline size_t van_emde_boas<T>::cluster_offset(size_t pos) const
		{
			return low(pos, upper_sqrt(_size));
		}

		template<typename T>
		inline size_t van_emde_boas<T>::current_offset(size_t cl_index, size_t cl_offset) const
		{
			return index(cl_index, cl_offset, upper_sqrt(_size));
		}

		template<typename T>
		inline void van_emde_boas<T>::empty_insert(size_t offset, const T& val)
		{
			record_type tmp{ offset,new T{val} };
			_min = _max = tmp;
		}

		template<typename T>
		inline bool van_emde_boas<T>::empty() const
		{
			return _min.second == nullptr;
		}


	}
}
