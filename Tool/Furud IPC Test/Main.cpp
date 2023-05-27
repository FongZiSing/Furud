#include <iostream>
#include <IPC.h>



void test()
{
	namespace IPC = Furud::IPC;
	constexpr IPC::GUID md5 = "hello world";
	std::cout << md5 << '\n';
}


int main() {
	test();
	return 0;
}
