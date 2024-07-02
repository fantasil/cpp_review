#pragma once
#include<cmath>
#include<array>
#ifndef fantasil_proto_veb_node_h
#define fantasil_proto_veb_node_h
#include<memory>


namespace fantasil {


#pragma region helper_func


	//map��һ�ű�����map[i]=map[i-1]^2;
	constexpr std::array<size_t, 5> map{ 2,4,16,256,65536 };
	
	//�����������ӱ��в�ѯU��U��N�Ŀ�ƽ��
	constexpr size_t find_U(size_t N)
	{
		if (N == 2)
			return 0;

		for (size_t i = 0; i != map.size(); ++i)
		{
			if (map[i] == N)
				return map[i - 1];
		}
		
		return 0;
	}

#pragma endregion
	
	/*---------------------------------һ���Ե�proto_vEB<T,N>----------------------------------------------*/
#pragma region proto_vEB<T,N>
	template<typename T,size_t N>
	struct proto_vEB {
		static_assert((N& (N - 1)) == 0);

		static constexpr size_t U = find_U(N);

		using key_type = size_t;
		using summary_type = proto_vEB<bool, U>;
		using summary_ptr = summary_type*;				//summayָ��ָ��cluster�Ļ��������洢����boolֵ
		using cluster_type = proto_vEB<T, U>;
		using cluster_ptr = cluster_type*;				//clusterָ��ָ���Ӵ�

	public:
		proto_vEB();

	public:

		bool exist(size_t index);					//ָ���ؼ���λ���Ƿ����
		T member(size_t index);						//��ȡ��Ա
		size_t min() const;							//��Сֵ���ڵĹؼ���
		size_t max() const;							//���ֵ���ڵĹؼ���
		size_t successor(size_t index) const;		//���λ��
		size_t precursor(size_t index) const;		//ǰ��λ��
		bool erase(size_t index);					//ɾ��ָ��λ�õ�Ԫ��
		void insert(size_t index, const T& val);	//ָ��λ�ò���Ԫ��
	public:
		inline size_t high(size_t index) const;
		inline size_t low(size_t index) const;
		inline size_t index(size_t high, size_t low) const;
	public:
		summary_ptr _summary;
		std::array<cluster_ptr, U> _cluster;
	};

	template<typename T,size_t N>
	proto_vEB<T,N>::proto_vEB()
	{
		if (N > 2)
		{
			_summary = new summary_type{};
			for (size_t i = 0; i != U; ++i)
			{
				_cluster[i] = new cluster_type{};
			}
		}
	};

	template<typename T, size_t N>
	bool proto_vEB<T, N>::exist(size_t index)
	{
		if (!_summary->exist(high(index)))
			return false;

		return _cluster[high(index)]->exist(low(index));
	}

	template<typename T, size_t N>
	inline T proto_vEB<T, N>::member(size_t index)
	{
		if(exist(index))
			return _cluster[high(index)]->member(low(index));
		else {
			throw std::logic_error{ "pos is empty" };
		}
	}


	template<typename T, size_t N>
	inline size_t proto_vEB<T, N>::min() const
	{
		size_t x = _summary->min();			//��С��
		size_t y = _cluster[x]->min();		//��С���ڵ���Сƫ��
		return index(x, y);					//�����ʵ��index
	}

	template<typename T, size_t N>
	size_t proto_vEB<T, N>::max() const
	{
		size_t x = _summary->max();			//����
		size_t y = _cluster[x]->max();		//�����ڵ����ƫ��
		return index(x, y);					//����ʵ��index
	}

	template<typename T, size_t N>
	inline size_t proto_vEB<T, N>::successor(size_t index) const
	{
		try {
			size_t y = _cluster[high(index)]->successor(low(index));	//��ǰ�����к�̵Ļ��ͷ��غ��
			return this->index(high(index), y);
		}
		catch (std::logic_error& e)
		{
			//�ô���û�к�̣�Ѱ���µĺ�̴�
			try {
				size_t x = _summary->successor(high(index));	//x�Ǻ�̴�
				size_t y = _cluster[x]->min();					//��̴��е���СԪ��λ��
				return this->index(x, y);
			}
			catch (std::logic_error& e)
			{
				//û�к�̴أ��Ǿ�û�к��
				throw e;
			}
		}

	}

	template<typename T, size_t N>
	inline size_t proto_vEB<T, N>::precursor(size_t index) const
	{
		try {
			//���������ǰ��
			size_t y = _cluster[high(index)]->precursor(low(index));
			return this->index(high(index), y);
		}
		catch (std::logic_error& e)
		{
			//����û��ǰ��
			try {
				//����ǰ�����ڵ����ֵ��ǰ���ؿ���Ϊ��
				size_t x = _summary->precursor(high(index));
				size_t y = _cluster[x]->max();
				return this->index(x, y);
			}
			catch (std::logic_error& e)
			{
				throw e;
			}
		}
	}

	template<typename T, size_t N>
	inline bool proto_vEB<T, N>::erase(size_t index)
	{
		bool flag = _cluster[high(index)]->erase(low(index));	//ɾ��Ԫ�أ��������Ƿ���Ҫ�ı�summary
		if (flag)
		{
			return _summary->erase(high(index));		//�ı�summary���������Ƿ���Ҫ�������ݸı�summary
		}
		return false;	//����Ҫ�����޸�summary
	}

