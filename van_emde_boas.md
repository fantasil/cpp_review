## van emde boas

���¼��veb�ڵ㡣������ͨ�����νṹ��˵��������Ƕ�����,�����������������ĸ߶�ϢϢ��أ���һ�����ǽӽ�ƽ��ģ���ô���ĸ߶��Ǻ���ģ�ƽ��Ķ������߶���lgN�ļ���N���ӽڵ��������ƽ��������������������Ҳ���ǵ����Ĳ�ͬ�����ԣ�ƽ�����ĸ߶�һ����log����ġ���veb�ڵ㲻ͬ���ϸ��proto_veb�У�����һ��Ľڵ���veb(N)����������N���ڵ㣬��ô��һ��Ľڵ����veb(lgN);,ֱ��Ҷ�ӽڵ����ڵĲ���veb(2)����ô���ĸ߶���lglgN.���õ�������������ζ�Ŷ��ڴ洢�ṹ�ĸ������ơ����ƾ�����ؼ�ֵ������[0-k]�������������ڼ��������������˱Ƚϲ�����Ӯ���˸��������ʱ�䣬Ҳ��ζ�Ŷ������ݵĸ������ơ�

### proto_veb

ԭ��proto_veb�У��ڵ�����������ϸ����Ƶģ������ڵ�����N����ô���µ�ÿһ��ڵ����Ҳ����������ô$N=2^{2^k}$,N��ʵ�ʿ�ѡ�����ޣ���Լ{2,4,16,256,65536...}��ÿ�����ֶ�ȡǰ���ƽ����

### normal_veb
һ���veb�ڵ������˶���N���ϸ����ƣ�����N����������2�������ݴΣ���$N=2^k$���ڵݹ�Ĺ����У���һ��Ľڵ�����N����ô��һ��Ĵص�������$U=2^{\lceil(lgN/2)\rceil}$,��һ��ÿ���е�������$DU=2^{\lfloor(lgN/2)\rfloor}$,��ô$N=U*DU$.������˵��
$$
N=32;\\
lg32=5;\\
\lceil(5/2)\rceil=3\\
U=2^3=8;\\
\lfloor(5/2)\rfloor=2\\
DU=2^2=4;\\
N=U*DU\\
$$

�������ſ����ƣ���$N=Z+$,��ô������N=������ȡ��Ϊ��ӽ���2���ݴΣ�;
���⣬normal_veb�ж���������Աmin��max���ֱ�ָ����ǰ���е���Сֵ�����ֵ

### min() and max()

��Ϊ��ǰ�ڵ�ֱ�Ӵ洢min��max��ֱ�ӷ��ؼ���

### high() and low() and index()

```c++

    //����pos�����pos��Ӧ�ص�����
    size_t high(size_t pos) const
    {
        return floor(pos/U);
    }

    //����pos�����pos�ڶ�Ӧ���ڵ�ƫ��
    size_t low(size_t pos) const
    {
        return pos%U;
    }

    //���ݴ������ʹ���ƫ�Ƽ����ڵ�ǰ�ڵ��е�ƫ��
    size_t index(size_t x,size_t y)
    {
        return x*U+y;
    }
```

### precursor() and successor()

Ѱ��ǰ����Ѱ�Һ�������Ƶ�

    1.���ȼ���Ӧλ���Ƿ���Ч
    2.���Բ�ѯpos���ڴ����Ƿ���ǰ��
    3.����о��ڴ���Ѱ��ǰ����������Ѱ��ǰ����
    4.�������ǰ���أ���ôǰ������ǰ�����ڵļ���ֵ
    5.�Ƚ�min<pos?�����true����ômin����ǰ��(ǰ�����м��)
    6.û��ǰ�����׳��쳣

ǰ���ͺ�̵Ĳ����߼����Ǿ���ģ����˵�5������Ϊmin�ǽ���ģ�max���б��ݵġ��������˼��ָ������������ڵ�ǰ�ڵ��������У�����min�д洢����max���ڵ�ǰ�ڵ��ĳ�������б��ݣ�����max�صļ���ֵ

```c++

precursor(size_t pos)
{
    if(exist(pos))  //λ����Ч
    {
        size_t min_low=...//��Ӧ���ڵ���Сƫ��
        if(low(pos)>min_low)    //˵��������Pos��ǰ��
        {
            return _cluster[high(pos)]->precursor(low(pos));    //����Ѱ������
        }else{
            size_t pre_cluster=...//����ǰ����
            if(pre_cluster!=nil)
                return _cluster[pre_cluster]->max();    //ǰ���صļ���ֵ����ǰ��
        }
    }
    throw e;    //û��ǰ�����׳��쳣
}

```


