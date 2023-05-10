#ifndef UNIQUE_PTR
#define UNIQUE_PTR

#include <memory>
template <typename T>
class UniquePtr {
 public:
  // 构造函数
  // 无参
  UniquePtr() : m_ptr(nullptr) {}
  // 有参或者空参
  UniquePtr(T* ptr) : m_ptr(ptr) {}
  // unique_ptr
  // release来释放该资源的所有权，让该资源由其他代码或外部系统来管理。
  UniquePtr(std::unique_ptr<T> src) : m_ptr(src.release()) {}
  UniquePtr(const UniquePtr& ptr) = delete;

  // 成员函数
  T* get() { return m_ptr; }
  void reset() {
    delete m_ptr;
    m_ptr = nullptr;
  }
  void reset(T* ptr) {
    delete m_ptr;
    m_ptr = ptr;
  }
  T* release() {
    T* tmp = m_ptr;
    m_ptr = nullptr;
    return tmp;
  }

  // 运算符重载
  // 返回指针指向的对应的内存
  T& operator*() { return *m_ptr; }
  // 返回指针指向对象的函数调用的结果
  T* operator->() { return m_ptr; }
  // 注意运算符返回的对象
  UniquePtr& operator=(const UniquePtr& ptr) = delete;
  bool operator==(UniquePtr& ptr) { return ptr.m_ptr == nullptr; }
  bool operator!=(UniquePtr& ptr) { return ptr.m_ptr != nullptr; }
  operator bool() { return m_ptr != nullptr; }  // 当函数成立时 if()才会执行下去

  ~UniquePtr() {
    delete m_ptr;
    m_ptr = nullptr;
  }

 private:
  T* m_ptr = nullptr;
};

#endif  // UNIQUE_PTR