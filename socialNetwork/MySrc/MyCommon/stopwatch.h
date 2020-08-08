#ifndef STOPWATCH_H
#define STOPWATCH_H


// This is not thread safe!!

#include <sys/time.h>

#include <limits>
#include <vector>
#include <numeric>
#include <algorithm>
#include <chrono>

static struct timeval tv;

typedef struct stopwatch {
 uint64_t start = 0;
 uint64_t stop = 0;
 uint64_t total = 0;
 uint64_t count = 0;
} stopwatch;


uint64_t now();
void sw_start(stopwatch* sw);
void sw_stop(stopwatch* sw, bool clear=false);
double sw_getAVG(stopwatch sw);


// Current time, microseconds since the epoch
uint64_t now() {
  gettimeofday(&tv, NULL);
  int64_t ret;
  ret = tv.tv_sec;
  ret = ret * 1000 * 1000 + tv.tv_usec;
  return ret;
}


void sw_start(stopwatch* sw) {
  sw->start = now();
}

void sw_stop(stopwatch* sw, bool clear) {
  sw->stop = now();

  if (!clear) {
    sw->total += sw->stop - sw->start;
    sw->count++;
  }
}

double sw_getAVG(stopwatch sw){
  return (sw.total * 1.0) / sw.count;
}


template <typename TimeT = std::chrono::microseconds>
class Stopwatch
{
public:
    Stopwatch() : m_times(), m_mean(), m_st_dev() {
        m_times.clear();
    }

    // benchmark(Fun&& fun, Args&&... args)
    // {
    //     using result_t = typename std::result_of< Fun(Args...) >::type;
        
    //     std::vector< result_t > results;
    //     m_times.clear();
    //     auto n = m_num_iter + m_throw_away;

    //     for (auto i = 0; i != n; i++)
    //     {
    //         auto pair = measure_execution(std::forward<Fun>(fun), 
    //                                       std::forward<Args>(args)...);
    //         m_times.push_back(pair.first);
    //         results.push_back(pair.second);
    //     }
        
    //     compute_mean();
    //     compute_st_dev();
        
    //     return results;
    // }

    void start() {
        _start = std::chrono::steady_clock::now();
    }

    void stop() {
        _stop = std::chrono::steady_clock::now();
    
        auto duration = std::chrono::duration_cast<TimeT>( _stop - _start ).count();
        m_times.push_back(duration);

        _total += duration;
        _count++;
    }

    void post_process(){
        compute_mean();
        compute_st_dev();
    }

    typename TimeT::rep mean() const
    {
        return m_mean;
    }
    
    typename TimeT::rep standard_deviation() const
    {
        return m_st_dev;
    }
    

private:
    // template <typename Fun, typename... Args>
    // static std::pair< typename TimeT::rep,  typename std::result_of< Fun(Args...) >::type >
    // measure_execution(Fun&& fun, Args&&... args)
    // {
    //     auto t1 = std::chrono::steady_clock::now();
    //     auto result = std::forward<Fun>(fun)(std::forward<Args>(args)...);
    //     auto t2 = std::chrono::steady_clock::now();
    
    //     auto time = std::chrono::duration_cast<TimeT>( t2 - t1 ).count();
    //     return std::make_pair(time, result);
    // }

    void compute_mean()
    {
        auto sum = std::accumulate(m_times.begin() + m_throw_away, m_times.end(), 0);
        m_mean = sum / _count;
    }

    void compute_st_dev()
    {
        std::vector<typename TimeT::rep> diff(_count);
        std::transform(m_times.begin() + m_throw_away, m_times.end(), diff.begin(),
                       [this](typename TimeT::rep t) {return t - this->m_mean;});
        
        auto sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0);
        m_st_dev = std::sqrt(sq_sum / _count);
    }
    
    std::chrono::steady_clock::time_point _start, _stop;
    
    typename TimeT::rep _total = 0;
    uint64_t _count = 0;

    std::vector<typename TimeT::rep> m_times;

    int m_throw_away = 0;
    
    typename TimeT::rep m_mean = -1;
    typename TimeT::rep m_st_dev = -1;
};


#endif //STOPWATCH_H