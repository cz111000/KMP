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
	printf("�����ڵڣ�%d��λ��\n", kmp(resource, search));
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
	next[0] = -1;	//����ֵ����ȡ�ô�ֵ�������i++
	next[1] = 0;	//�ض���ƥ������ַ���
	int j = next[2 - 1];	//i��2��ʼ����jΪnext[i - 1]��ֵ�����ж�
	for (int i = 2;i < length;i++) {
		while ( j != 0 && search[j] != search[i - 1])	//���jΪ0˵�����ַ�֮ǰ���κ�ƥ���ַ������������ƥ��ֱ�ӽ����·�if������������Ƶ���ƥ���ַ����ж���ƥ���ַ�����һ���ַ��Ƿ�����ַ�ƥ��
			j = next[j];
		if (search[j] == search[i - 1])	//���jΪ0˵�����ַ�֮ǰ���κ�ƥ���ַ�������ôֱ���жϴ��ַ����Ƿ�Ϳ�ͷ�����ַ�ƥ��
			j++;
		next[i] = j;
	}
}