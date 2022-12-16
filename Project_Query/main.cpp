#include "Query.h"
/// @brief 查询 argv[1] 文件名 argv[2]单词 argv[3]（可能）符号 argv[4] 单词
int main(int argc, char *argv[])
{
	std::ifstream ifs(argv[1]);
	TextQuery text(ifs);

	if (argc == 2)
	{
		Query q = Query(argv[2]);
		QueryResult res = q.eval(text);
		q.rep();
		print(std::cout, res);
	}
	else if (argc == 3)
	{
		Query q = ~Query(argv[2]);
		QueryResult res = q.eval(text);
		q.rep();
		print(std::cout, res);
	}
	else
	{
		if (argv[3] == "&")
		{
			Query binaryQ = Query(argv[2]) & Query(argv[4]);
			QueryResult resx = binaryQ.eval(text);
			binaryQ.rep();
			print(std::cout, resx);
		}
		else if (argv[3] == "|")
		{
			Query binaryQ = Query(argv[2]) | Query(argv[4]);
			QueryResult resx = binaryQ.eval(text);
			binaryQ.rep();
			print(std::cout, resx);
		}
	}

	return 0;
}