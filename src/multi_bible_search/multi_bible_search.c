#include <limits.h>
#include <Python.h>
#include <stdint.h>
#include <string.h>
#include "hashtable.h"
#include "rank.h"

#define NUM_TABLES 31

// This is an ever so slight, single use optimization over itoa
static inline void ref_to_str(uint_fast16_t num, char* str) {
    // This covers the range of possible chapter and verse values of a reference. 
    static const char* numbers[] = { "\0", // NULL sentinel
        "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
        "11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
        "21", "22", "23", "24", "25", "26", "27", "28", "29", "30",
        "31", "32", "33", "34", "35", "36", "37", "38", "39", "40",
        "41", "42", "43", "44", "45", "46", "47", "48", "49", "50",
        "51", "52", "53", "54", "55", "56", "57", "58", "59", "60",
        "61", "62", "63", "64", "65", "66", "67", "68", "69", "70",
        "71", "72", "73", "74", "75", "76", "77", "78", "79", "80",
        "81", "82", "83", "84", "85", "86", "87", "88", "89", "90",
        "91", "92", "93", "94", "95", "96", "97", "98", "99", "100",
        "101", "102", "103", "104", "105", "106", "107", "108", "109", "110",
        "111", "112", "113", "114", "115", "116", "117", "118", "119", "120",
        "121", "122", "123", "124", "125", "126", "127", "128", "129", "130",
        "131", "132", "133", "134", "135", "136", "137", "138", "139", "140",
        "141", "142", "143", "144", "145", "146", "147", "148", "149", "150",
        "151", "152", "153", "154", "155", "156", "157", "158", "159", "160",
        "161", "162", "163", "164", "165", "166", "167", "168", "169", "170",
        "171", "172", "173", "174", "175", "176", "\0" // NULL sentinel
    };
    
    strcat(str, numbers[num]);
}
static inline void ref_to_str_colon(uint_fast16_t num, char* str) {
    // This covers the range of possible chapter and verse values of a reference. 
    static const char* numbers[] = { "\0", // NULL sentinel
        "1:", "2:", "3:", "4:", "5:", "6:", "7:", "8:", "9:", "10:", 
        "11:", "12:", "13:", "14:", "15:", "16:", "17:", "18:", "19:", 
        "20:", "21:", "22:", "23:", "24:", "25:", "26:", "27:", "28:", 
        "29:", "30:", "31:", "32:", "33:", "34:", "35:", "36:", "37:", 
        "38:", "39:", "40:", "41:", "42:", "43:", "44:", "45:", "46:", 
        "47:", "48:", "49:", "50:", "51:", "52:", "53:", "54:", "55:", 
        "56:", "57:", "58:", "59:", "60:", "61:", "62:", "63:", "64:", 
        "65:", "66:", "67:", "68:", "69:", "70:", "71:", "72:", "73:", 
        "74:", "75:", "76:", "77:", "78:", "79:", "80:", "81:", "82:", 
        "83:", "84:", "85:", "86:", "87:", "88:", "89:", "90:", "91:", 
        "92:", "93:", "94:", "95:", "96:", "97:", "98:", "99:", "100:", 
        "101:", "102:", "103:", "104:", "105:", "106:", "107:", "108:", 
        "109:", "110:", "111:", "112:", "113:", "114:", "115:", "116:", 
        "117:", "118:", "119:", "120:", "121:", "122:", "123:", "124:", 
        "125:", "126:", "127:", "128:", "129:", "130:", "131:", "132:", 
        "133:", "134:", "135:", "136:", "137:", "138:", "139:", "140:", 
        "141:", "142:", "143:", "144:", "145:", "146:", "147:", "148:", 
        "149:", "150:", "151:", "152:", "153:", "154:", "155:", "156:", 
        "157:", "158:", "159:", "160:", "161:", "162:", "163:", "164:", 
        "165:", "166:", "167:", "168:", "169:", "170:", "171:", "172:", 
        "173:", "174:", "175:", "176:", "\0" // NULL sentinel
    };
    
    strcat(str, numbers[num]);
}

