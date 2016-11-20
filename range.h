#pragma once

#include <algorithm>

template <typename T>
class Range 
{
public:
	Range() = default;
	Range(T low, T high) : low_(low), high_(high) {}
	bool Within(T val) const {
		return low_ < val && val < high_;
	}
	bool Within(T val, T epsilon) const {
		return low_ - epsilon < val && val < high_ + epsilon;
	}
	bool WithinEq(T val) const {
		return low_ <= val && val <= high_;
	}
	bool Within(const Range<T>& range) const {
		return low_ > range.high_ && high_ > range.high_;
	}
	bool IsIntersect(const Range<T>& range) const {
		return low_ <= range.high_ || range.low_ <= high_;
	}
	Range<T> Intersect(const Range<T>& range) const {
		return Range<T>(std::max(low_, range.low_), std::min(high_, range.high_));
	}
	T GetLow() const {
		return low_;
	}
	T GetHigh() const {
		return high_;
	}
private:
	T low_, high_;
};