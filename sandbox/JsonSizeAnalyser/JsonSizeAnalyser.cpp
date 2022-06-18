#include "JsonSizeAnalyser.h"
#include "CustomImgui.h"

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
		m_jsonSizeDisplayList.clear();

		std::vector<std::uint8_t> compact = json::to_cbor(m_parsedJson);
		m_totalSize = compact.size();
		m_totalSizef = static_cast<float>(m_totalSize);

		ParseObjectSize(m_parsedJson, m_totalSize);
		std::reverse(m_jsonSizeDisplayList.begin(), m_jsonSizeDisplayList.end());
	}

	for (const auto& object : m_jsonSizeDisplayList)
	{
		std::ostringstream outLeft;
		outLeft << std::get<0>(object) << "[" << std::get<3>(object) << "B" << "(" << std::get<2>(object) << " key B)" << "]";

		//Text is clipped by the front, so add a lot of padding so that the text is alligned always at the right side
		std::ostringstream outRight;
		outRight << "                                                                                                                                                                                                 ";
		outRight << std::get<1>(object) << "  " << std::get<4>(object) * 100 << "%%";

		
		ImGui::Text(outLeft.str().c_str());
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(ImColor(154, 38, 23)));
		ImGui::ProgressBar(std::get<4>(object), ImVec2(-1.0f, -0.0f), outRight.str().c_str());
		ImGui::PopStyleColor();
	}

	ImGui::End();

	return true;
}

size_t JsonSizeAnalyser::ParseObjectSize(json json, size_t parentSize, std::string padding /*= ""*/)
{
	if (json.is_null())
	{
		return 0;
	}

	std::ostringstream newPadding;
	newPadding << padding << "	";

	// if json is object
	if (json.is_object())
	{
		size_t sumSize = 0;

		for (auto& el : json.items())
		{
			std::vector<std::uint8_t> compact = json::to_cbor(el);
			size_t size = compact.size();
			sumSize += size;
		}

		for (auto& el : json.items())
		{
			std::ostringstream out;
			out << el.key();

			std::vector<std::uint8_t> compact = json::to_cbor(el);
			size_t size = compact.size();

			size_t childSize = ParseObjectSize(el.value(), size, newPadding.str());

			// if value is an item (string, int, bool etc...), directly dump its value in the current field
			if (!el.value().is_array() && !el.value().is_null() && !el.value().is_object())
			{
				out << " : " << el.value().dump();
			}

			m_jsonSizeDisplayList.push_back(std::make_tuple(newPadding.str(), out.str(), size - childSize, size, (static_cast<float>(size) / m_totalSizef), (static_cast<float>(size) / sumSize)));
		}
		return sumSize;
	}

	// if json is array
	if (json.is_array())
	{
		size_t sumSize = 0;

		for (auto& el : json.items())
		{
			std::vector<std::uint8_t> compact = json::to_cbor(el);
			size_t size = compact.size();
			sumSize += size;

			ParseObjectSize(el.value(), size, newPadding.str());
		}
		return sumSize;
	}

	//json is value
	{
		std::vector<std::uint8_t> compact = json::to_cbor(json);
		size_t size = compact.size();
		
		return size;
	}
}