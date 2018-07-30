#include <iostream>
#include "BMDSwitcherAPI_h.h"
#include "lemsibutil.h"
#include <comutil.h>

typedef void(*javaCallbackFunction)(void);


LEMSIBATEM DWORD testSampleErrorCode();
LEMSIBATEM void testPointerParameter(DWORD* value);
LEMSIBATEM bool testSystem();
LEMSIBATEM DWORD testParameterString(const char* any_string);
LEMSIBATEM const char* testParameterReturnString(const char* any_string);
LEMSIBATEM const char* testReturnCStringFromCFString();

LEMSIBATEM void test_enumerateInputArrayGetSize(DWORD* out_array_size);
LEMSIBATEM void test_enumerateInputArray(const char* out_input_name_array[], long long out_input_id_array[], const char* out_input_port_type_array[]);


LEMSIBATEM void addJavaCallback(DWORD watch_for_event, javaCallbackFunction sigfunc);
LEMSIBATEM void raiseEvent(DWORD raise_event);

class SwitcherCallback {

public:
	void switchCall();
};