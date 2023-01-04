#pragma once

#include "Json.h"
#include "JsonValue.h"
#include <stdexcept>
#include <assert.h>
class JsonParser
{
public:
	const std::string str;
	size_t i;
	bool failed;
	JsonParser(const std::string& in) :str(in) { i = 0; failed = false; }
	void fail(const std::string& msg)
	{
		failed = true;
		throw std::logic_error(msg);
	}
	void consume_whitespace()
	{
		while (str[i] == ' ' or str[i] == '\r' or str[i] == '\n' or str[i] == '\t')
			i++;
	}

	char get_next_token()
	{
		consume_whitespace();
		if (failed) return 0;
		if (i == str.size())
		{
			fail("unexpected end of input");
			return 0;
		}
		return str[i++];
	}

	Json parse_number()
	{
		size_t start_pos = i;
		if (str[i] == '-')
			i++;

		while (in_range(str[i], '0', '9'))
			i++;

		if (str[i] != '.' and str[i] != 'e' and str[i] != 'E')
			return std::atoi(str.c_str() + start_pos);

		if (str[i] == '.')
		{
			i++;
			while (in_range(str[i], '0', '9'))
				i++;
		}
		if (str[i] == 'e' or str[i] == 'E')
		{
			i++;
			if (str[i] == '+' or str[i] == '-')
				i++;
			while (in_range(str[i], '0', '9'))
				i++;
		}
		return std::strtod(str.c_str() + start_pos, nullptr);
	}

	std::string parse_string()
	{
		std::string out;
		while (true)
		{
			if (i == str.size())
			{
				fail("unexpected end of input in string");
				return "";
			}
			char ch = str[i++];

			if (ch == '"')
				return out;
			if (ch != '\\')
			{
				out += ch;
				continue;
			}
			if (i == str.size())
			{
				fail("unexpected end of input in string");
				return "";
			}
			ch = str[i++];
			if (ch == 'b') {
				out += '\b';
			}
			else if (ch == 'f') {
				out += '\f';
			}
			else if (ch == 'n') {
				out += '\n';
			}
			else if (ch == 'r') {
				out += '\r';
			}
			else if (ch == 't') {
				out += '\t';
			}
			else if (ch == '"' || ch == '\\' || ch == '/') {
				out += ch;
			}
		}
	}

	Json expect(const std::string& expected, Json res) {
		assert(i != 0);
		i--;
		if (str.compare(i, expected.length(), expected) == 0) {
			i += expected.length();
			return res;
		}
		else {
			fail("parse error: expected " + expected + ", got " + str.substr(i, expected.length()));
			return Json();
		}
	}
	Json parse_json()
	{
		char ch = get_next_token();
		if (failed)
			return Json();

		if (ch == '-' || (ch >= '0' && ch <= '9')) {
			i--;
			return parse_number();
		}

		if (ch == 't')
			return expect("true", true);

		if (ch == 'f')
			return expect("false", false);

		if (ch == 'n')
			return expect("null", Json());

		if (ch == '"')
			return parse_string();

		if (ch == '{') {
			std::unordered_map<std::string, Json> data;
			ch = get_next_token();
			if (ch == '}')
				return data;

			while (1) {
				if (ch != '"')
				{
					fail("expected '\"' in object");
					return Json();
				}

				std::string key = parse_string();
				if (failed)
					return Json();

				ch = get_next_token();
				if (ch != ':')
				{
					fail("expected ':' in object");
					return Json();
				}

				data[std::move(key)] = parse_json();
				if (failed)
					return Json();

				ch = get_next_token();
				if (ch == '}')
					break;
				if (ch != ',')
				{
					fail("expected ',' in object ");
					return Json();
				}
				ch = get_next_token();
			}
			return data;
		}

		if (ch == '[') {
			std::vector<Json> data;
			ch = get_next_token();
			if (ch == ']')
				return data;

			while (1) {
				i--;
				data.push_back(parse_json());
				if (failed)
					return Json();

				ch = get_next_token();
				if (ch == ']')
					break;
				if (ch != ',')
				{
					fail("expected ',' in list");
					return Json();
				}

				ch = get_next_token();
				(void)ch;
			}
			return data;
		}

		fail("expected value");
		return Json();

	}

};

