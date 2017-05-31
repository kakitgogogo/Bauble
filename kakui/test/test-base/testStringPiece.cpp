#include "kakui/base/Condition.h"
#include "kakui/base/CountDownLatch.h"
#include "kakui/base/Types.h"
#include "kakui/base/StringPiece.h"
#include "kakui/base/Atomic.h"
#include "kakui/base/BlockingQueue.h"
#include <iostream>
using namespace std;
using namespace kakui;

int main()
{
	StringPiece piece("kakui kakit!");
	cout << piece.startsWith("kakui") << endl;
	cout << piece.endsWith("kakit!") << endl;
}
