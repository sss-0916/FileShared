#include "httplib.h"
#include <iostream>

using namespace httplib;

int main(){
	// 实例化客户端
	Client _cli("172.17.59.128", 9999);

	// 向服务器发送请求并获取相应
	auto res = _cli.Get("/filelist");

	// 获取失败
	if(res == nullptr){
		std::cout << "Get FileList Failed!\n";

		return -1;
	}

	// 获取文件列表成功
	if(res->status == 200){
		std::cout << "The FileList is Below: \n";

		// 打印文件列表
		std::cout << res->body;
	}

	return 0;
}
