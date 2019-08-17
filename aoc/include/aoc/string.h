#ifndef AOC_STRING_H
#define AOC_STRING_H

#include <aoc/utility.h>

typedef struct string {
	char* start;
	char* end;
} string;

size_t string_length(string s);

string string_copy(string s);
char* string_to_cstring(string s);
string string_of_cstring(char const* s);

void string_free(string s);

struct string_split {
	string first;
	string rest;
};

struct string_split string_split_at(string s, char ch);

// returns whether to continue
bool _Aoc_string_lines_iterator_next(string* current, string* rest);

#ifdef _MSC_VER
int str_scanf(
		string s, _Scanf_format_string_params_(0) char const* fmt, ...);
#else
int str_scanf(string s, char const* fmt, ...)
		__attribute__((format(scanf, 2, 3)));
#endif

#define S(...) string_of_cstring(__VA_ARGS__)
#define string_format_arg(s) (int)(string_length(s)), (s).start
#define string_length(s) ((size_t)((s).end - (s).start))
#define string_for_each(name, self)                                    \
	slice_for_each(char, name, s.start, s.end)
#define string_lines_for_each(name, data)                              \
	for (string name = {0}, __##name##_rest = (data);                    \
			 _Aoc_string_lines_iterator_next(&name, &(__##name##_rest));)

#endif // AOC_STRING_H