	template<typename T, size_t N>
	void proto_vEB<T, N>::insert(size_t index, const T& val)
	{
		_cluster[high(index)]->insert(low(index), val);		//�ڴ��ڲ���Ԫ��
		_summary->insert(high(index), true);				//�޸�summary��Ϣ
	}




	template<typename T, size_t N>
	inline size_t proto_vEB<T, N>::high(size_t index) const
	{
		return std::ceil(index / U);
	}

	template<typename T, size_t N>
	inline size_t proto_vEB<T, N>::low(size_t index) const
	{
		return index%U;
	}

	template<typename T, size_t N>
	inline size_t proto_vEB<T, N>::index(size_t high, size_t low) const
	{
		return high * U + low;
	}

#pragma endregion

	/*------------------------------------����������proto_vEB<T,2>-----------------------------------------------*/
#pragma region proto_vEB<T,2>



	template<typename T>
	struct proto_vEB<T, 2> 
	{
		using key_type = size_t;
	public:

		proto_vEB();
	public:
		bool exist(size_t index);
		T member(size_t index);

		size_t min() const;
		size_t max() const;
		size_t successor(size_t index) const;
		size_t precursor(size_t index) const;
		bool erase(size_t pos);
		void insert(size_t pos, const T& val);
		std::array<T*, 2> _data;
	};

	template<typename T>
	proto_vEB<T, 2>::proto_vEB()
	{
		_data[0] = _data[1] = nullptr;
	}

	template<typename T>
	bool proto_vEB<T, 2>::exist(size_t index)
	{
		return _data[index] != nullptr;
	}

	template<typename T>
	T proto_vEB<T, 2>::member(size_t index)
	{
		return *_data[index];
	}

	template<typename T>
	size_t proto_vEB<T, 2>::min() const
	{
		if (_data[0])
			return 0;
		else if (_data[1])
			return 1;
		else
			throw std::logic_error{ "proto_vEB<T,2>::min():emtpy set" };
	}

	template<typename T>
	size_t proto_vEB<T, 2>::max() const
	{
		if (_data[1])
			return 1;
		else if (_data[0])
			return 0;
		else
			throw std::logic_error{ "proto_vEB<T,2>::max():empty set" };
	}

	template<typename T>
	size_t proto_vEB<T, 2>::successor(size_t index) const
	{
		if (index == 0 && _data[1])
			return 1;
		throw std::logic_error{ "proto_vEB<T,2>::successor()::no successor" };
	}

	template<typename T>
	size_t proto_vEB<T, 2>::precursor(size_t index) const
	{
		if (index == 1 && _data[0])
			return 0;
		throw std::logic_error{ "proto_vEB<T,2>::precussor():no precursor" };
	}

	template<typename T>
	bool proto_vEB<T, 2>::erase(size_t pos)
	{
		size_t y{ 0 };
		pos == 0 ? y = 1 : y = 0;
		if (_data[pos])
			delete _data[pos];
		_data[pos] = nullptr;	//ɾ��Ԫ��
		if (!_data[y])
			return true;		//��Ҫ�����޸�summary
		return false;			//����Ķ�
	}

	template<typename T>
	inline void proto_vEB<T, 2>::insert(size_t pos, const T& val)
	{
		T* tmp = new T{ val };
		_data[pos] = tmp;
	}



#pragma endregion

	/*-------------------------------------��������proto_vEB<bool,2>-------------------------------------------------*/
#pragma region proto_vEB<bool,2>


	template<>
	struct proto_vEB<bool, 2>
	{
		using key_type = size_t;
	public:
		proto_vEB()
		{
			_data[0] = _data[1] = false;
		}
	public:
		bool exist(size_t index)
		{
			return _data[index];
		}

		bool member(size_t index)
		{
			return _data[index];
		}


		size_t min() const
		{
			if (_data[0])
				return 0;
			else if (_data[1])
				return 1;
			else
				throw std::logic_error{ "proto_vEB<bool,2>::min():empty set" };
		}

		size_t max() const
		{
			if (_data[1])
				return 1;
			else if (_data[0])
				return 0;
			else
				throw std::logic_error{ "proto_vEB<bool,2>::max():empty set" };
		}

		size_t successor(size_t index) const {
			if (index == 0 && _data[1])
				return 1;
			throw std::logic_error{ "proto_vEB<bool,2>::successor():no successor" };
		}

		size_t precursor(size_t index) const
		{
			if (index == 1 && _data[0])
				return 0;
			throw std::logic_error{ "proto_vEB<bool,2>::precursor():no precursor" };
		}

		bool erase(size_t index)
		{
			size_t y{ 0 };
			index == 0 ? y = 1 : y = 0;
			_data[index] = false;
			if (!_data[y])
				return true;
			return false;
		}

		void insert(size_t index,bool val=true)
		{
			_data[index] = val;
		}

	public:

		std::array<bool, 2> _data;
	};


#pragma endregion


}

#endif
