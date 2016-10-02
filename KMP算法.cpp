#include <stdio.h>

int kmp(char* resource, char* search);
void makeNext(char* search, int* next);

int main(int argc, char* argv[])
{
	int i = 0;
	char resource[1000];
	fgets(resource, 1000, stdin);
	while (resource[i++] != '\n');resource[i - 1] = '\0';
	i = 0;
	char search[1000];
	fgets(search, 1000, stdin);
	while (search[i++] != '\n');search[i - 1] = '\0';
	printf("出现在第：%d个位置\n", kmp(resource, search));
	getchar();

	return 0;
}

int kmp(char* resource, char* search)
{
	int next[1000];
	makeNext(search, next);
	int j = 0;
	for (int i = 0;resource[i] != '\0';i++) {
		while (resource[i] != search[j] && j != -1)
			j = next[j];
		if (search[++j] == '\0')
			return i - j + 2;
	}
	return -1;
}

void makeNext(char* search, int* next)
{
	int length = -1;
	while (search[++length] != '\0');
	next[0] = -1;	//特殊值，当取得此值主串序号i++
	next[1] = 0;	//必定无匹配的子字符串
	int j = next[2 - 1];	//i从2开始，故j为next[i - 1]的值进行判断
	for (int i = 2;i < length;i++) {
		while ( j != 0 && search[j] != search[i - 1])	//如果j为0说明此字符之前无任何匹配字符串，如果出现匹配直接进入下方if，否则继续递推到子匹配字符串判断子匹配字符串后一个字符是否与此字符匹配
			j = next[j];
		if (search[j] == search[i - 1])	//如果j为0说明此字符之前无任何匹配字符串，那么直接判断此字符串是否和开头单个字符匹配
			j++;
		next[i] = j;
	}
}