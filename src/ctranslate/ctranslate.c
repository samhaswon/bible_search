#include <Python.h>
#include <string.h>

// This is an ever so slight, single use optimization over itoa
void ref_to_str(int num, char* str) {
    // This covers the range of possible chapter and verse values of a reference. 
    static const char* const numbers[] = {
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
        "171", "172", "173", "174", "175", "176"
    };
    
    strcat(str, numbers[num - 1]);
}

static PyObject* rtranslate(PyObject* self, PyObject* args) {
    long reference;
    const char reference_buffer[20];    // Buffer for up to the longest reference
    if (!PyArg_ParseTuple(args, "l", &reference))
        return NULL;
    int book = reference / 1000000;       // Extract the part of the number that represents the book
    int remainder = reference % 1000000;  // Extract the remaining part after removing the book
    int chapter = remainder / 1000;       // Extract the chapter reference
    int verse = remainder % 1000;         // Extract the verse reference

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
            strcpy(reference_buffer, "Unknown ");
            break;
    }

    // string buffer for the numbers of the reference
    char num_ref_buf[4];

    // Add the chapter number and colon
    ref_to_str(chapter, reference_buffer);
    strcat(reference_buffer, ":");

    // Add the verse number
    ref_to_str(verse, reference_buffer);

    return PyUnicode_FromStringAndSize(reference_buffer, strlen(reference_buffer));
}

static PyMethodDef methods[] = {
    {"rtranslate", rtranslate, METH_VARARGS, "Gets the name of a book based on its index."},
    {NULL, NULL, 0, NULL} // Sentinel
};

static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "rtranslate",   // Module name
    NULL,           // Module documentation
    -1,             // Size of per-interpreter state or -1
    methods         // Method definitions
};

PyMODINIT_FUNC PyInit_ctranslate(void) {
    return PyModule_Create(&moduledef);
}