#include<iostream>
#include<vector>
#include<deque>
#include<string>
#include<algorithm>
class Person{
public:
    Person(std::string name, int score){
        this->mName = name;
        this->mScore = score;
    }
    std::string mName;
    int mScore;
};

void createPerson(std::vector<Person> &v){
    std::string nameSeed = "ABCDE";
    for(int i = 0; i < 5; i++){
        std::string name = "Ñ¡ÊÖ";
        name += nameSeed[i];

        int score = 0;

        Person p(name,score);
        v.push_back(p);
    }

}

void setScore(std::vector<Person>&v){
    for(std::vector<Person>::iterator it = v.begin(); it != v.end(); it++){

        std::deque<int> d;
        for(int i = 0; i<10; i++){
            int score = rand() % 41 +60; // 60~100
            d.push_back(score);
        }

        sort(d.begin(),d.end());

        d.pop_back();
        d.pop_front();

        int sum = 0;
        for(std::deque<int>::iterator dit = d.begin(); dit != d.end(); dit++){
            sum += *dit;
        }
        int avg = sum/d.size();

        it->mScore = avg;

    }
}

void showScore(std::vector<Person> &v){
    for(std::vector<Person>::iterator it = v.begin(); it!=v.end();it++){
        std::cout << "1: "<<(*it).mName <<" 2: "<<(*it).mScore << std::endl;
    }
}

int main(){
    std::vector<Person> v;
    createPerson(v);

    setScore(v);
    for(std::vector<Person>::iterator it = v.begin();it != v.end();it++){
        std::cout<<"1: "<<(*it).mName << " 2:"<<(*it).mScore << std::endl;
    }
    showScore(v);
    return 0;
}