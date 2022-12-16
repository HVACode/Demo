#include "TextResult.h"

TextQuery::TextQuery(std::ifstream &is) : m_pFile(new std::vector<std::string>)
{
	std::string text;
	while (std::getline(is, text))
	{
		m_pFile->push_back(text);

		int rowIdx = m_pFile->size() - 1; // 当前行号
		std::istringstream line(text);
		std::string word;
		while (line >> word)
		{
			auto &lines = m_wm[word]; // lines 是一个 shared_ptr，引用，所以改变也会改变 wm 元素
			if (!lines)		  // 第一次遇到单词，指针为空
			{
				lines.reset(new std::set<line_no>);
			}
			lines->insert(rowIdx);
		}
	}
}

QueryResult TextQuery::query(const std::string &sought) const
{
	static std::shared_ptr<std::set<line_no>> nodata(new std::set<line_no>);
	auto loc = m_wm.find(sought);
	if (loc == m_wm.end())
	{
		return QueryResult(sought, nodata, m_pFile);
	}
	else
	{
		return QueryResult(sought, loc->second, m_pFile);
	}
}

std::ostream &print(std::ostream &os, const QueryResult &qr)
{
	os << qr.m_strSoughtWord << " occurs " << qr.m_pLines->size() << " times" << std::endl;

	for (auto num : *(qr.m_pLines))
	{
		os << "\t(line " << num + 1 << ") "
		   << *(qr.m_pFile->begin() + num) << std::endl;
	}
	return os;
}