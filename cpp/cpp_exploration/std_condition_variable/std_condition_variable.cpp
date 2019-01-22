#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <deque>
#include <string>
#include <functional>
#include <condition_variable>

template <typename T>
class threadsafe_deque
{
public:
	void write(const T & input)
	{
		std::lock_guard<std::mutex> lock(m_deque_mutex);
		m_deque.push_front(input);
	}

	T read()
	{
		std::lock_guard<std::mutex> lock(m_deque_mutex);
		T output = m_deque.back();
		m_deque.pop_back();
		return output;
	}

	std::size_t size() const
	{
		return m_deque.size();
	}

protected:
	std::deque<T> m_deque;
	std::mutex m_deque_mutex;	
};

using output_deque_type = threadsafe_deque<std::string>;

void producer_thread(output_deque_type & queue, const bool & run)
{
	thread_local int iteration = 0;

	while (run)
	{
		std::cout << "writing iteration = " << iteration << std::endl;
		queue.write(std::to_string(iteration));

		++iteration;
	};
}

void consumer_thread(output_deque_type & queue, const bool & run)
{
	while (run)
	{
		if (queue.size() == 0) continue;
		std::cout << "current iteration = " << queue.read() << std::endl;
	};
}

int main()
{
	output_deque_type output_deque;

	bool producer_run = true;
	bool consumer_run = true;

	std::thread t_producer(producer_thread, std::ref(output_deque), std::cref(producer_run));
	std::thread t_consumer(consumer_thread, std::ref(output_deque), std::cref(consumer_run));

	std::this_thread::sleep_for(std::chrono::seconds(1));

	consumer_run = false;
	producer_run = false;

	t_consumer.join();
	t_producer.join();

	std::cout << "\nproducer and consumer threads stopped" << std::endl;

	std::getchar(); return 0;
}