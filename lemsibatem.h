#include "BMDSwitcherAPI_h.h"

LEMSIBATEM bool initSwitcher();
LEMSIBATEM void exitSwitcher();

LEMSIBATEM DWORD connectToSwitcher(DWORD switcherNumber, const char* ipAddress);
LEMSIBATEM DWORD* getSwitcherConnectToFailureCodesArray(DWORD* out_array_size);
LEMSIBATEM const char* getSwitcherName(DWORD switch_number);

LEMSIBATEM void enumerateInputArrayGetSize(DWORD* out_array_size);
LEMSIBATEM void enumerateInputArray(const char* out_input_name_array[], __int64 out_input_id_array[], const char* out_input_port_type_array[]);

LEMSIBATEM void performCut(__int64 source_input_id, __int64 destination_output_id);
LEMSIBATEM void performCutByIndex(DWORD src_index, DWORD dest_index);
LEMSIBATEM void performAuxOutputSourceChange(__int64 source_input_id, __int64 aux_output_id);
LEMSIBATEM void performAuxOutputSourceChangeByIndex(DWORD src_index, DWORD dest_index);

const char* getInputPortTypeName(BMDSwitcherPortType port_type);
