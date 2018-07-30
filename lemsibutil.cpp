#include "lemsibutil.h"

DWORD countArrayElements(DWORD sourceArray[])
{
	DWORD number_of_elements = 0;

	for (; sourceArray[number_of_elements] != END_OF_ARRAY; number_of_elements++);

	return number_of_elements;
}