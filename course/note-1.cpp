#include<iostream>
#include<string>

using namespace std;

int main()
{   
    int age;
    //三种方法初始化
    string name1 = "Mary";
    string name2 ("John");
    string name3 {"Jack"};
    name1 = name2;
    
    string *p = &name1;
    //p->length()等价于(*p).length()等价于name1.length()

    cin >> age >> name1; //遇到space停止读取

    string strl ("This is a Text");
    cout << strl << endl;

    getline(cin, name2);//读取一整行

    string str2;
    str2 = strl.substr(1,4);//从1开始，截取4个字符
    cout << str2 << endl;
    //insert(),append(const string& str)后面添加,replace(size_t pos, size_t len, const string& str)
    //erase(size_t pos = 0, size_t len = npos)删除

    //与其他的不同 string s 已经创建对象且已经初始化为空字符串
}