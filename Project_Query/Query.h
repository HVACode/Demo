#ifndef __QUERY_H__
#define __QUERY_H__
#include "TextResult.h"

class Query_base
{
	friend class Query;

protected:
	using line_no = TextQuery::line_no;
	virtual ~Query_base() = default;

private:
	/// @brief 返回与当前 Query 匹配的QueryResult
	virtual QueryResult eval(const TextQuery &) const = 0;
	/// @brief 表示查询的string
	virtual std::string rep() const = 0;
};

/// @brief 对外提供接口
class Query
{
	// 运算符需要访问私有的构造函数
	friend Query operator~(const Query &qr);
	friend Query operator|(const Query &lhs, const Query &rhs);
	friend Query operator&(const Query &lhs, const Query &rhs);

public:
	/// @brief 创建新的WordQuery
	Query(const std::string &s);
	// inline Query(const std::string &s)
	//     : m_q(new WordQuery(s))
	// {
	// }
	/// @brief 接口函数：调用Query_base 操作
	QueryResult eval(const TextQuery &t) const
	{
		return m_q->eval(t);
	}
	std::string rep() const
	{
		return m_q->rep();
		// 实际调用哪个版本的函数由 m_q 所指对象类型决定
	}

private:
	Query(std::shared_ptr<Query_base> query) : m_q(query)
	{
	}
	std::shared_ptr<Query_base> m_q;
};
Query operator~(const Query &qr);
Query operator&(const Query &lhs, const Query &rhs);
Query operator|(const Query &lhs, const Query &rhs);

std::ostream &operator<<(std::ostream &os, const Query &query);

/// @brief 查找string，在给定QueryResult对象上实际执行查询的唯一操作
class WordQuery : public Query_base
{
	/// @brief Query 使用WordQuery的构造
	friend class Query;
	WordQuery(const std::string &s) : m_queryWord(s) {}

	QueryResult eval(const TextQuery &t) const
	{
		return t.query(m_queryWord);
	}
	std::string rep() const { return m_queryWord; }
	std::string m_queryWord;
};

class NotQuery : public Query_base
{
	friend Query operator~(const Query &qr);
	NotQuery(const Query &q) : m_query(q) {}

	std::string rep() const;
	QueryResult eval(const TextQuery &) const;

	Query m_query;
};

class BinaryQuery : public Query_base
{
protected:
	BinaryQuery(const Query &lhs, const Query &rhs, std::string s) : m_lhs(lhs), m_rhs(rhs), m_operatorName(s)
	{
	}
	std::string rep() const;

	Query m_lhs, m_rhs;
	std::string m_operatorName;
};

class AndQuery : public BinaryQuery
{
	friend Query operator&(const Query &lhs, const Query &rhs);
	AndQuery(const Query &lhs, const Query &rhs) : BinaryQuery(lhs, rhs, "&")
	{
	}

	QueryResult eval(const TextQuery &) const;
};

class OrQuery : public BinaryQuery
{
	friend Query operator|(const Query &lhs, const Query &rhs);
	OrQuery(const Query &lhs, const Query &rhs) : BinaryQuery(lhs, rhs, "|")
	{
	}
	QueryResult eval(const TextQuery &) const;
};

#endif