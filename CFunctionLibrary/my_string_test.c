#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int call_str_functions(int argc, char const *argv[])
{
    const char *s, *l;
	char *p;
	int i;

	s = "abc bc cd ac";
	l= "bc";
    p = strstr(s, l);
	printf("%s\n", p);

	l = "xyzc";
	p = strpbrk(s, l);
	printf("%s\n", p);

	p = strchr(s, 'd');
	printf("%s %d\n", p, p - s);

	s = "0123456789";
	l = "01245";
	i = strspn(s, l);
	printf("i = %d\n", i);

	l = "45";
	i = strcspn(s, l);
	printf("i = %d\n", i);

	p = strdup(s);
	printf("p = %s\n", p);
	free(p);

    return 0;
}

int call_strtok_r(int argc, char *argv[])
{
	char *str1, *str2, *token, *subtoken;
	char *saveptr1, *saveptr2;
	int j;

	if (argc != 4) {
		fprintf(stderr, "Usage: %s string delim subdelim\n",
				argv[0]);
		exit(EXIT_FAILURE);
	}

	for (j = 1, str1 = argv[1]; ; j++, str1 = NULL)
	{
		token = strtok_r(str1, argv[2], &saveptr1);
		if (token == NULL)
			break;
		printf("%d: %s\n", j, token);

		for (str2 = token; ; str2 = NULL)
		{
			subtoken = strtok_r(str2, argv[3], &saveptr2);
			if (subtoken == NULL)
				break;
			printf(" --> %s\n", subtoken);
		}
	}

	exit(EXIT_SUCCESS);
}
