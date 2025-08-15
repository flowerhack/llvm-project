#include <iostream>
#include <vector>
#include <string.h>

void ModifyString(std::string *s) {
	s->append("!");
}

class DataCache {
public:
  // This member function declaration will also be matched.
  // Okay my code ONLY matched this one.
  // But God that's so much better than nothing.
  const std::vector<int>& get_cached_items() const;

private:
  std::vector<int> items_;
};

const std::vector<std::string>& get_global_strings();

const std::vector<double>& get_default_values() {
    static std::vector<double> defaults = {3.14, 2.71, 1.61};
    return defaults;
}

int main() {
	std::cout << "Initializing some_strings.\n";
	std::vector<std::string> some_strings;
	some_strings.push_back("one");
	some_strings.push_back("two");
	some_strings.push_back("three");
	// Three tools:
	// * suspicious-copy-in-range-loop (as written right now)
	// * performance-for-range-copy  with no options
	// * performance-for-range-copy with WarnOnAllAutoCopies enabled

	std::cout << "Modify copies; print them as you modify them.\n";
	// Case 1.
	// These strings are being COPIED and then MODIFIED, so:
	// SHOULD WARN? YES
	// DOES WARN?
	// suspicious-copy-in-range-loop: YES
	// ~ performance-for-range-copy with no options: NO
	// performance-for-range-copy with WarnOnAllAutoCopies enabled: YES
	for (auto x : some_strings) {
		ModifyString(&x);
		std::cout << x << "\n";
		//std::cout << &x << "\n";
	}

	// Case 2.
	// These strings are being COPIED but left UNMODIFIED, so:
	// SHOULD WARN? NO
	// DOES WARN?
	// ~ suspicious-copy-in-range-loop: YES
	// performance-for-range-copy with no options: NO
	// ~ performance-for-range-copy with WarnOnAllAutoCopies enabled: YES
	for (auto x : some_strings) {
		std::cout << "hi\n";
		std::cout << x << "\n";
		//std::cout << &x << "\n";
	}

	// Case 3.
	// These strings are being NEITHER COPIED, NOR MODIFIED, so
	// SHOULD WARN? NO
	// DOES WARN?
	// suspicious-copy-in-range-loop: NO
	// performance-for-range-copy with no options: NO
	// performance-for-range-copy with WarnOnAllAutoCopies enabled: NO
	for (auto& x : some_strings) {
		std::cout << x << "\n";
		std::cout << &x << "\n";
	}

	// Case 4.
	// These strings are being NOT COPIED but referenced, and MODIFIED, so
	// SHOULD WARN? NO
	// DOES WARN?
	// suspicious-copy-in-range-loop: NO
	// performance-for-range-copy with no options: NO
	// performance-for-range-copy with WarnOnAllAutoCopies enabled: NO
	for (auto& x : some_strings) {
		x = "modd'd!";
		std::cout << x << "\n";
	}
}