// Translate numeric references to Python strings
static inline PyObject* rtranslate(long reference) {
    // Buffer for up to the longest reference would be 20,
    // but it is bumped up to 24 for the longest invalid reference
    char reference_buffer[24];
    uint_fast8_t book = reference / 1000000;        // Extract the part of the number that represents the book
    uint_fast16_t remainder = reference % 1000000;  // Extract the remaining part after removing the book
    uint_fast16_t chapter = remainder / 1000;       // Extract the chapter reference
    uint_fast16_t verse = remainder % 1000;         // Extract the verse reference

    // Add to the reference buffer based on the book number
    switch(book) {
        case 1:
            strcpy(reference_buffer, "Genesis ");
            break;
        case 2:
            strcpy(reference_buffer, "Exodus ");
            break;
        case 3:
            strcpy(reference_buffer, "Leviticus ");
            break;
        case 4:
            strcpy(reference_buffer, "Numbers ");
            break;
        case 5:
            strcpy(reference_buffer, "Deuteronomy ");
            break;
        case 6:
            strcpy(reference_buffer, "Joshua ");
            break;
        case 7:
            strcpy(reference_buffer, "Judges ");
            break;
        case 8:
            strcpy(reference_buffer, "Ruth ");
            break;
        case 9:
            strcpy(reference_buffer, "1 Samuel ");
            break;
        case 10:
            strcpy(reference_buffer, "2 Samuel ");
            break;
        case 11:
            strcpy(reference_buffer, "1 Kings ");
            break;
        case 12:
            strcpy(reference_buffer, "2 Kings ");
            break;
        case 13:
            strcpy(reference_buffer, "1 Chronicles ");
            break;
        case 14:
            strcpy(reference_buffer, "2 Chronicles ");
            break;
        case 15:
            strcpy(reference_buffer, "Ezra ");
            break;
        case 16:
            strcpy(reference_buffer, "Nehemiah ");
            break;
        case 17:
            strcpy(reference_buffer, "Esther ");
            break;
        case 18:
            strcpy(reference_buffer, "Job ");
            break;
        case 19:
            strcpy(reference_buffer, "Psalms ");
            break;
        case 20:
            strcpy(reference_buffer, "Proverbs ");
            break;
        case 21:
            strcpy(reference_buffer, "Ecclesiastes ");
            break;
        case 22:
            strcpy(reference_buffer, "Song of Solomon ");
            break;
        case 23:
            strcpy(reference_buffer, "Isaiah ");
            break;
        case 24:
            strcpy(reference_buffer, "Jeremiah ");
            break;
        case 25:
            strcpy(reference_buffer, "Lamentations ");
            break;
        case 26:
            strcpy(reference_buffer, "Ezekiel ");
            break;
        case 27:
            strcpy(reference_buffer, "Daniel ");
            break;
        case 28:
            strcpy(reference_buffer, "Hosea ");
            break;
        case 29:
            strcpy(reference_buffer, "Joel ");
            break;
        case 30:
            strcpy(reference_buffer, "Amos ");
            break;
        case 31:
            strcpy(reference_buffer, "Obadiah ");
            break;
        case 32:
            strcpy(reference_buffer, "Jonah ");
            break;
        case 33:
            strcpy(reference_buffer, "Micah ");
            break;
        case 34:
            strcpy(reference_buffer, "Nahum ");
            break;
        case 35:
            strcpy(reference_buffer, "Habakkuk ");
            break;
        case 36:
            strcpy(reference_buffer, "Zephaniah ");
            break;
        case 37:
            strcpy(reference_buffer, "Haggai ");
            break;
        case 38:
            strcpy(reference_buffer, "Zechariah ");
            break;
        case 39:
            strcpy(reference_buffer, "Malachi ");
            break;
        case 40:
            strcpy(reference_buffer, "Matthew ");
            break;
        case 41:
            strcpy(reference_buffer, "Mark ");
            break;
        case 42:
            strcpy(reference_buffer, "Luke ");
            break;
        case 43:
            strcpy(reference_buffer, "John ");
            break;
        case 44:
            strcpy(reference_buffer, "Acts ");
            break;
        case 45:
            strcpy(reference_buffer, "Romans ");
            break;
        case 46:
            strcpy(reference_buffer, "1 Corinthians ");
            break;
        case 47:
            strcpy(reference_buffer, "2 Corinthians ");
            break;
        case 48:
            strcpy(reference_buffer, "Galatians ");
            break;
        case 49:
            strcpy(reference_buffer, "Ephesians ");
            break;
        case 50:
            strcpy(reference_buffer, "Philippians ");
            break;
        case 51:
            strcpy(reference_buffer, "Colossians ");
            break;
        case 52:
            strcpy(reference_buffer, "1 Thessalonians ");
            break;
        case 53:
            strcpy(reference_buffer, "2 Thessalonians ");
            break;
        case 54:
            strcpy(reference_buffer, "1 Timothy ");
            break;
        case 55:
            strcpy(reference_buffer, "2 Timothy ");
            break;
        case 56:
            strcpy(reference_buffer, "Titus ");
            break;
        case 57:
            strcpy(reference_buffer, "Philemon ");
            break;
        case 58:
            strcpy(reference_buffer, "Hebrews ");
            break;
        case 59:
            strcpy(reference_buffer, "James ");
            break;
        case 60:
            strcpy(reference_buffer, "1 Peter ");
            break;
        case 61:
            strcpy(reference_buffer, "2 Peter ");
            break;
        case 62:
            strcpy(reference_buffer, "1 John ");
            break;
        case 63:
            strcpy(reference_buffer, "2 John ");
            break;
        case 64:
            strcpy(reference_buffer, "3 John ");
            break;
        case 65:
            strcpy(reference_buffer, "Jude ");
            break;
        case 66:
            strcpy(reference_buffer, "Revelation ");
            break;
        default:
            // Return None
            Py_RETURN_NONE;
    }

    // Quick bounds check
    if (verse > 176 || chapter > 176) { Py_RETURN_NONE; }

    // Add the chapter number and colon
    ref_to_str_colon(chapter, reference_buffer);

    // Add the verse number
    ref_to_str(verse, reference_buffer);

    return PyUnicode_FromStringAndSize(reference_buffer, strlen(reference_buffer));
}

