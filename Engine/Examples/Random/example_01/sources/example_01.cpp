// example_01.cpp: ���������� ����� ����� ��� ����������� ����������.
//


#include "stdafx.h"
#include <Engine/Core/Random/Random.h>
#include "conio.h"
#include <string>
using std::string;





int _tmain(int argc, _TCHAR* argv[])
{
	float t;

	// ����� ���������� ����� �� 0 �� 1 �������, ������� ������ 100
	while((t = Random::_RandomFloat(0, 1000000)) > 100)
	{
		printf("\nRandonFloat(0-1kk).Find < 100: %f", t);
	}
	printf("\nRandonFloat: %f", t);
		
	
	getch();// �����; ��������� ���� � ���������� ��� �����������

	// ����� ���������� ����� �� 0 �� 1 �������, ������� ������ 10
	while((t = Random::_RandomFloat(0, 1000000)) > 10)
	{
		printf("\nRandonFloat(0-1kk).Find < 10: %f", t);
	}
	printf("\nRandonFloat: %f", t);// ������ 0
		

	getch();// �����; ��������� ���� � ���������� ��� �����������


	// ����� ���������� ����� �� 0 �� 100 �����, ������� ������ 100
	while((t = Random::_RandomFloat(0, 100000)) > 100)
	{
		printf("\nRandonFloat(0-100k).Find < 100: %f", t);
	}
	printf("\nRandonFloat: %f", t);
		
	
	getch();// �����; ��������� ���� � ���������� ��� �����������

	// ����� ���������� ����� �� 0 �� 100 �����, ������� ������ 10
	while((t = Random::_RandomFloat(0, 100000)) > 10)
	{
		printf("\nRandonFloat(0-100k).Find < 10: %f", t);
	}
	printf("\nRandonFloat: %f", t);
	

	getch();// �����; ��������� ���� � ���������� ��� �����������

	// ����� ���������� ����� �� 0 �� 100 �����, ������� ������ 1
	while((t = Random::_RandomFloat(0, 100000)) > 1)
	{
		printf("\nRandonFloat(0-100k).Find < 1: %f", t);
	}
	printf("\nRandonFloat: %f", t);// ������ 0
	


	getch();
	return 0;
}

