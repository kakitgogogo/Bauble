#include "kakui/base/LogStream.h"
#include "kakui/base/StringPiece.h"
#include <iostream>
#include <string>
using namespace std;
using namespace kakui;

int main()
{
	LogStream ls;
	ls << "k: " << 123 << 123.5 << 'c';
	cout << ls.buffer().toString() << endl;
}
