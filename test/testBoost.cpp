#include <iostream>
#include <boost/bind.hpp>
#include <string>
using namespace std;

class Hello
{
public:
	void say(string name) 
	{ cout << name << " say: hello world!" << endl; }
};

int main()
{
	Hello h;
	auto func = boost::bind(&Hello::say, &h, "张三");
	func();
	return 0;
}