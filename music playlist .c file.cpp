#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_SONGS 5000
// Structure for a song
typedef struct Song {
    char title[100];
    char artist[100];
    struct Song* next;
    struct Song* prev;
} Song;

// Structure for the playlist
typedef struct Playlist {
    char title[100];
    char cover_art[100];
    Song* head;
} Playlist;

// Function prototypes
Playlist* createPlaylist(char title[], char cover_art[]);
void addSong(Playlist* playlist, char title[], char artist[]);
void displayPlaylist(Playlist* playlist);
void freePlaylist(Playlist* playlist);
void sortPlaylist(Playlist* playlist);
void quickSort(Song** headRef);
Song* getLastNode(Song* head);
Song* partition(Song* low, Song* high);
void quickSortUtil(Song** headRef, Song* last); // Declaration of quickSortUtil
Song* searchSong(Playlist* playlist, char title[]);
void trimLeadingTrailingSpaces(char* str);

// Function to create a new playlist
Playlist* createPlaylist(char title[], char cover_art[]) {
    Playlist* playlist = (Playlist*)malloc(sizeof(Playlist));
    strcpy(playlist->title, title);
    strcpy(playlist->cover_art, cover_art);
    playlist->head = NULL;
    return playlist;
}

// Function to add a song to the playlist
void addSong(Playlist* playlist, char title[], char artist[]) {
    Song* newSong = (Song*)malloc(sizeof(Song));
    strcpy(newSong->title, title);
    strcpy(newSong->artist, artist);
    newSong->next = NULL;
    newSong->prev = NULL;

    if (playlist->head == NULL) {
        playlist->head = newSong;
    } else {
        Song* current = playlist->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newSong;
        newSong->prev = current;
    }
}

// Function to display the playlist
void displayPlaylist(Playlist* playlist) {
    printf("Playlist Title: %s\n", playlist->title);
    printf("Cover Art: %s\n", playlist->cover_art);
    printf("Tracklist:\n");

    Song* current = playlist->head;
    while (current != NULL) {
        printf("%s - %s\n", current->artist, current->title);
        current = current->next;
    }
}

// Function to free memory allocated for the playlist
void freePlaylist(Playlist* playlist) {
    Song* current = playlist->head;
    while (current != NULL) {
        Song* temp = current;
        current = current->next;
        free(temp);
    }
    free(playlist);
}

// Function to sort the playlist using quick sort algorithm
void sortPlaylist(Playlist* playlist) {
    quickSort(&(playlist->head));
}

// Function to perform quick sort
void quickSort(Song** headRef) {
    if (*headRef == NULL || (*headRef)->next == NULL)
        return;

    Song* last = getLastNode(*headRef);

    quickSortUtil(headRef, last);
}

// Function to get the last node of a linked list
Song* getLastNode(Song* head) {
    while (head != NULL && head->next != NULL)
        head = head->next;
    return head;
}

// Function to partition the linked list
Song* partition(Song* low, Song* high) {
    char pivotTitle[100];
    char pivotArtist[100];
    strcpy(pivotTitle, high->title);
    strcpy(pivotArtist, high->artist);

    Song* i = low->prev;

    for (Song* j = low; j != high; j = j->next) {
        if (strcmp(j->title, pivotTitle) < 0 || (strcmp(j->title, pivotTitle) == 0 && strcmp(j->artist, pivotArtist) < 0)) {
            i = (i == NULL) ? low : i->next;

            // Swap data
            char tempTitle[100];
            char tempArtist[100];
            strcpy(tempTitle, i->title);
            strcpy(tempArtist, i->artist);
            strcpy(i->title, j->title);
            strcpy(i->artist, j->artist);
            strcpy(j->title, tempTitle);
            strcpy(j->artist, tempArtist);
        }
    }

    i = (i == NULL) ? low : i->next;

    // Swap data
    char tempTitle[100];
    char tempArtist[100];
    strcpy(tempTitle, i->title);
    strcpy(tempArtist, i->artist);
    strcpy(i->title, high->title);
    strcpy(i->artist, high->artist);
    strcpy(high->title, tempTitle);
    strcpy(high->artist, tempArtist);

    return i;
}

// Utility function for quick sort
void quickSortUtil(Song** headRef, Song* last) {
    if (*headRef == NULL || last == NULL || *headRef == last || (*headRef)->prev == last)
        return;

    Song* pivot = partition(*headRef, last);

    quickSortUtil(headRef, pivot->prev);
    quickSortUtil(&(pivot->next), last);
}

// Function to search for a song in the playlist by title
Song* searchSong(Playlist* playlist, char title[]) {
    // Trim leading and trailing spaces from the search title
    char searchTitle[100];
    strcpy(searchTitle, title);
    trimLeadingTrailingSpaces(searchTitle);

    Song* current = playlist->head;
    while (current != NULL) {
        // Trim leading and trailing spaces from the current song title
        char currentTitle[100];
        strcpy(currentTitle, current->title);
        trimLeadingTrailingSpaces(currentTitle);

        // Perform case-insensitive comparison
        if (strcasecmp(currentTitle, searchTitle) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Function to trim leading and trailing spaces from a string
void trimLeadingTrailingSpaces(char* str) {
    int i = 0, j = strlen(str) - 1;

    // Trim leading spaces
    while (isspace(str[i])) {
        i++;
    }

    // Trim trailing spaces
    while (j >= i && isspace(str[j])) {
        j--;
    }

    // Shift the remaining characters to the beginning of the string
    int k;
    for (k = 0; k <= j - i; k++) {
        str[k] = str[i + k];
    }
    str[k] = '\0';
}

int main() {
    // Prompt the user to input playlist details
    char title[100];
    char cover_art[100];
    printf("Enter the title of the playlist: ");
    scanf("%99[^\n]", title);
    getchar(); // Consume newline character
    printf("Enter the cover art of the playlist: ");
    scanf("%99[^\n]", cover_art);
    getchar(); // Consume newline character

    // Create a new playlist
    Playlist* myPlaylist = createPlaylist(title, cover_art);

    // Prompt the user to input songs
    char songTitle[100];
    char songArtist[100];
    char choice;
    do {
        printf("Enter the title of the song: ");
        scanf("%99[^\n]", songTitle);
        getchar(); // Consume newline character
        printf("Enter the artist of the song: ");
        scanf("%99[^\n]", songArtist);
        getchar(); // Consume newline character

        // Add the song to the playlist
        addSong(myPlaylist, songTitle, songArtist);

        printf("Do you want to add another song? (y/n): ");
        scanf("%c", &choice);
        getchar(); // Consume newline character
    } while (choice == 'y' || choice == 'Y');

    // Display the playlist
    printf("\nPlaylist Details:\n");
    displayPlaylist(myPlaylist);

    // Sort the playlist using quick sort
    sortPlaylist(myPlaylist);

    // Display the sorted playlist
    printf("\nSorted Playlist:\n");
    displayPlaylist(myPlaylist);

    // Search for a song
    char searchTitle[100];
    printf("\nEnter the title of the song to search: ");
    scanf("%99[^\n]", searchTitle);
    getchar(); // Consume newline character
    Song* foundSong = searchSong(myPlaylist, searchTitle);
    if (foundSong != NULL) {
        printf("%s - %s found in the playlist.\n", foundSong->artist, foundSong->title);
    } else {
        printf("%s not found in the playlist.\n", searchTitle);
    }

    // Free memory allocated for the playlist
    freePlaylist(myPlaylist);

    return 0;
}
