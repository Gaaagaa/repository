
#include <thread>
#include <iostream>
#include <chrono>

class background_task
{
	// constructor/destructor
public:
	background_task(size_t st_task_id = 0)
        : m_st_task_id(st_task_id)
	{
		std::cout << std::this_thread::get_id() << " => constructor task : " << m_st_task_id << std::endl;
	}

	~background_task()
	{
		std::cout << std::this_thread::get_id() << " => destructor task : " << m_st_task_id << std::endl;
	}

    background_task(const background_task & xobject)
    {
        if (this != &xobject)
        {
            this->m_st_task_id = xobject.m_st_task_id + 1;
        }

        std::cout << std::this_thread::get_id() << " => copy constructor task : " << m_st_task_id << std::endl;
    }

    background_task & operator=(const background_task & xobject)
    {
        if (this != &xobject)
        {
            this->m_st_task_id = xobject.m_st_task_id + 1;
        }

        std::cout << std::this_thread::get_id() << " => operator = () task : " << this->m_st_task_id << std::endl;

        return *this;
    }

public:
	void operator()() const
	{
        size_t st_count = 0;
		while (1)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            if (++st_count > 10)
            {
                break;
            }

            std::cout << std::this_thread::get_id() << " => st_count: " << st_count << std::endl;
		}
	}

protected:
    size_t m_st_task_id;
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

