#include <Python.h>
#include <stdint.h>
#include <string.h>
#include "hashtable.h"
#include "rank.h"

#define NUM_TABLES 31

// This is an ever so slight, single use optimization over itoa
void ref_to_str(uint_fast16_t num, char* str) {
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
        "171", "172", "173", "174", "175", "176", 0 // NULL sentinel
    };
    
    strcat(str, numbers[num]);
}

// Translate numeric references to Python strings
static PyObject* rtranslate(long reference) {
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
    ref_to_str(chapter, reference_buffer);
    strcat(reference_buffer, ":");

    // Add the verse number
    ref_to_str(verse, reference_buffer);

    return PyUnicode_FromStringAndSize(reference_buffer, strlen(reference_buffer));
}

// Tokenizes a given string based on spaces
char **tokenize(const char *input_string, int *num_tokens, int *len_tokens) {
    // Allocate memory for token array
    char **tokens = calloc(strlen(input_string), sizeof(char *));
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
    return 0;
    PyObject *init_dict = NULL;
    // Parse arguments
    if (!PyArg_ParseTuple(args, "O", &init_dict)) {
        return -1;
    }
    allocate_tables(self);
    printf("Object instantiated\n");
    return 0;
}

static void SearchObject_destructor(SearchObject* self) {
    for (int i = 0; i < NUM_TABLES; i++) {
        delete_table(self->ht[i]);
    }
    free(self->ht);
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

    if (!PyArg_ParseTuple(args, "ss", &query1, &version)) {
        PyObject *exception_type = PyExc_RuntimeError;
        PyObject *exception_value = PyUnicode_FromString("Bad search arguments!\n");
        PyObject *exception_traceback = NULL;
        PyErr_SetObject(exception_type, exception_value);

        // Return None just in case
        Py_RETURN_NONE;
    }
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
    //printf("Table index: a=%d, b=%d; num tokens: %d\n", table_index.a, table_index.b, num_tokens);

    if (!table_index.a) {
        free(tokens);
        return result_list;
    }

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
                memcpy(&token_result_list[result_count], result_all->value, result_all->length * sizeof(long));
                /*for (size_t i = 0; i < result_all->length; i++) {
                    token_result_list[result_count + i] = result_all->value[i];
                }*/
                result_count += result_all->length;
            }

            // Get results for version:
            if (result_version != NULL) {
                memcpy(&token_result_list[result_count], result_version->value, result_version->length * sizeof(long));
                /*for (size_t i = 0; i < result_version->length; i++) {
                    token_result_list[result_count + i] = result_version->value[i];
                }*/
                result_count += result_version->length;
            }

            // If applicable, get results from extra index:
            if (result_combined != NULL) {
                memcpy(&token_result_list[result_count], result_combined->value, result_combined->length * sizeof(long));
                /*for (size_t i = 0; i < result_combined->length; i++) {
                    token_result_list[result_count + i] = result_combined->value[i];
                }*/
                result_count += result_combined->length;
            }
        }
        // Free the dynamically allocated tokens
        for (int i = 0; i < len_tokens; i++)
        {
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
    //printf("Ranking\n");
    quickSort(token_result_list, 0, token_result_list_len - 1);
    //printf("Sorted\n");
    result_count = rank(token_result_list, token_result_list_len, num_tokens);
    //printf("Num tokens: %d; result count: %zd, result list len: %zd\n", num_tokens, result_count, token_result_list_len);

    for (size_t i = 0; i < result_count && i < token_result_list_len; i++) {
        // Translate the reference and add it to the Python list
        str_ref = rtranslate(token_result_list[i]);
        if (str_ref == Py_None) {
            //result_count++;
            Py_DecRef(str_ref);
        }
        else {
            PyList_Append(result_list, str_ref);

            // We're done with it, and adding it increments the reference counter, so decrement that.
            Py_DecRef(str_ref);
        }
    }
    //printf("\nBuilt result list\n");

    // Free the dynamically allocated list
    if (token_result_list != NULL) {
        //printf("Try free! %p\n", result_list);
        free(token_result_list);
        //printf("We're free!\n");
    }

    return result_list;
}

// Load an index
PyObject *SearchObject_load(SearchObject *self, PyObject *args) {
    if (!self->ht) {
        allocate_tables(self);
    }
    PyObject* input_dict;
    char* version;
    if (!PyArg_ParseTuple(args, "O|s", &input_dict, &version)) {
        PyObject *exception_type = PyExc_RuntimeError;
        PyObject *exception_value = PyUnicode_FromString("Error getting loading arguments!\n");
        PyObject *exception_traceback = NULL;
        PyErr_SetObject(exception_type, exception_value);

        // Return None just in case
        Py_RETURN_NONE;
    }
    short table_index;
    if (!strcmp(version, "All")) {
        table_index = 0;
    }
    else if (!strcmp(version, "KJV-like")) {
        table_index = 1;
    }
    else if (!strcmp(version, "NIV")) {
        table_index = 2;
    }
    else {
        table_index = get_table_index(version).a;
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

    PyObject *key, *value, *long_ptr;
    Py_ssize_t pos = 0;
    // printf("Loading dictionary\n");

    // Iterate through the dictionary
    while (PyDict_Next(input_dict, &pos, &key, &value)) {
        Py_ssize_t list_len = PyList_Size(value);

        // Make a new C list of longs from the Python one
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
        for (Py_ssize_t i = 0; i < list_len; i++) {
            // printf("Getting item %d\n", i);
            long_ptr = PyList_GetItem(value, i);
            long_list[i] = PyLong_AsLong(long_ptr);
        }
        // printf("Got all items, adding the element\n");
        // New element for the hashtable
        struct element *e = (struct element*) malloc(sizeof(struct element));
        if (e == NULL) {
            free(long_list);
            // Raise an exception for the invalid version
            printf("Error! %d\n", table_index);
            char error_buff[100];
            strncpy(error_buff, "Memory allocation e failed when loading index!\n", sizeof(error_buff) - 1);
            PyObject *exception_type = PyExc_RuntimeError;
            PyObject *exception_value = PyUnicode_FromString(error_buff);
            PyErr_SetObject(exception_type, exception_value);

            // Return None just in case
            PyObject* none = Py_None;
            Py_IncRef(none);
            return none;
        }
        const char *c_key;
        c_key = PyUnicode_AsUTF8(key);
        strncpy(e->key, c_key, sizeof(e->key) - 1);
        e->value = long_list;
        e->length = list_len;
        add_element(self->ht[table_index], e);
    }
    // printf("Loaded\n");
    Py_RETURN_NONE;
}

// Method definitions
static PyMethodDef SearchObject_methods[] = {
    {"search", (PyCFunction)SearchObject_search, METH_VARARGS, "Search method"},
    {"load", (PyCFunction)SearchObject_load, METH_VARARGS, "Load dict method"},
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