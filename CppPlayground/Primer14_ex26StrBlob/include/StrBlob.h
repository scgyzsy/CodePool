#ifndef STRBLOB_H
#define STRBLOB_H

#include <vector>
using std::vector;

#include <string>
using std::string;

#include <initializer_list>
using std::initializer_list;

#include <memory>
using std::make_shared;
using std::shared_ptr;

#include <exception>

#include <stdexcept>    // std::out_of_range

class StrBlobPtr;
class ConstStrBlobPtr;

class StrBlob
{
    using size_type = vector<string>::size_type;
    friend class ConstStrBlobPtr;
    friend class StrBlobPtr;
    friend bool operator==(const StrBlob&, const StrBlob&);
    friend bool operator!=(const StrBlob&, const StrBlob&);
    friend bool operator< (const StrBlob&, const StrBlob&);
    friend bool operator> (const StrBlob&, const StrBlob&);
    friend bool operator<=(const StrBlob&, const StrBlob&);
    friend bool operator>=(const StrBlob&, const StrBlob&);

public:
    // 默认构造函数
    StrBlob() : data(make_shared<vector<string>>()) { }
    // 列表初始化构造函数
    StrBlob(initializer_list<string> il):data(make_shared<vector<string>>(il)) { }

    // 拷贝构造函数
    StrBlob(const StrBlob &sb) : data(make_shared<vector<string>>(*sb.data)){ }
    // 拷贝赋值函数
    StrBlob& operator=(const StrBlob&);

    // 移动构造函数
    StrBlob(StrBlob &&rhs) noexcept : data(std::move(rhs.data)) { }
    // 移动赋值函数
    StrBlob& operator=(StrBlob &&) noexcept;

    StrBlobPtr begin();
    StrBlobPtr end();

    ConstStrBlobPtr cbegin() const;
    ConstStrBlobPtr cend() const;

    string& operator[](size_t n);
    const string& operator[](size_t n) const;

    size_type size() const { return data->size(); }
    bool empty() const { return data->empty(); }

    void push_back(const string &t) { data->push_back(t); }
    void push_back(string &&s) { data->push_back(std::move(s)); }

    void pop_back();
    string& front();
    string& back();
    const string& front() const;
    const string& back() const;

private:
    void check(size_type, const string&) const;

    shared_ptr<vector<string>> data;
};

bool operator==(const StrBlob&, const StrBlob&);
bool operator!=(const StrBlob&, const StrBlob&);
bool operator< (const StrBlob&, const StrBlob&);
bool operator> (const StrBlob&, const StrBlob&);
bool operator<=(const StrBlob&, const StrBlob&);
bool operator>=(const StrBlob&, const StrBlob&);

inline void StrBlob::pop_back()
{
    check(0, "pop_back om empty StrBlob");
    data->pop_back();
}

inline string& StrBlob::front()
{
    check(0, "front on empty StrBlob");
    return data->front();
}

inline string& StrBlob::back()
{
    check(0, "back on empty StrBlob");
    return data->back();
}

inline const string& StrBlob::front() const
{
    check(0, "front on empty StrBlob");
    return data->front();
}

inline const string& StrBlob::back() const
{
    check(0, "back on empty StrBlob");
    return data->back();
}

inline void StrBlob::check(size_type i, const string &msg) const
{
    if (i >= data->size()) throw std::out_of_range(msg);
}

inline string& StrBlob::operator[](size_t n)
{
    check(0, "out of range");
    return data->at(n);
}

inline const string& StrBlob::operator[](size_t n) const
{
    check(0, "out of range");
    return data->at(n);
}

class StrBlobPtr
{
    friend bool operator==(const StrBlobPtr&, const StrBlobPtr&);
    friend bool operator!=(const StrBlobPtr&, const StrBlobPtr&);
    friend bool operator< (const StrBlobPtr&, const StrBlobPtr&);
    friend bool operator> (const StrBlobPtr&, const StrBlobPtr&);
    friend bool operator<=(const StrBlobPtr&, const StrBlobPtr&);
    friend bool operator>=(const StrBlobPtr&, const StrBlobPtr&);

public:
    StrBlobPtr() : curr(0) { }
    StrBlobPtr(StrBlob &s, size_t sz = 0) : wptr(s.data), curr(sz) { }
    string& deref() const;
    StrBlobPtr& incr();

