#include <iostream>
#include <string>
using namespace std;

string input;
int pos = 0;
char c;

bool E();
bool A();
bool T();
bool B();
bool F();
bool D();

char next()
{
	if(pos >= input.size()) return -1;
	return input[pos++];
}

// <E> -> <T> <A>
bool E()
{
	if(T())
	{
		return A();
	}
	return false;
}

// <A> -> + <T> <A> | - <T> <A> | ^
bool A()
{
	if(c == '+' || c == '-')
	{
		c = next();
		if(T())
		{
			return A();
		}
		else
		{
			return false;
		}
	}
	return true;
}

// <T> -> <F> <B>
bool T()
{
	if(F())
	{
		return B();
	}
	else
	{
		return false;
	}
}

// <B> -> * <F> <B> | / <F> <B> | ^
bool B()
{
	if(c == '*' || c == '/')
	{
		c = next();
		if(F())
		{
			return B();
		}
		else
		{
			return false;
		}
	}
	return true;
}

// <F> -> ( <E> ) | <D>
bool F()
{
	if(c == '(')
	{
		c = next();
		if(E())
		{
			if(c == ')')
			{
				c = next();
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else if(D())
	{
		return true;
	}
	return false;
}

// <D> -> x | y | z
bool D()
{
	if(c == 'x' || c == 'y' || c == 'z')
	{
		c = next();
		return true;
	}
	return false;
}

int main(int argc, char *argv[])
{
	if(argc != 2) return 1;
	input = argv[1];
	c = next();
	if(E() && (next() == -1))
	{
		cout << "Parse complete" << endl;
	}
	else
	{
		cout << "Parse failed" << endl;
	}
}