#include <random>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "utilities/trie.h"
#include "platform/platform.h"

template<typename T>
struct Stats {
    std::string name;
    double mean;
    double median;
    double stddev;
    double pct5;
    double pct95;
    double pct99;
    std::vector<T>* values;
};

// Given a vector of values (each a vector<T>) calcuate metrics on them
// and print to stdout.
template<typename T>
void print_values(std::vector<std::pair<std::string, std::vector<T>*> > values,
                  std::string unit)
{
    // First, calculate mean, median, standard deviation and percentiles of
    // each set of values, both for printing and to derive what the range of
    // the graphs should be.
    std::vector<Stats<T>> value_stats;
    for (const auto& t : values) {
        Stats<T> stats;
        stats.name = t.first;
        stats.values = t.second;
        std::vector<T>& vec = *t.second;

        // Calculate latency percentiles
        std::sort(vec.begin(), vec.end());
        stats.median = vec[(vec.size() * 50) / 100];
        stats.pct5 = vec[(vec.size() * 5) / 100];
        stats.pct95 = vec[(vec.size() * 95) / 100];
        stats.pct99 = vec[(vec.size() * 99) / 100];

        const double sum = std::accumulate(vec.begin(), vec.end(), 0.0);
        stats.mean = sum / vec.size();
        double accum = 0.0;
        std::for_each (vec.begin(), vec.end(), [&](const double d) {
            accum += (d - stats.mean) * (d - stats.mean);
        });
        stats.stddev = sqrt(accum / (vec.size() - 1));

        value_stats.push_back(stats);
    }

    // From these find the start and end for the spark graphs which covers the
    // a "reasonable sample" of each value set. We define that as from the 5th
    // to the 95th percentile, so we ensure *all* sets have that range covered.
    T spark_start = std::numeric_limits<T>::max();
    T spark_end = 0;
    for (const auto& stats : value_stats) {
        spark_start = (stats.pct5 < spark_start) ? stats.pct5 : spark_start;
        spark_end = (stats.pct95 > spark_end) ? stats.pct95 : spark_end;
    }

    printf("\n\n                                Percentile           \n");
    printf("  %-15s Median     95th     99th  Std Dev  Histogram of samples\n\n", "");
    // Finally, print out each set.
    for (const auto& stats : value_stats) {
        if (stats.median/1e6 < 1) {
            printf("%-15s %8.03f %8.03f %8.03f %8.03f  ",
                    stats.name.c_str(), stats.median/1e3, stats.pct95/1e3,
                    stats.pct99/1e3, stats.stddev/1e3);
        } else {
            printf("%-8s (x1e3) %8.03f %8.03f %8.03f %8.03f  ",
                    stats.name.c_str(), stats.median/1e6, stats.pct95/1e6,
                    stats.pct99/1e6, stats.stddev/1e6);
        }

        // Calculate and render Sparkline (requires UTF-8 terminal).
        const int nbins = 32;
        int prev_distance = 0;
        std::vector<size_t> histogram;
        for (unsigned int bin = 0; bin < nbins; bin++) {
            const T max_for_bin = (spark_end / nbins) * bin;
            auto it = std::lower_bound(stats.values->begin(),
                                       stats.values->end(),
                                       max_for_bin);
            const int distance = std::distance(stats.values->begin(), it);
            histogram.push_back(distance - prev_distance);
            prev_distance = distance;
        }

        const auto minmax = std::minmax_element(histogram.begin(), histogram.end());
        const size_t range = *minmax.second - *minmax.first + 1;
        const int levels = 8;
        for (const auto& h : histogram) {
            int bar_size = ((h - *minmax.first + 1) * (levels - 1)) / range;
            putchar('\xe2');
            putchar('\x96');
            putchar('\x81' + bar_size);
        }
        putchar('\n');
    }
    printf("%51s  %-14d %s %14d\n\n", "",
           int(spark_start/1e3), unit.c_str(), int(spark_end/1e3));
}

static void perf_char() {
    std::ifstream dictFile("/usr/share/dict/words");
    std::string line;
    std::vector<hrtime_t> insert, exists, erase;
    // get dictionary in memory for random insert
    while(std::getline(dictFile, line) && i<65536) {
        dict.push_back(line);
    }

    {
        std::mt19937 gen(0); // fixed seed
        std::shuffle(dict.begin(), dict.end(), gen); // move it around

        // time insert
        for (auto s: dict) {
            hrtime_t start = gethrtime();
            trie.insert(s);
            insert.push_back(gethrtime() - start);
        }
    }

    {
        std::mt19937 gen(2); // fixed seed
        std::shuffle(dict.begin(), dict.end(), gen); // move it around
        for (auto s: dict) {
            hrtime_t start = gethrtime();
            if (!trie.exists(s.c_str(), s.c_str() + s.length())) {
                std::cerr << "Failed to find value " << s << std::endl;
                return 0;
            }
            exists.push_back(gethrtime() - start);
        }
    }

   {
        std::mt19937 gen(55); // fixed seed
        std::shuffle(dict.begin(), dict.end(), gen); // move it around
        for (auto s: dict) {
            hrtime_t start = gethrtime();
            trie.erase(s);
            erase.push_back(gethrtime() - start);
        }
    }

    std::vector<std::pair<std::string, std::vector<hrtime_t>*> > all_timings;
    all_timings.push_back(std::make_pair("insert", &insert));
    all_timings.push_back(std::make_pair("exists", &exists));
    all_timings.push_back(std::make_pair("erase", &erase));
    print_values(all_timings, "µs");
}

static void perf_int() {
    {
        std::mt19937 gen(0); // fixed seed
        std::shuffle(dict.begin(), dict.end(), gen); // move it around

        // time insert
        for (auto s: dict) {
            hrtime_t start = gethrtime();
            trie.insert(s);
            insert.push_back(gethrtime() - start);
        }
    }
}

int main() {
    perf_char();
    perf_int();

    return 0;
}