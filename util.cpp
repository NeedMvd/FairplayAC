#include "util.h"
#include <iostream>
#include <array>
#include <memory>
#include <cstdlib>
#include <wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")


BOOL Util::RespawnAsChild(LPCWSTR ProcessName)
{
	DWORD ProcID = Util::getRunningProcessId(ProcessName);
	if (!ProcID)
		return FALSE;

	HANDLE hFirstInstance = OpenProcess(PROCESS_TERMINATE, FALSE, ProcID);

	if (!TerminateProcess(hFirstInstance, 0))
		return FALSE;

	STARTUPINFOA StartupInfo;
	PROCESS_INFORMATION ProcessInfo;
	ZeroMemory(&StartupInfo, sizeof(STARTUPINFOA));
	ZeroMemory(&ProcessInfo, sizeof(PROCESS_INFORMATION));

	if (CreateProcessA("C:\\Windows\\notepad.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo, &ProcessInfo))
		return TRUE;
	else return FALSE;
}

BOOL Util::EscalatePrivelages()
{
	HANDLE tokenHandle;
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!LookupPrivilegeValue(NULL, SE_LOAD_DRIVER_NAME, &luid))
		return FALSE;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &tokenHandle))
		return FALSE;

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(tokenHandle, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
		return FALSE;

	CloseHandle(tokenHandle);
	return TRUE;
}


//Check if process is running
DWORD Util::isProcessRunning(LPCWSTR processName)
{
	//Use ToolHelp32 to enumerate all running processes on the system
	HANDLE hSnap;
	PROCESSENTRY32 procEntry;

	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	procEntry.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hSnap, &procEntry))
	{
		if (lstrcmp(procEntry.szExeFile, processName) == 0)
			return true;
	}

	while (Process32Next(hSnap, &procEntry))
	{
		if (lstrcmp(procEntry.szExeFile, processName) == 0)
			return true;
	}

	return false;
}

//Obtain a process ID for a given process
DWORD Util::getRunningProcessId(LPCWSTR processName)
{
	HANDLE hSnap;
	PROCESSENTRY32 procEntry;

	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	procEntry.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hSnap, &procEntry))
	{
		if (lstrcmp(procEntry.szExeFile, processName) == 0)
		{
			return procEntry.th32ProcessID;
		}
	}

	while (Process32Next(hSnap, &procEntry))
	{
		if (lstrcmp(procEntry.szExeFile, processName) == 0)
		{
			return procEntry.th32ProcessID;
		}
	}

	return 0;
}


BOOL Util::isWindowsDefenderEnabled() {
    HRESULT hres;

    // Inicializa COM.
    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres)) {
        std::cout << "Falha ao inicializar COM. Error code = 0x" << std::hex << hres << std::endl;
        return false;
    }

    // Configura a segurança COM.
    hres = CoInitializeSecurity(
        NULL,
        -1,
        NULL,
        NULL,
        RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE,
        NULL
    );

    if (FAILED(hres)) {
        std::cout << "Falha ao inicializar a segurança. Error code = 0x" << std::hex << hres << std::endl;
        CoUninitialize();
        return false;
    }

    // Obtém o localizador para o provedor WMI.
    IWbemLocator* pLoc = NULL;

    hres = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator, (LPVOID*)&pLoc);

    if (FAILED(hres)) {
        std::cout << "Falha ao criar uma instância IWbemLocator. Error code = 0x" << std::hex << hres << std::endl;
        CoUninitialize();
        return false;
    }

    IWbemServices* pSvc = NULL;

    // Conecta-se ao servidor WMI.
    hres = pLoc->ConnectServer(
        BSTR(L"ROOT\\Microsoft\\Windows\\Defender"), // Namespace WMI do objeto.
        NULL, // Nome de usuário. NULL = atual usuário
        NULL, // Senha do usuário. NULL = atual
        0,    // Locale. NULL indica o atual
        NULL, // Flags de segurança.
        0,    // Autoridade
        0,    // Contexto
        &pSvc // ponteiro para IWbemServices proxy
    );

    if (FAILED(hres)) {
        std::cout << "Não foi possível conectar. Error code = 0x" << std::hex << hres << std::endl;
        pLoc->Release();
        CoUninitialize();
        return false;
    }

    // Configura a segurança para permitir chamadas WMI.
    hres = CoSetProxyBlanket(
        pSvc,                         // Indica o proxy para configurar
        RPC_C_AUTHN_WINNT,            // Autenticação RPC
        RPC_C_AUTHZ_NONE,             // Autorização RPC
        NULL,                         // Nome do servidor
        RPC_C_AUTHN_LEVEL_CALL,       // Nível de autenticação RPC
        RPC_C_IMP_LEVEL_IMPERSONATE,  // Nível de personificação RPC
        NULL,                         // Informações de autenticação do cliente
        EOAC_NONE                     // Capacidades do proxy
    );

    if (FAILED(hres)) {
        std::cout << "Não foi possível configurar a segurança do proxy. Error code = 0x" << std::hex << hres << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return false;
    }

    // Define a consulta WMI.
    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(
        BSTR("WQL"),
        BSTR("SELECT * FROM MSFT_MpComputerStatus"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator);

    if (FAILED(hres)) {
        std::cout << "Falha na consulta. Error code = 0x" << std::hex << hres << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return false;
    }

    // Obtém os dados da consulta.
    IWbemClassObject* pclsObj = NULL;
    ULONG uReturn = 0;
    bool defenderEnabled = false;

    while (pEnumerator) {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

        if (0 == uReturn) {
            break;
        }

        VARIANT vtProp;

        // Obtém o valor da propriedade "AntivirusEnabled".
        hr = pclsObj->Get(L"AntivirusEnabled", 0, &vtProp, 0, 0);
        defenderEnabled = vtProp.boolVal != 0;

        VariantClear(&vtProp);
        pclsObj->Release();
    }

    pSvc->Release();
    pLoc->Release();
    pEnumerator->Release();
    CoUninitialize();

    return TRUE;
}
