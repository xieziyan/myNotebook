# LeetCode 笔记

### 703.数据流中的第K大元素
最大堆，和最小堆的学习。原本初始化先排序，插入，排序的方式插入。但是在第八个测试案例会超时。于是改为对初始元素进行排序，然后add使用折半查找插入，然后选取第k个元素。后来发现可以使用堆来做，初始化一个最小堆，当第i个元素进堆后如果堆的大小超过K，出堆一个元素。这样子，堆顶的就是第K大元素。C++ STL 中

    priority_queue<int, vector<int>, greater<int> > p;
表示小根堆
    
    priority_queue<int, vector<int>, less<int> > p;

表示大根堆。
