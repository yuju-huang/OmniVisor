#include <algorithm>
#include <cmath>
#include <numeric>
#include <iostream>
#include <map>

#include "accum.hh"

// TODO: Histogram rather than all samples?

using namespace std;

void Accum::clear(void)
{
    samples_.clear();
    sorted_ = false;
}

void Accum::add_sample(uint64_t val)
{
    samples_.push_back(val);
    sorted_ = false;
}

void Accum::print_samples(void)
{
    for (auto i : samples_) {
        cout << i << endl;
    }
}

void Accum::print_freq(void)
{
    std::map<uint64_t, uint64_t> map;
    for (auto i : samples_) {
        if (map.find(i) == map.end()) {
            map[i] = 1;
        } else {
            map[i]++;
        }
    }

    for (const auto& kv : map) {
        cout << kv.first << ", " << kv.second << endl;
    }
}

double Accum::mean(void)
{
    double avg = 0;
    double sz = size();

    for (auto i : samples_) {
        avg += double(i) / sz;
    }

    return avg;
}

double Accum::stddev(void)
{
    double avg = mean();
    double sum = 0;

    for (auto i : samples_) {
        double diff = double(i) - avg;
        sum += diff * diff;
    }

    return sqrt(sum / size());
}

uint64_t Accum::percentile(double percent)
{
    if (not sorted_) {
        sort(samples_.begin(), samples_.end());
        sorted_ = true;
    }
    return samples_[ceil(double(size()) * percent) - 1];
}

uint64_t Accum::min(void)
{
    if (not sorted_) {
        sort(samples_.begin(), samples_.end());
        sorted_ = true;
    }
    return samples_[0];
}

uint64_t Accum::max(void)
{
    if (not sorted_) {
        sort(samples_.begin(), samples_.end());
        sorted_ = true;
    }
    return samples_[samples_.size() - 1];
}

vector<uint64_t>::size_type Accum::size(void) { return samples_.size(); }
