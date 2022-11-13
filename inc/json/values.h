#ifndef _VALUES_H
#define _VALUES_H

#include <stdio.h>

#include "util.h"

typedef enum JSONType {
	JSON_OBJECT,
	JSON_ARRAY,
	JSON_NUMBER,
	JSON_STRING,
	JSON_BOOLEAN,
	JSON_NULL,
	JSON_UNDEFINED
} JSONType;

typedef struct JSONValue {
	JSONType type;
	void *value;
} JSONValue;

typedef void JSONNull;

typedef struct JSONBoolean {
	u8 boolean;
} JSONBoolean;

typedef struct JSONString {
	char *chars;
	size_t len;
} JSONString;

typedef struct JSONNumber {
	double number;
	u8 decimal_place;
} JSONNumber;

typedef struct JSONArray {
	JSONValue **values;
	size_t len;
} JSONArray;

typedef struct JSONObject {
	JSONString **keys;
	JSONValue **values;
	size_t len;
} JSONObject;

JSONValue *createJSONValue(JSONType type, void *value);
JSONObject *createJSONObject(JSONString **keys, JSONValue **values, size_t len);
JSONArray *createJSONArray(JSONValue **values, size_t len);
JSONNumber *createJSONNumber(double number, u8 decimal_place);
JSONString *createJSONString(char *chars, size_t len);
JSONBoolean *createJSONBoolean(u8 boolean);
JSONNull *createJSONNull();

void printJSONValue(JSONValue *jsonValue, FILE *stream);
void printJSONObject(JSONObject *jsonObject, FILE *stream);
void printJSONArray(JSONArray *jsonArray, FILE *stream);
void printJSONNumber(JSONNumber *jsonNumber, FILE *stream);
void printJSONString(JSONString *jsonString, FILE *stream);
void printJSONBoolean(JSONBoolean *jsonBoolean, FILE *stream);

#endif
