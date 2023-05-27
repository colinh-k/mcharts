#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>

#include <assert.h>
#include <ctype.h>  // isspace

#define TARGET_URL "https://www.billboard.com/charts/hot-100/"
// these xpaths were determined by manually examining the html from the url
#define XPATH_TRACK "//li[contains(@class, \"o-chart-results-list__item\")]/h3[@id=\"title-of-a-story\"]"
#define XPATH_ARTIST (XPATH_TRACK "/following-sibling::span")

typedef struct {
    // data is nbytes long, and contains exactly that much data at all times
    // i.e., the buffer can only grow, not shrink
    size_t nbytes;
    char *data;
} TextBuffer_t;

// defer cleaning up all allocated data here
void cleanup(CURL *handle, TextBuffer_t *tb) {
    if (handle != NULL)
        curl_easy_cleanup(handle);
    if (tb != NULL) {
        if (tb->data != NULL)
            free(tb->data);
        free(tb);
    }
    // always do global cleanup once here
    curl_global_cleanup();
}

void error(const char *msg) {
    fprintf(stderr, "ERROR: %s\nEXITING...\n", msg);
    exit(EXIT_FAILURE);
}

// callback to process each data chunk as curl recieves it
size_t process_network_data(void *buffer, size_t size, size_t nmemb, void *userp) {
    const size_t nbytes = size * nmemb;
    // char *textbuf = (char *) buffer;

    TextBuffer_t *tb = (TextBuffer_t *) userp;
    // resize the buffer if necessary
    char *temp = realloc(tb->data, tb->nbytes + nbytes);
    if (temp == NULL) {
        fprintf(stderr, "out of memory (realloc returned NULL)\n");
        return 0;
    }

    tb->data = temp;
    memcpy(&(tb->data[tb->nbytes]), buffer, nbytes);
    tb->nbytes += nbytes;

    return nbytes;
}

// allocate a textbuffer via malloc (caller should free all the data buf too)
TextBuffer_t *create_textbuffer() {
    TextBuffer_t *tb = (TextBuffer_t *) malloc(sizeof(TextBuffer_t));
    if (tb == NULL) {
        cleanup(NULL, tb);
        error("failed to initialize text buffer");
    }
    tb->nbytes = 0;
    tb->data = (char *) malloc(1);
    if (tb->data == NULL) {
        cleanup(NULL, tb);
        error("failed to initialize text buffer data");
    }
    return tb;
}

// configure the curl handle with options
CURL *create_handle(TextBuffer_t *tb) {
    CURL *handle = NULL;

    handle = curl_easy_init();
    if (handle == NULL) {
        cleanup(handle, tb);
        error("failed to initialize curl handle");
    }

    // set target url of the resource we want
    curl_easy_setopt(handle, CURLOPT_URL, TARGET_URL);
    // make curl pass all incoming data to our callback functino
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, process_network_data);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *) tb);
    // 2 seconds for connection establishment
    curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, 2L);
    // complete transfer in 20 secs
    curl_easy_setopt(handle, CURLOPT_TIMEOUT, 20L);
    curl_easy_setopt(handle, CURLOPT_HTTPAUTH, CURLAUTH_ANYSAFE);
    return handle;
}

// download the latest chart data via curl
TextBuffer_t *get_data() {
    // gbl init (once per program instance)
    curl_global_init(CURL_GLOBAL_ALL);

    TextBuffer_t *tb = create_textbuffer();
    CURL *handle = create_handle(tb);

    // actual download is performed here (handled via the callback installed when handle was created)
    CURLcode result = curl_easy_perform(handle);
    if (result != CURLE_OK) {
        cleanup(handle, tb);
        error("transfer failed");
    }

    // curl_easy_cleanup(handle);
    // curl_global_cleanup();
    // cleanup(handle, tb);
    cleanup(handle, NULL);
    return tb;
}

