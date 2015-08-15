/*******************************************************************************
********************************************************************************
*
*  Author:           Lingurar Petru-Mugurel
*  Written:          12 Aug 2015, 18:43:17:783
*  Last updated:           ---
*
*  Compilation:  gcc -std=c11 -Wall -Werror -Wextra -pedantic -Wshadow
*   (gcc 5.1)
*
*  Description:
*  Will read all words from a user indicated text file, counting all the words
*  that are repeated and at the end print a beautiful dynamically
*  constructed table.
*
*  Bugs:
*  --- None ---
*
*  TODO:
*  --- None ---
*
*  Notes:
*  Sudoku is for pussies!
*
********************************************************************************
*******************************************************************************/



#include <stdio.h>
#include <ctype.h>          // isspace
#include <string.h>         // strcat
#include <stdlib.h>         // malloc
#include <time.h>
#include <assert.h>
#include <stdbool.h>        // C99 addition


// will use a linked list to hold our "map" of words
typedef struct words {
    // data used to construct the table
    char *word;
    size_t repetitions;

    struct words *next;
} Words;

// Words *head = NULL;


char* get_input_file();
Words *count_words(char*);
void print_words_table(Words*, const size_t);
size_t calculate_rep_width(size_t);     // recursive function
void superfluous_delay();               // but cool nonetheless
void blastoff();                        // create some tension
void clear_stdin();
void print_specifier(int, char);
void free_list(Words*);



int main()
{
    puts("\n\t\tWords Table\n");

    puts("\nWelcome!"
         "\nThis program will:"
         "\n\t- read a text file"
         "\n\t- count how many times each word occurs"
         "\n\t- present this info into a beautiful dinamically constructed "
         "table");

    puts("\n\nSo let's get to it!\n");

    //char *input_file_name = get_input_file();

    puts("\nNow that we have a valid input file"
         "\nI'm gonna do my magic on it. Might take a while...");

    //superfluous_delay();
    Words *words_map = count_words("jb.txt");
    //superfluous_delay();

    puts("\nDone!");
    //blastoff();

    // print our table
    print_words_table(words_map, 200);

    // even though the OS will probably free the heap allocated memory when the
    // program exists, it is good practice to always clean after us ourself
    free_list(words_map);

    //exit(EXIT_SUCCESS);
    //getchar();
    puts("\n\n\nThat's it folks!\n\n");
    return 0;
}



char* get_input_file()
{
    typedef enum {
        FALSE, TRUE
    } Bool;

    Bool valid_input = FALSE;
    Bool valid_name = FALSE;
    FILE *input_file = NULL;
    char *input_file_name = NULL;

    while (!valid_name)
    {
        clear_stdin();
        printf("\nPlease enter the name of the input file:"
               "\n(Full address location if not in the current directory)"
               "\n(max 1000 chars) -> ");
        input_file_name = calloc(1000, 1);
        if (input_file_name == NULL)
        {
            perror("\nMemory allocation failed.\nSadly.\n");
            exit(EXIT_FAILURE);
        }
        fgets(input_file_name, 1000, stdin);
        // remove the '\n' appended by fgets
        input_file_name[strcspn(input_file_name, "\n")] = '\0';
        // resize the array
        realloc(input_file_name, strlen(input_file_name) + 1);
        assert(input_file != NULL);

        input_file = fopen(input_file_name, "r");
        if (!input_file)
        {
            puts("\nError! The input file could not be opened!");
            abramburica:
            printf("Try again? [N] / [Y] -> ");
            char try_again;
            while ((try_again = fgetc(stdin)) == '\n')
                continue;
            if (try_again == 'Y' || try_again == 'y')
                continue;
            else
            {
                puts("\nHasta la vista baby!\n");
                blastoff();
                exit(EXIT_SUCCESS);
            }
        }
        else
        {
            valid_name = TRUE;
            break;
        }
    }


    // if we have a valid input file, check to see if it has any characters
    while (!valid_input)
    {
        int ch;     // int for to accommodate EOF
        // read every character in the file in hope there's one alnum char
        if ((ch = fgetc(input_file)) != EOF)
        {
            if (isalnum(ch))
            {
                valid_input = TRUE;
                break;
            }
        }
        else
        {
            puts("\nError: The input file does not contain any valid "
                 "characters!");
            goto abramburica;
        }
    }

    // if we have a valid input_file, return it.
    if (valid_name && valid_input) {
        //rewind(input_file);     // read from it to find an alnum character
        fclose(input_file);
        return input_file_name;
    }
}


