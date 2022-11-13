#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "json/values.h"

JSONValue *createJSONValue(JSONType type, void *value) {
	JSONValue *v = (JSONValue *) malloc(sizeof(JSONValue));
	v->type = type;
	v->value = value;
	return v;
}

JSONObject *createJSONObject(JSONString **keys, JSONValue **values, size_t len) {
	JSONObject *o = (JSONObject *) malloc(sizeof(JSONObject));
	o->keys = keys;
	o->values = values;
	o->len = len;
	return o;
}

JSONArray *createJSONArray(JSONValue **values, size_t len) {
	JSONArray *a = (JSONArray *) malloc(sizeof(JSONArray));
	a->values = values;
	a->len = len;
	return a;
}

JSONNumber *createJSONNumber(double number, u8 decimal_place) {
	JSONNumber *n = (JSONNumber *) malloc(sizeof(JSONNumber));
	n->number = number;
	n->decimal_place = decimal_place;
	return n;
}

JSONString *createJSONString(char *chars, size_t len) {
	JSONString *s = (JSONString *) malloc(sizeof(JSONString));
	s->chars = strncpy((char *) malloc((len + 1) * sizeof(char)), chars, len);
	s->chars[len] = 0;
	s->len = len;
	return s;
}

JSONBoolean *createJSONBoolean(u8 boolean) {
	JSONBoolean *b = (JSONBoolean *) malloc(sizeof(JSONBoolean));
	b->boolean = boolean;
	return b;
}

JSONNull *createJSONNull() {
	return NULL;
}

void printJSONValue(JSONValue *jsonValue, FILE *stream) {
	JSONType type = jsonValue->type;
	void *value = jsonValue->value;
	switch (type) {
	case JSON_OBJECT:
		printJSONObject(value, stream);
		break;
	case JSON_ARRAY:
		printJSONArray(value, stream);
		break;
	case JSON_NUMBER:
		printJSONNumber(value, stream);
		break;
	case JSON_STRING:
		printJSONString(value, stream);
		break;
	case JSON_BOOLEAN:
		printJSONBoolean(value, stream);
		break;
	case JSON_NULL:
		fprintf(stream, "null");
		break;
	case JSON_UNDEFINED:
	default:
		fprintf(stream, "undefined");
		break;
	}
}

void printJSONObject(JSONObject *jsonObject, FILE *stream) {
	size_t i;
	fprintf(stream, "{");
	for (i = 0; i + 1 < jsonObject->len; i++) {
		printJSONString(jsonObject->keys[i], stream);
		printf(": ");
		printJSONValue(jsonObject->values[i], stream);
		fprintf(stream, ", ");
	}
	if (i < jsonObject->len) {
		printJSONString(jsonObject->keys[i], stream);
		printf(": ");
		printJSONValue(jsonObject->values[i], stream);
	}
	fprintf(stream, "}");
}

void printJSONArray(JSONArray *jsonArray, FILE *stream) {
	size_t i;
	fprintf(stream, "[");
	for (i = 0; i + 1 < jsonArray->len; i++) {
		printJSONValue(jsonArray->values[i], stream);
		fprintf(stream, ", ");
	}
	if (i < jsonArray->len) {
		printJSONValue(jsonArray->values[i], stream);
	}
	fprintf(stream, "]");
}

void printJSONNumber(JSONNumber *jsonNumber, FILE *stream) {
	fprintf(stream, "%.*f", jsonNumber->decimal_place, jsonNumber->number);
}

void printJSONString(JSONString *jsonString, FILE *stream) {
	fprintf(stream, "\"%*s\"", (int) jsonString->len, jsonString->chars);
}

void printJSONBoolean(JSONBoolean *jsonBoolean, FILE *stream) {
	fprintf(stream, jsonBoolean->boolean ? "true" : "false");
}

void printJSONNull(JSONNull *jsonNull, FILE *stream) {
	fprintf(stream, "null");
}
