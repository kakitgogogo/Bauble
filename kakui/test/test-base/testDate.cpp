#include "kakui/base/Date.h"
#include <iostream>
#include <string>
using namespace std;
using namespace kakui;

int main()
{
	Date d(1999, 9, 9);
	cout << d.toIsoString() << endl;
}
