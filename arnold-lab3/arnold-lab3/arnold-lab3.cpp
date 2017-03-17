// arnold-lab3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include <Windows.h> // Not using <Windows.h> directly due to a conflict with the Winsock API.
#include <winsock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#pragma comment(lib, "IPHLPAPI.lib")



int main()
{

	FILE* passwords;
	passwords = fopen("passwords.txt", "w");

	printf("Password 1 is: FGHIJK6789:;\n");
	fprintf(passwords, "Password 1 is: FGHIJK6789:;\n");
	printf("Password 2 is: AQAEE@GBDC;G\n");
	fprintf(passwords, "Password 2 is: AQAEE@GBDC;G\n");
	printf("Password 3 is: GN\"EH\CU[(+*;G\n");
	fprintf(passwords, "Password 3 is: GN\"EH\CU[(+*;G\n");

	char pass4[13];
	// Determine Password 4 using the users hard disk volume information.
	DWORD serial = 0;
	GetVolumeInformationW(TEXT("C:\\"), NULL, NULL, &serial, NULL, NULL, NULL, NULL);
	sprintf(pass4, "%d", serial);

	printf("Password 4 is: %s\n", pass4);
	fprintf(passwords, "Password 4 is: %s\n", pass4);
	printf("(Password 4 is system specific)\n");
	fprintf(passwords, "(Password 4 is system specific)\n");

	// Determine Password 5... somehow.
	// Password5 appears to use Winsock functions to determine the answer.
	// To try and find the answer, I will reference the following MSDN page.
	// https://msdn.microsoft.com/en-us/library/windows/desktop/aa365915(v=vs.85).aspx
	// Sample code at the bottom of the article is used as a base for the following solution.

	printf("\n\nPassword 5? I want to assume this is the \n");
	printf("Physical Address entry in the structure \n");
	printf("but a few different permutations on that idea \n");
	printf("didn't give me the result I was looking for. \n");
	printf("This last one was definitely tricky and I didn't \n");
	printf("have the time to wrap it up before the due date. \n");
	printf("My appologies. \n");

	fprintf(passwords, "\n\nPassword 5? I want to assume this is the \n");
	fprintf(passwords, "Physical Address entry in the structure \n");
	fprintf(passwords, "but a few different permutations on that idea \n");
	fprintf(passwords, "didn't give me the result I was looking for. \n");
	fprintf(passwords, "This last one was definitely tricky and I didn't \n");
	fprintf(passwords, "have the time to wrap it up before the due date. \n");
	fprintf(passwords, "My appologies. \n");

	// Determine Password 5... somehow.
	// Password5 appears to use Winsock functions to determine the answer.
	// To try and find the answer, I will reference the following MSDN page.
	// https://msdn.microsoft.com/en-us/library/windows/desktop/aa365915(v=vs.85).aspx
	// Sample code at the bottom of the article is used as a base for the following solution.

	DWORD dwSize = 0;
	DWORD dwRetVal = 0;

	unsigned int i = 0;
	// Set the flags to pass to GetAdaptersAddresses
	ULONG flags = 0;// GAA_FLAG_INCLUDE_PREFIX;

					// default to unspecified address family (both)
	ULONG family = AF_INET;
	PIP_ADAPTER_ADDRESSES pAddresses = NULL;

	#define WORKING_BUFFER_SIZE 15000
	ULONG outBufLen = WORKING_BUFFER_SIZE; // 376 is used here as in the Matryoshka program.

	pAddresses = (IP_ADAPTER_ADDRESSES *)HeapAlloc(GetProcessHeap(), 0, outBufLen); // Allocate 376 bytes on the heap?

	dwRetVal = GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

	if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
		HeapFree(GetProcessHeap(), 0, (&pAddresses));
		pAddresses = NULL;
	}

	if (dwRetVal == NO_ERROR) {

		char buf[300];
		// We do this again for some reason?
		dwRetVal = GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);
		if (dwRetVal == NO_ERROR)
		{
			printf("offsets: i=%x\n", offsetof(_IP_ADAPTER_ADDRESSES_LH, Alignment));
			printf("offsets: i=%x\n", offsetof(_IP_ADAPTER_ADDRESSES_LH, Length));
			printf("offsets: i=%x\n", offsetof(_IP_ADAPTER_ADDRESSES_LH, IfIndex));
			printf("offsets: i=%x\n", offsetof(_IP_ADAPTER_ADDRESSES_LH, Next));
			printf("offsets: i=%x\n", offsetof(_IP_ADAPTER_ADDRESSES_LH, AdapterName));
			printf("offsets: i=%x\n", offsetof(_IP_ADAPTER_ADDRESSES_LH, FirstUnicastAddress));
			printf("offsets: i=%x\n", offsetof(_IP_ADAPTER_ADDRESSES_LH, FirstMulticastAddress));
			printf("offsets: i=%x\n", offsetof(_IP_ADAPTER_ADDRESSES_LH, FirstDnsServerAddress));
			printf("offsets: i=%x\n", offsetof(_IP_ADAPTER_ADDRESSES_LH, DnsSuffix));
			printf("offsets: i=%x\n", offsetof(_IP_ADAPTER_ADDRESSES_LH, Description));
			printf("offsets: i=%x\n", offsetof(_IP_ADAPTER_ADDRESSES_LH, FriendlyName));
			printf("offsets: P=%x\n", offsetof(_IP_ADAPTER_ADDRESSES_LH, PhysicalAddress));  // PA is at offset 2C...
			printf("offsets: i=%x\n", offsetof(_IP_ADAPTER_ADDRESSES_LH, PhysicalAddressLength));

			char physaddr[8];
			char *ptr;
			ZeroMemory(physaddr, 8);

			// The following code produces the hex representation of a physical (MAC) address?
			if (pAddresses->PhysicalAddressLength != 0) {
				printf("\tPhysical address: ");
				for (i = 0; i < (int)pAddresses->PhysicalAddressLength;
					i++) {
					if (i == (pAddresses->PhysicalAddressLength - 1))
						printf("%X\n",
						(int)pAddresses->PhysicalAddress[i]);

					else
						printf("%X",
						(int)pAddresses->PhysicalAddress[i]);

					physaddr[i] = (long)pAddresses->PhysicalAddress[i];
				}
			}
		}
	}

	// I would have expected this to be the solution but every permutation I tried
	// didn't lead to a good result.
	fclose(passwords);
    return 0;
}

