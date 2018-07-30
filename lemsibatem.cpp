#define LEMSIBATEM extern "C" __declspec(dllexport)
#include "lemsibatem.h"
#include "lemsibutil.h"
#include <atlstr.h>
#include <comutil.h>

#define NUMBER_OF_INPUTS 30

static IBMDSwitcherDiscovery* switcherDiscovery = NULL;

static IBMDSwitcher* switchOne = NULL;
BSTR switchOneName = NULL;

IBMDSwitcher* switchTwo = NULL;
BSTR switchTwoName = NULL;

IBMDSwitcher* switchThree = NULL;
BSTR switchThreeName = NULL;

IBMDSwitcherMixEffectBlock *mixEffectBlock = NULL;

long long masterInputIdArray[NUMBER_OF_INPUTS];
long long* masterInputPortTypesArray = NULL;

DWORD* getSwitcherInputPropertyIdArray(DWORD* out_array_size)
{
	DWORD static inputPropertyId[] = {
		bmdSwitcherInputPropertyIdPortType,
		bmdSwitcherInputPropertyIdInputAvailability,
		bmdSwitcherInputPropertyIdShortName,
		bmdSwitcherInputPropertyIdLongName,
		bmdSwitcherInputPropertyIdIsProgramTallied,
		bmdSwitcherInputPropertyIdIsPreviewTallied,
		bmdSwitcherInputPropertyIdAvailableExternalPortTypes,
		bmdSwitcherInputPropertyIdCurrentExternalPortType,
		END_OF_ARRAY
	};

	*out_array_size = countArrayElements(inputPropertyId);

	return inputPropertyId;
}


DWORD* getSwitcherConnectToFailureCodesArray(DWORD* out_array_size)
{
	DWORD static connectToFailureCodes[] = {
		bmdSwitcherConnectToFailureCorruptData,
		bmdSwitcherConnectToFailureIncompatibleFirmware,
		bmdSwitcherConnectToFailureNoResponse,
		bmdSwitcherConnectToFailureStateSync,
		bmdSwitcherConnectToFailureStateSyncTimedOut,
		END_OF_ARRAY
	};

	*out_array_size = countArrayElements(connectToFailureCodes);

	return connectToFailureCodes;
}


bool initSwitcher()
{
	HRESULT result;
	
	CoInitialize(NULL);

	if (switcherDiscovery == NULL)
		result = CoCreateInstance(CLSID_CBMDSwitcherDiscovery, NULL, CLSCTX_ALL, IID_IBMDSwitcherDiscovery, (void**)&switcherDiscovery);


	if (result == S_OK)
		return true;
	else
		return false;
}


void exitSwitcher()
{
	if (switchOne)
		switchOne->Release();
	if (switchTwo)
		switchTwo->Release();
	if (switchThree)
		switchThree->Release();

	if (mixEffectBlock)
		mixEffectBlock->Release();
}


DWORD connectToSwitcher(DWORD switcherNumber, const char* ipAddress)
{
	if (switcherNumber == 0)
		return 0;

	DWORD switchCaseNumber = switcherNumber;

	BSTR ip = _com_util::ConvertStringToBSTR(ipAddress);
	
	DWORD failure = 0;
	
	switch (switchCaseNumber)
	{
	case 1:
	{
		if (switchOne == NULL){
			if (switcherDiscovery != NULL)
			{
				HRESULT result;
				BMDSwitcherConnectToFailure reasonForFailure;
				result = switcherDiscovery->ConnectTo(ip, &switchOne, &reasonForFailure);
				failure = reasonForFailure;
				if (result == S_OK)
				{
					switchOne->GetProductName(&switchOneName);
					IBMDSwitcherMixEffectBlockIterator *mixEffectBlockIterator = NULL;

					switchOne->CreateIterator(IID_IBMDSwitcherMixEffectBlockIterator, (void**)&mixEffectBlockIterator);

					if (mixEffectBlockIterator)
						if (mixEffectBlockIterator->Next(&mixEffectBlock) != S_OK)
							return -2;

					return -1;
				}
			}

		}

	}break;

	case 2:break;
	case 3:break;

	}
	
	return failure;
}


const char* getSwitcherName(DWORD switch_number)
{
	switch (switch_number)
	{
	case 1:
	{
		const char* name = _com_util::ConvertBSTRToString(switchOneName);;
		return name;
	}
	case 2:break;
	case 3:break;
	default:break;
	}

	return NULL;
}

void enumerateInputArrayGetSize(DWORD* out_array_size)
{
	*out_array_size = NUMBER_OF_INPUTS;
}

BSTR inputNameString[NUMBER_OF_INPUTS];
BMDSwitcherInputId inputID[NUMBER_OF_INPUTS];
__int64 inputPortType[NUMBER_OF_INPUTS];


