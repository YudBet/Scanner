#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "strhandler.h"

FILE *source, *result;

enum STATUS {SPACE, PUNCT, NUM, ALPHA};
enum CLASS {IDEN = 1, REWD, INTE, FLOT, CHAR, STR, OPER, SPEC, SLC, MLC};

char REWDS[] = "|do|if|for|int|auto|case|char|else|enum|break|float|union|while| \
	|double|extern|return|sizeof|static|struct|switch|default|continue|register|";
char OPERS[] = "|+|-|*|/|=|,|";
char SPECS[] = "|{|}|(|)|;|'|:|\"|";


void scanSourceLineByLine(FILE *source, FILE *result);
void scanLine(int line_no, char *line, FILE *result);
void scanToken(char *token, int line_no);

int scanMLC(char *token, int line_no);
int scanSLC(char *token, int line_no);
int scanOPER(char *token, int line_no);
int scanSPEC(char *token, int line_no);
int scanSTR(char *token, int line_no);
int scanCHAR(char *token, int line_no);
int scanREWD(char *token, int line_no);
int scanIDEN(char *token, int line_no);
int scanINTE(char *token, int line_no);
int scanFLOT(char *token, int line_no);

int getTokenCountByStartIndexs(char *line, int *token_starts);
int getCurStatus(char c);


int getCurStatus(char c) {

	if (isalpha(c) || c == '_') return ALPHA;
	if (isdigit(c)) return NUM;
	if (ispunct(c)) return PUNCT;
	return SPACE;
}

int f = 0;
int getTokenCountByStartIndexs(char *line, int *token_starts) {

	int token_count = 0;
	int len = strlen(line);
	int pre_status = SPACE, cur_status = SPACE;
	int dot_f = 0, f_f = 0, e_f = 0, pn_f = 0;
	int i, start = 0;
	for (i = 0; i < len; i++) {

		char ch = line[i];
		cur_status = getCurStatus(ch);

		if (f && ch != '*') continue;	


		if (pre_status == ALPHA && cur_status == NUM) continue;


		if (cur_status == NUM) e_f = pn_f = 0;
		else dot_f = 0;

		if (dot_f && cur_status == NUM) {
			f_f = 1;
			dot_f = 0;
			pre_status = NUM;
			continue;
		}
		if (ch == '.' && !dot_f) {
			dot_f = 1;
			if (f_f || e_f || pn_f) {
				f_f = e_f = pn_f = 0;
				pre_status = SPACE;
			}
			else if (pre_status == NUM) {
				f_f = 1;
				dot_f = 0;
				continue;
			}
		}
		if (f_f && (ch == 'e' || ch == 'E')) {
			f_f = 0;
			e_f = 1;
			continue;
		}
		if (e_f && (ch == '+' || ch == '-')) {
			pn_f = 1;
			e_f = 0;
			continue;
		}
		

		if (pre_status != cur_status || (pre_status == cur_status && cur_status == PUNCT)) {

			token_count++;
			token_starts[start] = i;
			start++;
			
			if (ch == '/' && line[i + 1] == '/') {
				token_starts[start] = i + 2;
				return token_count;
			}
			if (ch == '/' && line[i + 1] == '*') {
				token_starts[start] = i + 2;
				i++;
				f = 1;
				continue;
			}
			if (ch == '*' && line[i + 1] == '/') {
				token_starts[start] = i + 2;
				i++;
				f = 0;
				continue;
			}

			if (ch == '\'') while ((ch = line[++i]) != '\'' && i < len);
			if (ch == '\"') while ((ch = line[++i]) != '\"' && i < len);
		}

		pre_status = cur_status;
	}

	token_starts[start] = len;
	return token_count;
}


int mlc_f = 0, start_no;
int scanMLC(char *token, int line_no) {

	if (token[0] == '/' && token[1] == '*') {

		start_no = line_no;
		mlc_f = 1;
		return 1;
	}
	if (mlc_f == 1 && strcmp(token, "*/") == 0) {

		mlc_f = 0;
		fprintf(result, "%d-%d\\%d\\%s\n", start_no, line_no, MLC, "多行註解");
		return 1;
	}
	else if (mlc_f == 1) return 1;
	return 0;
}

int scanSLC(char *token, int line_no) {

	if (strcmp(token, "//") == 0) {
		
		fprintf(result, "%d\\%d\\%s\n", line_no, SLC, "單行註解");
		return 1;
	}
	return 0;
}

int scanOPER(char *token, int line_no) {

	if (isstrexist(OPERS, token)) {

		fprintf(result, "%d\\%d\\%s\n", line_no, OPER, token);
		return 1;
	}
	return 0;
}

int scanSPEC(char *token, int line_no) {

	if (isstrexist(SPECS, token)) {

		fprintf(result, "%d\\%d\\%s\n", line_no, SPEC, token);
		return 1;
	}
	return 0;
}

