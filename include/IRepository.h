#include <vector>

template <typename T>
struct IRepository {
    virtual std::vector<T> readAll() = 0;
    virtual T create(const T& entity) = 0;
    virtual T read(int id) = 0;
    virtual T update(const T& entity) = 0;
    virtual T del(const T& entity) = 0;
    
    virtual std::vector<T> read(unsigned int start, unsigned int limit) = 0;
};