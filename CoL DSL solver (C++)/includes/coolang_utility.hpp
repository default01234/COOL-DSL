/*
 * utility.hpp
 *
 *  Created on: 23-九月-23
 *      Author: -
 */
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <deque>
#include <algorithm>
#include <boost/icl/continuous_interval.hpp>
#include <deque>
#include <algorithm>
#include <limits>

#ifndef UTILITY_HPP_
#define UTILITY_HPP_
 /**
  * @brief output formatted date "yy(yyyy)mmddhhmmss" or other formats you want.
  * @param format
  * @return
  */
std::string getFormattedDate(const std::string& format);

enum class OpType {
	lt, lte, gt, gte, eq, ne
};
template<typename T = double>
class CombinedInterval {
public:
	typedef boost::icl::continuous_interval<T> Interval;

	// Constructor
	CombinedInterval() {}

	void addRelationalInterval(OpType op, T value) {
		switch (op) {
		case OpType::lt:
			addInterval(Interval::right_open(-std::numeric_limits<T>::infinity(), value));
			break;
		case OpType::lte:
			addInterval(Interval::closed(-std::numeric_limits<T>::infinity(), value));
			break;
		case OpType::gt:
			addInterval(Interval::left_open(value, std::numeric_limits<T>::infinity()));
			break;
		case OpType::gte:
			addInterval(Interval::closed(value, std::numeric_limits<T>::infinity()));
			break;
		case OpType::eq:
			addInterval(Interval::closed(value, value));
			break;
		case OpType::ne:
			addInterval(Interval::right_open(-std::numeric_limits<T>::infinity(), value));
			addInterval(Interval::left_open(value, std::numeric_limits<T>::infinity()));
			break;
		}
	}

	// Overload ^ operator to find intersection
	CombinedInterval operator^(const CombinedInterval& other) const {
		CombinedInterval result;
		for (const auto& interval1 : intervals) {
			for (const auto& interval2 : other.intervals) {
				auto intersection = interval1 & interval2; // intersection
				if (!(intersection.lower() == intersection.upper() && !boost::icl::contains(intersection, intersection.lower()))) {
					result.addInterval(intersection);
				}
			}
		}
		return result;
	}

	T getDefaultValue() const {
		if (!intervals.empty()) {
			const auto& firstInterval = intervals.front();
			if (firstInterval.lower() == firstInterval.upper() && !boost::icl::contains(firstInterval, firstInterval.lower())) {
				throw std::exception("Empty interval");
			}

			if (firstInterval.lower() == -std::numeric_limits<T>::infinity()) {
				return firstInterval.upper() / 2; // Adjust according to your logic
			}
			if (firstInterval.upper() == std::numeric_limits<T>::infinity()) {
				return firstInterval.lower() * 2; // Adjust according to your logic
			}

			return (firstInterval.lower() + firstInterval.upper()) / 2;
		}
		else {
			throw std::exception("Intervals have no intersection");
		}
	}


	CombinedInterval& operator=(const CombinedInterval& other) {
		if (this != &other) {
			this->intervals = other.intervals;
		}
		return *this;
	}

	int size() const {
		return intervals.size();
	}

private:
	std::deque<Interval> intervals;

	void addInterval(const Interval& interval) {
		intervals.push_back(interval);
		sortIntervals();
	}

	void sortIntervals() {
		std::sort(intervals.begin(), intervals.end(), [](const Interval& a, const Interval& b) {
			if (a.lower() == -std::numeric_limits<T>::infinity() && b.lower() != -std::numeric_limits<T>::infinity()) {
				return true; // a comes before b
			}
			if (b.lower() == -std::numeric_limits<T>::infinity() && a.lower() != -std::numeric_limits<T>::infinity()) {
				return false; // b comes before a
			}

			if (a.lower() == b.lower()) {
				if (a.upper() == std::numeric_limits<T>::infinity() && b.upper() != std::numeric_limits<T>::infinity()) {
					return false; // a comes after b
				}
				if (b.upper() == std::numeric_limits<T>::infinity() && a.upper() != std::numeric_limits<T>::infinity()) {
					return true; // b comes after a
				}

				return a.upper() < b.upper();
			}

			return a.lower() < b.lower();
			});
	}
	// Check if two intervals can be merged
	static bool canMerge(const Interval& a, const Interval& b) {
		return boost::icl::intersects(a, b) || boost::icl::touches(a, b);
	}

	// Merge two intervals
	static Interval mergeIntervals(const Interval& a, const Interval& b) {
		return a + b; // Combine intervals into their hull
	}
};
#endif /* UTILITY_HPP_ */
