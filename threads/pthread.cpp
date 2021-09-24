#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <random>

using namespace std;


/*  전역변수 */

// 뮤텍스 선언
mutex values_mtx;
mutex cout_mtx;

// 벡터 선언
vector<int> values;



// 랜덤숫자 함수 선언
int randGen(const int& min, const int& max) {
    static thread_local mt19937 generator(hash<thread::id>()(this_thread::get_id()));
    uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}



// 스레드에 사용될 함수 선언
void threadFnc(int tid) {
	// Calculate the result.
	cout_mtx.lock();
	cout << "Starting thread " << tid << ".\n";
	cout_mtx.unlock();
	
	values_mtx.lock();
	int val = values[0];
	values_mtx.unlock();
	
	int rval = randGen(0, 10);
	val += rval;
	
	cout_mtx.lock();
	cout << "Thread#" << tid << " adding " << rval << ". New value: " << val << ".\n";
	cout_mtx.unlock();
	
	values_mtx.lock();
	values.push_back(val);
	values_mtx.unlock();
}


int main() {
	// Set global data in queue.
	values.push_back(42);

	// 최초 입력 확인
	cout<< "init : "<< values[0] <<endl;
	
	/* Start the threads, wait for them to finish. */
	// 4개의 스래드 할당 (함수, 인자)
	thread tr1(threadFnc, 1);
	thread tr2(threadFnc, 2);
	thread tr3(threadFnc, 3);
	thread tr4(threadFnc, 4);
	
	// .join은 스레드 종료 대기
	tr1.join();
	tr2.join();
	tr3.join();
	tr4.join();
	
	// Read the calculated values.
	cout << "Input: " << values[0] << ", Result 1: " << values[1] << ", Result 2: " << values[2] << ", Result 3: " << values[3] << ", Result 4: " << values[4] << "\n";
	
	
	return 1;
}
