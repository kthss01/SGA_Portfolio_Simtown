#pragma once

#include "SIngletonBase.h"

#define MAXSTRSIZE 128

class TextData : public SingletonBase<TextData>
{
private:
	FILE * fp;
public:
	TextData();
	~TextData();

	HRESULT Init();

	// Save
	void TextSave(char* saveFileName, vector<string> vStr);
	char* VectorArrayCombine(vector<string> vArray);

	// Load
	// �� �ٿ� ���� ������
	vector<string> TextLoad(char* loadFileName);
	// �����͵��� ����Ʈ
	vector<string> CharArraySeparation(char charArray[]);

	// ���� ���� ����� ���
	void CreateWriteTextDataFilePointer(char * fileName) {
		fopen_s(&fp, fileName, "w");
	}
	void TextWrite(vector<string> vStr);
	vector< vector<string> > TextRead();
	void CreateReadTextDataFilePointer(char * fileName) {
		fopen_s(&fp, fileName, "r");
	}
	void CloseTextDataFilePointer() { fclose(fp); }
};

#define TEXTDATA TextData::GetSingleton()

