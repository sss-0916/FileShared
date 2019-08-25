#include "httplib.h"
#include <ifaddrs.h>
#include <iostream>
#include <boost/algorithm/string.hpp>

using namespace httplib;

// LocalFileShared客户端
class LFSClient{
	public:
		// 客户端启动
		void Start(){
			while(1){
				// 主界面
				mainInterface();

				int choose = 0;
				std::cout << "请选择您要进行的操作: ";
				std::cin >> choose;

				switch(choose){
					case 1:
						getOnlineHost(); break;
					case 2:
						getFileList(); break;
					case 3:
						downloadFile(); break;
					default :
						return;
				}
			}
		}

	private:
		void mainInterface(){
			std::cout << "===============\n";
			std::cout << "1. 获取在线主机" << std::endl;
			std::cout << "2. 获取文件列表" << std::endl;
			std::cout << "3. 下载指定文件" << std::endl;
			std::cout << "others. 退出" << std::endl;
			std::cout << "===============\n";
		}

		// 获取局域网所有主机
		bool getLocalHost(){
			struct ifaddrs* addrs = nullptr;

			// 本机IP
			std::string ip;
			// 子网掩码
			std::string mask;

			// 获取本机网卡信息
			int ret = getifaddrs(&addrs);
			// 获取本机网卡信息失败
			if(ret < 0){
				return false;
			}

			while(addrs != nullptr){
				if(addrs->ifa_addr->sa_family == AF_INET){
					// 保存转换出的点分十进制
					ip.resize(INET_ADDRSTRLEN);
					mask.resize(INET_ADDRSTRLEN);

					// 获取IP地址
					// 32位二进制转换为点分十进制数
					inet_ntop(
						AF_INET, &((struct sockaddr_in*)(addrs->ifa_addr))->sin_addr,
						&ip[0], INET_ADDRSTRLEN
					);

					// 跳过回环网卡
					if(ip == "127.0.0.1"){
						addrs = addrs->ifa_next;
						continue;
					}

					// 获取子网掩码
					// 32为二进制转换为点分十进制数
					inet_ntop(
						AF_INET, &((struct sockaddr_in*)(addrs->ifa_netmask))->sin_addr,
						&mask[0], INET_ADDRSTRLEN
					);
				}

				addrs = addrs->ifa_next;
			}

			// 释放存储网卡信息的资源
			freeifaddrs(addrs);

			// 子网掩码
			uint32_t mask_n;
			inet_pton(AF_INET, &mask[0], &mask_n);
			uint32_t mask_h = ntohl(mask_n);

			// 主机数量
			uint32_t hostNum = ~mask_h;

			// IP地址
			uint32_t ip_n;
			inet_pton(AF_INET, &ip[0], &ip_n);
			uint32_t ip_h = ntohl(ip_n);

			// 网络号
			uint32_t networkNum_h = ip_h & mask_h;

			// 保存当前遍历到的主机
			std::string hosti;
			hosti.resize(INET_ADDRSTRLEN);

			// 遍历所有主机，将其保存到主机列表中
			for(uint32_t i = 2816; i <= 3071; ++i){
				// 计算局域网内主机
				uint32_t hosti_h = networkNum_h + i;
				// 转为网络字节序
				uint32_t hosti_n = htonl(hosti_h);

				// IP地址二进制形式转换为点分十进制
				inet_ntop(AF_INET, &hosti_n, &hosti[0], INET_ADDRSTRLEN);

				// 放入主机列表
				_host_list.push_back(hosti);
			}

			return true;
		}

		// 获取在线主机列表
		bool getOnlineHost(){
			// 获取局域网内所有主机
			getLocalHost();

			//for(size_t i = 0; i < _host_list.size(); ++i){
			for(size_t i = 128; i <= 128; ++i){
				// 实例化一个客户端
				Client _cli(_host_list[i].c_str(), 9999, 1);

				// 发送获取主机配对请求
				auto res = _cli.Head("/hostmatching");

				// 主机配对失败
				if(res == nullptr){
					continue;
				}

				// 响应成功，将其添加到在线主机列表
				if(res->status == 200){
					_online_list.push_back(_host_list[i]);
				}
			}

			// 打印在线主机
			std::cout << "The Online Host is Below: \n";
			for(size_t i = 0; i < _online_list.size(); ++i){
				std::cout << i << ". " << _online_list[i] << std::endl;
			}

			return true;
		}

		// 获取指定主机的共享文件列表
		bool getFileList(){
			// 选择指定主机
			std::cout << "请选择主机号: ";
			std::cin >> _host_idx;

			// 实例化客户端对象
			Client _cli(_online_list[_host_idx].c_str(), 9999);

			// 向服务器发送获取文件列表请求并接收服务端返回的响应
			auto res = _cli.Get("/filelist");

			// 响应失败
			if(res == nullptr){
				return false;
			}

			// 将响应正文中的文件名切分出来，保存到文件列表中
			boost::split(_file_list, res->body, boost::is_any_of("\n"));

			// 文件列表打印
			std::cout << "The FileList is Below: \n";
			for(size_t i = 0; i < _file_list.size(); ++i){
				std::cout << i << ". " << _file_list[i] << std::endl;
			}

			return true;
		}

		// 下载指定主机的指定文件
		bool downloadFile(){
			// 选择要下载的文件
			int choose = 0;
			std::cout << "请输入要下载文件编号: ";
			std::cin >> choose;

			// 实例化客户端对象
			Client _cli(_online_list[_host_idx].c_str(), 9999);

			// 请求路径
			std::string path = "/filelist/";
			path += _file_list[choose];

			// 向服务端发送请求并且接收服务端的响应
			auto res = _cli.Get(path.c_str());
			if(res == nullptr){
				return false;
			}

			std::cout << "test!\n";

			// 下载成功
			if(res->status == 200){
				// 获取正文长度
				std::string len = res->get_header_value("Content-Length");

				// 定义一个流，将字符串转为数字
				std::stringstream temp;
				temp << len;
				size_t fsize;
				temp >> fsize;

				// 下载路径
				std::string path = "./Download_Dir/";
				path += _file_list[choose];

				// 打开文件
				std::fstream fs(path.c_str(), std::ios::out | std::ios::binary);

				// 将响应正文写入文件
				fs.write(res->body.c_str(), fsize);

				fs.close();

				// 下载成功
				std::cout << "File " << _file_list[choose] << " Download Success!\n";
			}

			return true;
		}

	private:
		// 主机在主机列表中的下标
		int _host_idx;

		// 保存获取到的主机列表
		std::vector<std::string> _host_list;

		// 保存在线主机列表
		std::vector<std::string> _online_list;

		// 保存获取到的文件列表
		std::vector<std::string> _file_list;
};

int main(){
	// LFS客户端对象
	LFSClient _lfs;

	// 启动客户端
	_lfs.Start();

	return 0;
}