// Tokenizes a given string based on spaces
static inline char **tokenize(const char *input_string, int *num_tokens, int *len_tokens) {
    // Allocate memory for token array
    char **tokens = calloc(strlen(input_string) + 1, sizeof(char *));
    if (tokens == NULL) {
        // Handle memory allocation failure
        return NULL;
    }

    const char *ptr = input_string;
    const char *start;
    int count = 0;

    // Tokenize the string based on spaces
    while (*ptr != '\0') {
        // Skip leading non-alphabetic characters
        while (*ptr != '\0' && !isalpha(*ptr)) {
            ptr++;
        }

        // Find the start of the token
        start = ptr;

        // Find the end of the token
        while (*ptr != '\0' && isalpha(*ptr)) {
            ptr++;
        }

        // Extract the token
        if (ptr != start) {
            tokens[count] = malloc(ptr - start + 1);
            if (tokens[count] == NULL) {
                // Handle memory allocation failure
                for (int i = 0; i < count; i++) {
                    free(tokens[i]);
                }
                free(tokens);
                return NULL;
            }
            strncpy(tokens[count], start, ptr - start);
            tokens[count][ptr - start] = '\0';
            count++;
        }
    }
    *len_tokens = sizeof(tokens);
    *num_tokens = count;
    return tokens;
}

// Structure to hold data for the module
typedef struct {
    PyObject_HEAD
    PyObject *versions;
    struct hashtable **ht;
} SearchObject;

// Pair of associated references
typedef struct pair{
    // Version index
    short a;
    // Extra index (if applicable)
    short b;
} pair;


// Allocates empty tables
void allocate_tables(SearchObject *self) {
    self->ht = calloc(NUM_TABLES, sizeof(struct hashtable));
    if (self->ht == NULL) {
        printf("Error allocating internal tables\n");
        return;
    }
    for (int i = 0; i < NUM_TABLES; i++) {
        self->ht[i] = (struct hashtable*) malloc(sizeof(struct hashtable));
        if (self->ht[i] == NULL) {
            printf("Error allocating internal table\n");
            return;
        }
        self->ht[i]->elements = NULL;
        self->ht[i]->size = 0;
        self->ht[i]->num_elements = 0;
    }
}

