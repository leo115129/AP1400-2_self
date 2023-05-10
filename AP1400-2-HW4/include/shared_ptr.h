#ifndef SHARED_PTR
#define SHARED_PTR

#include <memory>
template <typename T>
class SharedPtr {
 private:
  T* m_ptr = nullptr;
  size_t* count = nullptr;

 public:
  // 构造与析构
  SharedPtr() : m_ptr(nullptr), count(new size_t(0)) {}
  SharedPtr(T* ptr) : m_ptr(ptr), count(new size_t(1)) {}
  // 要自己创建一个新的堆 不能直接指向模板的
  SharedPtr(std::shared_ptr<T> ptr) : count(new size_t(1)) {
    m_ptr = new T(*ptr.get());
  }
  SharedPtr(const SharedPtr<T>& other) : m_ptr(other.m_ptr) {
    ++*(other.count);
    count = other.count;
  }
  ~SharedPtr() {
    if (count == nullptr) {
      return;
    } else if (*count == 0) {
      delete count;
      return;
    } else {
      --(*count);
      if (*count == 0) {
        delete count;
        count = nullptr;
        delete m_ptr;
        m_ptr = nullptr;
      }
    }
  }

  // 成员函数
  T* get() { return m_ptr; }
  size_t use_count() {
    if (count != nullptr)
      return *count;
    else
      return 0;
  }
  void reset() {
    delete m_ptr;
    m_ptr = nullptr;
    delete count;
    count = nullptr;
  }
  void reset(T* ptr) {
    reset();
    m_ptr = ptr;
    count = new size_t(1);
  }

  // 运算符重载
  T& operator*() { return *m_ptr; }
  T* operator->() { return m_ptr; }
  operator bool() { return m_ptr != nullptr; }
  SharedPtr<T>& operator=(const SharedPtr<T>& other) {
    // this is const pointer
    if (this != &other) {
      ~SharedPtr();
      m_ptr = other.m_ptr;
      count = other.count;
      ++(*other.count);
    }
    return *this;
  }
};
#endif  // SHARED_PTR