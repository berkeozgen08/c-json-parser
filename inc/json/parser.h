#ifndef _PARSER_H
#define _PARSER_H

#include "json/values.h"

#define PARSER_COUNT 6

typedef enum JSONParserStatus {
	PARSER_SUCCESS,
	/* PARSER_SKIPPED, */
	PARSER_ERROR
} JSONParserStatus;

/* typedef struct JSONParserError {
	int line;
	int column;
} JSONParserError; */

typedef struct JSONParserResult {
	JSONParserStatus status;
	/* JSONParserError error; */
	JSONValue *result;
	size_t consumed;
} JSONParserResult;

JSONParserResult *createJSONParserResult(JSONParserStatus status, JSONValue *value, size_t consumed);
void freeJSONParserResult(JSONParserResult *r);

JSONParserResult *parseJSONObject(char *str);
JSONParserResult *parseJSONArray(char *str);
JSONParserResult *parseJSONNumber(char *str);
JSONParserResult *parseJSONString(char *str);
JSONParserResult *parseJSONBoolean(char *str);
JSONParserResult *parseJSONNull(char *str);
JSONParserResult *parseJSONValue(char *str);

#endif