Words *count_words(char * input_file_name)
{
    // We'll use a dynamic array to hold the word read. More fun.
    size_t word_capacity, current_capacity;
    current_capacity = word_capacity = 6;       // fits best with my text

    // Construct a new Words linked list. To be returned at end.
    Words *head = malloc(sizeof(Words));
    assert(head != NULL);
    Words *tail = malloc(sizeof(Words));
    assert(tail != NULL);
    head = tail = NULL;

    int ch;                // buffer for every char read from the input file
    size_t char_count;     // will count the characters for each word read

    FILE *input_file = fopen(input_file_name, "r");
    assert(input_file != NULL);
    while ((ch = fgetc(input_file)) != EOF)
    {
        // ignore any non-valid characters
        if (!isalnum(ch))
            continue;       // bad char, start again

        // reset current_capacity in the event that it was increased below
        current_capacity = word_capacity;

        // create a new dynamically allocated array to hold the word
        char *word_read = malloc(word_capacity);
        assert(word_read != NULL);
        char_count = 0;
        bool bad_ch = false;

        while (isalnum(ch))                 /* read the word */
        {
            if (char_count == current_capacity - 1) // -1 for the \0
            {
                current_capacity *= 1.5;
                // make sure to avoid a memory leak if realloc fails
                char *more_chars = realloc(word_read, current_capacity);
                if (more_chars != NULL)
                    word_read = more_chars;
            }
            word_read[char_count] = ch;    /* store it */
            char_count++;
            ch = fgetc(input_file);

        }
        word_read[char_count] = '\0';      /* cap word with NULL */

        // construct our linked list, ahem map of words
        Words *current = head;
        bool duplicate = false;
        while (current != NULL)
        {
            if (strcasecmp(current->word, word_read) == 0)
            {
                current->repetitions++;
                duplicate = true;
                break;
            }
            current = current->next;
        }

        // if we have a new word, create a new node and append it to the list
        if (!duplicate)
        {
            Words *new_node = malloc(sizeof(Words));
            assert(new_node != NULL);
            new_node->word = word_read;
            new_node->repetitions = 1;
            new_node->next = NULL;

            // construct the first node of our list
            if (head == NULL) {
                head = tail = new_node;
            }
            else    // if we have at least one constructed node in our list
            {
                tail->next = new_node;
                tail = new_node;
            }
        }
    }

    fclose(input_file);

    return head;
}


void print_words_table(Words *head, const size_t table_width)
{
    // TODO: Implement a small check to assure the required "table_width"
    // accommodates our widest word. Read all words, store the biggest width,
    // In this case call a new small function to ask the user to
    // enter a new value for table width and from that function call again this
    // function. Whew !


    Words *current = head;
    Words *currentY = head;
    size_t max_rep = 0;

    //TODO: in functiile de mai jos foloseste current nu currentY

    // get the maximum highest number of identical words
    // will determine the length of our Y axis
    while (currentY != NULL)
    {
        if (currentY->repetitions > max_rep)
            max_rep = currentY->repetitions;
        currentY = currentY->next;
    }

    // get the total number of identical words in our list
    // will determine the length of our X axis
    currentY = head;
    size_t total_words_no = 0;
    while (currentY != NULL)
    {
        total_words_no++;
        currentY = currentY->next;
    }


    // for the table to look just right will need to calculate at runtime
    // the space occupied by the max_rep variable - how many spaces it occupies
    char timesx[] = "times x 1";
    size_t timesx_width = strlen(timesx);
    size_t rep_width = calculate_rep_width(max_rep);
    timesx_width += rep_width;
    size_t current_width = timesx_width;

    bool need_another_table = false;

    // print the table body
    puts("\n\n");
    while (max_rep > 0) {
        printf("times x %u | ", max_rep--);
        currentY = head;
        current_width = 0;
        while (currentY != NULL)
        {
            size_t word_width = strlen(currentY->word);
            current_width += word_width + 1;   // +1 for the words separator
            if (current_width > table_width)
            {
                need_another_table = true;
                current = currentY;
                currentY = NULL;
            }
            else
            {
                if (currentY->repetitions > max_rep)
                    // call a small function to make sure this will be centered
                    // +1 because we print the words with delimiters between
                    print_specifier(word_width, '*');
                else
                    print_specifier(word_width, ' ');  // for debugging

                currentY = currentY->next;
            }
        }
        putchar('\n');
    }


    // print the words at the bottom of the table


    // use the same trick from above to print center aligned
    // printf ("%*s\n", space/2 + strlen(string)/2,string);
//    for (size_t x = 0; x != current_width; x++)
//        putchar('-');
//    putchar('\n');
    printf("%*s%*c ", (int)timesx_width-2, "Word", (int)timesx_width/2-2, '|');
    Words *currentX = head;

    while (currentX != (current != head ? current : NULL)) {
        printf("%s ", currentX->word);
        currentX = currentX->next;
    }

    if (need_another_table)
        print_words_table(current, table_width);
}


size_t calculate_rep_width(size_t rep)
{
    static int count = 0;
    // TODO: intreaba-l pe Horia cum resetezi count la 0
    count++;
    if (rep >= 10)
        calculate_rep_width(rep / 10);

    return count;
}


void superfluous_delay()
{
    time_t then;
    time(&then);
    srand((unsigned)time(NULL));
    // get a random float number from 0.0 to 2.0
    float delay = (float)(rand() / (float)(RAND_MAX / 2.0f));
    while (difftime(time(NULL), then) < 1);
    puts("\n\t...");
}


void blastoff()
{
    for (int x = 3; x != 0; x--)
    {
        time_t then;
        time(&then);
        while (difftime(time(NULL), then) < 1.0);
        printf("\n\t%d", x);
    }
}


void clear_stdin()
{
    while (getchar() != '\n')
        ;
}


void print_specifier(int width, char symbol)
{
    /* We have 3 possible scenarios */
    // 1 - width is one
    if (width == 1)
        printf("%c|", symbol);

        // 2 - width is an even number
    else if ((width % 2) == 0)
        printf("%*c%c%*c", width / 2, ' ',
               symbol,
               width / 2, '|');

        // 3 - width is an odd number
    else if ((width % 2) == 1)
        printf("%*c%c%*c", width / 2, ' ',
               symbol,
               width - width / 2, '|');
}


void free_list(Words* head)
{
    Words* tmp = NULL;

    while (head != NULL)
    {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}