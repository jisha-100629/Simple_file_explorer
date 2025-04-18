#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <time.h>

#define MAX_PATH 1024
#define LOG_FILE "file_explorer_log.txt"
#define COLOR_RED 12
#define COLOR_GREEN 10
#define COLOR_YELLOW 14
#define COLOR_BLUE 9
#define COLOR_RESET 7

void set_color(int color);
void list_files(const char *path);
void change_directory(const char *path);
void make_directory(const char *path);
void delete_file_or_directory(const char *path);
void rename_file_or_directory(const char *oldname, const char *newname);
void search_files(const char *path, const char *pattern);
void display_file_properties(const char *file);
void copy_file(const char *source, const char *destination);
void log_operation(const char *operation);
void clear_input_buffer();
void display_welcome_message();
void display_fun_quote();
void show_loading_animation(const char *action);

void set_color(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void log_operation(const char *operation) {
    FILE *log = fopen(LOG_FILE, "a");
    if (log) {
        fprintf(log, "%s\n", operation);
        fclose(log);
    }
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void list_files(const char *path) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    char fullpath[MAX_PATH];

    snprintf(fullpath, sizeof(fullpath), "%s\\*", path); 

    hFind = FindFirstFile(fullpath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        perror("Could not open directory");
        return;
    }

    set_color(COLOR_BLUE);
    printf("\nContents of %s:\n", path);
    set_color(COLOR_RESET);

    do {
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            printf("[DIR]  %s\n", findFileData.cFileName);
        } else {
            printf("      %s\n", findFileData.cFileName);
        }
    } while (FindNextFile(hFind, &findFileData) != 0);
    
    FindClose(hFind);
    log_operation("Listed files");
}

void change_directory(const char *path) {
    if (SetCurrentDirectory(path)) {
        set_color(COLOR_GREEN);
        printf("Directory changed successfully.\n");
        set_color(COLOR_RESET);
        log_operation("Changed directory");
    } else {
        set_color(COLOR_RED);
        perror("Failed to change directory");
        set_color(COLOR_RESET);
    }
}

void make_directory(const char *path) {
    if (CreateDirectory(path, NULL)) {
        set_color(COLOR_GREEN);
        printf("Directory created successfully.\n");
        set_color(COLOR_RESET);
        log_operation("Created directory");
    } else {
        set_color(COLOR_RED);
        perror("Failed to create directory");
        set_color(COLOR_RESET);
    }
}

void delete_file_or_directory(const char *path) {
    printf("Are you sure you want to delete '%s'? (y/n): ", path);
    char confirm = getchar();
    clear_input_buffer();
    if (confirm == 'y' || confirm == 'Y') {
        show_loading_animation("Deleting");

        if (RemoveDirectory(path) || DeleteFile(path)) {
            set_color(COLOR_GREEN);
            printf("Deleted successfully.\n");
            set_color(COLOR_RESET);
            log_operation("Deleted file/directory");
        } else {
            set_color(COLOR_RED);
            perror("Failed to delete");
            set_color(COLOR_RESET);
        }
    } else {
        set_color(COLOR_YELLOW);
        printf("Delete operation cancelled.\n");
        set_color(COLOR_RESET);
    }
}

void rename_file_or_directory(const char *oldname, const char *newname) {
    if (MoveFile(oldname, newname)) {
        set_color(COLOR_GREEN);
        printf("Renamed successfully.\n");
        set_color(COLOR_RESET);
        log_operation("Renamed file/directory");
    } else {
        set_color(COLOR_RED);
        perror("Failed to rename");
        set_color(COLOR_RESET);
    }
}

void search_files(const char *path, const char *pattern) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    char fullpath[MAX_PATH];
    snprintf(fullpath, sizeof(fullpath), "%s\\*", path);
    hFind = FindFirstFile(fullpath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Error searching for files in directory: %s\n", path);
        return;
    }

    printf("Search Results for '%s' in %s:\n", pattern, path);
    do {
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            continue;
        }
        if (strstr(findFileData.cFileName, pattern)) {
            printf("Found: %s\n", findFileData.cFileName);
        }
    } while (FindNextFile(hFind, &findFileData) != 0);
    FindClose(hFind);
}

