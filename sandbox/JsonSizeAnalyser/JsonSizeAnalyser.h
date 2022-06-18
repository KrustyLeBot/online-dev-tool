#pragma once
#include <sstream>
#include "Json/json.hpp"

#include "imgui.h"

using json = nlohmann::json;

class JsonSizeAnalyser
{
public:
	JsonSizeAnalyser(int id)
	: m_id(id)
	{
		std::ostringstream os;
		os << "Json Size Analyser - " << m_id;
		m_name = os.str();

		for (const auto& c : m_jsonDefault)
		{
			m_str.push_back(c);
		}
		m_str.push_back('\0');
	}

	bool Draw();

private:
	size_t ParseObjectSize(json json, size_t parentSize, std::string padding = "");

private:
	bool m_isOpen = true;
	int m_id = 0;
	std::string m_name;
	json m_parsedJson;
	ImVector<char> m_str;

	std::string m_jsonDefault = "{\"glossary\":{\"title\":\"example glossary\",\"GlossDiv\":{\"title\":\"S\",\"GlossList\":{\"GlossEntry\":{\"ID\":\"SGML\",\"SortAs\":\"SGML\",\"GlossTerm\":\"Standard Generalized Markup Language\",\"Acronym\":\"SGML\",\"Abbrev\":\"ISO 8879:1986\",\"GlossDef\":{\"para\":\"A meta-markup language, used to create markup languages such as DocBook.\",\"GlossSeeAlso\":[\"GML\",\"XML\"]},\"GlossSee\":\"markup\"}}}}}";

	//<padding, field name, keybyte size, object byte size, percentage total, percentage parent object>
	size_t m_totalSize;
	float m_totalSizef;
	std::vector<std::tuple<std::string, std::string, size_t, size_t, float, float>> m_jsonSizeDisplayList;
};

