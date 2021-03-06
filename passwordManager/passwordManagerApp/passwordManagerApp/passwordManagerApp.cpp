// passwordManagerApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "passwordManagerEnclave_u.h"
#include "sgx_urts.h"
#include <stdio.h>
#include <iostream>
#include "sgx_capable.h"
#include "sgx_uae_service.h"
#include "sgx_tcrypto.h"

#define ENCLAVE_FILE _T("passwordManagerEnclave.signed.dll")
#define MAX_BUF_LEN 100

using namespace std;

sgx_enclave_id_t enclaveId = NULL;
sgx_launch_token_t token = {0};
int updated;


bool authenticate(char *username, char *password)
{
	int ret = -1;
	authenticate(enclaveId,&ret,username,password,MAX_BUF_LEN);
	if(ret < 0) return false;
	return true;
}

bool decrypt(string *password)
{
	//do crypto stuff
	return true;
}

bool destroyEnclave()
{
	if(sgx_destroy_enclave(enclaveId) != SGX_SUCCESS)
		//printf("Error %x: cant destroy enclave\n", ret);
		return false;
	return true;
}

bool destroyEnclave(sgx_enclave_id_t id)
{
	if(sgx_destroy_enclave(id) != SGX_SUCCESS)
		return false;
	return true;
}


bool encrypt(string *password)
{
	//do crypto stuff
	return true;
}



int getSize()
{
	int size;
	return getSize(enclaveId);
	return size;
}

bool initializeEnclave()
{
	if(sgx_create_enclave(ENCLAVE_FILE, SGX_DEBUG_FLAG, &token, &updated, 
		&enclaveId, NULL) != SGX_SUCCESS)
		//printf("Error %#x: cannot create enclave\n", ret);
		return false;
	return true;
}

bool initializeEnclave(sgx_enclave_id_t *id)
{
	if(sgx_create_enclave(ENCLAVE_FILE, SGX_DEBUG_FLAG, &token, &updated, 
		id, NULL) != SGX_SUCCESS)
		return false;
	return true;
}

//main function used to debug/demo enclave without GUI
int main()
{
	sgx_status_t ret = SGX_SUCCESS;
	int retval, option=1;
	char *username, *password, *input;

	/*if(sgx_is_capable(&updated) != SGX_ENABLED) 
	{
		printf("Error %#x: SGX is not enabled on this device\n", ret);
		return -1;
	}
	*/

	username = (char*)malloc(MAX_BUF_LEN*sizeof(char));
	password = (char*)malloc(MAX_BUF_LEN*sizeof(char));
	input = (char*)malloc(MAX_BUF_LEN*sizeof(char));

	if(!initializeEnclave())
	{
		printf("Error %#x: cannot create enclave\n", ret);
		return -1;
	}


	//convert to GUI buttons
	while(option != 0)
	{
		cout << "OPTIONS:\n1 - Add User\n2 - Add account\n3 - Add URL\n";
		cout << "4 - Get Password\n5 - Get Account\n6 - Get URL\n";
		cout << "7 - Remove User\n0 - Quit\n" << endl;
		scanf_s("%i",&option);

		switch(option)
		{
		case 1:
			cout << "\nusername: ";
			cin >> username;
			cout << "password: ";
			cin >> password;
			addUser(enclaveId,&retval,username,password,MAX_BUF_LEN);
			if(retval < 0)
				cout << "Failed to add user: " << username << endl;
			continue;
		case 2:
			cout << "\nusername: ";
			cin >> username;
			findUser(enclaveId,&retval,username,MAX_BUF_LEN);
			if(retval < 0)
			{
				cout << username << " does not exist." << endl;
				continue;
			}
			cout << "account: ";
			cin >> input;
			setAccount(enclaveId,&retval,username,input,MAX_BUF_LEN);
			if(retval < 0)
				cout << "Failed to add account info for " << username << endl;
			continue;
		case 3:
			cout << "\nusername: ";
			cin >> username;
			findUser(enclaveId,&retval,username,MAX_BUF_LEN);
			if(retval < 0)
			{
				cout << username << " does not exist." << endl;
				continue;
			}
			cout << "URL: ";
			cin >> input;
			setUrl(enclaveId,&retval,username,input,MAX_BUF_LEN);
			if(retval < 0)
				cout << "Failed to add url for " << username << endl;
			continue;
		case 4:
			cout << "\nusername: ";
			cin >> username;
			findUser(enclaveId,&retval,username,MAX_BUF_LEN);
			if(retval < 0)
			{
				cout << username << " does not exist." << endl;
				continue;
			}
			getPassword(enclaveId,&retval,username,MAX_BUF_LEN);
			cout << username << endl;
			continue;
		case 5:
			cout << "\nusername: ";
			cin >> username;
			findUser(enclaveId,&retval,username,MAX_BUF_LEN);
			if(retval < 0)
			{
				cout << username << " does not exist." << endl;
				continue;
			}
			getAccount(enclaveId,&retval,username,MAX_BUF_LEN);
			cout << username << endl;
			continue;
		case 6:
			cout << "\nusername: ";
			cin >> username;
			findUser(enclaveId,&retval,username,MAX_BUF_LEN);
			if(retval < 0)
			{
				cout << username << " does not exist." << endl;
				continue;
			}
			getUrl(enclaveId,&retval,username,MAX_BUF_LEN);
			cout << username << endl;
			continue;
		case 7:
			cout << "\nusername: ";
			cin >> username;
			findUser(enclaveId,&retval,username,MAX_BUF_LEN);
			if(retval < 0)
			{
				cout << username << " does not exist." << endl;
				continue;
			}
			removeUser(enclaveId,&retval,username,MAX_BUF_LEN);
			continue;

		default:
			printf("Invalid option.\n");
			continue;

		}



	}




	if(!destroyEnclave())
	{
		printf("Error %x: cant destroy enclave\n", ret);
		return -1;
	}


	return 0;
}