// Associates a version with a table index, and extra index if applicable
pair get_table_index(const char* version) {
    // All is assumed, and 0
    // KJV-Like is 1
    // NIV is 2
    pair indices;
    indices.b = 0;
    switch (version[0])
    {
    case 'A':
        switch (version[1])
        {
        // ACV
        case 'C':
            indices.a = 3;
            break;
        // AKJV
        case 'K':
            indices.a = 4;
            indices.b = 1;
            break;
        // AMP
        case 'M':
            indices.a = 5;
            break;
        // ASV
        case 'S':
            indices.a = 6;
            break;
        default:
            indices.a = 0;
            break;
        }
        break;

    case 'B':
        switch (version[1])
        {
        // BBE
        case 'B':
            indices.a = 7;
            break;
        // BSB
        case 'S':
            indices.a = 8;
            break;
        default:
            indices.a = 0;
            break;
        }
        break;

    case 'C':
        // CSB
        indices.a = 9;
        break;

    case 'D':
        switch (version[1])
        {
        // Darby
        case 'a':
            indices.a = 10;
            break;
        // DRA
        case 'R':
            indices.a = 11;
            break;

        default:
            indices.a = 0;
            break;
        }
        break;

    case 'E':
        switch (version[1])
        {
        // EBR
        case 'B':
            indices.a = 12;
            break;
        // ESV
        case 'S':
            indices.a = 13;
            break;

        default:
            indices.a = 0;
            break;
        }
        break;

    // GNV
    case 'G':
        indices.a = 14;
        break;

    case 'K':
        switch (strlen(version))
        {
        // KJV
        case 3:
            indices.a = 15;
            indices.b = 1;
            break;
        // KJV 1611
        case 8:
            indices.a = 16;
            indices.b = 1;
            break;

        default:
            indices.a = 0;
            break;
        }
        break;

    // LSV
    case 'L':
        indices.a = 17;
        break;

    // MSG
    case 'M':
        indices.a = 18;
        break;

    case 'N':
        switch (version[1])
        {
        // NASB 1995
        case 'A':
            indices.a = 19;
            break;

        // NET
        case 'E':
            indices.a = 20;
            break;

        // NIVs
        case 'I':
            // NIV 1984
            if (version[4] == '1') {
                indices.a = 21;
            }
            // NIV 2011
            else {
                indices.a = 22;
            }
            indices.b = 2;
            break;

        // NKJV
        case 'K':
            indices.a = 23;
            break;

        // NLT
        case 'L':
            indices.a = 24;
            break;

        default:
            indices.a = 0;
            break;
        }
        break;

    case 'R':
        switch (version[1])
        {
        // RNKJV
        case 'N':
            indices.a = 25;
            indices.b = 1;
            break;

        // RSV
        case 'S':
            indices.a = 26;
            break;

        // RWV
        case 'W':
            indices.a = 27;
            break;

        default:
            indices.a = 0;
            break;
        }
        break;

    // UKJV
    case 'U':
        indices.a = 28;
        indices.b = 1;
        break;

    // WEB
    case 'W':
        indices.a = 29;
        break;

    // YLT
    case 'Y':
        indices.a = 30;
        break;

    default:
        indices.a = 0;
        break;
    }
    return indices;
}

// Function to initialize the SearchObject
static int SearchObject_init(SearchObject *self, PyObject *args) {
    allocate_tables(self);
    return 0;
}

// Free the object's dynamically allocated memory
static void SearchObject_destructor(SearchObject* self) {
    if (self->ht != NULL) {
        for (int i = 0; i < NUM_TABLES; i++) {
            if (self->ht[i] != NULL) {
                delete_table(self->ht[i]);
                free(self->ht[i]);
            }
        }
        free(self->ht);
    }
}

// Get the versions list
static PyObject *SearchObject_versions(SearchObject *self, void *closure) {
    Py_INCREF(self->versions);
    return self->versions;
}

