#include "httplib.h"
#include <fstream>
#include <iostream>

using namespace httplib;

// 下载文件存放路径
#define DownLoad_Dir "./Download_Dir"

int main(){
	// 实例化客户端对象
	Client _cli("172.17.59.128", 9999);

	// 向服务端发送请求，并获取服务端响应
	auto res = _cli.Get("/filelist/hehe.cc");

	// 获取正文长度
	std::string len = res->get_header_value("Content-Length");

	// 定义一个流，将字符串转为数字
	std::stringstream temp;
	temp << len;
	size_t fsize;
	temp >> fsize;

	// 获取响应失败
	if(res == nullptr){
		return -1;
	}

	// 下载成功
	if(res->status == 200){
		// 打开文件
		std::fstream fs("./Download_Dir/hehe.cc", std::ios::out | std::ios::binary);

		// 将响应正文写入文件中
		fs.write(res->body.c_str(), fsize);

		fs.close();
	}

	return 0;
}
