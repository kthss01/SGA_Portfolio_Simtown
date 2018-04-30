#include "stdafx.h"
#include "TextData.h"


TextData::TextData()
{
}


TextData::~TextData()
{
}

HRESULT TextData::Init()
{
	return S_OK;
}

void TextData::TextSave(char * saveFileName, vector<string> vStr)
{
	// api�� �������ִ� save �Լ� ���
	HANDLE file;
	char str[MAXSTRSIZE];
	DWORD write;

	// �������ִµ� ũ����� ���������� ��
	strncpy_s(str, MAXSTRSIZE, VectorArrayCombine(vStr), MAXSTRSIZE);

	// CreateFile Binary �������� ��
	// Binary 16���� ������ �������� ��
	// Binary �ѱ� �������� ������ �־���
	file = CreateFile(
		saveFileName,	// ������ ���� �Ǵ� �ε��� ������ �̸�
		GENERIC_WRITE,	// �����̳� ��ġ�� ����ų� ������ ����
		0,				// ���� ���� ��� �Է�
		NULL,			// ���� �Ǵ� ��ġ�� �� �� ���� �� Ư��
						// �׻� ó������ �ۼ��ϰڴٴ°� �� �����
						// ���� �ٲ��ָ� ���κ��� ���� �� ����
		CREATE_ALWAYS,	// �����̳� ��ġ�� �� �� ���� �ൿ
		FILE_ATTRIBUTE_NORMAL,	// ����, ��ġ�� �� ��, ���� �� ���� Ư��
		NULL			// ������� ������ ���� �� Ư�� etc ����Ǵ� �ڵ�
	);

	WriteFile(file, str, MAXSTRSIZE, &write, NULL);
	CloseHandle(file);
}

char * TextData::VectorArrayCombine(vector<string> vArray)
{
	char str[MAXSTRSIZE];
	ZeroMemory(str, sizeof(str));

	for (int i = 0; i < vArray.size(); i++) {
		strncat_s(str, MAXSTRSIZE, vArray[i].c_str(), MAXSTRSIZE);

		// ������ ���ڴ� �����Ϸ��� �ؼ�
		if ((i + 1) < vArray.size()) {
			strcat_s(str, ",");
		}
	}

	return str;
}

vector<string> TextData::TextLoad(char * loadFileName)
{
	HANDLE file;
	char str[MAXSTRSIZE];
	DWORD read;

	ZeroMemory(str, sizeof(str));

	file = CreateFile(
		loadFileName,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	ReadFile(file, str, MAXSTRSIZE, &read, NULL);
	CloseHandle(file);

	return CharArraySeparation(str);
}

vector<string> TextData::CharArraySeparation(char charArray[])
{
	vector<string> vArray;
	char* separator = (char*)",";

	char* context = NULL;

	char* token = strtok_s(charArray, separator, &context);
	vArray.push_back(token);
	while (token) {
		token = strtok_s(NULL, separator, &context);
		if (token != NULL)
			vArray.push_back(token);
	}

	return vArray;;
}

void TextData::TextWrite(vector<string> vStr)
{
	char str[MAXSTRSIZE];

	ZeroMemory(str, sizeof(str));

	strcpy_s(str, MAXSTRSIZE, VectorArrayCombine(vStr));
	fprintf(fp, "%s\n", str);
}

vector<vector<string>> TextData::TextRead()
{
	vector< vector<string> > vvStr;

	char str[MAXSTRSIZE];

	while (fscanf_s(fp, "%s", &str, MAXSTRSIZE) != EOF) {
		vvStr.push_back(CharArraySeparation(str));
	}

	return vvStr;
}