// Method to perform a search
PyObject *SearchObject_search(SearchObject *self, PyObject *args) {
    if (!self->ht) {
        return PyList_New(0);
    }
    char *query1,     // The query string
         *version,    // The version to query
         **tokens;    // The tokenized form of the query
    // Maximum number of results to return to Python
    Py_ssize_t max_results = PY_SSIZE_T_MAX;

    if (!PyArg_ParseTuple(args, "ss|n", &query1, &version, &max_results)) {
        PyObject *exception_type = PyExc_RuntimeError;
        PyObject *exception_value = PyUnicode_FromString("Bad search arguments!\n");
        PyObject *exception_traceback = NULL;
        PyErr_SetObject(exception_type, exception_value);

        // Return None just in case
        Py_RETURN_NONE;
    }
    make_lower(query1);
    // Hash table indicies to get from
    pair table_index = get_table_index(version);

    // Python list of results
    PyObject* result_list = PyList_New(0);

    // Temporary pointer to the Python string of the reference
    PyObject* str_ref;

    int num_tokens = 0,     // Number of tokens in the query
        len_tokens = 0;     // Allocated length of the token list

    // Tokenize the query
    tokens = tokenize(query1, &num_tokens, &len_tokens);

    // Pointer to the C list of results
    long *token_result_list = NULL;
    size_t result_count = 0,            // Current number of results
           token_result_list_len = 0;   // Allocated length of the result list

    // Temporary result pointers
    struct element *result_all = NULL,         // Results of all versions
                   *result_version = NULL,     // Results of the particular version
                   *result_combined = NULL;    // Results from any combined index (if applicable)

    // If the version is invalid, return. Just in case something is wrong in the Python adapter
    if (!table_index.a) {
        free(tokens);
        return result_list;
    }

    // Make sure we have tokens, then search
    if (tokens) {
        for (int i = 0; i < num_tokens; i++) {
            // Get results for all, adding to the number of total results
            result_all = get_element(self->ht[0], tokens[i]);
            token_result_list_len += result_all != NULL ? result_all->length : 0;

            // Get results for the particular version, adding to the number of total results
            result_version = get_element(self->ht[table_index.a], tokens[i]);
            token_result_list_len += result_version != NULL ? result_version->length : 0;

            // If there is a combined index, search that too
            if (table_index.b != 0) {
                result_combined = get_element(self->ht[table_index.b], tokens[i]);
                token_result_list_len += result_combined != NULL ? result_combined->length : 0;
            }

            // Copy previous token results
            if (result_count && token_result_list_len > result_count) { //  && token_result_list ?
                token_result_list = realloc(token_result_list, sizeof(long) * token_result_list_len);
                if (token_result_list == NULL) {
                    printf("Internal allocation error\n");
                    return result_list;
                }
            }
            else if (token_result_list_len > 0 && token_result_list == NULL) {
                // Allocate a temporary list of results
                token_result_list = malloc(sizeof(long) * token_result_list_len);
                if (token_result_list == NULL) {
                    printf("Internal allocation error\n");
                    return result_list;
                }
            }
            else {
                continue;
            }

            // Add results from all
            if (result_all != NULL) {
                // Merge results from all
                merge(token_result_list, result_count, result_all->value, result_all->length);
                result_count += result_all->length;
            }

            // Get results for version:
            if (result_version != NULL) {
                // Merge results from this version
                merge(token_result_list, result_count, result_version->value, result_version->length);
                result_count += result_version->length;
            }

            // If applicable, get results from extra index:
            if (result_combined != NULL) {
                // Merge any results from the extra index
                merge(token_result_list, result_count, result_combined->value, result_combined->length);
                result_count += result_combined->length;
            }
        }
        // Free the dynamically allocated tokens
        for (int i = 0; i < len_tokens; i++)
        {
            // Since tokens is over allocated, we can just stop at the first NULL
            if (tokens[i] == NULL) {
                break;
            }
            free(tokens[i]);
        }
        // Free the list of tokens
        free(tokens);
    }
    else {
        // Raise exception?
        printf("Failed to tokenize query\n");
    }

    // Rank the results, storing the length of the deduplicated portion of the array
    result_count = rank(token_result_list, token_result_list_len, num_tokens);

    for (size_t i = 0; i < result_count && i < token_result_list_len && i < max_results; i++) {
        // Translate the reference and add it to the Python list
        str_ref = rtranslate(token_result_list[i]);
        // Make sure the result isn't None. Basically another double check of the Python side of things.
        if (str_ref == Py_None) {
            // If it was None, free it
            Py_XDECREF(str_ref);
        }
        else {
            // Add the resulting Python string to the list
            PyList_Append(result_list, str_ref);

            // We're done with it, so decrement the reference counter.
            Py_DecRef(str_ref);
        }
    }

    // Free the dynamically allocated list 
    if (token_result_list != NULL) {
        free(token_result_list);
    }

    // Give Python it's form of the results
    return result_list;
}

/* 
 * Load an index of either a version or multiple versions.
 * Ideally, this would take in a file pointer or file name and just do the parsing work on the C side of things.
 * For now, this works even if slow.
 */ 
