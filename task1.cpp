#include <iostream>
#include <string>
using namespace std;

// one node is one song
struct Song {
    int id;
    string name;
    int minutes;
    int seconds;
    Song* next;   // points to next song
    Song* prev;   // points to previous song
};

Song* head = NULL;      // first song
Song* tail = NULL;      // last song
Song* current = NULL;   // song that is playing right now

// prints one song's details
void printSong(Song* s) {
    cout << "ID: " << s->id << " | Name: " << s->name << " | Duration: "
         << s->minutes << ":";
    if (s->seconds < 10) {
        cout << "0";   // so 3:5 shows as 3:05
    }
    cout << s->seconds << endl;
}

// Add song at the end of the playlist
void addSong(int id, string name, int min, int sec) {
    Song* newSong = new Song;
    newSong->id = id;
    newSong->name = name;
    newSong->minutes = min;
    newSong->seconds = sec;
    newSong->next = NULL;
    newSong->prev = tail;

    if (head == NULL) {
        // list was empty, so this is the first song
        head = newSong;
        tail = newSong;
        current = newSong;
    } else {
        tail->next = newSong;
        tail = newSong;
    }
    cout << "Song added." << endl;
}

// find a song by id, returns NULL if not found
Song* findSong(int id) {
    Song* temp = head;
    while (temp != NULL) {
        if (temp->id == id) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// Delete song by id
void deleteSong(int id) {
    Song* temp = findSong(id);
    if (temp == NULL) {
        cout << "Song not found." << endl;
        return;
    }

    // fixing the link of the previous node
    if (temp->prev != NULL) {
        temp->prev->next = temp->next;
    } else {
        head = temp->next;   // deleting the first song
    }

    // fixing the link of the next node
    if (temp->next != NULL) {
        temp->next->prev = temp->prev;
    } else {
        tail = temp->prev;   // deleting the last song
    }

    // if the deleted song was playing, go back to the first song
    if (current == temp) {
        current = head;
    }

    delete temp;
    cout << "Song deleted." << endl;
}

// Displaying list from front
void displayForward() {
    if (head == NULL) {
        cout << "Playlist is empty." << endl;
        return;
    }
    Song* temp = head;
    while (temp != NULL) {
        printSong(temp);
        temp = temp->next;
    }
}

// Displaying from last to first
void displayBackward() {
    if (tail == NULL) {
        cout << "Playlist is empty." << endl;
        return;
    }
    Song* temp = tail;
    while (temp != NULL) {
        printSong(temp);
        temp = temp->prev;
    }
}

// Search song by id and show details
void searchSong(int id) {
    Song* s = findSong(id);
    if (s == NULL) {
        cout << "Song not found." << endl;
    } else {
        printSong(s);
    }
}

// Play next song
void playNext() {
    if (current == NULL) {
        cout << "Playlist is empty." << endl;
    } else if (current->next == NULL) {
        cout << "This is the last song, no next song." << endl;
    } else {
        current = current->next;
        cout << "Now playing: ";
        printSong(current);
    }
}

// Play previous song
void playPrevious() {
    if (current == NULL) {
        cout << "Playlist is empty." << endl;
    } else if (current->prev == NULL) {
        cout << "This is the first song, no previous song." << endl;
    } else {
        current = current->prev;
        cout << "Now playing: ";
        printSong(current);
    }
}

// Reverse the playlist in place by swapping next and prev of every node
void reversePlaylist() {
    if (head == NULL) {
        cout << "Playlist is empty." << endl;
        return;
    }
    Song* temp = head;
    Song* swapper = NULL;
    while (temp != NULL) {
        // swap next and prev of this node
        swapper = temp->prev;
        temp->prev = temp->next;
        temp->next = swapper;
        // old next is now stored in prev, so move using prev
        temp = temp->prev;
    }
    // swap head and tail
    swapper = head;
    head = tail;
    tail = swapper;
    cout << "Playlist reversed." << endl;
}

// deleting all nodes at the end so there is no memory leak
void freePlaylist() {
    Song* temp = head;
    while (temp != NULL) {
        Song* nextNode = temp->next;
        delete temp;
        temp = nextNode;
    }
    head = NULL;
    tail = NULL;
    current = NULL;
}

int main() {
    int choice;

    do {
        cout << "\n===== PLAYLIST MENU =====" << endl;
        cout << "1. Add Song" << endl;
        cout << "2. Delete Song" << endl;
        cout << "3. Display Playlist Forward" << endl;
        cout << "4. Display Playlist Backward" << endl;
        cout << "5. Search Song" << endl;
        cout << "6. Play Next Song" << endl;
        cout << "7. Play Previous Song" << endl;
        cout << "8. Reverse Playlist" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            int id, min, sec;
            string name;
            cout << "Enter song ID: ";
            cin >> id;
            cin.ignore();   // remove the leftover enter key
            cout << "Enter song name: ";
            getline(cin, name);
            cout << "Enter duration minutes: ";
            cin >> min;
            cout << "Enter duration seconds: ";
            cin >> sec;
            addSong(id, name, min, sec);
        } else if (choice == 2) {
            int id;
            cout << "Enter song ID to delete: ";
            cin >> id;
            deleteSong(id);
        } else if (choice == 3) {
            displayForward();
        } else if (choice == 4) {
            displayBackward();
        } else if (choice == 5) {
            int id;
            cout << "Enter song ID to search: ";
            cin >> id;
            searchSong(id);
        } else if (choice == 6) {
            playNext();
        } else if (choice == 7) {
            playPrevious();
        } else if (choice == 8) {
            reversePlaylist();
        } else if (choice == 0) {
            cout << "Exiting..." << endl;
        } else {
            cout << "Invalid choice." << endl;
        }

    } while (choice != 0);

    freePlaylist();
    return 0;
}