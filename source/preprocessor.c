#include "../headers/preprocessor.h"

#include <stdio.h>
#include <string.h>
#include "../headers/parser.h"
#include "../headers/table.h"

/* The various kinds of lines that the preprocessor's `read_parse_line` can give. */
typedef enum line_kind_t {
  LINE_NORMAL, /* A line with no special meaning to the preprocessor. */
  LINE_MCRO, /* A line that starts a macro definition with `mcro`. */
  LINE_MCROEND, /* A line that ends a macro definition with `mcroend`. */
  LINE_MACROCALL /* A line that calls a macro by its name. */
} line_kind_t;

/**
 * Reads a line from the input file, stores it, and parses it for any preprocessor-related actions.
 * May fail if:
 * - The line is longer than the 80-character limit.
 * - Syntax errors in preprocessor-related actions were detected.
 *
 * @param file The file to read a line from.
 * @param line An array to store the line's contents in.
 * @param macro_name A pointer to store the macro's name at, if the line is an `mcro` line or a possible macro call.
 * @param line_kind A pointer to store the line's kind at.
 * @return The operation's result.
 */
result_t read_parse_line(FILE *file, char line[MAX_LINE], char *macro_name, line_kind_t *line_kind) {
  word_t word;
  char *cur_line;
  read_line_status_t read_status = read_line(file, line);

  if (read_status == READ_LINE_TOO_LONG) {
    return ERR_LINE_TOO_LONG;
  }

  cur_line = line;

  read_word(&cur_line, &word);

  if (word.kind == WORD_MCRO) {
    /* A line that starts with `mcro` begins a macro definition. */
    read_word(&cur_line, &word);
    ASSERT(word.kind == WORD_IDENTIFIER, ERR_INVALID_MACRO_NAME)

    if (macro_name) {
      strcpy(macro_name, word.value);
    }

    ASSERT(is_end(cur_line), ERR_EXTRANEOUS_TEXT_MACRO)

    *line_kind = LINE_MCRO;
    return SUCCESS;
  }

  if (word.kind == WORD_MCROEND) {
    ASSERT(is_end(cur_line), ERR_EXTRANEOUS_TEXT_MCROEND)

    *line_kind = LINE_MCROEND;
    return SUCCESS;
  }

  /* If this line is just a single non-keyword token with no tokens after it, it may be a macro call. */
  if (word.kind == WORD_IDENTIFIER && is_end(cur_line)) {
    if (macro_name) {
      strcpy(macro_name, word.value);
    }
    *line_kind = LINE_MACROCALL;
    return SUCCESS;
  }

  *line_kind = LINE_NORMAL;
  return SUCCESS;
}

/**
 * Prints the given line into the file.
 * The line may end with either a newline or a null terminator - the newline will be included.
 *
 * @param out The file to write the line into.
 * @param line The string of the line to write. May end with a newline or a null terminator.
 */
void print_line(FILE *out, char *line) {
  while (*line) {
    fputc(*line, out);
    if (*line == '\n') {
      break;
    }
    line++;
  }
}

/**
 * Prints a macro from the `in` file to the `out` file.
 * The `in` file is assumed to be positioned at the line where the macro begins.
 * It prints each line from `in` to `out`, until it encounters an `mcroend` line.
 *
 * @param out The file to write the macro's contents into.
 * @param in The input file, which must be seeked to the beginning of the macro's contents.
 */
void print_macro(FILE *out, FILE *in) {
  char line[MAX_LINE];
  line_kind_t line_kind;

  while (read_parse_line(in, line, NULL, &line_kind) == SUCCESS && line_kind != LINE_MCROEND) {
    print_line(out, line);
  }
}

result_t preprocess(char *input_file_path, char *output_file_path, table_t *macro_table) {
  result_t result = SUCCESS;
  FILE *in_file;
  FILE *out_file;
  char line[MAX_LINE];
  char macro_name[MAX_LINE];
  int line_number = 0;

  in_file = fopen(input_file_path, "rb");

  /* If the input file is unavailable, exit. */
  if (in_file == NULL) {
    return ERR_INPUT_FILE_FAIL;
  }

  out_file = fopen(output_file_path, "w");
  if (out_file == NULL) {
    fclose(in_file);
    return ERR_OUTPUT_FILE_FAIL;
  }

  while (!feof(in_file)) {
    line_kind_t parse_status;
    result_t parse_result;

    line_number++;

    parse_result = read_parse_line(in_file, line, macro_name, &parse_status);
    if (parse_result != SUCCESS) {
      result = ERR_PREPROCESS_FAILED;
      print_error(input_file_path, line_number, parse_result);
      continue;
    }

    if (parse_status == LINE_NORMAL) {
      /* A line with no special meaning to the preprocessor. We output it as is. */
      print_line(out_file, line);
    }
    else if (parse_status == LINE_MCRO) {
      /* A macro has been defined. */
      long offset = ftell(in_file);

      long *table_value;
      result_t add_result = table_add(macro_table, macro_name, (void **) &table_value);
      if (add_result != SUCCESS) {
        result = add_result;
        goto cleanup;
      }
      *table_value = offset;

      /* We skip over lines until we reach an `mcroend` line. */
      /* According to the assignment, we do not need to check for nested macros, */
      /* and we can assume that an `mcro` definition will always have a matching `mcroend`. */
      do {
        line_number++;
        parse_result = read_parse_line(in_file, line, NULL, &parse_status);
        if (parse_result != SUCCESS) {
          result = ERR_PREPROCESS_FAILED;
          print_error(input_file_path, line_number, parse_result);
        }
      }
      while (parse_status != LINE_MCROEND);
    }
    else if (parse_status == LINE_MACROCALL) {
      /* A line with a single word in it may be a macro call. */

      FILE *temp;
      long *offset = table_get(macro_table, macro_name);
      if (offset == NULL) {
        /* If there's no macro by this name, we output the line as is. */
        print_line(out_file, line);
        continue;
      }

      /* We open the input file again - but this time we seek it so that it'll start reading at the offset where the
       * macro's content begins. */
      temp = fopen(input_file_path, "r");
      if (fseek(temp, *offset, SEEK_SET) != 0) {
        /* If `fseek` has failed, we cannot proceed. */
        result = ERR_FSEEK_FAILED;
        fclose(temp);
        goto cleanup;
      }

      print_macro(out_file, temp);
      fclose(temp);
    }
  }

cleanup:
  fclose(in_file);
  fclose(out_file);

  return result;
}