PyObject *SearchObject_load(SearchObject *self, PyObject *args) {
    // The dictionary from Python
    PyObject* input_dict;

    // The version string being loaded
    char* version;
    if (!PyArg_ParseTuple(args, "O|s", &input_dict, &version)) {
        PyObject *exception_type = PyExc_RuntimeError;
        PyObject *exception_value = PyUnicode_FromString("Error getting loading arguments!\n");
        PyObject *exception_traceback = NULL;
        PyErr_SetObject(exception_type, exception_value);

        // Return None just in case
        Py_RETURN_NONE;
    }
    // The index of the table to load into
    short table_index;
    // Check if the version is a combined index
    if (!strcmp(version, "All")) {
        table_index = 0;
    }
    else if (!strcmp(version, "KJV-like")) {
        table_index = 1;
    }
    else if (!strcmp(version, "NIV")) {
        table_index = 2;
    }
    // If not, use `get_table_index` to find the right one for this version
    else {
        table_index = get_table_index(version).a;
        // Make sure thtat the version is valid
        if (!table_index) {
            // Raise an exception for the invalid version
            printf("Error! %d\n", table_index);
            char error_buff[100];
            strncpy(error_buff, "Invalid version: ", sizeof(error_buff) - 1);
            strncat(error_buff, version, sizeof(error_buff) - 1);
            PyObject *exception_type = PyExc_RuntimeError;
            PyObject *exception_value = PyUnicode_FromString(error_buff);
            PyObject *exception_traceback = NULL;
            PyErr_SetObject(exception_type, exception_value);

            // Return None just in case
            Py_RETURN_NONE;
        }
    }
    if (self->ht[table_index]->size > 0) {
        Py_RETURN_NONE;
    }

    PyObject *key,      // Dictionary key for iteration
             *value,    // Pointer to the (list) value in question
             *long_ptr; // Pointer to a number stored in the current list
    Py_ssize_t pos = 0;
    // printf("Loading dictionary\n");

    // Iterate through the dictionary
    while (PyDict_Next(input_dict, &pos, &key, &value)) {
        // Length of the current list 
        Py_ssize_t list_len = PyList_Size(value);

        // Make a new C list of longs from the length of the Python one
        long *long_list = (long *) malloc(sizeof(long) * list_len);
        if (long_list == NULL) {
            // Raise an exception for the invalid version
            printf("Error! %d\n", table_index);
            char error_buff[100];
            strncpy(error_buff, "Memory allocation l failed when loading index!\n", sizeof(error_buff));
            PyObject *exception_type = PyExc_RuntimeError;
            PyObject *exception_value = PyUnicode_FromString(error_buff);
            PyErr_SetObject(exception_type, exception_value);

            // Return None just in case
            PyObject* none = Py_None;
            Py_IncRef(none);
            return none;
        }
        // Copy the Python list into the C one
        for (Py_ssize_t i = 0; i < list_len; i++) {
            long_ptr = PyList_GET_ITEM(value, i);
            long_list[i] = PyLong_AsLong(long_ptr);
        }
        // Create a new element for the hashtable
        struct element *e = (struct element*) malloc(sizeof(struct element));
        if (e == NULL) {
            free(long_list);
            // Raise an exception for the memory allocation failure
            printf("Error! %d\n", table_index);
            char error_buff[100];
            strncpy(error_buff, "Memory allocation e failed when loading index!\n", sizeof(error_buff) - 1);
            PyObject *exception_type = PyExc_RuntimeError;
            PyObject *exception_value = PyUnicode_FromString(error_buff);
            PyErr_SetObject(exception_type, exception_value);

            // Return None just in case
            Py_RETURN_NONE;
        }
        
        const char *c_key;  // Pointer to the key from Python

        // Get the key from the Python dictionary and use it for the new element
        c_key = PyUnicode_AsUTF8(key);

        // Build the element
        strncpy(e->key, c_key, sizeof(e->key) - 1);
        e->value = long_list;
        e->length = list_len;

        // And add it
        add_element(self->ht[table_index], e);
    }
    Py_RETURN_NONE;
}

