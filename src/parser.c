#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "util.h"
#include "json/parser.h"

#define ERROR() (createJSONParserResult(PARSER_ERROR, createJSONValue(JSON_UNDEFINED, NULL), __LINE__))
#define SUCCESS(TYPE, VALUE, CONSUMED) (createJSONParserResult(PARSER_SUCCESS, createJSONValue(TYPE, VALUE), CONSUMED))

JSONParserResult *createJSONParserResult(JSONParserStatus status, JSONValue *value, size_t consumed) {
	JSONParserResult *r = (JSONParserResult *) malloc(sizeof(JSONParserResult));
	r->status = status;
	r->result = value;
	r->consumed = consumed;
	return r;
}

void freeJSONParserResult(JSONParserResult *r) {
	
}

JSONParserResult *parseJSONObject(char *str) {
	if (*str != '{') return ERROR();
	JSONString **keys = (JSONString **) malloc(0);
	JSONValue **values = (JSONValue **) malloc(0);
	size_t len = 0;
	JSONParserResult *parser_result;
	size_t i = 1;
	u8 comma = 0;
	while (isspace(str[i])) i++;
	while (str[i] && str[i] != '}') {
		while (isspace(str[i])) i++;
		parser_result = parseJSONString(str + i);
		if (parser_result->status == PARSER_ERROR) {
			return parser_result;
		}
		i += parser_result->consumed;
		keys = (JSONString **) realloc(keys, (len + 1) * sizeof(JSONString **));
		keys[len] = parser_result->result->value;
		while (isspace(str[i])) i++;
		if (str[i] != ':') {
			return ERROR();
		}
		i++;
		while (isspace(str[i])) i++;

		parser_result = parseJSONValue(str + i);
		if (parser_result->status == PARSER_ERROR) {
			return parser_result;
		}
		i += parser_result->consumed;
		values = (JSONValue **) realloc(values, (len + 1) * sizeof(JSONValue **));
		values[len] = parser_result->result;
		len++;

		while (isspace(str[i])) i++;
		if (str[i] != ',' && str[i] != '}') {
			return ERROR();
		}
		if (str[i] == ',') {
			i++;
			comma = 1;
		}
		while (isspace(str[i])) i++;
		if (comma && str[i] == '}') {
			return ERROR();
		}
		comma = 0;
	}
	if (str[i] == '}') {
		return SUCCESS(JSON_OBJECT, createJSONObject(keys, values, len), i + 1);
	} else {
		return ERROR();
	}
}

JSONParserResult *parseJSONArray(char *str) {
	if (*str != '[') return ERROR();
	JSONValue **values = (JSONValue **) malloc(0);
	size_t len = 0;
	JSONParserResult *parser_result;
	size_t i = 1;
	u8 comma = 0;
	while (isspace(str[i])) i++;
	while (str[i] && str[i] != ']') {
		while (isspace(str[i])) i++;
		parser_result = parseJSONValue(str + i);
		if (parser_result->status == PARSER_ERROR) {
			return parser_result;
		}
		i += parser_result->consumed;
		values = (JSONValue **) realloc(values, (len + 1) * sizeof(JSONValue **));
		values[len] = parser_result->result;
		len++;

		while (isspace(str[i])) i++;
		if (str[i] != ',' && str[i] != ']') {
			return ERROR();
		}
		if (str[i] == ',') {
			i++;
			comma = 1;
		}
		while (isspace(str[i])) i++;
		if (comma && str[i] == ']') {
			return ERROR();
		}
		comma = 0;
	}
	if (str[i] == ']') {
		return SUCCESS(JSON_ARRAY, createJSONArray(values, len), i + 1);
	} else {
		return ERROR();
	}
}

JSONParserResult *parseJSONNumber(char *str) {
	if (!isdigit(*str)) return ERROR();
	size_t i = 0;
	int digit;
	double number = 0;
	int decimal_place = 0;
	while (isdigit(str[i]) || str[i] == '.') {
		if (str[i] == '.') {
			if (!i) {
				return ERROR();
			}
			decimal_place = 1;
		} else {
			digit = str[i] - '0';
			if (decimal_place) {
				number += digit * pow(10, -1 * decimal_place);
				decimal_place++;
			} else {
				number *= 10;
				number += digit;
			}
		}
		i++;
	}
	if (decimal_place == 1) {
		return ERROR();
	} else {
		return SUCCESS(JSON_NUMBER, createJSONNumber(number, MAX(decimal_place - 1, 0)), i);
	}
}

JSONParserResult *parseJSONString(char *str) {
	if (*str != '"') return ERROR();
	size_t i = 1;
	while (str[i] && (str[i] != '"' || str[i - 1] == '\\')) {
		i++;
	}
	if (str[i] != '"') {
		return ERROR();
	} else {
		return SUCCESS(JSON_STRING, createJSONString(str + 1, i - 1), i + 1);
	}
}

JSONParserResult *parseJSONBoolean(char *str) {
	return strncmp(str, "true", 4)
		? strncmp(str, "false", 5)
			? ERROR()
			: SUCCESS(JSON_BOOLEAN, createJSONBoolean(0), 5)
		: SUCCESS(JSON_BOOLEAN, createJSONBoolean(1), 4);
}

JSONParserResult *parseJSONNull(char *str) {
	return strncmp(str, "null", 4)
		? ERROR()
		: SUCCESS(JSON_NULL, createJSONNull(), 4);
}

JSONParserResult *parseJSONValue(char *str) {
	JSONParserResult *(*parsers[PARSER_COUNT])(char *) = {
		parseJSONNull,
		parseJSONBoolean,
		parseJSONNumber,
		parseJSONString,
		parseJSONArray,
		parseJSONObject
	};
	JSONParserResult *parser_result;
	int i;
	for (i = 0; i < PARSER_COUNT; i++) {
		parser_result = parsers[i](str);
		if (parser_result->status == PARSER_SUCCESS) {
			return parser_result;
		}
	}
	return parser_result;
}
