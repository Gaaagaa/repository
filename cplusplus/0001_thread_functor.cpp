
#include <thread>
#include <iostream>
#include <chrono>

class background_task
{
	// constructor/destructor
public:
	background_task(size_t st_count = 0)
        : m_st_count(st_count)
	{
		std::cout << std::this_thread::get_id() << " => constructor task" << std::endl;
	}

	~background_task()
	{
		std::cout << std::this_thread::get_id() << " => destructor task" << std::endl;
	}

public:
	void operator()() const
	{
        size_t st_count = 0;
		while (1)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            if (++st_count > 60)
            {
                break;
            }

            std::cout << std::this_thread::get_id() << " => st_count: " << st_count << std::endl;
		}
	}

protected:
    size_t m_st_count;
};

int main(int argc, char * argv[])
{
    std::thread * xthread_ptr = nullptr;

    do 
    {
        background_task xtask(1024);
        xthread_ptr = new std::thread(xtask);
    } while (0);

    if (nullptr != xthread_ptr)
    {
        if (xthread_ptr->joinable())
            xthread_ptr->join();
        delete xthread_ptr;
        xthread_ptr = nullptr;
    }

	return 0;
}

