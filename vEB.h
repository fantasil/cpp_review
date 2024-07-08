#pragma once
#include<utility>
#include<vector>
#include<assert.h>
#include<cmath>
#include<limits>

namespace fantasil
{

	namespace veb {
		//����ȡ����ӽ���2���ݴ�
		inline size_t next_power_of_2(size_t x)
		{
			x -= 1;			//�����������λΪn
			x |= x >> 1;	//�θ�λ��n-1Ϊ1,(n��n-1��Ϊ1)
			x |= x >> 2;	//����ȥ����λΪ1,(n��n-3��Ϊ1��
			x |= x >> 4;	//һ������
			x |= x >> 8;
			x |= x >> 16;
			x |= x >> 32;	//(����n��λ�þ�Ϊ1��
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
			van_emde_boas(size_t n);	//ָ����С����Ч����
			~van_emde_boas() {}
		public:
			bool exist(size_t pos) const;	//posλ�õ�Ԫ���Ƿ����
			T min() const;			//��СԪ��
			T max() const;			//���Ԫ��
			T precursor(size_t pos) const;	//ǰ��Ԫ��
			T successor(size_t pos) const;	//���Ԫ��
			void insert(size_t pos, const T& val);	//ָ��λ�ò���Ԫ��
			void erase(size_t pos);	//ɾ��ָ��λ��Ԫ��
			T member(size_t pos) const;	//��ȡָ��λ�õ�Ԫ��
		public:
			inline size_t capacity() const;	//�ڵ�����
			size_t min_pos() const;		//��Сֵ������
			size_t max_pos() const;		//���ֵ������
			size_t precursor_pos(size_t pos) const;	//ǰ��������
			size_t successor_pos(size_t pos) const;	//��̵�����
			size_t cluster_index(size_t pos) const;		//pos����Ӧ�Ĵ�����
			size_t cluster_offset(size_t pos) const;	//pos�ڶ�Ӧ���ڵ�ƫ��
			size_t current_offset(size_t cl_index, size_t cl__offset) const;	//i����ƫ��Ϊoffset��λ���ڵ�ǰ�ڵ��ڵ�ƫ��
			void empty_insert(size_t offset,const T& val);
			inline bool empty() const;
		public:
			//data member
			size_t _size;			//�ڵ��СN
			record_type _min;			//�洢��Сֵ�Ľڵ�
			record_type _max;			//�洢���ֵ�Ľڵ�
			summary_ptr _summary;		//��Ϣ�ڵ�
			std::vector<cluster_ptr> _cluster;	//������
			static T invalid_obj;
			static const size_t invalid_pos = std::numeric_limits<size_t>::max();
		};

		

		template<typename T>
		van_emde_boas<T>::van_emde_boas(size_t n)
		{

			if (n < 2)
				throw std::logic_error{ "size�������2" };	
			_size = next_power_of_2(n);
			_min = _max = { invalid_pos,nullptr };	//_min��_max�Ǳ����ʼ����
			if (n > 2)
			{
				//��Ҫ��ʼ��_summary��_cluster;
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
					return *_min.second;	//�����ṹ
				}
				throw std::logic_error{ "precursor():no pre elem" };
			}
			else if(_max.second&&pos>_max.first){
				return *_max.second;	//max����ǰ��
			}
			
			if (!_cluster[cluster_index(pos)]->empty())
			{
				//��Ӧ�ز�Ϊ��
				size_t min_pos = _cluster[cluster_index(pos)]->min_pos();
				if (cluster_offset(pos) > min_pos)
				{
					//�ڴ���Ѱ��ǰ��
					return _cluster[cluster_index(pos)]->precursor(cluster_offset(pos));
				}
			}

			//Ѱ�Һ�̴صļ���ֵ��Ϊǰ��
			try {
				size_t pre_cluster = _summary->precursor_pos(cluster_index(pos));
				return _cluster[pre_cluster]->max();
			}
			catch (std::logic_error& e)
			{
				//û�к�̴أ��鿴_min
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
				throw std::logic_error{ "no suc elem" };	//�����ṹ
			}
			else if (_min.second&&pos < _min.first)
			{
				return *_min.second;						//_min���Ǻ��
			}
			else {

				if (!_cluster[cluster_index(pos)]->empty())
				{
					//��Ӧ�ز�Ϊ��
					size_t max_offset = _cluster[cluster_index(pos)]->max_pos();
					if (cluster_offset(pos) < max_offset)
					{
						//��Ӧ�����к��
						return _cluster[cluster_index(pos)]->successor(cluster_offset(pos));
					}
				}
				 
				//����ʹ�ú�̴ص���Сֵ
				try {
					//����ʹ�ú�̴صļ�Сֵ��Ϊ���
					size_t suc_cluster = _summary->successor_pos(cluster_index(pos));
					return _cluster[suc_cluster]->min();
				}
				catch (std::logic_error& e)
				{
					throw e;	//û�к��
				}
				
			}
		}

