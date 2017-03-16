#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

regex_t regex;

int KEYWORDS_SIZE = 48;
int OPERATORS_SIZE  = 2;
int LITERAL_SIZE = 2;
int IDENTIFIERS_SIZE = 1;
int NUMERIC_LITERALS_SIZE = 1;
int hasMatched = 0;

char* KEYWORDS[] = {"^ALL\\b", "^AND\\b", "^COUNT\\b", "^MAX\\b", "^AVG\\b", "^MIN\\b", "^SUM\\b", "^ANY\\b", "^AS\\b", "^WHERE\\b", "^BY\\b","^VARCHAR\\b","^VARCHARACTER\\b", "^CHAR\\b", "^CHARACTER\\b", "^CREATE\\b", "^DECIMAL\\b", "^DELETE\\b", "^DESC\\b", "^DISTINCT\\b", "^FOREIGN\\b", "^FROM\\b", "^GRANT\\b", "^GROUP\\b", "^HAVING\\b", "^IN\\b", "^INSERT\\b", "^INT\\b", "^INTEGER\\b", "^INTO\\b", "^IS\\b", "^KEY\\b", "^LIKE\\b", "^NOT\\b", "^NULL\\b", "^NUMERIC\\b", "^OPTION\\b", "^OR\\b", "^ORDER\\b", "^PRIMARY\\b", "^REFERENCES\\b", "^SELECT\\b", "^TABLE\\b", "^UNION\\b", "^UNIQUE\\b", "^UPDATE\\b", "^USER\\b", "^VALUES\\b" };

char* OPERATORS[] = {"^[=|<>|<|>|<=|>=]", "^[-*+/(),.;]"};

char* LITERALS[] = {"^[\"][A-Za-z%%][][A-Za-z0-9_%%]*[\"]", "^[\'][A-Za-z%%][][A-Za-z0-9_%%]*[\']"};

char* IDENTIFIERS[] = {"^[A-Za-z][A-Za-z0-9_]*\\b"};

char* NUMERIC_LITERALS[] = {"^[0-9]+|[0-9]+\".\"[0-9]*|\".\"[0-9]*\\b"};

char *trimwhitespace(char *str)
{
  while(isspace((unsigned char)*str)) str++;
  return str;
}

void printPart(char *temp, regoff_t startIndex, regoff_t endIndex) {
	int i = startIndex;
	while(i < endIndex ) {
		printf("%c", temp[i]);
		i++;
	}
	printf("\n");
}

char *check(char *temp, char *pattern, char *printMessage) {
	if((temp == NULL) || (temp[0] == '\0')) return temp;
	temp = trimwhitespace(temp);
	size_t noOfMatches = 2;
	regmatch_t* matchPointers = (regmatch_t *)(malloc(noOfMatches * sizeof(regmatch_t)));

	int returnCode = regcomp(&regex, pattern, REG_EXTENDED | REG_ICASE);
	int matchCode = regexec(&regex, temp, noOfMatches, matchPointers, 0);
	if (matchCode == REG_NOMATCH)
		return temp;
	else{
		regoff_t startIndex = matchPointers[0].rm_so;
		regoff_t endIndex = matchPointers[0].rm_eo;
		printf("%s: ", printMessage);
		printPart(temp, startIndex, endIndex);
		temp = &temp[endIndex];
		
		hasMatched = 1;
		return temp;
	}

}

void lexify_line(char *temp) {
	char * temp2 = temp;
	while((temp != NULL) && (temp[0] != '\0')) {
		hasMatched = 0;
		int i=0;
		while((hasMatched == 0) && i<KEYWORDS_SIZE ){
			temp = check(temp, KEYWORDS[i], "KEYWORD");
			i++;
		}
		
		i=0;
		while((hasMatched == 0) && i<OPERATORS_SIZE){
			temp = check(temp, OPERATORS[i], "OPERATOR");
			i++;
		}
		i=0;
		while((hasMatched == 0) && i<LITERAL_SIZE){
			temp = check(temp, LITERALS[i], "LITERAL");
			i++;
		}
		i=0;
		while((hasMatched == 0) && i<IDENTIFIERS_SIZE){
			temp = check(temp, IDENTIFIERS[i], "IDENTIFIER");
			i++;
		}
		i=0;
		while((hasMatched == 0) && i<NUMERIC_LITERALS_SIZE){
			temp = check(temp, NUMERIC_LITERALS[i], "LITERAL");
			i++;
		}
	}

}

char *read_line(FILE *stream, int *line_size, int *read_status)
{
    char *line = NULL;
    int size = 0;
    *read_status = getline(&line, &size, stream);
    *line_size = size;
    return line;
}

void run_loop(FILE *stream)
{
    int read_status, line_size;
    char *line;
    do
    {
        line = read_line(stream, &line_size, &read_status);
        if (read_status == -1)
            return;
        if (line[0] == '\n')
            continue;
        lexify_line(line);
    }
    while (1);
}

int main(int argc, char **argv)
{
    if (argc == 1)
        run_loop(stdin);
    else if (argc == 2)
    {
        FILE *stream = fopen(argv[1], "r");
        if (stream)
            run_loop(stream);
    }
    else if (argc > 2)
        fprintf(stderr, "Too many arguments supplied!\n");
    return EXIT_SUCCESS;
}
