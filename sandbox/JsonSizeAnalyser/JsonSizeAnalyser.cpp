#include "JsonSizeAnalyser.h"
#include "CustomImgui.h"

void ShowObject(json json, std::string padding = "");

bool JsonSizeAnalyser::Draw()
{
	if (!ImGui::Begin(m_name.c_str(), &m_isOpen))
	{
		ImGui::End();
		return false;
	}

	if (!m_isOpen)
	{
		ImGui::End();
		return false;
	}

	ImGui::Text("Json input");

	MyInputTextMultiline("##MyStr", &m_str, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16));

	if (ImGui::Button("Parse"))
	{
		m_parsedJson = json::parse(m_str);
	}

	//Parse every key and display the object
	ShowObject(m_parsedJson);

	ImGui::End();

	return true;
}

void ShowObject(json json, std::string padding /*= ""*/)
{
	if (json.is_null())
	{
		return;
	}

	std::ostringstream newPadding;
	newPadding << padding << "	";

	// if json is object
	if (json.is_object())
	{
		for (auto& el : json.items())
		{
			std::ostringstream out;
			out << newPadding.str() << "-" << el.key();

			std::vector<std::uint8_t> v_bson = json::to_bson(el);
			out << "     " << v_bson.size() << "Bytes";

			ImGui::Text(out.str().c_str());
			ShowObject(el.value(), newPadding.str());
		}
		return;
	}

	// if json is array
	if (json.is_array())
	{
		std::ostringstream out;
		out << newPadding.str() << "-" << json.dump();

		for (auto& el : json.items())
		{
			ShowObject(el.value(), newPadding.str());
		}
		return;
	}

	//json is value
	{
		std::ostringstream out;
		out << newPadding.str() << "-" << json.dump();

		ImGui::Text(out.str().c_str());
	}
}