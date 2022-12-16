#ifndef __TEXTRESULT_H__
#define __TEXTRESULT_H__

#include "head.h"

class QueryResult;
class TextQuery
{
public:
	using line_no = std::vector<std::string>::size_type;

	TextQuery(std::ifstream &);
	QueryResult query(const std::string &) const;

private:
	/// @brief 输入文件
	std::shared_ptr<std::vector<std::string>> m_pFile;
	/// @brief 单词到所在行号集合的映射
	std::map<std::string, std::shared_ptr<std::set<line_no>>> m_wm;
};

class QueryResult
{
	using line_no = std::vector<std::string>::size_type;

	friend std::ostream &print(std::ostream &, const QueryResult &);

public:
	QueryResult(std::string s,
		    std::shared_ptr<std::set<line_no>> p,
		    std::shared_ptr<std::vector<std::string>> f) : m_strSoughtWord(s), m_pLines(p), m_pFile(f)
	{
	}
	std::set<line_no>::const_iterator Cbegin()
	{
		return m_pLines->cbegin();
	}
	std::set<line_no>::const_iterator Cend()
	{
		return m_pLines->cend();
	}
	std::shared_ptr<std::vector<std::string>> get_file()
	{
		return m_pFile;
	}

private:
	std::string m_strSoughtWord;			   // 查询单词
	std::shared_ptr<std::set<line_no>> m_pLines;	   // 出现的行号
	std::shared_ptr<std::vector<std::string>> m_pFile; // 输入文件
};

/// @brief 打印某单词的查询结果
std::ostream &print(std::ostream &os, const QueryResult &qr);

#endif