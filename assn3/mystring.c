#include <stddef.h>

int mystrlen(char* str) {
	int i = 0;
	while (str[i]) i++;
	return i;
}

int mystrcmp(char* str1, char* str2) {
	char char1, char2;
	int i = 0, j = 0;
	while (str1[i]) i++;
	while (str2[j]) j++;

	if (i > j) return 1;
	else if (i < j) return -1;
	else {
		while (1)
		{
			char1 = *str1++;
			char2 = *str2++;

			if (char1 != char2)
			{
				if (char1 < char2) return -1;
				else if (char1 > char2) return 1;
			}

			if (!char1) break;

		}
		return 0;
	}
}

char* mystrcpy(char* toStr, char* fromStr) {
	int i = 0;
	while (fromStr[i])
	{
		toStr[i] = fromStr[i];
		i++;
	}
	toStr[i] = NULL;
	return toStr;
}

char* mystrlower(char* str) {
	int i = 0;
	while (str[i])
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] = str[i] + 32;
		i++;
	}
	return str;
}
