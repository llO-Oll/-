//遍历 vector 容器。
#include <iostream>
//需要引入 vector 头文件
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <utility>

using namespace std;
template<class T>
class MyArray{
public:
    // 有参构造 参数 容量
    MyArray(int capacity){
        this -> mCapacity = capacity;
        this -> mSize = 0;
        this -> pAddress = new T[capacity];
    }

    // 拷贝构造
    MyArray(const MyArray & arr){
        this->mCapacity = arr.mCapacity;
        this->mSize = arr.mSize;

        // 深拷贝
        this->pAddress = new T[arr.mCapacity];

        // 将arr中的数据都拷贝过来
        for(int i = 0; i< this->mSize;i++){
            this->pAddress[i] = arr.pAddress[i];
        }
    }

    // operator=
    MyArray& operator=(const MyArray& arr){
        //先判断原来堆区是否有数据，如果有先释放
        if(this->pAddress !=NULL){
            delete[] this->pAddress;
            this->pAddress = NULL;
            this->mCapacity = 0;
            this->mSize = 0;
        }

        // 深拷贝
        this->mCapacity = arr.mCapacity;
        this->mSize = arr.mSize;
        this->pAddress = new T[arr.mCapacity];
        for(int i = 0;i<this->mSize;i++){
            this->pAddress[i] = arr.pAddress[i];
        }
        return *this;
    }
    // 尾插法
    void Push_Back(const T & val){
        //判断容量是否等于大小
        if(this -> mCapacity == this->mSize){
            return;
        }
        this->pAddress[this->mSize] = val;
        this->mSize++;//更新数组大小
    }

    //尾删法
    void Pop_Back(){
        //访问不到最后的元素，逻辑删除
        if(this->mSize == 0){
            return;
        }
        this->mSize--;
    }

    //通过下标访问数组元素
    T& operator[](int index){
        return this->pAddress[index];
    }

    //返回数组容量
    int getCapacity(){
        return this->mCapacity;
    }

    //返回数组大小
    int getMsize(){
        return this->mSize;
    }

    // 析构函数
    ~MyArray(){
        if(this->pAddress!=NULL){
            delete this->pAddress;

        }
    }

private:
    int mCapacity;
    int mSize;
    T *pAddress;    //指针指向堆区开辟的真实数组
};

void test01(){
    MyArray<int>arr1(5);
    MyArray<int>arr2(arr1);
    MyArray<int> arr3(100);
    arr3 = arr1;
}

void test02(){
    MyArray<int> arr1(5);
    for(int i = 0;i<5;i++){
        arr1.Push_Back(i);
    }
}

int main(){
    test02();
    return 0;
}