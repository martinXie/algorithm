#include<iostream>
#include<string>
#include<deque>
using namespace std;
#define MAX_SYMBOL_NUM 3
string input="10011";
void print(deque<int>& data)
{
	char* p = const_cast<char*>(input.c_str());
	int i = 0;
	for (auto it = data.begin(); it != data.end(); it++)
	{
		while (i != *it)
		{
			cout << *(p + i);
			i++;
		}
		cout << endl;
	}
}
//[begin, begin+len)
void divide(int begin, int len, deque<int>& data)
{
	if (data.size() == MAX_SYMBOL_NUM -1)
	{
		data.push_back(begin + len);
		cout << "************" << endl;
		print(data);
		data.pop_back();
		return;
	}

	for (int i = begin+1; i < begin+len; i++)
	{
		if (data.size() < MAX_SYMBOL_NUM - 1)
		{
			data.push_back(i);
			divide(i, begin + len - i, data);
			data.pop_back();
		}
	}
}

int main()
{
	deque<int> data;
	divide(0, input.size(), data);
	return 1;
}

