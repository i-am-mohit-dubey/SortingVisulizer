#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <chrono>
#include <SFML/Graphics.hpp>

// Colors
const sf::Color bgCol({ 42, 54, 59 });
const sf::Color barCol({ 153, 184, 152 });
const sf::Color hgtBarCol({ 232, 74, 95 });
const sf::Color textCol({ 254, 206, 171 });

// Sizes
const sf::Vector2u windowSize({ 990, 540 });
const unsigned int title_ch_size = 30;

// Shape
sf::RectangleShape bar;

// Text
sf::Font font("fonts/pixel_font.ttf");
sf::Text title(font);

// Function Declaration
void manage_events(sf::RenderWindow& window);
float calc_time_span(const std::chrono::time_point<std::chrono::high_resolution_clock>& start);
int show_menu(const std::vector<std::string>& avail_algos);
std::vector<int> randomVec(const int size, const float maxHeight);
void draw_vec(sf::RenderWindow& window, const std::vector<int>& nums, const int hgt_idx);

// Merge Sort
class Merge_Sort {
private:
	static void merge(int st, int mid, int end, std::vector<int>& nums, sf::RenderWindow& window) {
		static std::vector<int> temp(nums.size());
		int i = st;
		int j = mid + 1;
		int t = st;
		while (i <= mid && j <= end) {
			if (nums[i] < nums[j]) {
				temp[t++] = nums[i++];
			}
			else {
				temp[t++] = nums[j++];
			}
		}
		while (i <= mid) {
			temp[t++] = nums[i++];
		}
		while (j <= end) {
			temp[t++] = nums[j++];
		}
		for (int k = st; k <= end; k++) {
			nums[k] = temp[k];
			manage_events(window);
			window.clear(bgCol);
			window.draw(title);
			draw_vec(window, nums, k);
			window.display();
		}
	}
public:
	static void merge_sort(int st, int end, std::vector<int>& nums, sf::RenderWindow& window) {
		if (st < end) {
			int mid = st + (end - st) / 2;
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
			if (nums[i] < nums[end]) {
				std::swap(nums[i], nums[idx++]);
				manage_events(window);
				window.clear(bgCol);
				window.draw(title);
				draw_vec(window, nums, idx);
				window.display();
			}
		}
		std::swap(nums[end], nums[idx]);
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
			int key = nums[i];
			int j = i - 1;
			while (j >= 0 && key < nums[j]) {
				nums[j + 1] = nums[j--];
				manage_events(window);
				window.clear(bgCol);
				window.draw(title);
				draw_vec(window, nums, j);
				window.display();
			}
			nums[j + 1] = key;
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
			freq[num]++;
		}
		int idx = 0;
		for (int i = 0; i <= max_size; i++) {
			while (freq[i]) {
				nums[idx++] = i;
				freq[i]--;
				manage_events(window);
				window.clear(bgCol);
				window.draw(title);
				draw_vec(window, nums, idx);
				window.display();
			}
		}
	}
};

// Bubble Sort
class Bubble_Sort {
public:
	static void bubble_sort(std::vector<int>& nums, sf::RenderWindow& window) {
		for (int i = 0; i < nums.size(); i++) {
			for (int j = 0; j < nums.size() - i - 1; j++) {
				if (nums[j] > nums[j + 1]) {
					std::swap(nums[j], nums[j + 1]);
					manage_events(window);
					window.clear(bgCol);
					window.draw(title);
					draw_vec(window, nums, j);
					window.display();
				}
			}
		}
	}
};

// Selection Sort
class Selection_Sort {
private:
	static int min_elm_idx(const int st, const std::vector<int>& nums, sf::RenderWindow& window) {
		int min_idx = st;
		for (int i = st; i < nums.size(); i++) {
			if (nums[i] < nums[min_idx]) {
				min_idx = i;
				manage_events(window);
				window.clear(bgCol);
				window.draw(title);
				draw_vec(window, nums, min_idx);
				window.display();
			}
		}
		return min_idx;
	}
public:
	static void selection_sort(std::vector<int>& nums, sf::RenderWindow& window) {
		for (int i = 0; i < nums.size(); i++) {
			int min_idx = min_elm_idx(i, nums, window);
			std::swap(nums[i], nums[min_idx]);
		}
	}
};

