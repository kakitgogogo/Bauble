#include "kakui/base/Timestamp.h"
#include <iostream>
using namespace std;
using namespace kakui;

int main()
{
	Timestamp t = Timestamp::now();
	cout << t.toFormattedString(true) << endl;
}
