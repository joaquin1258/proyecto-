#pragma once
#include <windows.h>


typedef LONG NTSTATUS;
typedef PVOID BCRYPT_ALG_HANDLE;
typedef PVOID BCRYPT_KEY_HANDLE;
typedef PVOID BCRYPT_HANDLE;

#define BCRYPT_SHA256_ALGORITHM L"SHA256"
#define BCRYPT_ALG_HANDLE_HMAC_FLAG 0x00000008


#define BCRYPT_AES_ALGORITHM L"AES"
#define BCRYPT_CHAINING_MODE L"ChainingMode"
#define BCRYPT_CHAIN_MODE_CBC L"ChainingModeCBC"
#define BCRYPT_BLOCK_PADDING 0x00000001


__declspec(dllimport) NTSTATUS __stdcall BCryptOpenAlgorithmProvider(BCRYPT_ALG_HANDLE *phAlgorithm, LPCWSTR pszAlgId, LPCWSTR pszImplementation, ULONG dwFlags);
__declspec(dllimport) NTSTATUS __stdcall BCryptCloseAlgorithmProvider(BCRYPT_ALG_HANDLE hAlgorithm, ULONG dwFlags);
__declspec(dllimport) NTSTATUS __stdcall BCryptSetProperty(BCRYPT_HANDLE hObject, LPCWSTR pszProperty, PUCHAR pbInput, ULONG cbInput, ULONG dwFlags);


__declspec(dllimport) NTSTATUS __stdcall BCryptDeriveKeyPBKDF2(BCRYPT_ALG_HANDLE hAlgorithm, PUCHAR pbPassword, ULONG cbPassword, PUCHAR pbSalt, ULONG cbSalt, ULONGLONG cIterations, PUCHAR pbDerivedKey, ULONG cbDerivedKey, ULONG dwFlags);


__declspec(dllimport) NTSTATUS __stdcall BCryptGenerateSymmetricKey(BCRYPT_ALG_HANDLE hAlgorithm, BCRYPT_KEY_HANDLE *phKey, PUCHAR pbKeyObject, ULONG cbKeyObject, PUCHAR pbSecret, ULONG cbSecret, ULONG dwFlags);
__declspec(dllimport) NTSTATUS __stdcall BCryptEncrypt(BCRYPT_KEY_HANDLE hKey, PUCHAR pbInput, ULONG cbInput, PVOID pPaddingInfo, PUCHAR pbIV, ULONG cbIV, PUCHAR pbOutput, ULONG cbOutput, ULONG *pcbResult, ULONG dwFlags);
__declspec(dllimport) NTSTATUS __stdcall BCryptDecrypt(BCRYPT_KEY_HANDLE hKey, PUCHAR pbInput, ULONG cbInput, PVOID pPaddingInfo, PUCHAR pbIV, ULONG cbIV, PUCHAR pbOutput, ULONG cbOutput, ULONG *pcbResult, ULONG dwFlags);
__declspec(dllimport) NTSTATUS __stdcall BCryptDestroyKey(BCRYPT_KEY_HANDLE hKey);