PyObject *SearchObject_unload(SearchObject *self, PyObject *args) {
    // If the hashtable DNE, then just return
    if (!self->ht) {
        Py_RETURN_NONE;
    }
    char* version;  // The version to unload
    if (!PyArg_ParseTuple(args, "s", &version)) {
        // Raise a runtime error if the argument is not a string
        PyObject *exception_type = PyExc_RuntimeError;
        PyObject *exception_value = PyUnicode_FromString("Error getting unload version arguments!\n");
        PyErr_SetObject(exception_type, exception_value);

        // Return None just in case
        Py_RETURN_NONE;
    }
    // The index in the table to remove
    short table_index = get_table_index(version).a;
    // Make sire that the input string was a valid version
    if (!table_index) {
        // Raise an exception for the invalid version
        printf("Error! %d\n", table_index);
        char error_buff[100];
        strncpy(error_buff, "Invalid version: ", sizeof(error_buff) - 1);
        strncat(error_buff, version, sizeof(error_buff) - 1);
        PyObject *exception_type = PyExc_RuntimeError;
        PyObject *exception_value = PyUnicode_FromString(error_buff);
        PyObject *exception_traceback = NULL;
        PyErr_SetObject(exception_type, exception_value);

        // Return None just in case
        Py_RETURN_NONE;
    }
    // Just return none if there is nothing to free
    if (self->ht[table_index] == NULL) {
        Py_RETURN_NONE;
    }

    // Free the table's dynamically allocated memory
    delete_table(self->ht[table_index]);

    // Zero out the relevant attributes for potential later use
    self->ht[table_index]->elements = NULL;
    self->ht[table_index]->size = 0;
    self->ht[table_index]->num_elements = 0;

    Py_RETURN_NONE;
}

PyObject *SearchObject_index_size(SearchObject *self, PyObject *args) {
    // If the hashtable DNE, then just return
    if (!self->ht) {
        Py_RETURN_NONE;
    }
    // Accumulator for the number of bytes here
    unsigned long num_bytes = 0;
    // Loop through each table
    for (int i = 0; i < NUM_TABLES; i++) {
        // If the table is not allocated, just skip that
        if (self->ht[i] == NULL) {
            continue;
        }
        // Loop through all of the elements
        for (size_t j = 0; j < self->ht[i]->size; j++) {
            if (self->ht[i]->elements[j] != NULL) {
                // Add the size of the struct itself
                num_bytes += sizeof(struct element);
                // Also add the size of the array pointed to by the element
                num_bytes += self->ht[i]->elements[j]->length * sizeof(long);
            }
        }
        // Add the size of the hash table's array of elements
        num_bytes += self->ht[i]->size * sizeof(struct element*);
    }
    num_bytes += sizeof(struct hashtable) * NUM_TABLES;
    // The object's struct size is included in the definition of the object, which will be read by Python, so I won't add that here as well
    return PyLong_FromUnsignedLong(num_bytes);
}

// Method definitions
static PyMethodDef SearchObject_methods[] = {
    {"search", (PyCFunction)SearchObject_search, METH_VARARGS, "Search method"},
    {"load", (PyCFunction)SearchObject_load, METH_VARARGS, "Load dict method"},
    {"unload", (PyCFunction)SearchObject_unload, METH_VARARGS, "Unload version method"},
    {"index_size", (PyCFunction)SearchObject_index_size, METH_VARARGS, "Gets the size of the index in bytes"},
    {NULL} // Sentinel
};

// Getter definition for the versions attribute
static PyGetSetDef SearchObject_getsetters[] = {
    {"versions", (getter)SearchObject_versions, NULL, "List of versions", NULL},
    {NULL} // Sentinel
};

// Definition of the BibleSearch
static PyTypeObject BibleSearch = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "BibleSearch",
    .tp_doc = "BibleSearch",
    .tp_basicsize = sizeof(SearchObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc)SearchObject_init,
    .tp_dealloc = (destructor)SearchObject_destructor,
    .tp_methods = SearchObject_methods,
    .tp_getset = SearchObject_getsetters,
};

// Module initialization function
static PyModuleDef biblesearch = {
    PyModuleDef_HEAD_INIT,
    .m_name = "biblesearch",
    .m_doc = "",
    .m_size = -1
};


// Module initialization entry point
PyMODINIT_FUNC PyInit_multi_bible_search(void) {
    PyObject *m;
    if (PyType_Ready(&BibleSearch) < 0) {
        return NULL;
    }
    m = PyModule_Create(&biblesearch);
    if (m == NULL) {
        return NULL;
    }
    Py_INCREF(&BibleSearch);
    PyModule_AddObject(m, "BibleSearch", (PyObject *)&BibleSearch);
    return m;
}
