#include "head.h"
/// @brief 输出文件内容，使用方式：./a.out filename
int main(int argc, char *argv[])
{
    std::ifstream ifs(argv[1]);
    std::string line;
    size_t i = 0;
    while (getline(ifs, line))
    {
        if (!line.empty())
        {
            ++i;
            std::cout << i << ": " << line << std::endl;
        }
    }
    return 0;
}
