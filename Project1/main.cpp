#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <chrono>
#include <SFML/Graphics.hpp>

// Colors
const sf::Color bgCol({ 6, 57, 77 });
const sf::Color barCol({ 234, 161, 9 });
const sf::Color hgtBarCol({ 202, 11, 11 });
const sf::Color textCol({ 250, 218, 193 });

// Sizes
const sf::Vector2u windowSize({ 990, 540 });
const unsigned int title_ch_size = 30;

// Shape
sf::RectangleShape bar;

// Text
sf::Font font("fonts/pixel_font.ttf");
sf::Text title(font);

// Function Declaration
int show_menu(const std::vector<std::string>& avail_algos);
std::vector<int> randomVec(const int size, const float maxHeight);
void draw_vec(sf::RenderWindow& window, const std::vector<int>& nums, const int hgt_idx);

// Merge Sort
class Merge_Sort {
private:
	static void merge(int st, int mid, int end, std::vector<int>& nums, sf::RenderWindow& window) {
		std::vector<int> temp;
		int i = st;
		int j = mid + 1;
		while (i <= mid && j <= end) {
			if (nums.at(i) < nums.at(j)) {
				temp.push_back(nums.at(i));
				i++;
			}
			else {
				temp.push_back(nums.at(j));
				j++;
			}
		}
		while (i <= mid) {
			temp.push_back(nums.at(i));
			i++;
		}
		while (j <= end) {
			temp.push_back(nums.at(j));
			j++;
		}
		for (int k = st; k <= end; k++) {
			window.clear(bgCol);
			window.draw(title);
			draw_vec(window, nums, k);
			window.display();
			nums.at(k) = temp.at(k - st);
		}
	}
public:
	static void merge_sort(int st, int end, std::vector<int>& nums, sf::RenderWindow& window) {
		if (st < end) {
			int mid = (st + end) / 2;
			merge_sort(st, mid, nums, window);
			merge_sort(mid + 1, end, nums, window);
			merge(st, mid, end, nums, window);
		}
	}
};

// Quick Sort
class Quick_Sort {
private:
	static int partition(int st, int end, std::vector<int>& nums, sf::RenderWindow& window) {
		int idx = st;
		for (int i = st; i < end; i++) {
			if (nums.at(i) < nums.at(end)) {
				std::swap(nums.at(i), nums.at(idx++));
				window.clear(bgCol);
				window.draw(title);
				draw_vec(window, nums, idx);
				window.display();
			}
		}
		std::swap(nums.at(end), nums.at(idx));
		return idx;
	}
public:
	static void quick_sort(int st, int end, std::vector<int>& nums, sf::RenderWindow& window) {
		if (st < end) {
			int pivot = partition(st, end, nums, window);
			quick_sort(st, pivot - 1, nums, window);
			quick_sort(pivot + 1, end, nums, window);
		}
	}
};


// Insertion Sort
class Insertion_Sort {
public:
	static void insertion_sort(std::vector<int>& nums, sf::RenderWindow& window) {
		for (int i = 1; i < nums.size(); i++) {
			float key = nums.at(i);
			int j = i - 1;
			while (j >= 0 && key < nums.at(j)) {
				nums.at(j + 1) = nums.at(j);
				window.clear(bgCol);
				window.draw(title);
				draw_vec(window, nums, j);
				window.display();
				j--;
			}
			nums.at(j + 1) = key;
		}
	}
};

// Count Sort
class Count_Sort {
private:
	static int max_elm(std::vector<int>& nums) {
		int max_elm = 0;
		for (int num : nums) {
			max_elm = std::max(max_elm, num);
		}
		return max_elm;
	}
public:
	static void count_sort(std::vector<int>& nums, sf::RenderWindow& window) {
		int max_size = max_elm(nums);
		std::vector<int> freq(max_size + 1, 0);
		for (int num : nums) {
			freq.at(num)++;
		}
		int idx = 0;
		for (int i = 0; i <= max_size; i++) {
			while (freq.at(i)) {
				nums.at(idx++) = i;
				freq.at(i)--;
				window.clear(bgCol);
				window.draw(title);
				draw_vec(window, nums, idx);
				window.display();
			}
		}
	}
};

// Buuble Sort
class Bubble_Sort {
public:
	static void bubble_sort()
};

// Main Function
int main() {
	// Init Nums
	int size_of_nums;
	std::cout << "Enter size of array you want to sort: ";
	std::cin >> size_of_nums;
	std::vector<int> nums = randomVec(size_of_nums, windowSize.y / 1.25f);

	// Init Available Algos
	std::vector<std::string> avail_algos = { "Merge Sort" , "Quick Sort", "Insertion Sort", "Count Sort"};

	// Menu
	int choice = show_menu(avail_algos);

	// Window
	sf::RenderWindow window(sf::VideoMode(windowSize), "Visual Sorting");
	window.setVerticalSyncEnabled(true);

	// Main loop
	while (window.isOpen()) {
		auto start = std::chrono::high_resolution_clock::now();
		switch (choice) {
		case 1:
			title.setString(avail_algos.at(0));
			title.setCharacterSize(title_ch_size);
			Merge_Sort::merge_sort(0, size_of_nums - 1, nums, window);
			break;
		case 2:
			title.setString(avail_algos.at(1));
			title.setCharacterSize(title_ch_size);
			Quick_Sort::quick_sort(0, size_of_nums - 1, nums, window);
			break;
		case 3:
			title.setString(avail_algos.at(2));
			title.setCharacterSize(title_ch_size);
			Insertion_Sort::insertion_sort(nums, window);
			break;
		case 4:
			title.setString(avail_algos.at(3));
			title.setCharacterSize(title_ch_size);
			Count_Sort::count_sort(nums, window);
			break;
		default:
			break;
		}
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
		std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
		window.close();
	}

	return 0;
}

// Display Menu
int show_menu(const std::vector<std::string>& avail_algos) {
	std::cout << "> Menu:" << std::endl;
	for (int i = 0; i < avail_algos.size(); i++) {
		std::cout << '\t' << i + 1 << ": " << avail_algos.at(i) << std::endl;
	}
	int choice;
	std::cout << "Which algo you wanna watch: ";
	std::cin >> choice;
	return choice;
}

// Generate Random Vector
std::vector<int> randomVec(const int size, const float maxHeight) {
	std::vector<int> nums(size);
	for (int i = 1; i <= size; i++) {
		nums.at(i - 1) = static_cast<int>(i * (maxHeight / size));
	}

	std::random_device seed;
	std::minstd_rand rng(seed());
	std::shuffle(nums.begin(), nums.end(), rng);
	return nums;
}

// Draw a vector to render window
void draw_vec(sf::RenderWindow& window, const std::vector<int>& nums, const int hgt_idx) {
	static float width_of_bar = windowSize.x / static_cast<float>(nums.size());
	for (int i = 0; i < nums.size(); i++) {
		bar.setSize({ width_of_bar, static_cast<float>(nums.at(i)) });
		bar.setPosition({ i * width_of_bar, static_cast<float>(windowSize.y -nums.at(i)) });
		bar.setFillColor((i == hgt_idx) ? hgtBarCol : barCol);
		window.draw(bar);
	}
}