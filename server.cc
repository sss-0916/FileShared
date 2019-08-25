#include "httplib.h"
#include <boost/filesystem.hpp>

using namespace httplib;

// 取别名
namespace bf = boost::filesystem;

// 共享目录
#define SHARED_DIR "./Shared_Dir"

// LocalFileShared服务器
class LFServer{
	public:
		// 服务器启动
		void Start(uint16_t port){
			// 主机配对请求响应
			_ser.Get("/hostmatching", hostMatching);

			// 文件列表获取响应
			_ser.Get("/filelist", fileList);

			// 文件下载响应
			// (.*)正则表达式，匹配任意字符任意次
			_ser.Get("/filelist/(.*)", fileDownload);

			// 监听
			_ser.listen("0.0.0.0", port);
		}

	private:
		// 响应附近主机配对请求
		static void hostMatching(const Request& req, Response& res){
			// 配对成功
			res.status = 200;
		}

		// 响应附近主机获取文件列表请求
		static void fileList(const Request& req, Response& res){
			// 获取成功
			res.status = 200;

			// 指定共享目录
			bf::path share_path(SHARED_DIR);

			// 结束位置
			bf::directory_iterator end;

			// 迭代器开始位置
			bf::directory_iterator dit(share_path);

			// 设置数据流类型
			res.set_header("Content-Type", "text/html");

			while(dit != end){
				if(!bf::is_directory(*dit)){
					// 将遍历到的非目录文件放入正文
					std::string temp = dit->path().string();

					// 找到最后一个/所在下标
					size_t pos = temp.find_last_of('/');

					// 截取出文件名
					std::string result = temp.substr(pos + 1, std::string::npos);

					// 放入正文
					res.body += result + "\n";
				}

				++dit;
			}
		}

		// 响应附近主机文件下载请求
		static void fileDownload(const Request& req, Response& res){
			// 成功
			res.status = 200;

			// 二进制流，将文件下载下来
			res.set_header("Content-Type", "application/octet-stream");

			// 拿到请求路径
			std::string temp = req.path;

			// 切割出文件名
			size_t pos = temp.find_last_of('/');

			// 组合出完整路径
			std::string path = SHARED_DIR;
			path += '/' + temp.substr(pos + 1, std::string::npos);

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

	private:
		// 服务器对象
		Server _ser;
};

int main(){

	// LFS服务器对象
	LFServer _lfs;

	// 启动服务器
	_lfs.Start(9999);

	return 0;
}