// remove leading and trailing whitespace. copies non-white chars
// to the start of the string buffer, and terminates the non-white chars
// with '\0'
void xml_str_strip(xmlChar *str) {
    if (str == NULL) return;
    xmlChar *str_beginning = str;
    // find first non-whitespace char
    while (isspace(*str)) str++;

    // test if there are no non whitespace chars (required, since we are about to traverse backwards)
    if (*str == '\0') return;

    // subtract 1 here in order to reach the correct offset (to see why, work thru an example by hand)
    // pointer to the last non-null char in the string
    xmlChar *str_end = str_beginning + xmlStrlen(str_beginning) - 1;

    // find the first non whitespace char from the end of the string
    while (isspace(*str_end)) str_end--;
    // copy chars between the leading and trailing whitespace
    // to the start of the string
    while (str <= str_end) {
        // continue until end of string
        *str_beginning = *str;
        str++;
        str_beginning++;
    }
    // null terminate the string
    *str_beginning = '\0';
}

int main() {
    // FILE *file = fopen("hot100.html", "r");
    // // move to end of file
    // fseek(file, 0, SEEK_END);
    // // get number of bytes in the file
    // long nbytes = ftell(file);
    // rewind(file);  // reset position to beginning of file

    // // load file into buffer
    // char *buf = malloc(sizeof(char) * nbytes);
    // fread(buf, sizeof(char), nbytes, file);

    TextBuffer_t *tb = get_data();

    // parse the html
    int opts = HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING |
               HTML_PARSE_NOBLANKS | HTML_PARSE_NONET;
    htmlDocPtr doc = htmlReadMemory(tb->data, tb->nbytes, TARGET_URL, NULL, opts);
    if (doc == NULL) error("failed to create html doc");

    xmlChar *xpath_artist = (xmlChar *) XPATH_ARTIST;
    xmlChar *xpath_track = (xmlChar *) XPATH_TRACK;
    xmlXPathContextPtr context = xmlXPathNewContext(doc);

    xmlXPathObjectPtr result_artist = xmlXPathEvalExpression(xpath_artist, context);
    xmlXPathObjectPtr result_track = xmlXPathEvalExpression(xpath_track, context);
    xmlXPathFreeContext(context);

    if (result_artist == NULL || result_track == NULL) error("failed to eval xpath expression");

    xmlNodeSetPtr nodeset_artist = result_artist->nodesetval;
    xmlNodeSetPtr nodeset_track = result_track->nodesetval;
    if (xmlXPathNodeSetIsEmpty(nodeset_artist) || xmlXPathNodeSetIsEmpty(nodeset_track)) {
        xmlXPathFreeObject(result_artist);
        xmlXPathFreeObject(result_track);
        error("one of the nodesets was empty");
    }

    assert(nodeset_artist->nodeNr == nodeset_track->nodeNr && "expected equivalent quantity of artist and track records");

    printf("%5s    %-30s    %-30s\n", "rank", "track", "artist");
    for (size_t i = 0; i < 69; i++) printf("-");
    printf("\n");
    for (int i = 0; i < nodeset_artist->nodeNr; i++) {
        const xmlNode *node_artist = nodeset_artist->nodeTab[i]->xmlChildrenNode;
        const xmlNode *node_track = nodeset_track->nodeTab[i]->xmlChildrenNode;
        xmlChar *text_artist = xmlNodeListGetString(doc, node_artist, 1);
        xmlChar *text_track = xmlNodeListGetString(doc, node_track, 1);

        // printf("BEFORE:\n'%s'\n'%s'\n", text_track, text_artist);
        xml_str_strip(text_track);
        xml_str_strip(text_artist);
        printf("[%3d]    %-30s    %-30s\n", i+1, text_track, text_artist);
        xmlFree(text_artist);
        xmlFree(text_track);
    }


    xmlXPathFreeObject(result_artist);
    xmlXPathFreeObject(result_track);

    cleanup(NULL, tb);

    // fclose(file);
    // free(buf);

    return EXIT_SUCCESS;
}