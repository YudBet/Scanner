#include <stdio.h>
#include <string.h>
#include <ctype.h>

void strtrim(char *str);
void substr(char *s, char *sub, int start, int len);
int isstrexist(char *set, char *s);


void strtrim(char *s) {

	int start = 0;
	while (!(isalnum(s[start]) || ispunct(s[start]))) start++;

	int len = strlen(s);
	if (s[len - 1] == '\n')
		s[len - 1] = '\0';
	
	int nl = (s[len - 1] == '\n');
	int valid_len = len - (start + nl);

	int i = 0;
	for (i = 0; i < valid_len; i++)
		s[i] = s[start + i];
	s[i] = '\0';
}

void substr(char *s, char *sub, int start, int len) {

	int i, j = start;
	for (i = 0; i < len; i++, j++)
		sub[i] = s[j];
	sub[i] = '\0';
}

int isstrexist(char *set, char *s) {

	char sub[32] = "|";
	strcat(sub, s);
	strcat(sub, "|");

	if (strstr(set, sub) != NULL) return 1;
	return 0;
}