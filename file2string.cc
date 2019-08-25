#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <sstream>

int main(){
	//const char* path = "./Shared_Dir/hello.txt";
	std::string path = "./Shared_Dir/hehe.cc";

	std::ifstream ifs(path.c_str());
	std::istreambuf_iterator<char> beg(ifs), end;
	std::string str(beg, end);

	std::cout << str << std::endl;

	//std::ifstream ifs(path.c_str(), std::ios::binary);

	//std::ostringstream buf;

	//ifs >> buf;

	//char ch;
	//while(buf && ifs.get(ch)){
	//		buf.put(ch);
	//}

	//std::string result = buf.str();

	//std::cout << result << std::endl;

	//std::string temp;
	//char temp[1001];

	//int fd = open(path, O_RDWR);

	//write(fd, &temp[0], 1000);
	//read(fd, temp, 1000);

	//std::cout << temp << std::endl;

	//close(fd);

	return 0;
}
