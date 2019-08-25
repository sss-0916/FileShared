#include "httplib.h"
#include <iostream>

using namespace httplib;

int main(){
	// 实例化一个客户端对象
	Client _cli("172.17.59.128", 9999);

	// 获取服务端响应
	auto res = _cli.Head("/hostmatching");

	// 判断响应状态码，如果为200，表示主机配对成功
	if(res->status == 200){
		std::cout << "host matching successful!" << std::endl;
	}
	else{
		std::cout << "host matching failed!" << std::endl;
	}

	return 0;
}
