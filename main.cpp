#pragma GCC optimize("O3", "unroll-loops")

#include <string>
#include <iostream>
#include <fstream>
#include <functional>
#include <cassert>
#include <mutex>
#include <vector>
#include <thread>



inline constexpr size_t 
shift_mix(size_t v) {
	return v ^ (v >> 47);
}

inline size_t
unaligned_load(const char* p) {
	size_t result;
	__builtin_memcpy(&result, p, sizeof(result));
	return result;
}

/*

// Loads n bytes, where 1 <= n < 8.
inline size_t
load_bytes(const char* p, int n) {
	size_t result = 0;
	--n;
	do {
		result = (result << 8) + static_cast<unsigned char>(p[n]);
	} while (--n >= 0);
	return result;
}

size_t _Hash_bytes(const void* ptr, size_t len, size_t seed) {
	static const size_t mul = (((size_t) 0xc6a4a793UL) << 32UL)
				  + (size_t) 0x5bd1e995UL;
	const char* const buf = static_cast<const char*>(ptr);

	// Remove the bytes not divisible by the sizeof(size_t).  This
	// allows the main loop to process the data as 64-bit integers.
	const int len_aligned = len & ~0x7;
	const char* const end = buf + len_aligned;
	size_t hash = seed ^ (len * mul);
	for (const char* p = buf; p != end; p += 8) {
		const size_t data = shift_mix(unaligned_load(p) * mul) * mul;
		hash ^= data;
		hash *= mul;
	}
	if ((len & 0x7) != 0) {
		const size_t data = load_bytes(end, len & 0x7);
		hash ^= data;
		hash *= mul;
	}
	hash = shift_mix(hash) * mul;
	hash = shift_mix(hash);
	return hash;
}
*/

void next(std::string& s) {
	for(int i = s.size() - 1; i >= 0; --i) {
		if(s[i] == 'z') {
			s[i] = 'A';
			break;
		} else if (s[i] == 'Z') {
			s[i] = '0';
			break;
		} else if (s[i] == '9') {
			s[i] = '_';
			break; } else if (s[i] == '_') {
			s[i] = 'a';
		} else {
			s[i] += 1;
			break;
		}
	}
}

constexpr size_t seed = 0xc70f6907UL;
constexpr size_t mul = (((size_t) 0xc6a4a793UL) << 32UL)
		+ (size_t) 0x5bd1e995UL;
constexpr size_t mul_inverse = 6879827495036328381ull;
constexpr size_t len = 15;
constexpr size_t hash_init = seed ^ (len * mul);
constexpr size_t posible_bucket = 976369;
constexpr size_t target_hash = posible_bucket * 413251235;
constexpr size_t target_hash_before = shift_mix(shift_mix(target_hash) * mul_inverse) * mul_inverse;

constexpr size_t thread_count = 12;
std::string suffix[thread_count];

bool recover_suffix(std::string& pref, int thread_index) {
	size_t hash = hash_init ^ (shift_mix(unaligned_load(pref.c_str()) * mul) * mul);
	hash *= mul;
	hash ^= target_hash_before;
	if (hash >> 56) {
		return false;
	}
	for (int i = 0; i < 7; ++i) {
		char c = hash & 255;
		suffix[thread_index][i] = c;
		if (!(std::isalpha(c) || std::isdigit(c) || c == '_')) {
			return false;
		}
		hash >>= 8;
	}
	return true;
}


const std::string filename = "data/aboba3";
std::ofstream fout(filename);
int count = 0;
constexpr int target_count = 15;

std::mutex sync_mutex;

void bruteforce(int thread_index) {
	std::string prefix(8, 'a' + thread_index);
	bool success;
	for(;; next(prefix)) {
		if (recover_suffix(prefix, thread_index)) {
			std::lock_guard<std::mutex> sync_lock_guard(sync_mutex);
			if(++count > target_count) {
				break;
			}
			fout << prefix + suffix[thread_index] << std::endl;
		}
	}
}



int main() {

	std::vector<std::thread> threads;
	for(int i = 0; i < thread_count; ++i) {
		suffix[i] = std::string(7, 'a');
		threads.push_back(std::thread(bruteforce, i));
	}

	for(std::thread& thread : threads) {
		thread.join();
	}
	return 0;
}