		template<typename T>
		inline void van_emde_boas<T>::insert(size_t pos, const T& val)
		{
			if (empty())
			{
				empty_insert(pos, val);	//�ձ����
			}
			else
			{
				record_type p{ pos,new T{val} };
				if (pos < _min.first)
				{
					std::swap(_min, p);	//ȡ����Сֵ
				}

				if (_size > 2)
				{
					if (_cluster[cluster_index(pos)]->empty())
					{
						//���ڴ�Ϊ�գ���ô��Ҫsummary����
						_summary->insert(cluster_index(p.first), cluster_index(p.first));
						_cluster[cluster_index(p.first)]->empty_insert(cluster_offset(p.first), *(p.second));
					}
					else {
						//����ݹ���缴��
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
				//����һ��Ԫ�أ�ɾ������
				delete _min.second;
				_min = _max = { invalid_pos,nullptr };
				return;
			}
			else if(_size==2){
				//�����ṹ
				if (pos == 0)	//ɾ��_min
				{
					delete _min.second;
					_min = _max;
				}
				else {			//ɾ��_max
					delete _max.second;
					_max = _min;
				}
				return;
			}
			if (pos == _min.first)
			{
				//��ǰ�ڵ��µ��״�
				size_t first_cluster = _summary->min_pos();
				size_t offset = _cluster[first_cluster]->min_pos();
				size_t x = current_offset(first_cluster, offset);	//��СԪ���ڵ�ǰ�ڵ��ƫ��λ��
				//record_type second_min = _cluster[first_cluster]->_min;	//��СԪ��
				std::swap(_min.second,_cluster[first_cluster]->_min.second);		//����Ԫ��
				_min.first = x;										//����ƫ��
				pos = x;
			}
			_cluster[cluster_index(pos)]->erase(cluster_offset(pos));	//�ݹ�ɾ��posλ�õ�Ԫ�أ�pos��������СԪ��

			if (_cluster[cluster_index(pos)]->empty())
			{	//�����Ӧ��Ϊ�գ���ɾ���غ�
				_summary->erase(cluster_index(pos));
			}
			//���ɾ������_max�����ø���_max
			else if(_max.first == pos)
			{
				if (_summary->empty())
				{
					//�����ǰ��Ϊ�գ���ô
					_max = _min;
				}
				else {
					size_t max_cluster = _summary->max_pos();	//�ڵ��ڵ���������
					if (!_cluster[max_cluster]->empty())
					{
						//ʹ�����غ��ڵļ���ֵȡ��_max
						size_t off = _cluster[max_cluster]->max_pos();	//����ֵ�ڶ�Ӧ���ڵ�ƫ��
						size_t cur_off = current_offset(max_cluster, off);	//��ֵ�ڵ�ǰ�ڵ��ڵ�ƫ��
						_max.first = cur_off;
						_max.second = _cluster[max_cluster]->_min.second;	//ʹ�ôδ�ֵȡ��_max
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
				throw std::logic_error{ "no pre elem" };	//�����ṹ(size=2)
			}
			else if (pos > _max.first)
			{
				return _max.first;							//ǰ����max
			}
			else {

				if (!_cluster[cluster_index(pos)]->empty())
				{
					size_t min = _cluster[cluster_index(pos)]->min_pos();	//������Сƫ��,����ز�Ϊ��
					if (cluster_offset(pos) > min)
					{
						//������ǰ��
						size_t offset = _cluster[cluster_index(pos)]->precursor_pos(cluster_offset(pos));
						return current_offset(cluster_index(pos), offset);
					}
				}
				
				//ǰ�����ڵļ���ֵΪǰ��
				try {
					size_t pre_cluster = _summary->precursor(cluster_index(pos));
					size_t offset = _cluster[pre_cluster]->max_pos();
					return current_offset(pre_cluster, offset);
				}
				catch (std::logic_error& e)
				{
					//ǰ���ز����ڣ����Ǽ�Сֵ���ڣ���ô��Сֵ����ǰ��
					if (_min.second && pos > _min.first)
						return _min.first;
					throw e;	//ǰ���ز�����
				}
				

				//try {
				//	size_t min = _cluster[cluster_index(pos)]->min_pos();	//������Сƫ��,����ز�Ϊ��
				//	if (cluster_offset(pos) > min)
				//	{
				//		//������ǰ��
				//		size_t offset = _cluster[cluster_index(pos)]->precursor_pos(cluster_offset(pos));
				//		return current_offset(cluster_index(pos), offset);
				//	}
				//	else {
				//		//ǰ�����ڵļ���ֵΪǰ��
				//		try {
				//			size_t pre_cluster = _summary->precursor(cluster_index(pos));
				//			size_t offset = _cluster[pre_cluster]->max_pos();
				//			return current_offset(pre_cluster, offset);
				//		}
				//		catch (std::logic_error& e)
				//		{
				//			//ǰ���ز����ڣ����Ǽ�Сֵ���ڣ���ô��Сֵ����ǰ��
				//			if (_min.second && pos > _min.first)	
				//				return _min.first;
				//			throw e;	//ǰ���ز�����
				//		}
				//	}
				//}
				//catch (std::logic_error& e)
				//{
				//	//ǰ�����ڵļ���ֵΪǰ��
				//	try {
				//		size_t pre_cluster = _summary->precursor(cluster_index(pos));
				//		size_t offset = _cluster[pre_cluster]->max_pos();
				//		return current_offset(pre_cluster, offset);
				//	}
				//	catch (std::logic_error& e)
				//	{
				//		//ǰ���ز����ڣ����Ǽ�Сֵ���ڣ���ô��Сֵ����ǰ��
				//		if (_min.second && pos > _min.first)
				//			return _min.first;
				//		throw e;	//ǰ���ز�����
				//	}
				//}
				
			}
		}

		template<typename T>
		inline size_t van_emde_boas<T>::successor_pos(size_t pos) const
		{
			if (_size == 2)
			{
				//�����ṹ
				if (pos == 0 && _max.first == 1)
					return 1;
				throw std::logic_error{ "no suc elem" };

			}
			else if (_min.second && pos < _min.first)
			{
				return _min.first;	//��Сֵ���Ǻ��
			}
			else {


				if (!_cluster[cluster_index(pos)]->empty())
				{
					//���������ڴ���Ѱ�Һ��
					size_t max_pos = _cluster[cluster_index(pos)]->max_pos();
					if (cluster_offset(pos) < max_pos)
					{
						//��Ӧ���ڴ��ں��
						size_t offset = _cluster[cluster_index(pos)]->successor_pos(cluster_offset(pos));
						return current_offset(cluster_index(pos), offset);
					}
				}
				 
				//��̴صļ�Сֵ��Ϊ���
				try {
					size_t suc_cluster = _summary->successor_pos(cluster_index(pos));
					size_t offset = _cluster[suc_cluster]->min_pos();
					return current_offset(suc_cluster, offset);
				}
				catch (std::logic_error& e)
				{
					throw e;	//û�к��
				}
				


				//try {
				//	//���������ڴ���Ѱ�Һ��
				//	size_t max_pos = _cluster[cluster_index(pos)]->max_pos();
				//	if (cluster_offset(pos) < max_pos)
				//	{
				//		//��Ӧ���ڴ��ں��
				//		size_t offset = _cluster[cluster_index(pos)]->successor_pos(cluster_offset(pos));
				//		return current_offset(cluster_index(pos), offset);
				//	}
				//	else {
				//		//��̴صļ�Сֵ��Ϊ���
				//		try {
				//			size_t suc_cluster = _summary->successor_pos(cluster_index(pos));
				//			size_t offset = _cluster[suc_cluster]->min_pos();
				//			return current_offset(suc_cluster, offset);
				//		}
				//		catch (std::logic_error& e)
				//		{
				//			throw e;	//û�к��
				//		}
				//	}
				//}
				//catch (std::logic_error& e)
				//{
				//	//���ڴ�Ϊ��
				//	//��̴صļ�Сֵ��Ϊ���
				//	try {
				//		size_t suc_cluster = _summary->successor_pos(cluster_index(pos));
				//		size_t offset = _cluster[suc_cluster]->min_pos();
				//		return current_offset(suc_cluster, offset);
				//	}
				//	catch (std::logic_error& e)
				//	{
				//		throw e;	//û�к��
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
