#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

int sum(char *s, int *i);

int nbr(char *s, int *i)
{
	int a = 0;
	if (isdigit(s[*i]))
	{
		a = s[*i] - '0';
		(*i)++;
		return (a);
	}
	while (s[*i] == '(')
	{
		(*i)++;
		a = sum(s, i);
		(*i)++;
	}
	return a;
}

int product(char *s, int *i)
{
	int a = nbr(s, i);
	while (s[*i] == '*')
	{
		(*i)++;
		a *= nbr(s, i);
	}
	return a;
}

int sum(char *s, int *i)
{
	int a = product(s, i);
	while (s[*i] == '+')
	{
		(*i)++;
		a += product(s, i);
	}
	return a;
}

int parse(char *s)
{
	if (*s == '+' || *s == '*' || *s == ')')
		return (printf("Unexpected token '%c'\n", *s), 0);
	int opened = 0;
	for (int i = 0; s[i]; i++)
	{
		if (isdigit(s[i]) && (isdigit(s[i + 1]) || s[i + 1] == '('))
			return (printf("Unexpected token '%c'\n", s[i + 1]), 0);
		if ((s[i] == '+' || s[i] == '*'))
		{
			if (!s[i + 1])
				return (printf("Unexpected end of input\n"), 0);
			if (s[i + 1] == '+' || s[i + 1] == '*' || s[i + 1] == ')')
				return (printf("Unexpected token '%c'\n", s[i + 1]), 0);
		}
		if (s[i] == '(')
		{
			opened++;
			if (!s[i + 1])
				return (printf("Unexpected end of input\n"), 0);
			if (s[i + 1] == '+' || s[i + 1] == '*' || s[i + 1] == ')')
				return (printf("Unexpected token '%c'\n", s[i + 1]), 0);
		}
		if (s[i] == ')')
		{
			opened--;
			if (opened < 0)
				return (printf("Unexpected token ')'\n"), 0);
			if (isdigit(s[i + 1]) || s[i + 1] == '(')
				return (printf("Unexpected token '%c'\n", s[i + 1]), 0);
		}
	}
	if (opened > 0)
		return (printf("Unexpected end of input\n"), 0);
	return (1);
}

int main(int argc, char **argv)
{
	if (argc != 2 || !parse(argv[1]))
		return 1;
	int i = 0;
	printf("%d\n", sum(argv[1], &i));
}