void enumerateInputArray(const char* out_input_name_array[], long long out_input_id_array[], const char* out_input_port_type_array[])
{

	for (DWORD x = 0; x<NUMBER_OF_INPUTS; x++)
	{
		inputNameString[x] = nullptr;
		inputID[x] = 0;
		inputPortType[x] = 0;

		masterInputIdArray[x] = 0;
	}

	if (switchOne)
	{
		IBMDSwitcherInput* input = NULL;
		IBMDSwitcherInputIterator* inputIterator = NULL;

		HRESULT result;

		result = switchOne->CreateIterator(IID_IBMDSwitcherInputIterator, (void**)&inputIterator);
		if (result != S_OK)
			throw (1);

		DWORD index = 0;
		while (S_OK == inputIterator->Next(&input))
		{
			BMDSwitcherInputId id;
			BSTR name;
			const char* inputName = NULL;


			result = input->GetInputId(&id);
			if (result != S_OK)
				throw (2);

			inputID[index] = id;
			out_input_id_array[index] = id;
			masterInputIdArray[index] = id;

			result = input->GetInt(bmdSwitcherInputPropertyIdPortType, &inputPortType[index]);
			if (result != S_OK)
				throw(3);

			BMDSwitcherPortType port_type = (BMDSwitcherPortType)inputPortType[index];
			out_input_port_type_array[index] = getInputPortTypeName(port_type);

			result = input->GetString(bmdSwitcherInputPropertyIdLongName, &name);
			if (result != S_OK)
				throw(4);

			inputNameString[index] = name;
			inputName = _com_util::ConvertBSTRToString(name);
			if (inputName == NULL)
				throw(5);

			out_input_name_array[index] = inputName;

			input->Release();
			index++;
		}

		//masterInputIdArray = out_input_id_array;
		masterInputPortTypesArray = inputPortType;

		inputIterator->Release();

	}

}

const char* getInputPortTypeName(BMDSwitcherPortType port_type)
{
	switch (port_type)
	{
	case bmdSwitcherPortTypeExternal: return "External";
	case bmdSwitcherPortTypeBlack: return "Black";
	case bmdSwitcherPortTypeColorBars: return "Color Bars";
	case bmdSwitcherPortTypeColorGenerator: return "Color Generator";
	case bmdSwitcherPortTypeMediaPlayerFill: return "Media Player Fill";
	case bmdSwitcherPortTypeMediaPlayerCut: return "Media Player Cut";
	case bmdSwitcherPortTypeSuperSource: return "Type Super Source";
	case bmdSwitcherPortTypeMixEffectBlockOutput: return "Mix Effect Block Output";
	case bmdSwitcherPortTypeAuxOutput: return "Aux Output";
	case bmdSwitcherPortTypeKeyCutOutput: return "Key Cut Output";
	default: break;
	}

	return "unknown";
}


void performCut(long long source_input_id, long long destination_output_id)
{
	//mixEffectBlock->SetInt(bmdSwitcherMixEffectBlockPropertyIdProgramInput, destination_output_id);
	mixEffectBlock->SetInt(bmdSwitcherMixEffectBlockPropertyIdPreviewInput, source_input_id);
	mixEffectBlock->PerformCut();
}


void performCutByIndex(DWORD src_index, DWORD dest_index)
{
	mixEffectBlock->SetInt(bmdSwitcherMixEffectBlockPropertyIdProgramInput, masterInputIdArray[src_index]);
	mixEffectBlock->SetInt(bmdSwitcherMixEffectBlockPropertyIdPreviewInput, masterInputIdArray[dest_index]);
	mixEffectBlock->PerformCut();
}


void performAuxOutputSourceChange(long long source_input_id, long long aux_output_id)
{
	IBMDSwitcherInputAux* inputAux = NULL;
	IBMDSwitcherInput* input = NULL;
	IBMDSwitcherInputIterator* iterator = NULL;
	BMDSwitcherInputId id = 0;
	BMDSwitcherInputId aux_id = aux_output_id;

	HRESULT result;

	result = switchOne->CreateIterator(IID_IBMDSwitcherInputIterator, (void**)&iterator);

	if (result != S_OK)
		throw(100);

	while (S_OK == iterator->Next(&input))
	{
		result = input->GetInputId(&id);
		if (result != S_OK)
			throw(200);

		if (id == aux_id)
		{
			result = input->QueryInterface(IID_IBMDSwitcherInputAux, (void**)&inputAux);
			if (result != S_OK)
				throw(300);

			result = inputAux->SetInputSource(source_input_id);
			if (result != S_OK)
				throw(400);

			goto done;
		}

		input->Release();
	}

done:
	{
		if (inputAux)
			inputAux->Release();
		if (input)
			input->Release();
		if (iterator)
			iterator->Release();
	}
}


void performAuxOutputSourceChangeByIndex(DWORD src_index, DWORD dest_index)
{
	IBMDSwitcherInputAux* inputAux = NULL;
	IBMDSwitcherInput* input = NULL;
	IBMDSwitcherInputIterator* iterator = NULL;
	BMDSwitcherInputId id;
	long long dest = masterInputIdArray[dest_index];
	long long src = masterInputIdArray[src_index];

	switchOne->CreateIterator(IID_IBMDSwitcherInputIterator, (void**)&iterator);

	while (iterator->Next(&input))
	{
		input->GetInputId(&id);
		if (id == dest)
		{
			input->QueryInterface(IID_IBMDSwitcherInputAux, (void**)&inputAux);
			inputAux->SetInputSource(src);
			goto done;
		}
	}

done:
	{
		inputAux->Release();
		input->Release();
		iterator->Release();
	}
}