int scanSTR(char *token, int line_no) {

	int len = strlen(token);
	if (token[0] == '\"') {
		if (len == 1 || token[len - 1] != '\"') {

			fprintf(result, "%d\\%d\\%s\n", line_no, -1, "Expected: \"");
			return 1;
		} 
		char sliteral[32];
		substr(token, sliteral, 1, len - 2);
		fprintf(result, "%d\\%d\\%c\n%d\\%d\\%s\n%d\\%d\\%c\n",
			line_no, SPEC, '\"',
			line_no, STR, sliteral,
			line_no, SPEC, '\"');
		return 1;
	}
	return 0;
}

int scanCHAR(char *token, int line_no) {

	int len = strlen(token);
	if (token[0] == '\'') {
		if (len == 1 || token[len - 1] != '\'') {

			fprintf(result, "%d\\%d\\%s\n", line_no, -1, "Expected: \'");
			return 1;
		} 
		char chliteral[32];
		substr(token, chliteral, 1, len - 2);
		fprintf(result, "%d\\%d\\%c\n%d\\%d\\%s\n%d\\%d\\%c\n",
			line_no, SPEC, '\'',
			line_no, CHAR, chliteral,
			line_no, SPEC, '\'');
		return 1;
	}
	return 0;
}

int scanREWD(char *token, int line_no) {

	if (isstrexist(REWDS, token)) {

		fprintf(result, "%d\\%d\\%s\n", line_no, REWD, token);
		return 1;
	}
	return 0;
}

int scanIDEN(char *token, int line_no) {

	if (strlen(token) == 1 && token[0] == '_') {

		fprintf(result, "%d\\%d\\%s%s\n", line_no, -1, "Undefine token: ", token);
		return 1;
	}
	if (token[strlen(token) - 1] == '_') {

		fprintf(result, "%d\\%d\\%s%s\n", line_no, -1, "IDEN end with '_': ", token);
		return 1;
	}
	if (isalpha(token[0]) || token[0] == '_') {
		
		fprintf(result, "%d\\%d\\%s\n", line_no, IDEN, token);
		return 1;
	}
	return 0;
}

int scanINTE(char *token, int line_no) {

	int i = 0;
	while (isdigit(token[i++]));
	if (i <= strlen(token)) return 0;

	fprintf(result, "%d\\%d\\%s\n", line_no, INTE, token);
	return 1;
}

int scanFLOT(char *token, int line_no) {

	int len = strlen(token);
	if (len == 1) return 0;
	if (strstr(token, ".") != NULL) {

		if (!isdigit(token[len - 1]) && token[len - 1] != '.') {

			fprintf(result, "%d\\%d\\%s%s\n", line_no, -1, "FLOT exponent: ", token);
			return 1;
		}
		fprintf(result, "%d\\%d\\%s\n", line_no, FLOT, token);
		return 1;
	}
	return 0;
}


void scanToken(char *token, int line_no) {

	if (scanMLC(token, line_no)) return;
	if (scanSLC(token, line_no)) return;
	if (scanSTR(token, line_no)) return;
	if (scanCHAR(token, line_no)) return;
	if (scanOPER(token, line_no)) return;
	if (scanSPEC(token, line_no)) return;
	if (scanREWD(token, line_no)) return;
	if (scanIDEN(token, line_no)) return;
	if (scanINTE(token, line_no)) return;
	if (scanFLOT(token, line_no)) return;
	fprintf(result, "%d\\%d\\%s%s\n", line_no, -1, "Undefine token: ", token);
}

void scanLine(int line_no, char *line, FILE *result) {

	strtrim(line);
	if (strlen(line) == 0) return;
	
	int token_starts[128] = {0};
	int token_count = getTokenCountByStartIndexs(line, token_starts);

	int i;
	char token[32];
	int start, len;
	for (i = 0; i < token_count; i++) {

		start = token_starts[i];
		if (line[start] == ' ') continue;

		len = token_starts[i + 1] - start;
		substr(line, token, start, len);

		scanToken(token, line_no);
	}
}

void scanSourceLineByLine(FILE *source, FILE *result) {

	int line_no = 1;
	char line[128];

	while (fgets(line, sizeof(line), source) != NULL) {
		
		scanLine(line_no, line, result);
		line_no++;
	}
	if (mlc_f == 1)
		printf("%d\\%d\\%s\n", line_no - 1, -1, "Expected: */");
}

int main(void) {
	
	if ((source = fopen("sample.c", "r")) == NULL) {

		printf("%s\n", "\"sample.c\" is not found.");
		return 1;
	}

	result = fopen("result.txt", "w");
	scanSourceLineByLine(source, result);

	fclose(result);
	fclose(source);

	system("pause");
	return 0;
}