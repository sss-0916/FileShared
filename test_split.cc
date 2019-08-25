#include "boost/algorithm/string.hpp"
#include <iostream>
#include <vector>

int main(){
	// 存放切分出来的字符串
	std::vector<std::string> result;

	// 待切分的字符串
	std::string before = "hello.txt\nhehe.cc";

	// 切分字符串，以'\n'为分割符
	boost::split(result, before, boost::is_any_of("\n"));

	// 结果打印
	for(size_t i = 0; i < result.size(); ++i){
		std::cout << i << ". " << result[i] << std::endl;
	}

	return 0;
}