    string& operator[](size_t n);
    const string& operator[](size_t n) const;

private:
    shared_ptr<vector<string>> check(size_t, const string&) const;

    std::weak_ptr<vector<string>> wptr;
    size_t curr;
};

bool operator==(const StrBlobPtr&, const StrBlobPtr&);
bool operator!=(const StrBlobPtr&, const StrBlobPtr&);
bool operator< (const StrBlobPtr&, const StrBlobPtr&);
bool operator> (const StrBlobPtr&, const StrBlobPtr&);
bool operator<=(const StrBlobPtr&, const StrBlobPtr&);
bool operator>=(const StrBlobPtr&, const StrBlobPtr&);

inline string& StrBlobPtr::deref() const
{
    auto p = check(curr, "dereference past end");
    return (*p)[curr];
}

inline StrBlobPtr& StrBlobPtr::incr()
{
    check(curr, "increment past end of StrBlobPtr");
    ++curr;
    return *this;
}

inline shared_ptr<vector<string>> StrBlobPtr::check(size_t i, const string &msg) const
{
    auto ret = wptr.lock();
    if(!ret) throw std::runtime_error("unbound StrBlobPtr");
    if (i >= ret->size()) throw std::out_of_range(msg);
    return ret;
}

inline string& StrBlobPtr::operator[](size_t n)
{
    auto p = check(n, "dereference out of range");
    return (*p)[n];
}

inline const string& StrBlobPtr::operator[](size_t n) const
{
    auto p = check(n, "dereference out of range");
    return (*p)[n];
}


class ConstStrBlobPtr
{
    friend bool operator==(const ConstStrBlobPtr&, const ConstStrBlobPtr&);
    friend bool operator!=(const ConstStrBlobPtr&, const ConstStrBlobPtr&);
    friend bool operator< (const ConstStrBlobPtr&, const ConstStrBlobPtr&);
    friend bool operator> (const ConstStrBlobPtr&, const ConstStrBlobPtr&);
    friend bool operator<=(const ConstStrBlobPtr&, const ConstStrBlobPtr&);
    friend bool operator>=(const ConstStrBlobPtr&, const ConstStrBlobPtr&);
public:
    ConstStrBlobPtr() : curr(0) { }
    ConstStrBlobPtr(const StrBlob &s, size_t sz = 0) : wptr(s.data), curr(sz){ }

    const string& deref() const;
    ConstStrBlobPtr& incr();

    const string& operator[](size_t n) const;

private:
    std::shared_ptr<vector<string>> check(size_t, const string&) const;

    std::weak_ptr<vector<string>> wptr;
    size_t curr;
};

bool operator==(const ConstStrBlobPtr&, const ConstStrBlobPtr&);
bool operator!=(const ConstStrBlobPtr&, const ConstStrBlobPtr&);
bool operator< (const ConstStrBlobPtr&, const ConstStrBlobPtr&);
bool operator> (const ConstStrBlobPtr&, const ConstStrBlobPtr&);
bool operator<=(const ConstStrBlobPtr&, const ConstStrBlobPtr&);
bool operator>=(const ConstStrBlobPtr&, const ConstStrBlobPtr&);

inline const string& ConstStrBlobPtr::deref() const
{
    auto p = check(curr, "dereference past end");
    return (*p)[curr];
}

inline ConstStrBlobPtr& ConstStrBlobPtr::incr()
{
    check(curr, "increment past end of StrBlobPtr");
    ++curr;
    return *this;
}

inline std::shared_ptr<vector<string>> ConstStrBlobPtr::check(size_t i, const string &msg) const
{
    auto ret = wptr.lock();
    if (!ret) throw std::runtime_error("unbound StrBlobPtr");
    if (i >= ret->size()) throw std::out_of_range(msg);
    return ret;
}

inline const string& ConstStrBlobPtr::operator[](size_t n) const
{
    auto p = check(n, "dereference out of range.");
    return (*p)[n];
}

#endif // STRBLOB_H
