#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main(){

    string s = "hello world!";
    istringstream iss(s);
    string temp;

    while (getline(iss, temp,' ')) {
        cout<<temp<<endl;
    }
}