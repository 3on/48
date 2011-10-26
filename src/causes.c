#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "levenshtein.h"

void friendsFor(int iword, size_t wordCount, char**words, int* network)
{
  size_t i = 0;
  size_t l;
  char* word = words[iword];
  
  while(i < wordCount)
  {
    if (words[i] && (network[i] == -1))
      l = levenshtein_distance(word, words[i]);
    else
      return ;
    
    if ( (l == 1) )
    {
      network[i] = iword;
      friendsFor(i, wordCount, words, network);
    }
    ++i;
  }
}

size_t initArrayWords(char** words)
{
  FILE*   file;
  size_t  wordCount = 0;
  char*   buffer;
  
  file = fopen("word.list", "r");
  buffer = malloc(100);
  while( fscanf(file, "%s\n", buffer) > 0)
  {
    words[wordCount] = malloc(100);
    strcpy(words[wordCount], buffer);
    wordCount++;
  }
  fclose(file);
  words[wordCount] = NULL;
  free(buffer);
  
  return wordCount;
}

int* initNetwork(size_t wordCount)
{
  int* network;
  size_t  i = 0;
  
  network = malloc(wordCount * sizeof(int));
  while ( i < wordCount)
  {
    network[i] = -1;
    ++i;
  }
  
  return network;
}

int wordsId(char** words, size_t wordCount, char* word)
{
  int     iword;
  size_t  i = 0;
  
  i = 0;
  while (i < wordCount)
  {
    if (0 == strcmp(word, words[i]))
    {
      iword = i;
      break;
    }
    ++i;
  }
  
  return iword;
}

void  saveNetwork(char** words, size_t wordCount, int* network)
{
  FILE*   output;
  size_t  i = 0;

  output = fopen("word.network", "w");
	printf("\t\tsaving as 'word.network'\n");
  i = 0;  
  while( i < wordCount) {
    fprintf(output, "%zu %s %i", i, words[i], network[i]);
    if (network[i] != -1 )
      fprintf(output, " %s\n", words[network[i]]);
    else
      fprintf(output, "\n");
    ++i;
  }
  fclose(output);
}

void nodeJSON(char** words, int root, size_t wordCount, int* network, FILE* output)
{
  size_t  i = 0;
  char    coma = 0;
  
  fprintf(output, "{\"id\" : \"%i\", \"name\" : \"%s\", \"children\" : [", root, words[root]);
  while (i <wordCount)
  {
    if (root == network[i])
    {
      if (coma)
        fprintf(output, ", ");
      nodeJSON(words, i, wordCount, network, output);
      coma = 1;
    }
    ++i;
  }
  fprintf(output, "]}");
}

void  saveNetworkJSON(char** words, int root, size_t wordCount, int* network)
{
  FILE*   output;
  
  output = fopen("network.js", "w");
	printf("\t\tsaving as 'network.js'\n");
  fprintf(output, "json = ");
  nodeJSON(words, root, wordCount, network, output);

  fclose(output);
}


void printNetworkInfo(char** words, int iword, size_t wordCount,int*  network)
{
  size_t  i = 0;
  size_t  netWorkSize = 0;
  int     friends[wordCount];
  int     mostFriends;
  int     max = -1;
  
  while (i < wordCount)
  {
    friends[i] = 0;
     ++i;
  }
  
  i = 0;
  while (i < wordCount)
  {
    if (network[i] != -1)
    {
        friends[network[i]]++;
        ++netWorkSize;
    }
    ++i;
  }
  
  i = 0;
  while (i < wordCount)
  {
    if (friends[i] > max)
    {
      max = friends[i];
      mostFriends = i;
    }
    ++i;
  }
  printf("\tnetwork size: %zu\n", netWorkSize);
  printf("\t'%s' has most friends (%i friends)\n", words[mostFriends], max);
}

int main (int argc, char const *argv[])
{
  char**   words;
  int*    network;
  size_t  wordCount = 0;
  char*   word;
  int     iword;
  
  words = malloc(sizeof(char*) * 264070);
  
  /*
    Init the array of words
  */
  printf("-> Loading words\n");
  wordCount = initArrayWords(words);
  printf("\tnumber of words: %zu\n", wordCount );
  printf("\tfirst word: %s\n\tlast word: %s\n", words[0], words[wordCount - 1]);
  
  /*
    Init the array networks
     - network[key] will give -1 is not in the network
     - or the key of the words with who he is friend
  */
  printf("-> Creating the 'network'\n");
  network = initNetwork(wordCount);  
  
  /*
    Call the recursive function which looks for relations between words/friends
  */
  word = (char*) "causes";
  iword = wordsId(words, wordCount, word);
  printf("\t'%s' has for id '%i' in array words\n", word, iword);
  friendsFor(iword, wordCount, words, network);
  
  /*
    Print some data
  */
  printf("-> Network informations\n");
  printNetworkInfo(words, iword, wordCount, network);
  
  /*
    Save the result to a file
  */
  printf("-> Saving the 'network'\n");
  
  printf("\texporting in json\n");
  saveNetworkJSON(words, iword, wordCount, network);
  
  printf("\texporting row text\n");
  saveNetwork(words, wordCount, network);
  
  return 0;
}