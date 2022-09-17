#include <Windows.h>

typedef unsigned __int64 QWORD;

// Steep v1.23.7023440 - Ubisoft Connect

class TimeManager
{
	// Points to a 24-hour value.
	float* GetTimePointer()
	{
		QWORD base = reinterpret_cast<QWORD>(GetModuleHandle(NULL));
		QWORD address = base + 0x2DEB8B0;

		return reinterpret_cast<float*>(address);
	}

public:
	float GetTime()
	{
		return *(GetTimePointer());
	}

	void SetTime(float value)
	{
		*(GetTimePointer()) = value;
	}

	__declspec(property(get = GetTime, put = SetTime)) float Time;
};

DWORD __stdcall ModThread(HMODULE modModule)
{
	DisableThreadLibraryCalls(modModule);
	auto timeManager = new TimeManager();

	while (true)
	{
		if (GetAsyncKeyState(VK_END))
			break;

		// The engine takes care to reset the value back to 0 once exceeding 24.
		// Adding 0.0015 each 100 millisecond will result in adding 54 minutes game time each minute real time
		timeManager->Time += 0.0015f;

		Sleep(100);
	}

	delete timeManager;
	
	MessageBox(GetForegroundWindow(), L"Mod unloaded!", L"Info", MB_OK | MB_ICONASTERISK);
	FreeLibraryAndExitThread(modModule, 0);

	return 0;
}

BOOL __stdcall DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ModThread, hModule, 0, 0);
		break;
	}
	}
	return TRUE;
}