#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "WordSorting.h"

int IsFirstWordLengthGreaterThanSecond(string w1, string w2) {
	if (strlen(w1) > strlen(w2)) {
		return 1;
	}
	else if (strlen(w1) == strlen(w2)) {
		return -1;
	}
	else {
		return 0;
	}
}


int IsFirstWordLexicographicallyFirstThanSecond(string w1, string w2) {
	int isw2lexicographycallyafter = strcmp(w1, w2);
	if (isw2lexicographycallyafter < 0) {
		return 0;
	}
	else if (isw2lexicographycallyafter == 0) {
		return -1;
	}
	else {
		return 1;
	}
}

string ReadTextFromFile(FILE* file) {
	assert(file != NULL);
	fseek(file, 0, SEEK_END);
	FILE* end = file;
	fseek(file, 0, SEEK_SET);
	int i = 0;
	int number_of_symbols = end - file + 1;
	string res = (string)malloc(number_of_symbols*sizeof(string));
	assert(res != NULL);
	do {
		fread(res[i], sizeof(string), 1, file);
		i++;
	} while (file != EOF);
	res[i] = '\0';
	return res;
}

ListOfUniqueWords DivideTextToWords(string text) {
	int start_of_word = 0, end_of_word = 0;
	int len = 0, n = 0, m = 0;
	int number_of_words = 1, word_number = 0;
	while (text[n] != '\0') {
		if (text[n] == ' ') number_of_words++;
		n++;
	}
	n = 0;
	string* list_of_words = (string*)malloc(number_of_words*sizeof(string));
	assert(list_of_words != NULL);
	ListOfUniqueWords res;
	while (text[n + 1] != '\0') {
		if (text[n] != ' ') {
			string word;
			start_of_word = n;
			m = start_of_word + 1;
			while ((text[m] != ' ') & (text[m] != '\0')) m++;
			end_of_word = m;
			n = m;
			len = end_of_word - start_of_word;
			word = (char*)malloc(len + 1);
			assert(word != NULL);
			for (int i = 0; i < len; i++) {
				word[i] = text[start_of_word + i];
			}
			word[len] = '\0';

			list_of_words[word_number] = word;
			word_number++;
		};
		n++;
	}
	res.len = word_number;
	res.arr = list_of_words;
	return res;
}


string* InsertWordToPosition(string array[], string word, int position, int n_words) {
	int i = 0;
	for (int i = n_words; i > position; i--) {
		array[i] = array[i - 1];
	}
	i = 0;
	array[position] = word;
	return array;
}


ListOfUniqueWords ReadUniqueStringsFromArrayWithAutomaticSorting(string preprocessed_text[], const int number_of_words) {
	int comp_length, comp_lex;
	int position = 0;
	int nunique_words = 1, array_size = number_of_words;
	ListOfUniqueWords sorted_words;
	sorted_words.arr = (string*)malloc(sizeof(string) * array_size);
	assert(sorted_words.arr != NULL);
	if (number_of_words == 0) {
		free(sorted_words.arr);
		sorted_words.arr = NULL;
		sorted_words.len = 0;
		return sorted_words;
	}
	sorted_words.arr[0] = preprocessed_text[0];

	for (int i = 1; i < number_of_words; i++) {
		for (int j = position; j >= 0; j--) {

			comp_length = IsFirstWordLengthGreaterThanSecond(preprocessed_text[i], sorted_words.arr[j]);
			//comp_lex = IsFirstWordLexicographicallyFirstThanSecond(preprocessed_text[i], sorted_words.arr[j]);
			comp_lex = strcmp(preprocessed_text[i], sorted_words.arr[j]);

			if (comp_length == 1) {
				if (j == 0) {
					nunique_words++;
					position++;
					sorted_words.arr = InsertWordToPosition(sorted_words.arr, preprocessed_text[i], j, nunique_words);
					break;
				}
				continue;
			}
			else if (comp_length == 0) {
				nunique_words++;
				position++;
				sorted_words.arr = InsertWordToPosition(sorted_words.arr, preprocessed_text[i], j + 1, nunique_words);
				break;
			}
			else if (comp_length == -1) {
				if (comp_lex < 0) {
					if (j == 0) { 
						nunique_words++;
						sorted_words.arr = InsertWordToPosition(sorted_words.arr, preprocessed_text[i], j, nunique_words);
					}
					// do nothing and move forward
					continue;
				}
				else if (comp_lex > 0) {
					//preprocessed_text[i] goes to sorted_words[j+1] place and remaining part is shifted
					nunique_words++;
					position++;
					sorted_words.arr = InsertWordToPosition(sorted_words.arr, preprocessed_text[i], j+1, nunique_words);
					break;
				}
				else if (comp_lex == 0) {
					break;
				}
			}
		}
	}
	sorted_words.len = nunique_words;
	//realloc(sorted_words.arr, nunique_words + 1);
	return sorted_words;
}


void PrintWordsWithNumberOfLetters(ListOfUniqueWords list) {
	for (int i = 0; i < list.len; i++) {
		printf("%s %d\n", list.arr[i], strlen(list.arr[i]));
	}
}

void PrintWordsLongerThanFixedNumberOfLetters(ListOfUniqueWords list, int N) {
	if (list.arr == NULL) {
		printf("There is no words!");
		return;
	}
	int i = 0;
	for (i = 0; i < list.len; i++) {
		if (strlen(list.arr[i]) <= N) break;
	}
	list.len = i;
	string buf;
	for (int i = 0; i < list.len; i++) {
		for (int j = 1; j < list.len; j++) {
			if (IsFirstWordLexicographicallyFirstThanSecond(list.arr[j], list.arr[i]) == 1) {
				buf = list.arr[i];
				list.arr[i] = list.arr[j];
				list.arr[j] = buf;
			}
		}
	}
	for (int i = 0; i < list.len; i++) {
		printf("%s\n", list.arr[i]);
	}
}

//void main() { return; };