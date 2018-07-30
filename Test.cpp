#define LEMSIBATEM extern "C" __declspec(dllexport)
#include "Test.h"
#include <atlstr.h>

javaCallbackFunction javaCallback = nullptr;
int event = 0;

void SwitcherCallback::switchCall()
{
	javaCallback();
}


void addJavaCallback(DWORD watch_for_event, javaCallbackFunction sigfunc)
{
	event = watch_for_event;
	javaCallback = sigfunc;
}



void raiseEvent(DWORD raise_event)
{
	SwitcherCallback *switchCall = new SwitcherCallback();
	if (event == raise_event)
		switchCall->switchCall();

	delete switchCall;
}


void testPointerParameter(DWORD* value)
{
	DWORD testArray[] = { '11', '22', '33', '44', '55', 'aa', 'bb', 'cc', 'dd', 'ee', END_OF_ARRAY };
	DWORD x = countArrayElements(testArray);

	DWORD give_value = x;

	*value = give_value;
}


DWORD testSampleErrorCode()
{
	return 'AAA';
}


bool testSystem()
{
	return true;
}


DWORD testParameterString(const char* any_string)
{
	const char* my_string = any_string;

	return 0;
}


const char* testParameterReturnString(const char* any_string)
{
	const char* my_string = any_string;

	return my_string;
}


char test_switchOneName[256];
const char* test_switchOneName2 = nullptr;
BSTR test_switchOneName3;

void test_switchOneNameFunction(char* pointer_to_name)
{
	int size = sizeof("This is my String ");
	const char* name = (const char*)malloc(size);
	memset((void*)name, NULL, size);
	memcpy((void*)name, "This is my String ", size);
	for (int x = 0; x < size;x++)
		pointer_to_name[x] = name[x];
}


const char* testReturnCStringFromCFString()
{
	test_switchOneNameFunction(test_switchOneName);
	int size = sizeof("This is my String ");
	test_switchOneName2 = (const char*)malloc(size);
	memset((void*)test_switchOneName2, NULL, size);
	memcpy((void*)test_switchOneName2, "This is my String ", size);

	BSTR bst = _com_util::ConvertStringToBSTR(test_switchOneName);
	
	
	//SysFreeString(bst);

	const char* name = _com_util::ConvertBSTRToString(bst);
	return  name;

}


#define NUMBER_OF_INPUTS 10
__int64* test_masterInputIdArray;


void test_enumerateInputArrayGetSize(DWORD* out_array_size)
{
	*out_array_size = NUMBER_OF_INPUTS;
}


void test_enumerateInputArray(const char* out_input_name_array[], long long out_input_id_array[], const char* out_input_port_type_array[])
{
	const char* inputName = "Hello";
	const char* portName = "(port)";
	BSTR inputNameString[NUMBER_OF_INPUTS];
	long long inputID[NUMBER_OF_INPUTS];

	for (int x = 0; x<NUMBER_OF_INPUTS; x++)
	{
		inputNameString[x] = nullptr;
		inputID[x] = NULL;
	}

	for (int x = 0; x<NUMBER_OF_INPUTS; x++)
	{
		inputNameString[x] = SysAllocString(L"Test Input");
		if (inputNameString[x] == NULL)
			inputName = "It is NULL!";
		else
			inputName = _com_util::ConvertBSTRToString(inputNameString[x]);

		out_input_port_type_array[x] = portName;
		out_input_id_array[x] = inputID[x] = x;
		out_input_name_array[x] = inputName;

	}
	test_masterInputIdArray = out_input_id_array;

	for (int y = 0; y<NUMBER_OF_INPUTS; y++)
		test_masterInputIdArray[y] += out_input_id_array[y];

	out_input_id_array[0] = test_masterInputIdArray[NUMBER_OF_INPUTS - 1];

}
