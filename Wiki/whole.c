/*
 * big_example_commented.c
 * Large overview of common C syntax and idioms with detailed comments
 */

#include <stdio.h>   // Standard I/O
#include <stdlib.h>  // malloc, calloc, free, exit
#include <string.h>  // strcpy, strlen, strcmp
#include <math.h>    // sqrt
#include <errno.h>   // errno constants

/* =========================================================
   Macros
   ========================================================= */
// Macros are text substitutions done by the preprocessor
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))  // Calculate array length
#define MAX(a, b) ((a) > (b) ? (a) : (b))                // Max of two values
#define UNUSED(x) (void)(x)                               // Suppress unused variable warnings

/* =========================================================
   Enums
   ========================================================= */
// Enum constants are integer values; useful for readable states
typedef enum {
    LOG_INFO,   // 0
    LOG_WARN,   // 1
    LOG_ERROR   // 2
} LogLevel;

typedef enum {
    ERR_OK = 0,      // Success
    ERR_NULL_PTR,    // Null pointer error
    ERR_ALLOC,       // Allocation failed
    ERR_IO           // File I/O error
} ErrorCode;

/* =========================================================
   Structs
   ========================================================= */
// Simple 2D point
typedef struct {
    double x;
    double y;
} Point;

// Struct representing a moving physics object
typedef struct {
    Point position;
    Point velocity;
} PhysicsBody;

// Simple user struct with name and ID
typedef struct {
    char name[32];  // Fixed-length array
    int id;
} User;

// Database holding dynamically allocated array of Users
typedef struct {
    User *users;
    size_t count;
} UserDatabase;

/* =========================================================
   Typedefs & function pointers
   ========================================================= */
// Comparator type for qsort, pointer to function returning int
typedef int (*Comparator)(const void *, const void *);

/* =========================================================
   Globals & statics
   ========================================================= */
static int g_log_enabled = 1;  // Static global, visible only in this file

/* =========================================================
   Function declarations (prototypes)
   ========================================================= */
void log_message(LogLevel level, const char *msg);

double point_distance(const Point *a, const Point *b); // Pass by pointer
void physics_update(PhysicsBody *body, double dt);     // Update physics object

ErrorCode userdb_init(UserDatabase *db, size_t count); // Initialize database
void userdb_free(UserDatabase *db);                    // Free memory
void userdb_print(const UserDatabase *db);             // Print all users

int compare_users_by_id(const void *a, const void *b); // qsort comparator

ErrorCode write_text_file(const char *path, const char *text); // Write string to file
char *read_text_file(const char *path);                        // Read whole file into buffer

/* =========================================================
   Main
   ========================================================= */
int main(void)
{
    /* --------------------------
       Basic structs
       -------------------------- */
    Point p1 = {0.0, 0.0};  // Initialize struct with brace list
    Point p2 = {3.0, 4.0};

    printf("Distance: %.2f\n", point_distance(&p1, &p2)); // Use pointer

    /* --------------------------
       Nested structs (PhysicsBody)
       -------------------------- */
    PhysicsBody body = {
        .position = {0.0, 0.0},   // Designated initializers
        .velocity = {1.0, 0.5}
    };

    physics_update(&body, 2.0);  // Update with delta time
    printf("Body position: (%.2f, %.2f)\n",
           body.position.x, body.position.y);

    /* --------------------------
       Dynamic memory + arrays
       -------------------------- */
    UserDatabase db;
    if (userdb_init(&db, 3) != ERR_OK) {
        log_message(LOG_ERROR, "Failed to init user database");
        return 1;
    }

    // Populate database
    strcpy(db.users[0].name, "Alice"); db.users[0].id = 3;
    strcpy(db.users[1].name, "Bob");   db.users[1].id = 1;
    strcpy(db.users[2].name, "Charlie"); db.users[2].id = 2;

    /* --------------------------
       Function pointer + qsort
       -------------------------- */
    qsort(db.users, db.count, sizeof(User), compare_users_by_id); // Sort users by ID
    userdb_print(&db);

    /* --------------------------
       File I/O
       -------------------------- */
    if (write_text_file("example.txt", "Hello from C\n") != ERR_OK) {
        log_message(LOG_ERROR, "File write failed");
    }

    char *file_content = read_text_file("example.txt");
    if (file_content) {
        printf("File says: %s", file_content);
        free(file_content);  // Free dynamically allocated buffer
    }

    userdb_free(&db);  // Free database memory
    return 0;
}

/* =========================================================
   Logging
   ========================================================= */
void log_message(LogLevel level, const char *msg)
{
    if (!g_log_enabled) return; // Global switch

    const char *prefix = "";
    switch (level) {
        case LOG_INFO:  prefix = "[INFO]"; break;
        case LOG_WARN:  prefix = "[WARN]"; break;
        case LOG_ERROR: prefix = "[ERROR]"; break;
    }

    fprintf(stderr, "%s %s\n", prefix, msg); // Write to stderr
}

/* =========================================================
   Math / physics
   ========================================================= */
double point_distance(const Point *a, const Point *b)
{
    if (!a || !b) return 0.0;   // Defensive programming
    double dx = b->x - a->x;
    double dy = b->y - a->y;
    return sqrt(dx * dx + dy * dy); // Hypotenuse
}

void physics_update(PhysicsBody *body, double dt)
{
    if (!body) return;
    body->position.x += body->velocity.x * dt; // Simple Euler integration
    body->position.y += body->velocity.y * dt;
}

/* =========================================================
   User database
   ========================================================= */
ErrorCode userdb_init(UserDatabase *db, size_t count)
{
    if (!db) return ERR_NULL_PTR; // Check input

    db->users = calloc(count, sizeof(User)); // Allocate array
    if (!db->users) return ERR_ALLOC;

    db->count = count;
    return ERR_OK;
}

void userdb_free(UserDatabase *db)
{
    if (!db) return;
    free(db->users);
    db->users = NULL;
    db->count = 0;
}

void userdb_print(const UserDatabase *db)
{
    if (!db) return;
    for (size_t i = 0; i < db->count; i++) {
        printf("User %zu: %s (id=%d)\n",
               i, db->users[i].name, db->users[i].id);
    }
}

// Comparator function for qsort
int compare_users_by_id(const void *a, const void *b)
{
    const User *ua = a;
    const User *ub = b;
    return ua->id - ub->id; // Returns negative/zero/positive
}

/* =========================================================
   File I/O
   ========================================================= */
ErrorCode write_text_file(const char *path, const char *text)
{
    FILE *f = fopen(path, "w");
    if (!f) return ERR_IO;

    fputs(text, f);
    fclose(f);
    return ERR_OK;
}

char *read_text_file(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);         // Go to end
    long size = ftell(f);           // Get size
    rewind(f);                      // Reset pointer

    char *buffer = malloc(size + 1); // Allocate buffer
    if (!buffer) {
        fclose(f);
        return NULL;
    }

    fread(buffer, 1, size, f);      // Read all content
    buffer[size] = '\0';             // Null-terminate
    fclose(f);
    return buffer;
}
