#ifndef THREADING_H
#define THREADING_H

#include <functional>
#include <future>
#include <mutex>

namespace threading {

template <typename F, typename... Ts>
inline auto runAsync(F&& f, Ts&&... params)
{
    return std::async(std::launch::async,
        std::forward<F>(f),
        std::forward<Ts>(params)...);
}

using LockType = std::unique_lock<std::mutex>;
using TaskType = std::function<void()>;

}

#endif // THREADING_H
