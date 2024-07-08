## van emde boas

以下简称veb节点。对于普通的树形结构来说，最常见的是二叉树,它的许多操作都和树的高度息息相关，若一个树是接近平衡的，那么树的高度是合理的，平衡的二叉树高度是lgN的级别，N是子节点的数量。平衡的其他多叉树，其区别也就是底数的不同，所以，平衡树的高度一般是log级别的。但veb节点不同，严格的proto_veb中，若上一层的节点是veb(N)，可以容纳N个节点，那么下一层的节点就是veb(lgN);,直到叶子节点所在的层数veb(2)，那么它的高度是lglgN.更好的运行条件，意味着对于存储结构的更多限制。限制就是其关键值必须是[0-k]的整数。类似于计数排序，它抛弃了比较操作，赢得了更快的运行时间，也意味着对于数据的更多限制。

### proto_veb

原型proto_veb中，节点的数量是做严格限制的：顶级节点若是N，那么向下的每一层节点必须也是整数，那么$N=2^{2^k}$,N的实际可选项有限，大约{2,4,16,256,65536...}；每个数字都取前面的平方。

### normal_veb
一般的veb节点抛弃了对于N的严格限制，对于N，它可以是2的任意幂次，即$N=2^k$，在递归的过程中，上一层的节点若是N，那么下一层的簇的数量是$U=2^{\lceil(lgN/2)\rceil}$,下一层每簇中的数量是$DU=2^{\lfloor(lgN/2)\rfloor}$,那么$N=U*DU$.举例来说若
$$
N=32;\\
lg32=5;\\
\lceil(5/2)\rceil=3\\
U=2^3=8;\\
\lfloor(5/2)\rfloor=2\\
DU=2^2=4;\\
N=U*DU\\
$$

若继续放宽限制，令$N=Z+$,那么可以令N=（向上取整为最接近的2的幂次）;
此外，normal_veb中多了两个成员min和max，分别指代当前簇中的最小值和最大值

### min() and max()

因为当前节点直接存储min和max，直接返回即可

### high() and low() and index()

```c++

    //根据pos计算出pos对应簇的索引
    size_t high(size_t pos) const
    {
        return floor(pos/U);
    }

    //根据pos计算出pos在对应簇内的偏移
    size_t low(size_t pos) const
    {
        return pos%U;
    }

    //根据簇索引和簇内偏移计算在当前节点中的偏移
    size_t index(size_t x,size_t y)
    {
        return x*U+y;
    }
```

### precursor() and successor()

寻找前驱和寻找后继是类似的

    1.首先检查对应位置是否有效
    2.尝试查询pos所在簇内是否有前驱
    3.如果有就在簇内寻找前驱，否则尝试寻找前驱簇
    4.如果存在前驱簇，那么前驱就是前驱簇内的极大值
    5.比较min<pos?，如果true，那么min就是前驱(前驱特有检查)
    6.没有前驱，抛出异常

前驱和后继的查找逻辑上是镜像的，除了第5条，因为min是仅存的，max是有备份的。仅存的意思是指，它不会出现在当前节点的任意簇中，仅在min中存储，而max则在当前节点的某个簇中有备份，就是max簇的极大值

```c++

precursor(size_t pos)
{
    if(exist(pos))  //位置有效
    {
        size_t min_low=...//对应簇内的最小偏移
        if(low(pos)>min_low)    //说明簇内有Pos的前驱
        {
            return _cluster[high(pos)]->precursor(low(pos));    //簇内寻找索引
        }else{
            size_t pre_cluster=...//查找前驱簇
            if(pre_cluster!=nil)
                return _cluster[pre_cluster]->max();    //前驱簇的极大值就是前驱
        }
    }
    throw e;    //没有前驱，抛出异常
}

```


### insert() and erase()

在我有限的认知中，一个数据结构若需要维持某种特性，往往就得在insert()和erase()中严格维护这样的特性。veb的特性也是在insert和erase中维持的

#### insert()

在某个位置插入一个数据，基本逻辑大约就是

    1.如果当前簇为空，那么进行empty_insert()
    2.如果对应簇为空,那么进行empty_insert()之后，还需要修改summary信息
    3.如果所在簇非空，那么就递归调用自身

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
                swap(pos,min) ; //更新极小值
            if(u>2)
            {
                if(cluster[high(pos)]->empty())
                {
                    //2.如果对应簇为空
                    cluster[high(pos)]->empty_insert(low(pos)); //
                    summary->insert(high(pos));     //修改summary信息

                }else{
                    cluster[high(pos)]->insert(low(pos));   //3.递归调用插入
                }
            }
        }
    }
```

删除一个数据，基本逻辑

    1.若节点内仅有一个元素，删除即可
    2.若是基础节点，那么就需要修改min和max
    3.如果删除的是极小值，那么在当前节点内找到次小值代替它，并删除最小值
    4.接着递归删除
    5.若删除后的对应簇为空，那么就更新summary
    6.若删除的是极大值，那么就需要在当前节点内找到次大值替代。
    
```c++
    void erase(pos)
    {
        if(min==max)
        {
            min=max=nil;    //1.仅有一个元素
            return;
        }
        if(size==2)
        {                   //2.基础结构
            if(pos==0)
                min=1;
            else
                min=0;
            max=min;
            return;
        }
        if(pos==min)
        {               //3.次小值取代极小值
            size_t first_cluster=summary->min();    //当前节点的首簇
            size_t offset_1=cluster[first_cluster]->min() //首簇内极小值的偏移
            size_t offset_2=index(first_cluster,offset_1);  //次小值在当前节点的偏移
            min=offset_2;
            pos=offset_2;
        }
        cluster[high(pos)]->erase(low(pos));    //4递归删除

        if(cluster[high(pos)]->empty())
            summary->erase(high(pos));  //5.更新summary;

        if(pos==max)
        {
            //6.
            size_t max_cluster=summary->max();  //最大簇
            if(max_cluster!=nil)
            {
                size_t offset_1=cluster[max_cluster]->max(); //簇内最大偏移
                size_t offset_2=index(max_cluster,offset_1);    //当前节点内的偏移
                _max=offset_2; 
            }else{
                //当前簇内仅有最小值
                max=min;
        
            }
           
        }


    }
```

### summary
总的来说，递归结构本身对我来说有些难以想象(递归结构很难在脑海中去模拟),虽然本身的效率是好的，但是多出了许多的限制和边界条件，边界条件无疑是编程的复杂度的来源，带来了更多的检测和更多的DEBUG，所以调试起来真的蛮头痛的。这里也仅仅是提供了一个大纲，更多的细节需要自己去填充，譬如说

    1.min()和max()按理来说应该返回存储的元素，他类型应该是T，而我多次使用size_t接受最小值在簇内的偏移，这显然需要拆分
    2.当N>2的时候，min和max需要在结构中保存对应的偏移量，但是在veb<2>结构中，则不需要，它的偏移量是隐藏的，因此，veb<N>和veb<2>的min和max需要自己组织，保持一致
    3.就这样吧，脑壳有点痛
