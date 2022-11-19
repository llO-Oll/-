#include<iostream>
#include<vector>
#include<list>
#include<string>
#include<algorithm>
class Person{
public:
    Person(std::string name,int age,int height){
        this->mName = name;
        this->mAge = age;
        this->mHeight = height;
    }
    std::string mName;
    int mAge;
    int mHeight;
};

// 指定排序规则
bool comparePerson(Person &p1, Person &p2){
    // 按照年龄 升序
    if(p1.mAge == p2.mAge){
        //年龄相同 按照身高降序
        return p1.mHeight>p2.mHeight;
    }
    return p1.mAge < p2.mAge;
}

void printList( const std::list<int>&L){
    for(std::list<int>::const_iterator it = L.begin(); it !=L.end();it++){
        std::cout<<*it<<std::endl;
    }
}


void test01(){



/*

    std::list<int> L1;
    L1.push_back(10);
    L1.push_back(20);
    L1.push_back(30);

    // 遍历容器
    printList(L1);

    // 区间构造
    std::list<int>L2(L1.begin(),L1.end());
    printList(L2);

    // 拷贝构造
    std::list<int>L3(L2);
*/
}