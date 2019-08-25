#include "httplib.h"
#include <fstream>
#include <sstream>
#include <boost/filesystem.hpp>
#include <iostream>

using namespace httplib;

// 取别名
namespace bf = boost::filesystem;

// 文件下载
void Download(const Request& req, Response& res){
	// 成功
	res.status = 200;

	// 二进制流，将文件下载下来
	res.set_header("Content-Type", "application/octet-stream");

	// 拿到请求路径
	std::string temp = req.path;

	// 切割出文件名
	size_t pos = temp.find_last_of('/');

	// 组合出一个完成路径
	std::string path = "./Shared_Dir/";
	path += temp.substr(pos + 1, std::string::npos);

	// 获取文件大小
	size_t fsize = bf::file_size(path);

	// 打开文件
	std::fstream fs(path.c_str(), std::ios::binary | std::ios::in);
	// 文件未打开
	if(!fs.is_open()){
		return;
	}

	// 设置body的空间大小
	res.body.resize(fsize);

	// 将文件内容读取到响应正文
	fs.read(&res.body[0], fsize);

	// 关闭文件
	fs.close();
}

int main(){

	// 服务器对象
	Server _ser;

	// 响应文件下载
	_ser.Get("/filelist/(.*)", Download);

	// 监听
	_ser.listen("0.0.0.0", 9999);

	return 0;
}
