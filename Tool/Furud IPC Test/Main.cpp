#include <iostream>
#include <IPC.h>



void test()
{
	namespace IPC = Furud::IPC;
	constexpr IPC::GUID md5 = "hello world1111111111111111111111111111111111111111111111111111111111111111111111111111111";
	std::cout << md5 << '\n';
}


int main(int argc, char** argv) {
	test();
	TestIPC(argc, argv);
	return 0;
}
