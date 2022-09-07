#pragma once

#include <condition_variable>
#include <cstddef>
#include <functional>
#include <list>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

template <typename DataType, typename ResultType> class ThreadController {
private:
  size_t _no_threads;
  std::vector<std::thread> _threads;
  std::condition_variable _condition;
  std::mutex _data_mutex;
  std::mutex _result_mutex;
  std::mutex _thread_mutex;
  std::queue<DataType> _data_queue;
  std::function<ResultType(DataType)> _thread_function;
  std::list<ResultType> _result_list;
  bool _started = false;
  bool _stopped = false;

private:
  bool isDataEmpty();
  void addResult(const ResultType &result);
  void threadInstance();

public:
  ThreadController() = delete;
  ThreadController(const ThreadController<DataType, ResultType> &) = delete;
  ThreadController(std::function<ResultType(DataType)> thread_function,
                   size_t no_threads);

  void start();
  std::list<ResultType> join();
  void reset();
  DataType getData();
  void addData(const DataType &data);
};

// implementation
//
//
//
//
//
//
//
//
//

template <typename DataType, typename ResultType>
bool ThreadController<DataType, ResultType>::isDataEmpty() {
  std::unique_lock lock(_data_mutex);
  return _data_queue.empty();
}

template <typename DataType, typename ResultType>
void ThreadController<DataType, ResultType>::addResult(
    const ResultType &result) {
  std::unique_lock lock(_result_mutex);
  _result_list.push_front(result);
}

template <typename DataType, typename ResultType>
void ThreadController<DataType, ResultType>::threadInstance() {
  {
    std::unique_lock lock(_thread_mutex);
    _condition.wait(lock, [&]() -> bool { return _started; });
  }
  while (true) {
    ResultType result;
    while (!isDataEmpty()) {
      DataType data = getData();
      result = _thread_function(data);
      addResult(result);
    }

    if (_stopped) {
      return;
    }
    std::unique_lock data_lock(_data_mutex);
    _condition.wait(data_lock,
                    [&]() -> bool { return _stopped || !_data_queue.empty(); });
  }
}

template <typename DataType, typename ResultType>
ThreadController<DataType, ResultType>::ThreadController(
    std::function<ResultType(DataType)> thread_function, size_t no_threads)
    : _thread_function(thread_function), _no_threads(no_threads) {
  while (no_threads--) {
    _threads.push_back(std::thread(&ThreadController::threadInstance, this));
  }
}

template <typename DataType, typename ResultType>
void ThreadController<DataType, ResultType>::start() {
  _started = true;
  _condition.notify_all();
}

template <typename DataType, typename ResultType>
std::list<ResultType> ThreadController<DataType, ResultType>::join() {
  _stopped = true;
  _condition.notify_all();
  for (std::thread &th : _threads) {
    th.join();
  }
  return _result_list;
}

template <typename DataType, typename ResultType>
void ThreadController<DataType, ResultType>::reset() {
  _started = false;
  _stopped = false;
  _threads.clear();
  _data_queue.clear();
}

template <typename DataType, typename ResultType>
DataType ThreadController<DataType, ResultType>::getData() {
  std::unique_lock lock(_data_mutex);
  DataType temp = _data_queue.front();
  _data_queue.pop();
  return temp;
}

template <typename DataType, typename ResultType>
void ThreadController<DataType, ResultType>::addData(const DataType &data) {
  std::unique_lock lock(_data_mutex);
  _data_queue.push(data);
}
