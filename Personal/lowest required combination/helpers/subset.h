#ifndef SUBSET_H
#define SUBSET_H

#include <vector>

// Helper functions to extract the used supplementary ints from Joey's solution
bool findSubset(const std::vector<int>& nums, int target, std::vector<int>& result, size_t index) {
    if (target == 0) {
        return true;
    }
    if (target < 0 || index == nums.size()) {
        return false;
    }

    // Include nums[index] in the subset and move to the next element
    result.push_back(nums[index]);
    if (findSubset(nums, target - nums[index], result, index + 1)) {
        return true;
    }
    // Exclude nums[index] from the subset and move to the next element
    result.pop_back();
    if (findSubset(nums, target, result, index + 1)) {
        return true;
    }

    return false;
}

std::vector<int> subsetSum(const std::vector<int>& nums, int target) {
    std::vector<int> result;
    if (findSubset(nums, target, result, 0)) {
        return result;
    }
    return {}; // Return an empty vector if no subset is found
}

#endif // SUBSET_H