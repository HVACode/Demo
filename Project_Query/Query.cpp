#include "Query.h"

Query::Query(const std::string &s)
    : m_q(new WordQuery(s))
{
}

std::ostream &
operator<<(std::ostream &os, const Query &query)
{
	return os << query.rep();
}

std::string NotQuery::rep() const
{
	return "~(" + m_query.rep() + ")";
}

Query operator~(const Query &qr)
{
	return std::shared_ptr<Query_base>(new NotQuery(qr));
}

Query operator&(const Query &lhs, const Query &rhs)
{
	return std::shared_ptr<Query_base>(new AndQuery(lhs, rhs));
}
Query operator|(const Query &lhs, const Query &rhs)
{
	return std::shared_ptr<Query_base>(new OrQuery(lhs, rhs));
	// return 语句负责将 shared_ptr 转换为 Query
}
std::string BinaryQuery::rep() const
{
	return "(" + m_lhs.rep() + " " + m_operatorName + " " + m_rhs.rep() + ")";
}

QueryResult OrQuery::eval(const TextQuery &text) const
{
	// Query类型的成员进行虚调用
	auto right = m_rhs.eval(text), left = m_lhs.eval(text);
	// 左侧对象的行号
	auto ret_lines = std::make_shared<std::set<line_no>>(right.Cbegin(), right.Cend());
	// 右侧对象的行号
	ret_lines->insert(left.Cbegin(), left.Cend());

	return QueryResult(rep(), ret_lines, left.get_file()); // left right 指向同一个文件
}
QueryResult AndQuery::eval(const TextQuery &text) const
{
	auto right = m_rhs.eval(text), left = m_lhs.eval(text);
	auto ret_lines = std::make_shared<std::set<line_no>>();
	// 合并
	std::set_intersection(left.Cbegin(), left.Cend(), right.Cbegin(), right.Cend(), std::inserter((*ret_lines), ret_lines->begin()));
	return QueryResult(rep(), ret_lines, left.get_file());
}

QueryResult NotQuery::eval(const TextQuery &text) const
{
	auto result = m_query.eval(text);
	auto ret_lines = std::make_shared<std::set<line_no>>();
	auto beg = result.Cbegin(), end = result.Cend();
	auto sz = result.get_file()->size(); // 文件行数
	for (std::size_t n = 0; n != sz; ++n)
	{
		if (beg == end || *beg != n)
		{
			ret_lines->insert(n);
		}
		else if (beg != end)
		{
			++beg;
		}
	}
	return QueryResult(rep(), ret_lines, result.get_file());
}