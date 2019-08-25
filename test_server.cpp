#include "httplib.h"

using namespace httplib;

void HelloWorld(const Request& req, Response& res){
	res.status = 200;
	res.set_header("Content-Type", "text/html");
	res.body = "<h1>hello, world!</h1>";
}

int main(){

	Server _ser;
	_ser.Get("/", HelloWorld);
	_ser.listen("0.0.0.0", 9999);

	return 0;
}
