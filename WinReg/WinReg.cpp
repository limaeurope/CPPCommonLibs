#include	"WinReg.hpp"

GS::UniString GetRegString(const GS::UniString& i_key, const GS::UniString& i_path, HKEY i_hKey/* = HKEY_CURRENT_USER*/, DWORD * const o_bufSize/* = NULL*/)
{
	char sBuffer[255];
	DWORD iBuffer = 255;

	LSTATUS _stat = RegGetValueW(i_hKey,
		i_path.ToUStr().Get(),		/*L"SOFTWARE\\GRAPHISOFT\\ARCHICAD\\Archicad 26.0.0 INT R1\\Add-On Manager",*/
		i_key.ToUStr().Get(),		/*L"Default Location",*/
		RRF_RT_REG_SZ + RRF_ZEROONFAILURE,
		NULL,
		&sBuffer,
		&iBuffer);

	wchar_t* ws = (wchar_t*)sBuffer;

	if (o_bufSize != NULL)
		*o_bufSize = iBuffer;

	return GS::UniString(ws);
}

GS::UniString GetRegStringOrDefault(const GS::UniString& i_defaultValue, const GS::UniString& i_key, const GS::UniString& i_path, HKEY i_hKey/* = HKEY_CURRENT_USER*/, DWORD * o_bufSize/* = NULL*/)
{
	GS::UniString iResult = GetRegString(i_key, i_path, i_hKey, o_bufSize);

	if (o_bufSize != NULL)
		return iResult;
	else
		return i_defaultValue;
}

GS::UniString GetRegStringOrSetDefault(const GS::UniString& i_defaultValue, const GS::UniString& i_key, const GS::UniString& i_path, HKEY i_hKey/* = HKEY_CURRENT_USER*/, DWORD* o_bufSize/* = NULL*/)
{
	GS::UniString iResult = GetRegString(i_key, i_path, i_hKey, o_bufSize);

	if (o_bufSize != NULL)
		return iResult;
	else
		SetRegString(i_defaultValue, i_key, i_path, i_hKey, o_bufSize);

		return i_defaultValue;
}

UInt32 GetRegInt(const GS::UniString& i_key, const GS::UniString& i_path, HKEY i_hKey/* = HKEY_CURRENT_USER*/, DWORD * o_bufSize/* = NULL*/)
{
	DWORD iBuffer = 0;
	DWORD iBufferSize = 255;

	LSTATUS _ = RegGetValueW(i_hKey,
		i_path.ToUStr().Get(),		/*L"SOFTWARE\\GRAPHISOFT\\ARCHICAD\\Archicad 26.0.0 INT R1\\Add-On Manager",*/
		i_key.ToUStr().Get(),		/*L"Use Default Location",*/
		RRF_RT_REG_DWORD,
		NULL,
		&iBuffer,
		&iBufferSize);

	if (o_bufSize != NULL)
		*o_bufSize = iBufferSize;

	return UInt16(iBuffer);
}

UInt32 GetRegIntOrDefault(const int i_defaultValue, const GS::UniString& i_key, const GS::UniString& i_path, HKEY i_hKey/* = HKEY_CURRENT_USER*/, DWORD * const o_bufSize/* = NULL*/)
{
	UInt32 iResult = GetRegInt(i_key, i_path, i_hKey, o_bufSize);

	if (o_bufSize != NULL)
		return iResult;
	else
		return i_defaultValue;
}

UInt32 GetRegIntOrSetDefault(const int i_defaultValue, const GS::UniString& i_key, const GS::UniString& i_path, HKEY i_hKey/* = HKEY_CURRENT_USER*/, DWORD * const o_bufSize/* = NULL*/)
{
	UInt32 iResult = GetRegInt(i_key, i_path, i_hKey, o_bufSize);

	if (o_bufSize != NULL)
		return iResult;
	else
		SetRegInt(i_defaultValue, i_key, i_path, i_hKey, o_bufSize);

		return i_defaultValue;
}

HKEY GetOrCreateRegPath(const GS::UniString& i_path, HKEY i_hKey = HKEY_CURRENT_USER)
{
	HKEY hKey;
	LSTATUS status = RegCreateKeyExW(i_hKey,
		(wchar_t*)(i_path.ToUStr().Get()),
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_WRITE,
		NULL,
		&hKey,
		NULL
	);

	return hKey;
}

void SetRegString(const GS::UniString& i_val, const GS::UniString& i_key, const GS::UniString& i_path, HKEY i_hKey/* = HKEY_CURRENT_USER*/, DWORD * const o_bufSize/* = NULL*/)
{
	HKEY hKey = GetOrCreateRegPath(i_path, i_hKey);

	auto wVal = (wchar_t*)(i_val.ToUStr().Get());
	auto key = (wchar_t*)(i_key.ToUStr().Get());

	auto _b = (BYTE*)wVal;
	auto _bs = (DWORD)(i_val.GetLength()*2);

	LSTATUS status = RegSetValueExW(hKey,
		key,
		0,
		REG_SZ,
		_b,
		_bs);

	if (o_bufSize != NULL)
		*o_bufSize = _bs;

	status = RegCloseKey(i_hKey);
	status = RegCloseKey(hKey);
}

void SetRegInt(const UInt32 i_val, const GS::UniString& i_key, const GS::UniString& i_path, HKEY i_hKey/* = HKEY_CURRENT_USER*/, DWORD * const o_bufSize/* = NULL*/)
{
	HKEY hKey = GetOrCreateRegPath(i_path, i_hKey);

	auto skey = (wchar_t*)(i_path.ToUStr().Get());
	auto key = (wchar_t*)(i_key.ToUStr().Get());

	auto _b = (const BYTE*)&i_val;
	auto _bs = (DWORD)sizeof(i_val);

	LSTATUS status = RegSetValueExW(hKey,
		key,
		0,
		REG_DWORD,
		_b,
		_bs);

	if (o_bufSize != NULL)
		*o_bufSize = _bs;

	status = RegCloseKey(i_hKey);
	status = RegCloseKey(hKey);
}

