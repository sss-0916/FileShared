#include <ifaddrs.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <vector>

int main(){
	struct ifaddrs* addrs = nullptr;

	// 本机IP
	std::string ip;
	// 子网掩码
	std::string mask;

	// 获取本机网卡信息
	getifaddrs(&addrs);

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
			// 32位二进制转换为点分十进制数
			inet_ntop(
				AF_INET, &((struct sockaddr_in*)(addrs->ifa_netmask))->sin_addr, 
				&mask[0], INET_ADDRSTRLEN
			);
		}

		addrs = addrs->ifa_next;
	}

	// 释放存储网卡信息的资源
	freeifaddrs(addrs);

	// 打印本机IP和子网掩码
	std::cout << "ip: " << ip << std::endl;
	std::cout << "mask: " << mask << std::endl;

	// 子网掩码
	uint32_t mask_n;
	inet_pton(AF_INET, &mask[0], (void*)&mask_n);
	uint32_t mask_h = ntohl(mask_n);

	// 主机数量
	uint32_t hostNum = ~mask_h;

	// IP地址
	uint32_t ip_n;
	inet_pton(AF_INET, &ip[0], (void*)&ip_n);
	uint32_t ip_h = ntohl(ip_n);

	// 网络号
	uint32_t networkNum_h = ip_h & mask_h;

	// 创建主机列表，保存局域网内所有主机
	std::vector<std::string> _host_list;


	// 保存当前遍历到的主机
	std::string hosti;
	hosti.resize(INET_ADDRSTRLEN);

	// 遍历所有主机，将其保存到主机列表中
	// 这里偷个懒，只遍历172.17.59.0 ~ 172.17.59.255
	//for(uint32_t i = 2816; i <= 3071; ++i){
	for(uint32_t i = 1; i <= hostNum; ++i){
		// 计算局域网内主机
		uint32_t hosti_h = networkNum_h + i;
		// 转为网络字节序
		uint32_t hosti_n = htonl(hosti_h);

		// IP地址二进制形式转换为点分十进制
		inet_ntop(AF_INET, &hosti_n, &hosti[0], INET_ADDRSTRLEN);

		// 放入主机列表
		_host_list.push_back(hosti);
	}

	// 打印主机列表中的所有IP地址
	int format = 0;
	for(size_t i = 0; i < _host_list.size(); ++i){
		std::cout << _host_list[i] << "\t";

		// 控制一行打印9个IP地址
		++format;
		if(format == 9){
			format = 0;
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;

	return 0;
}
