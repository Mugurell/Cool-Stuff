/*******************************************************************************
********************************************************************************
*
*  Author:           Lingurar Petru-Mugurel
*  Written:          12 Aug 2015, 18:43:17:783
*  Last updated:     16 Aug 2015, 16:49:55:353
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
#include <stdbool.h>        // C99 addition


// will use a linked list to hold our "map" of words
typedef struct words {
    // data used to construct the table
    char *word;
    unsigned repetitions;

    struct words *next;
} Words;

// Words *head = NULL;

// get a valid input file
char* get_input_file();
// construct the linked list: every word and the number of repetitions
Words *count_words(char*);
// print the words with their number of repetitions
// in a table constructed at runtime
void print_words_table(Words*, unsigned);

// little helper functions
bool valid_table_width(Words*, unsigned*);
unsigned calculate_rep_width(unsigned);
unsigned calculate_timesx_width(const unsigned);
void print_specifier(int, char);
void print_timesx(unsigned, const unsigned);
void free_list(Words*);
void superfluous_delay();               // but cool nonetheless
void blastoff();                        // create some tension
void clear_stdin();                     // consume any leftover input



int main()
{
    puts("\n\t\tWords Table\n");

    puts("\nWelcome!"
         "\nThis program will:"
         "\n\t- read a text file"
         "\n\t- count how many times each word occurs"
         "\n\t- present this info into a beautiful, dinamically constructed "
         "table");

    puts("\n\nSo let's get to it!\n");

    char *input_file_name = get_input_file();

    puts("\nNow that we have a valid input file"
         "\nI'm gonna do my magic on it. Might take a while...");

    superfluous_delay();
    Words *words_map = count_words(input_file_name);
    superfluous_delay();

    puts("\nDone!\n\n"
         "\nA nice table can now be drawn based on the content of your file");
    printf("\nAbout that, how wide would you want the table to be? -> ");
    unsigned preffered_table_widtd;
    fscanf(stdin, "%u", &preffered_table_widtd);

    blastoff();     // create some tension..

    // print our table
    print_words_table(words_map, preffered_table_widtd);

    // even though the OS will probably free the heap allocated memory when the
    // program exists, it is good practice to always clean after us ourself
    free_list(words_map);

    puts("\n\n\nThat's it folks!\n\n");
    clear_stdin();
    getchar();

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
        printf("\nPlease enter the name of the input file:"
               "\n(Full address location if not in the current directory)"
               "\n(max 1000 chars) -> ");
        input_file_name = calloc(1000, 1);
        if (input_file_name == NULL)
        {
            perror("Allocating memory for the input file name\n");
            exit(EXIT_FAILURE);
        }
        fgets(input_file_name, 1000, stdin);
        // remove the '\n' appended by fgets
        input_file_name[strcspn(input_file_name, "\n")] = '\0';
        // resize the array
        realloc(input_file_name, strlen(input_file_name) + 1);

        input_file = fopen(input_file_name, "r");
        if (!input_file)
        {
            perror("\nError");
            abramburica:;

            char try_again;
            while (try_again != 'Y' || try_again != 'y')
            {
                printf("\nTry again? [N] / [Y] -> ");
                try_again = fgetc(stdin);
                switch (try_again)
                {
                    case 'Y':
                    case 'y':
                        clear_stdin();
                        break;
                    case 'N':
                    case 'n':
                        puts("\nHasta la vista baby!\n");
                        blastoff();
                        exit(EXIT_SUCCESS);
                        break;      // unreachable code, yeah..
                    default:
                        clear_stdin();
                        break;
                }
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

    fclose(input_file);

    // if we get here means we have a valid input file
    return input_file_name;
}


Words *count_words(char * input_file_name)
{
    // We'll use a dynamic array to hold the word read. More fun.
    unsigned word_capacity, current_capacity;
    current_capacity = word_capacity = 6;       // fits best with my text

    // Construct a new Words linked list. To be returned at end.
    Words *head = malloc(sizeof(Words));
    if (head == NULL) {
        perror("Construct the head of the linked list\n");
        exit (EXIT_FAILURE);
    }
    Words *tail = malloc(sizeof(Words));
    if (tail == NULL)
    {
        perror("Construct the tail of the linked list\n");
        exit (EXIT_FAILURE);
    }
    head = tail = NULL;

    int ch;                // buffer for every char read from the input file
    unsigned char_count;     // will count the characters for each word read

    FILE *input_file = fopen(input_file_name, "r");
    if (input_file == NULL)
    {
        perror("Error: Couldn't open the input file\n");
        exit(EXIT_FAILURE);
    }
    while ((ch = fgetc(input_file)) != EOF)
    {
        // ignore any non-valid characters
        if (!isalnum(ch))
            continue;       // bad char, start again

        // reset current_capacity in the event that it was increased below
        current_capacity = word_capacity;

        // create a new dynamically allocated array to hold the word
        char *word_read = malloc(word_capacity);
        if (word_read == NULL)
        {
            perror("Allocating memory for the read word\n");
            exit (EXIT_FAILURE);
        }
        char_count = 0;

        while (isalnum(ch))                 /* read the word */
        {
            if (char_count == current_capacity - 1) // -1 for the \0
            {
                current_capacity *= 1.5;
                // make sure to avoid a memory leak if realloc fails
                char *more_chars = realloc(word_read, current_capacity);
                if (more_chars != NULL)
                    word_read = more_chars;
                else {
                    perror("Realloc of the read word failed!\n");
                    exit (EXIT_FAILURE);
                }
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
            if (new_node == NULL)
            {
                perror("Cannot construct a new node for he linked list\n");
                exit(EXIT_FAILURE);
            }
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


void print_words_table(Words *head, unsigned table_width)
{
    // TODO: Implement a small check to assure the required "table_width"
    // accommodates our widest word. Read all words, store the biggest width,
    // In this case call a new small function to ask the user to
    // enter a new value for table width and from that function call again this
    // function. Whew !

    // First assure that the requested table_width accomodates the widest word.

    if (!valid_table_width(head, &table_width))
        return;

    Words *current = head;      // current list head used for normal operations
    Words *currentY = head;     // current list head used in drawing the Y axis
    Words *currentX = head;     // current list head used in drqwing the X axis
    unsigned max_rep = 0;         // highest number of word repetitions

    // get the maximum highest number of identical words
    // will determine the length of our Y axis
    while (current != NULL)
    {
        if (current->repetitions > max_rep)
            max_rep = current->repetitions;
        current = current->next;
    }

    // get the total number of identical words in our list
    // will determine the length of our X axis
    current = head;
    unsigned total_words_no = 0;
    while (current != NULL)
    {
        total_words_no++;
        current = current->next;
    }

    // for the table to look just right will need to calculate at runtime
    // the space occupied by the max_rep variable - how many spaces it occupies
    unsigned timesx_initial_width = calculate_timesx_width(max_rep);

    // if the requested table width is smaller than the total width of the
    // identical words plus the table's structure
    bool need_another_table = false;



    // print the table body
    puts("\n\n");
    while (max_rep > 0) {
        print_timesx(timesx_initial_width, max_rep);
        currentY = head;
        unsigned current_width = 0;
        while (currentY != NULL)
        {
            unsigned word_width = strlen(currentY->word);
            current_width += word_width + 1;   // +1 for the words separator
            if (current_width > table_width)
            {
                need_another_table = true;
                current = currentY;
                currentY = NULL;
            }
            else
            {
                if (currentY->repetitions >= max_rep)
                    // call a small function to make sure this will be centered
                    // +1 because we print the words with delimiters between
                    print_specifier(word_width, '*');
                else
                    print_specifier(word_width, ' ');  // for debugging

                currentY = currentY->next;
            }
        }
        putchar('\n');
        max_rep--;
    }


    printf("%*s%*c ", (int)timesx_initial_width-2, "Word",
           (int)timesx_initial_width/2-2, '|');

    while (currentX != (current != head ? current : NULL)) {
        printf("%s ", currentX->word);
        currentX = currentX->next;
    }

    if (need_another_table)
        print_words_table(current, table_width);
}


bool valid_table_width(Words *head, unsigned *table_width)
{
    // find the biggest width for our words in the list
    Words *current = head;
    unsigned biggest_width = 0;

    while (current != NULL)
    {
        unsigned current_width = strlen(current->word);
        if (current_width > biggest_width)
            biggest_width = current_width;
        current = current->next;
    }

    // if the biggest_width is bigger than the requested table width,
    // ask the user for a new width
    while (*table_width < biggest_width)
    {
        puts("\nSeems like your requested table width is smaller than at least"
             "\none of the words which are to be printed.");

        bool valid_choice = false;
        while (!valid_choice)
        {
            clear_stdin();
            printf("\nEnter a New size [N]"
                   "\n\tor\nQuit the program [Q]:\n\t-> ");
            char choice = fgetc(stdin);
            switch (choice)
            {
                case 'Q':
                case 'q':
                    puts("\n\nOk then..\nBye!\n");
                    return false;
                case 'N':
                case 'n':
                    valid_choice = true;
                    break;
                default:
                    puts("\nHmmm.....\n");
                    break;
            }
        }

        printf("\n\nPlease enter a new size for the table width bigger than %u"
               "\nor type Q to quit this program: -> ", biggest_width);
        fscanf(stdin, "%u", table_width);
    }

    // There were only 2 ways to end this function
    // 1 - the user entered 'Q' to quit -> the switch-case will return false
    // 2 - enter a valid width, the while loop ends, and so we return true
    return true;
}


unsigned calculate_rep_width(unsigned rep)
{
    int count = 0;
    while (rep != 0) {
        rep /= 10;
        count++;
    }

    return count;
}


unsigned calculate_timesx_width(const unsigned max_rep)
{
    char timesx[] = "times x 1";
    unsigned timesx_width = strlen(timesx);
    unsigned rep_width = calculate_rep_width(max_rep);

    return timesx_width + rep_width;
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


void print_timesx(unsigned timesx_init_width, const unsigned curr_rep)
{
    printf("times x %d", curr_rep);
    unsigned curr_timesx_width = calculate_timesx_width(curr_rep);
    while (timesx_init_width-- > curr_timesx_width)
        putchar(' ');
    printf(" | ");
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


void superfluous_delay()
{
    time_t then;
    time(&then);
    srand((unsigned)time(NULL));
    // get a random float number from 0.0 to 2.0
    float delay = (float)(rand() / (float)(RAND_MAX / 2.0f));
    while (difftime(time(NULL), then) < delay);
    puts("\n\t...");
}


void blastoff()
{
    for (int x = 3; x > 0; x--)
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