//
// Created by croissant on 24/08/2022.
//

#ifndef ENGINETEST_TIMER_H
#define ENGINETEST_TIMER_H

#include <chrono>
#include <random>

class Timer {
private:
	// Type aliases to make accessing nested type easier
	using clock_type = std::chrono::steady_clock;
	using second_type = std::chrono::duration<double, std::ratio<1> >;

	std::chrono::time_point<clock_type> m_beg{clock_type::now()};

public:
	void reset() {
		m_beg = clock_type::now();
	}

	double elapsed() const {
		return std::chrono::duration_cast<second_type>(clock_type::now() - m_beg).count();
	}
};

//template<typename T>
//void shuffle(std::list<T>& lst) // shuffle contents of a list
//{
//	std::vector<T> temp(lst.begin(), lst.end());
//	std::random_shuffle(temp.begin(), temp.end());
//
//	std::copy(temp.begin(), temp.end(), lst.begin());
//}

#endif //ENGINETEST_TIMER_H
