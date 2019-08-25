#include <iostream>
#include <string>
#include <boost/filesystem.hpp>

namespace bf = boost::filesystem;

int main(){
	// 要遍历的路径
	std::string targetPath = "./Shared_Dir";

	// 路径
	bf::path myPath(targetPath);

	// 结束位置
	bf::directory_iterator end;

	for(bf::directory_iterator it(myPath); it != end; ++it){
		// 不是目录文件，打印
		if(!bf::is_directory(*it)){
			std::cout << it->path().string() << std::endl;
		}
	}

	return 0;
}
