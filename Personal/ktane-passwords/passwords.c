// find the first matching word from input chars from terminal
// each word is 5 letters long, and each letter space has exactly 6 possible letters it can be
// so in the loop, keep accepting letters from the terminal. the first six letters correspond to the first letter space, next 6 to the second letter space, and so on
// keep accepting letters from terminal until only one match is found

#include <stdio.h>
#include <string.h>

void findMatchingWords(char possible_letters[5][7], const char *wordlist[], int word_count);

int main() {
    const char *wordlist[] = {
        "about", "after", "again", "below", "could", "every", "first", "found", "great", "house",
        "large", "learn", "never", "other", "place", "plant", "point", "right", "small", "sound",
        "spell", "still", "study", "their", "there", "these", "thing", "think", "three", "water", "where", "which", "world", "would", "write"
    };
    const int word_count = sizeof(wordlist) / sizeof(wordlist[0]);
    char possible_letters[5][7]; // 6 letters + null terminator
    char input[7];
    int i, j;

    // Initialize possible letters for each position
    for (i = 0; i < 5; i++) {
        possible_letters[i][0] = '\0';
    }

    // Read letters from terminal
    for (i = 0; i < 5; i++) {
        printf("Enter 6 possible letters for position %d: ", i + 1);
        scanf("%6s", input);
        strncpy(possible_letters[i], input, 7);
        findMatchingWords(possible_letters, wordlist, word_count);
    }

    

    return 0;
}

void findMatchingWords(char possible_letters[5][7], const char *wordlist[], int word_count) {
    int i, j;
    int match_count = 0;
    const char *matched_word = NULL;
    
    for (i = 0; i < word_count; i++) {
        int match = 1;
        for (j = 0; j < 5; j++) {
            for(int k = 0; k < 6; k++) {
                if (wordlist[i][j] == possible_letters[j][k] || possible_letters[j][k] == '\0') {
                    break;
                }
                if (k == 5) {
                    match = 0;
                }
            }
            if (!match) {
                break;
            }
        }
        if (match) {
            match_count++;
            matched_word = wordlist[i];
        }
    }

    if (match_count == 1) {
        printf("Matching word found: %s\n", matched_word);
    } else {
        printf("Number of matching words: %d\n", match_count);
    }
    
}