void display_file_properties(const char *file) {
    WIN32_FILE_ATTRIBUTE_DATA fileData;
    if (GetFileAttributesEx(file, GetFileExInfoStandard, &fileData)) {
        SYSTEMTIME systemTime;
        FileTimeToSystemTime(&fileData.ftLastWriteTime, &systemTime);
        printf("File: %s\n", file);
        printf("Size: %lld bytes\n", ((long long)fileData.nFileSizeHigh << 32) + fileData.nFileSizeLow);
        printf("Last Modified: %02d/%02d/%04d %02d:%02d:%02d\n",
               systemTime.wMonth, systemTime.wDay, systemTime.wYear,
               systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
        set_color(COLOR_RESET);
    } else {
        set_color(COLOR_RED);
        printf("Unable to fetch file properties\n");
        set_color(COLOR_RESET);
    }
}

void copy_file(const char *source, const char *destination) {
    show_loading_animation("Copying");

    if (CopyFile(source, destination, FALSE)) {
        set_color(COLOR_GREEN);
        printf("File copied successfully.\n");
        set_color(COLOR_RESET);
        log_operation("Copied file");
    } else {
        set_color(COLOR_RED);
        perror("Failed to copy file");
        set_color(COLOR_RESET);
    }
}

void show_loading_animation(const char *action) {
    printf("\n%s... Please wait", action);
    int j;
    for (j = 0; j < 5; j++) {
        printf(".");
        Sleep(500);  
    }
    printf("\n");
}

void display_welcome_message() {
    set_color(COLOR_BLUE);
    printf("\n(: Welcome to the Simple File Explorer :)\n");
    set_color(COLOR_RESET);
}

void display_fun_quote() {
    const char *quotes[] = {
        "Life is too short for bad coffee!",
        "There's no place like 127.0.0.1!",
        "Procrastination is the art of keeping up with yesterday.",
        "The best way to predict the future is to invent it!"
    };
    srand(time(NULL));
    int index = rand() % 4;
    printf("\n* Fun Quote: \"%s\"\n", quotes[index]);
}

int main() {
    char current_path[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, current_path);

    int choice;
    char path[MAX_PATH], newname[MAX_PATH], pattern[MAX_PATH], dest[MAX_PATH];

    display_welcome_message();
    display_fun_quote();

    while (1) {
        printf("\nCurrent Directory: %s\n", current_path);
        printf("Menu:\n");
        printf("1. List files and directories\n");
        printf("2. Change directory\n");
        printf("3. Create a directory\n");
        printf("4. Delete file/directory\n");
        printf("5. Rename file/directory\n");
        printf("6. Search files\n");
        printf("7. View file properties\n");
        printf("8. Copy file\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1:
                list_files(current_path);
                break;
            case 2:
                printf("Enter directory to change: ");
                fgets(path, sizeof(path), stdin);
                path[strcspn(path, "\n")] = '\0';
                change_directory(path);
                GetCurrentDirectory(MAX_PATH, current_path);
                break;
            case 3:
                printf("Enter directory name to create: ");
                fgets(path, sizeof(path), stdin);
                path[strcspn(path, "\n")] = '\0';
                make_directory(path);
                break;
            case 4:
                printf("Enter file/directory name to delete: ");
                fgets(path, sizeof(path), stdin);
                path[strcspn(path, "\n")] = '\0';
                delete_file_or_directory(path);
                break;
            case 5:
                printf("Enter current file/directory name: ");
                fgets(path, sizeof(path), stdin);
                path[strcspn(path, "\n")] = '\0';
                printf("Enter new name: ");
                fgets(newname, sizeof(newname), stdin);
                newname[strcspn(newname, "\n")] = '\0';
                rename_file_or_directory(path, newname);
                break;
            case 6:
                printf("Enter search pattern: ");
                fgets(pattern, sizeof(pattern), stdin);
                pattern[strcspn(pattern, "\n")] = '\0';
                search_files(current_path, pattern);
                break;
            case 7:
                printf("Enter file name to view properties: ");
                fgets(path, sizeof(path), stdin);
                path[strcspn(path, "\n")] = '\0';
                display_file_properties(path);
                break;
            case 8:
                printf("Enter source file: ");
                fgets(path, sizeof(path), stdin);
                path[strcspn(path, "\n")] = '\0';
                printf("Enter destination file: ");
                fgets(dest, sizeof(dest), stdin);
                dest[strcspn(dest, "\n")] = '\0';
                copy_file(path, dest);
                break;
            case 9:
                printf("Exiting the File Explorer. Goodbye!\n");
                return 0;
            default:
                set_color(COLOR_RED);
                printf("Invalid choice! Please try again.\n");
                set_color(COLOR_RESET);
        }
    }

    return 0;
}