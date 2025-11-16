#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>   // unlink, rename
#include <errno.h>

char KEY = '$';              // Global key
const char *FILE_TYPE = "newgit";  // Global "file type" (extension)

// Detect if path ends with ".newgit"
int has_newgit_extension(const char *path) {
    size_t len = strlen(path);
    size_t ext_len = strlen(FILE_TYPE) + 1; // +1 for '.'

    if (len <= ext_len) return 0;

    const char *ext_start = path + (len - ext_len); // points to '.'
    if (ext_start[0] != '.') return 0;

    if (strcmp(ext_start + 1, FILE_TYPE) == 0) {
        return 1;
    }
    return 0;
}

// Build new file path:
// mode = 0 -> add ".newgit"
// mode = 1 -> remove ".newgit"
char *build_output_path(const char *input_path, int mode) {
    char *out = NULL;
    size_t len = strlen(input_path);
    size_t ext_len = strlen(FILE_TYPE) + 1; // ".newgit"

    if (mode == 0) {
        // Add ".newgit"
        out = (char *)malloc(len + ext_len + 1);
        if (!out) return NULL;
        strcpy(out, input_path);
        strcat(out, ".");
        strcat(out, FILE_TYPE);
    } else {
        // Remove ".newgit"
        if (len <= ext_len) return NULL;
        out = (char *)malloc(len - ext_len + 1);
        if (!out) return NULL;
        strncpy(out, input_path, len - ext_len);
        out[len - ext_len] = '\0';
    }
    return out;
}

/*
 * Single function you can call:
 *  - Pass full file path (till file).
 *  - If file has ".newgit" -> decrypt & remove ".newgit".
 *  - Else -> encrypt & add ".newgit".
 *  - Same location, original file removed, new one created.
 *
 *  Return: 0 on success, -1 on error.
 */
int newgit_encrypt_decrypt(char *filepath) {
    FILE *in = NULL, *out = NULL;
    char *final_path = NULL;
    char *temp_path = NULL;
    int is_decrypt = 0;
    int c;
    int result = -1;

    if (filepath == NULL) {
        fprintf(stderr, "Error: filepath is NULL\n");
        return -1;
    }

    // Detect encrypt / decrypt
    if (has_newgit_extension(filepath)) {
        is_decrypt = 1;  // decrypt and remove extension
        final_path = build_output_path(filepath, 1);
    } else {
        is_decrypt = 0;  // encrypt and add extension
        final_path = build_output_path(filepath, 0);
    }

    if (!final_path) {
        fprintf(stderr, "Error: failed to build final path\n");
        return -1;
    }

    // Build temp file path: "<final_path>.tmp"
    temp_path = (char *)malloc(strlen(final_path) + 5); // ".tmp" + '\0'
    if (!temp_path) {
        fprintf(stderr, "Error: malloc failed\n");
        free(final_path);
        return -1;
    }
    strcpy(temp_path, final_path);
    strcat(temp_path, ".tmp");

    // Open input file
    in = fopen(filepath, "rb");
    if (!in) {
        fprintf(stderr, "Error: cannot open input file '%s': %s\n",
                filepath, strerror(errno));
        free(final_path);
        free(temp_path);
        return -1;
    }

    // Open temp output file
    out = fopen(temp_path, "wb");
    if (!out) {
        fprintf(stderr, "Error: cannot open temp file '%s': %s\n",
                temp_path, strerror(errno));
        fclose(in);
        free(final_path);
        free(temp_path);
        return -1;
    }

    // XOR logic (same for encrypt / decrypt)
    while ((c = fgetc(in)) != EOF) {
        c = c ^ KEY;
        fputc(c, out);
    }

    fclose(in);
    fclose(out);

    // Delete original file
    if (unlink(filepath) != 0) {
        fprintf(stderr, "Warning: failed to delete original file '%s': %s\n",
                filepath, strerror(errno));
        // continue anyway, try to rename
    }

    // Rename temp -> final
    if (rename(temp_path, final_path) != 0) {
        fprintf(stderr, "Error: failed to rename '%s' to '%s': %s\n",
                temp_path, final_path, strerror(errno));
        free(final_path);
        free(temp_path);
        return -1;
    }

    printf("%s: %s -> %s\n",
           is_decrypt ? "Decrypted" : "Encrypted",
           filepath,
           final_path);

    result = 0;

    free(final_path);
    free(temp_path);
    return result;
}