### insert() and erase()

�������޵���֪�У�һ�����ݽṹ����Ҫά��ĳ�����ԣ������͵���insert()��erase()���ϸ�ά�����������ԡ�veb������Ҳ����insert��erase��ά�ֵ�

#### insert()

��ĳ��λ�ò���һ�����ݣ������߼���Լ����

    1.�����ǰ��Ϊ�գ���ô����empty_insert()
    2.�����Ӧ��Ϊ��,��ô����empty_insert()֮�󣬻���Ҫ�޸�summary��Ϣ
    3.������ڴطǿգ���ô�͵ݹ��������

```c++
    void empty_insert(size_t pos){
        _min=_max=pos;
    }

    void insert(size_t pos)
    {
        if(empty())
            empty_insert();     //1.
        else 
        {
            if(pos<min)
                swap(pos,min) ; //���¼�Сֵ
            if(u>2)
            {
                if(cluster[high(pos)]->empty())
                {
                    //2.�����Ӧ��Ϊ��
                    cluster[high(pos)]->empty_insert(low(pos)); //
                    summary->insert(high(pos));     //�޸�summary��Ϣ

                }else{
                    cluster[high(pos)]->insert(low(pos));   //3.�ݹ���ò���
                }
            }
        }
    }
```

ɾ��һ�����ݣ������߼�

    1.���ڵ��ڽ���һ��Ԫ�أ�ɾ������
    2.���ǻ����ڵ㣬��ô����Ҫ�޸�min��max
    3.���ɾ�����Ǽ�Сֵ����ô�ڵ�ǰ�ڵ����ҵ���Сֵ����������ɾ����Сֵ
    4.���ŵݹ�ɾ��
    5.��ɾ����Ķ�Ӧ��Ϊ�գ���ô�͸���summary
    6.��ɾ�����Ǽ���ֵ����ô����Ҫ�ڵ�ǰ�ڵ����ҵ��δ�ֵ�����
    
```c++
    void erase(pos)
    {
        if(min==max)
        {
            min=max=nil;    //1.����һ��Ԫ��
            return;
        }
        if(size==2)
        {                   //2.�����ṹ
            if(pos==0)
                min=1;
            else
                min=0;
            max=min;
            return;
        }
        if(pos==min)
        {               //3.��Сֵȡ����Сֵ
            size_t first_cluster=summary->min();    //��ǰ�ڵ���״�
            size_t offset_1=cluster[first_cluster]->min() //�״��ڼ�Сֵ��ƫ��
            size_t offset_2=index(first_cluster,offset_1);  //��Сֵ�ڵ�ǰ�ڵ��ƫ��
            min=offset_2;
            pos=offset_2;
        }
        cluster[high(pos)]->erase(low(pos));    //4�ݹ�ɾ��

        if(cluster[high(pos)]->empty())
            summary->erase(high(pos));  //5.����summary;

        if(pos==max)
        {
            //6.
            size_t max_cluster=summary->max();  //����
            if(max_cluster!=nil)
            {
                size_t offset_1=cluster[max_cluster]->max(); //�������ƫ��
                size_t offset_2=index(max_cluster,offset_1);    //��ǰ�ڵ��ڵ�ƫ��
                _max=offset_2; 
            }else{
                //��ǰ���ڽ�����Сֵ
                max=min;
        
            }
           
        }


    }
```

### summary
�ܵ���˵���ݹ�ṹ���������˵��Щ��������(�ݹ�ṹ�������Ժ���ȥģ��),��Ȼ�����Ч���Ǻõģ����Ƕ�����������ƺͱ߽��������߽����������Ǳ�̵ĸ��Ӷȵ���Դ�������˸���ļ��͸����DEBUG�����Ե������������ͷʹ�ġ�����Ҳ�������ṩ��һ����٣������ϸ����Ҫ�Լ�ȥ��䣬Ʃ��˵

    1.min()��max()������˵Ӧ�÷��ش洢��Ԫ�أ�������Ӧ����T�����Ҷ��ʹ��size_t������Сֵ�ڴ��ڵ�ƫ�ƣ�����Ȼ��Ҫ���
    2.��N>2��ʱ��min��max��Ҫ�ڽṹ�б����Ӧ��ƫ������������veb<2>�ṹ�У�����Ҫ������ƫ���������صģ���ˣ�veb<N>��veb<2>��min��max��Ҫ�Լ���֯������һ��
    3.�������ɣ��Կ��е�ʹ
