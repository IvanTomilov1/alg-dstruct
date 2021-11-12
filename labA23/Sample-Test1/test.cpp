#include "pch.h"

extern "C" {
#include "WordSorting.h"
}

TEST(DivideTextToWordsFunctionTesting, OneWordCase) {
	string input = "word";

	int desired_res_len = 1;
	string desired_res_arr[] = { "word" };

	ListOfUniqueWords res = DivideTextToWords(input);

	EXPECT_EQ(res.len, desired_res_len);

	for (int i = 0; i < desired_res_len; i++) {
		EXPECT_STREQ(res.arr[i], desired_res_arr[i]);
	}
}

TEST(DivideTextToWordsFunctionTesting, MultipleWordsCase) {
	string input = "this text is for testing function that divides text to list of words";

	int desired_res_len = 13;
	string desired_res_arr[] = { "this", "text", "is", "for", "testing", "function", "that", "divides", "text", "to", "list", "of", "words" };

	ListOfUniqueWords res = DivideTextToWords(input);

	EXPECT_EQ(res.len, desired_res_len);

	for (int i = 0; i < desired_res_len; i++) {
		EXPECT_STREQ(res.arr[i], desired_res_arr[i]);
	}
}


TEST(InsertWordToPositionTesting, MiddleCases) {
	string input[6] = { "a", "a", "a", "a", "a"};
	string desired_res[6] = { "a", "a", "word", "a", "a", "a" };

	string insert = "word";
	int position = 2;
	string* res = InsertWordToPosition(input, insert, position, 5);

	for (int i = 0; i < 6; i++) {
		EXPECT_STREQ(res[i], desired_res[i]);
		//EXPECT_TRUE(res[i] == desired_res[i]);
	}
}

TEST(InsertWordToPositionTesting, EndCase) {
	string input[6] = { "a", "a", "a", "a", "a" };
	string desired_res[6] = { "a", "a", "a", "a", "a", "word" };

	string insert = "word";
	int position = 5;
	string* res = InsertWordToPosition(input, insert, position, 5);

	for (int i = 0; i < 6; i++) {
		EXPECT_STREQ(res[i], desired_res[i]);
	}
}

TEST(InsertWordToPositionTesting, StartCase) {
	string input[6] = { "a", "a", "a", "a", "a" };
	string desired_res[6] = { "word", "a", "a", "a", "a", "a" };

	string insert = "word";
	int position = 0;
	string* res = InsertWordToPosition(input, insert, position, 5);

	for (int i = 0; i < 6; i++) {
		EXPECT_STREQ(res[i], desired_res[i]);
	}
}

TEST(ReadUniqueStringsFromArrayWithAutomaticSortingTesting, OnlyOneUniqueWord) {
	string input[9] = { "a", "a", "a", "a", "a", "a", "a", "a", "a" };
	ListOfUniqueWords desired_res;
	desired_res.len = 1;
	desired_res.arr = (string*)malloc(1 * sizeof(string));
	desired_res.arr[0] = "a";

	ListOfUniqueWords res = ReadUniqueStringsFromArrayWithAutomaticSorting(input, 9);

	EXPECT_EQ(res.len, desired_res.len);

	for (int i = 0; i < 1; i++) {
		EXPECT_STREQ(res.arr[i], desired_res.arr[i]);
	}
}

TEST(ReadUniqueStringsFromArrayWithAutomaticSortingTesting, BasicLexicographicalSorting) {
	string input[26] = { "z", "y", "x", "w", "v", "u", "t", "s", "r", "q", "p", "o", "n", "m", "l", "k", "j", "i", "h", "g", "f", "e", "d", "c", "b", "a" };
	int desired_res_len = 26;
	string desired_res_arr[] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z" };

	ListOfUniqueWords res = ReadUniqueStringsFromArrayWithAutomaticSorting(input, 26);

	EXPECT_EQ(res.len, desired_res_len);

	for (int i = 0; i < 26; i++) {
		EXPECT_STREQ(res.arr[i], desired_res_arr[i]);
	}
}

TEST(ReadUniqueStringsFromArrayWithAutomaticSortingTesting, LengthSorting) {
	string input[10] = { "a", "aa", "aaa", "aaaa", "aaaaa", "aaaaaa", "aaaaaaa", "aaaaaaaa", "aaaaaaaaa", "aaaaaaaaaa"};
	int desired_res_len = 10;
	string desired_res_arr[] = { "aaaaaaaaaa", "aaaaaaaaa", "aaaaaaaa", "aaaaaaa", "aaaaaa", "aaaaa", "aaaa", "aaa", "aa", "a" };

	ListOfUniqueWords res = ReadUniqueStringsFromArrayWithAutomaticSorting(input, 10);

	EXPECT_EQ(res.len, desired_res_len);

	for (int i = 0; i < 10; i++) {
		EXPECT_STREQ(res.arr[i], desired_res_arr[i]);
	}
}

TEST(ReadUniqueStringsFromArrayWithAutomaticSortingTesting, LengthAndLexicographicalSorting) {
	string input[15] = { "a", "b", "c", "d", "ab", "ac", "ad", "bc", "bd", "cd", "abc", "abd", "acd", "bcd", "abcd"};
	int desired_res_len = 15;
	string desired_res_arr[] = { "abcd", "abc", "abd", "acd", "bcd", "ab", "ac", "ad", "bc", "bd", "cd", "a", "b", "c", "d" };

	ListOfUniqueWords res = ReadUniqueStringsFromArrayWithAutomaticSorting(input, 15);

	EXPECT_EQ(res.len, desired_res_len);

	for (int i = 0; i < 15; i++) {
		EXPECT_STREQ(res.arr[i], desired_res_arr[i]);
	}
}

TEST(ReadUniqueStringsFromArrayWithAutomaticSortingTesting, LengthAndLexicographicalSortingWithRepetitions) {
	string input[26] = { "a", "a", "a", "b", "b", "c", "d", "ab", "ac", "ad", "ad", "ad", "bc", "bd", "cd", "abc", "abd", "abd", "abd", "acd", "bcd", "abcd", "a", "a", "a", "f" };
	int desired_res_len = 16;
	string desired_res_arr[] = { "abcd", "abc", "abd", "acd", "bcd", "ab", "ac", "ad", "bc", "bd", "cd", "a", "b", "c", "d", "f" };

	ListOfUniqueWords res = ReadUniqueStringsFromArrayWithAutomaticSorting(input, 26);

	EXPECT_EQ(res.len, desired_res_len);

	for (int i = 0; i < 15; i++) {
		EXPECT_STREQ(res.arr[i], desired_res_arr[i]);
	}
}