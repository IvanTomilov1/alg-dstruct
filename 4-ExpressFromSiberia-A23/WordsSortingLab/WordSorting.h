#pragma once

typedef char* string;

typedef struct {
	int len;
	string* arr;
} ListOfUniqueWords;

string ReadTextFromFile(FILE* file);

int IsFirstWordLexicographicallyFirstThanSecond(string w1, string w2);

string* InsertWordToPosition(string array[], string word, int position, int n_words);

ListOfUniqueWords ReadUniqueStringsFromArrayWithAutomaticSorting(string preprocessed_text[], const int number_of_words);

void PrintWordsLongerThanFixedNumberOfLetters(ListOfUniqueWords list, int N);

ListOfUniqueWords DivideTextToWords(string text);

void PrintWordsWithNumberOfLetters(ListOfUniqueWords list);