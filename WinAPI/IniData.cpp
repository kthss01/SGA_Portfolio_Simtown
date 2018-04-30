#include "stdafx.h"
#include "IniData.h"


IniData::IniData()
{
}


IniData::~IniData()
{
}

void IniData::AddData(const char * section, const char * key, const char * value)
{
	tagIniData iniData;
	//iniData.section = section;
	//iniData.key = key;
	//iniData.value = value;

	strcpy_s(iniData.section, sizeof(iniData.section), section);
	strcpy_s(iniData.key, sizeof(iniData.key), key);
	strcpy_s(iniData.value, sizeof(iniData.value), value);

	this->_vIniData.push_back(iniData);
}

void IniData::SaveData(const char * fileName)
{
	char str[256];
	char dir[256];

	ZeroMemory(str, sizeof(str));
	ZeroMemory(dir, sizeof(dir));

	// C���� �� ������Ʈ������ ��� �������� �Լ�
	// ������ D������ d���� 
	GetCurrentDirectory(256, str);
	// save ������ ����������
	sprintf_s(dir, "/save/%s.ini", fileName);
	strcat_s(str, dir);

	for (int i = 0; i < _vIniData.size(); i++) {
		WritePrivateProfileString(
			_vIniData[i].section,
			_vIniData[i].key,
			_vIniData[i].value,
			str);
	}
	_vIniData.clear();
}

char * IniData::LoadDataString(const char * fileName, const char * section, const char * key)
{
	char str[256];
	char dir[256];

	ZeroMemory(str, sizeof(str));
	ZeroMemory(dir, sizeof(dir));

	// C���� �� ������Ʈ������ ��� �������� �Լ�
	// ������ D������ d���� 
	GetCurrentDirectory(256, str);
	// save ������ ����������
	sprintf_s(dir, "/save/%s.ini", fileName);
	strcat_s(str, dir);

	char data[64] = { 0 };

	GetPrivateProfileString(
		section,
		key,
		"",			// value �� ������ � ���ڷ� ó������
		data,		// ���� ���� ����
		64,			// ũ��
		str			// ���
	);

	return data;
}

int IniData::LoadDataInteger(const char * fileName, const char * section, const char * key)
{
	char str[256];
	char dir[256];

	ZeroMemory(str, sizeof(str));
	ZeroMemory(dir, sizeof(dir));

	// C���� �� ������Ʈ������ ��� �������� �Լ�
	// ������ D������ d���� 
	GetCurrentDirectory(256, str);
	// save ������ ����������
	sprintf_s(dir, "/save/%s.ini", fileName);
	strcat_s(str, dir);

	// �Ǽ����� ��Ʈ������ �о�ͼ� ������ �ؾ���
	return GetPrivateProfileInt(section, key, 0, str);
}
