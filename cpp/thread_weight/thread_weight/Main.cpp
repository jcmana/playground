#include <thread>
#include <chrono>

void thread_impl()
{
	std::this_thread::sleep_for(std::chrono::seconds(5));
}

int main()
{
	std::thread thread_1(thread_impl);
	std::thread thread_2(thread_impl);
	std::thread thread_3(thread_impl);

	thread_1.join();
	thread_2.join();
	thread_3.join();

	return 0;
}
