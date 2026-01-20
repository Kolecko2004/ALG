/* =========================================================
   Dynamic memory examples
   ========================================================= */
void dynamic_memory_examples(void)
{
    /* --------------------------
       malloc: allocate single struct
       -------------------------- */
    Point *p = malloc(sizeof(Point));
    if (!p) { 
        log_message(LOG_ERROR, "malloc failed for Point");
        return;
    }
    p->x = 1.0;
    p->y = 2.0;
    printf("Point via malloc: (%.2f, %.2f)\n", p->x, p->y);
    free(p); // free memory when done

    /* --------------------------
       calloc: allocate zeroed array
       -------------------------- */
    size_t n = 5;
    int *arr = calloc(n, sizeof(int)); // all elements initialized to 0
    if (!arr) {
        log_message(LOG_ERROR, "calloc failed for int array");
        return;
    }
    for (size_t i = 0; i < n; i++) arr[i] = (int)i * 10;
    printf("Array via calloc: ");
    for (size_t i = 0; i < n; i++) printf("%d ", arr[i]);
    puts("");
    free(arr);

    /* --------------------------
       realloc: grow an array
       -------------------------- */
    size_t old_size = 3;
    size_t new_size = 6;
    double *dbl_arr = malloc(old_size * sizeof(double));
    if (!dbl_arr) return;
    for (size_t i = 0; i < old_size; i++) dbl_arr[i] = i * 1.1;

    dbl_arr = realloc(dbl_arr, new_size * sizeof(double)); // resize
    if (!dbl_arr) {
        log_message(LOG_ERROR, "realloc failed");
        return;
    }
    // initialize new elements
    for (size_t i = old_size; i < new_size; i++) dbl_arr[i] = i * 1.1;
    printf("Array via realloc: ");
    for (size_t i = 0; i < new_size; i++) printf("%.2f ", dbl_arr[i]);
    puts("");
    free(dbl_arr);

    /* --------------------------
       Array of structs
       -------------------------- */
    size_t count = 4;
    PhysicsBody *bodies = malloc(count * sizeof(PhysicsBody));
    if (!bodies) return;
    for (size_t i = 0; i < count; i++) {
        bodies[i].position.x = i * 1.0;
        bodies[i].position.y = i * 2.0;
        bodies[i].velocity.x = i * 0.1;
        bodies[i].velocity.y = i * 0.2;
    }
    for (size_t i = 0; i < count; i++) {
        printf("Body %zu: pos=(%.2f,%.2f) vel=(%.2f,%.2f)\n",
               i,
               bodies[i].position.x, bodies[i].position.y,
               bodies[i].velocity.x, bodies[i].velocity.y);
    }
    free(bodies);

    /* --------------------------
       Pointer-to-pointer (2D array)
       -------------------------- */
    size_t rows = 3, cols = 4;
    int **matrix = malloc(rows * sizeof(int*));
    if (!matrix) return;
    for (size_t i = 0; i < rows; i++) {
        matrix[i] = calloc(cols, sizeof(int)); // zeroed row
        if (!matrix[i]) {
            log_message(LOG_ERROR, "calloc failed for row");
            // free previously allocated rows
            for (size_t j = 0; j < i; j++) free(matrix[j]);
            free(matrix);
            return;
        }
    }
    // assign values
    for (size_t i = 0; i < rows; i++)
        for (size_t j = 0; j < cols; j++)
            matrix[i][j] = (int)(i * 10 + j);

    printf("2D array via malloc/calloc:\n");
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++)
            printf("%2d ", matrix[i][j]);
        puts("");
    }
    // free 2D array
    for (size_t i = 0; i < rows; i++) free(matrix[i]);
    free(matrix);
}