// Main Function
int main() {
	// Init Nums
	int size_of_nums;
	std::cout << "> Enter size of array you want to sort: ";
	std::cin >> size_of_nums;
	std::vector<int> nums = randomVec(size_of_nums, windowSize.y / 1.25f);

	// Init Available Algos
	std::vector<std::string> avail_algos = { "Merge Sort" , "Quick Sort", "Insertion Sort", "Count Sort", "Bubble Sort", "Selection Sort" };

	// Menu
	int choice = show_menu(avail_algos);

	// Window
	sf::RenderWindow window(sf::VideoMode(windowSize), "Visual Sorting");
	window.setVerticalSyncEnabled(true);

	// Main loop
	while (window.isOpen()) {
		std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
		switch (choice) {
		case 1:
			title.setString(avail_algos[0]);
			title.setCharacterSize(title_ch_size);
			Merge_Sort::merge_sort(0, size_of_nums - 1, nums, window);
			std::cout << "> Time taken: " << calc_time_span(start) << "s" << std::endl;
			break;
		case 2:
			title.setString(avail_algos[1]);
			title.setCharacterSize(title_ch_size);
			Quick_Sort::quick_sort(0, size_of_nums - 1, nums, window);
			std::cout << "> Time taken: " << calc_time_span(start) << "s" << std::endl;
			break;
		case 3:
			title.setString(avail_algos[2]);
			title.setCharacterSize(title_ch_size);
			Insertion_Sort::insertion_sort(nums, window);
			std::cout << "> Time taken: " << calc_time_span(start) << "s" << std::endl;
			break;
		case 4:
			title.setString(avail_algos[3]);
			title.setCharacterSize(title_ch_size);
			Count_Sort::count_sort(nums, window);
			std::cout << "> Time taken: " << calc_time_span(start) << "s" << std::endl;
			break;
		case 5:
			title.setString(avail_algos[4]);
			title.setCharacterSize(title_ch_size);
			Bubble_Sort::bubble_sort(nums, window);
			std::cout << "> Time taken: " << calc_time_span(start) << "s" << std::endl;
			break;
		case 6:
			title.setString(avail_algos[5]);
			title.setCharacterSize(title_ch_size);
			Selection_Sort::selection_sort(nums, window);
			std::cout << "> Time taken: " << calc_time_span(start) << "s" << std::endl;
			break;
		default:
			break;
		}

		window.close();
	}

	return 0;
}

// Manage events
void manage_events(sf::RenderWindow& window) {
	while (const std::optional event = window.pollEvent()) {
		if (event->is<sf::Event::Closed>()) {
			window.close();
		}
	}
}

// Calculate the time spanned
float calc_time_span(const std::chrono::time_point<std::chrono::high_resolution_clock>& start) {
	std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	return duration.count() / 1000.0;
}

// Display Menu
int show_menu(const std::vector<std::string>& avail_algos) {
	std::cout << "> Menu:" << std::endl;
	for (int i = 0; i < avail_algos.size(); i++) {
		std::cout << '\t' << i + 1 << ": " << avail_algos[i] << std::endl;
	}
	int choice;
	std::cout << "> Which algo you wanna watch: ";
	std::cin >> choice;
	return choice;
}

// Generate Random Vector
std::vector<int> randomVec(const int size, const float maxHeight) {
	std::vector<int> nums(size);
	for (int i = 1; i <= size; i++) {
		nums[i - 1] = static_cast<int>(i * (maxHeight / size));
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
		bar.setSize({ width_of_bar, static_cast<float>(nums[i]) });
		bar.setPosition({ i * width_of_bar, static_cast<float>(windowSize.y - nums[i]) });
		bar.setFillColor((i == hgt_idx) ? hgtBarCol : barCol);
		window.draw(bar);
	}
}