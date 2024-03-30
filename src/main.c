#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[20];
    char *value; // Dynamic memory allocation for value
} Variable;

Variable *variables = NULL; // Dynamic memory allocation for variables
int num_variables = 0; // Number of variables

void var(char *varname, char *value) {
    for (int i = 0; i < num_variables; i++) {
        if (strcmp(variables[i].name, varname) == 0) {
            free(variables[i].value); // Free existing value if variable already exists
            variables[i].value = strdup(value); // Duplicate string for value
            return;
        }
    }
    // If variable does not exist, create a new one
    variables = realloc(variables, (num_variables + 1) * sizeof(Variable));
    strcpy(variables[num_variables].name, varname);
    variables[num_variables].value = strdup(value);
    num_variables++;
}

void editvar(char *varname, char *value) {
    for (int i = 0; i < num_variables; i++) {
        if (strcmp(variables[i].name, varname) == 0) {
            free(variables[i].value);
            variables[i].value = strdup(value);
            return;
        }
    }
    printf("변수를 찾을 수 없음\n");
}

void print(char *varname) {
    for (int i = 0; i < num_variables; i++) {
        if (strcmp(variables[i].name, varname) == 0) {
            printf("%s\n", variables[i].value);
            return;
        }
    }
    printf("변수를 찾을 수 없음\n");
}

void input(char *varname) {
    char value[50];
    printf("");
    scanf("%s", value);
    for (int i = 0; i < num_variables; i++) {
        if (strcmp(variables[i].name, varname) == 0) {
            free(variables[i].value);
            variables[i].value = strdup(value);
            return;
        }
    }
    printf("변수를 찾을 수 없음\n");
}

int evaluate_arithmetic_expression(int var1, int var2, char *operator) {
    if (strcmp(operator, "+") == 0) {
        return var1 + var2;
    } else if (strcmp(operator, "-") == 0) {
        return var1 - var2;
    } else if (strcmp(operator, "*") == 0) {
        return var1 * var2;
    } else if (strcmp(operator, "/") == 0) {
        if (var2 != 0) {
            return var1 / var2;
        } else {
            printf("0으로 나눌 수 없습니다.\n");
            return 0;
        }
    } else if (strcmp(operator, "%") == 0) {
        return var1 % var2;
    }
    return 0;
}

void execute_commands_from_file(const char *filename);

void execute_if(char *condition_var1, char *operator, char *condition_var2, char *then) {
    int var1 = atoi(condition_var1);
    int var2 = atoi(condition_var2);
    if (evaluate_arithmetic_expression(var1, var2, operator)) {
        execute_commands_from_file(then);
    }
}

void execute_else(char *then) {
    execute_commands_from_file(then);
}

void execute_for(char *start, char *end, char *operation) {
    int start_value = atoi(start);
    int end_value = atoi(end);
    for (int i = start_value; i <= end_value; i++) {
        char index_str[50];
        sprintf(index_str, "%d", i);
        var("index", index_str);
        execute_commands_from_file(operation);
    }
}

void execute_import(char *module_name) {
    char module_filename[100];
    sprintf(module_filename, "%s.h", module_name);
    execute_commands_from_file(module_filename);
}

void execute_concat(char *varname, char *string1, char *string2) {
    char *result = malloc(strlen(string1) + strlen(string2) + 1);
    strcpy(result, string1);
    strcat(result, string2);
    var(varname, result);
}

void execute_substring(char *varname, char *string, int start, int length) {
    char *result = malloc(length + 1);
    strncpy(result, string + start, length);
    result[length] = '\0';
    var(varname, result);
}

void execute_search(char *varname, char *string, char *substring) {
    char *result = strstr(string, substring);
    if (result != NULL) {
        var(varname, result);
    } else {
        printf("Substring not found\n");
    }
}

void execute_commands_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("파일을 열 수 없음\n");
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, " ");
        if (token != NULL) {
            if (strcmp(token, "var") == 0) {
                char *varname = strtok(NULL, " ");
                char *value = strtok(NULL, " \n");
                var(varname, value);
            } else if (strcmp(token, "editvar") == 0) {
                char *varname = strtok(NULL, " ");
                char *value = strtok(NULL, " \n");
                editvar(varname, value);
            } else if (strcmp(token, "print") == 0) {
                char *varname = strtok(NULL, " \n");
                print(varname);
            } else if (strcmp(token, "input") == 0) {
                char *varname = strtok(NULL, " \n");
                input(varname);
            } else if (strcmp(token, "if") == 0) {
                char *condition_var1 = strtok(NULL, " ");
                char *operator = strtok(NULL, " ");
                char *condition_var2 = strtok(NULL, " ");
                char *then = strtok(NULL, " \n");
                execute_if(condition_var1, operator, condition_var2, then);
            } else if (strcmp(token, "else") == 0) {
                char *then = strtok(NULL, " \n");
                execute_else(then);
            } else if (strcmp(token, "elseif") == 0) {
                // Similar to 'if' block
            } else if (strcmp(token, "for") == 0) {
                char *start = strtok(NULL, " ");
                char *end = strtok(NULL, " ");
                char *operation = strtok(NULL, " \n");
                execute_for(start, end, operation);
            } else if (strcmp(token, "import") == 0) {
                char *module_name = strtok(NULL, " \n");
                execute_import(module_name);
            } else if (strcmp(token, "concat") == 0) {
                char *varname = strtok(NULL, " ");
                char *string1 = strtok(NULL, " ");
                char *string2 = strtok(NULL, " \n");
                execute_concat(varname, string1, string2);
            } else if (strcmp(token, "substring") == 0) {
                char *varname = strtok(NULL, " ");
                char *string = strtok(NULL, " ");
                int start = atoi(strtok(NULL, " "));
                int length = atoi(strtok(NULL, " \n"));
                execute_substring(varname, string, start, length);
            } else if (strcmp(token, "search") == 0) {
                char *varname = strtok(NULL, " ");
                char *string = strtok(NULL, " ");
                char *substring = strtok(NULL, " \n");
                execute_search(varname, string, substring);
            } else {
                // Similar to arithmetic expression evaluation
            }
        }
    }

    fclose(file);
}

int main() {
    char filename[100];
    printf("실행할 파일 이름을 입력하세요: ");
    scanf("%s", filename);
    execute_commands_from_file(filename);
    return 0;
}
