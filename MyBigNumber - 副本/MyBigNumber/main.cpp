#include <iostream>
#include "Number.h"
#include "BigInt.h"

int main()
{
	BigInt big("1", SIZE_128BIT);
	BigInt big2("-99999999999999999999999999999", SIZE_128BIT);

	big2 += 1;

	bool a = big2 >= big;

	return 0;
}