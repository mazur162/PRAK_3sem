#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

char *whitespace_characters = " \t\v\f\r"; // все кроме '\n'

int
compar (const void *str1, const void *str2) {
    return (strcmp(*(char* const*)str1, *(char* const*)str2));
}


char *
input_expression(void) {
    char c;
    int len = 0;
    char *str = malloc(sizeof(char));
    while ((c = getchar()) != '\n') {
        if (c == EOF) {
            perror("ERROR: Wrong symbol");
            exit(1);
        }
        str[len] = c;
        len++;
        str = (char *)realloc(str, (len + 1) * sizeof(char));
    }
    str[len] = '\0';
    return str;
}

int 
priority(char c) {
    if (c == '*' || c == '/') {
        return 3;
    }
    if (c == '+' || c == '-') {
        return 2;
    }
    if (c == '(') {
        return 1;
    }
    return 0;
}

int
is_variable_letter(char c) {
    if (isalpha(c) || c == '_') {
        return 1;
    }
    return 0;
}

int 
main(void) {
    char *str;  
    char *digits = "0123456789";
    char *math_operations = "+*-/";
    str = input_expression();
    int cur_ptr = 0;
    int first_variable_letter = 0;
    int br_counter = 0 , br_imbalance = 0 ;
    while (str[cur_ptr] != '\0') {

        // Ошибочная ситуация: ввели какой-то посторонний символ => сразу плохо
        // Пример: . или =
        if (!is_variable_letter(str[cur_ptr]) && strchr("()", str[cur_ptr]) == NULL
            && strchr(digits, str[cur_ptr]) == NULL 
            && strchr(math_operations, str[cur_ptr]) == NULL
            && strchr(whitespace_characters, str[cur_ptr]) == NULL) {
            perror("ERROR: Wrong symbol");
            exit(1);
        }

        // Ошибочная ситуация: после цифры следует буква (ошибочное имя переменной) 
        // или открывающая скобка, то есть пропущен знак математической операции
        // Пример: 9(a + b)
        if (strchr(digits, str[cur_ptr])) {
            while (strchr(whitespace_characters, str[cur_ptr])) {
                cur_ptr++;
            }

            if (is_variable_letter(str[cur_ptr + 1]) && first_variable_letter == 0) {
                perror("ERROR: Wrong variable name");
                exit(1);
            } 
            if (str[cur_ptr + 1] == '(') {
                perror("ERROR: Missed operation");
                exit(1);
            }
        }

        // Ситуцация: после знака мат. операции следует закрывающая скобка или ещё один знак операции
        // Примечание: я не реализовывала унарные минус и плюс, поэтому эта Ошибочная ситуация считается ошибочной
        // Пример: 1+-1 или (a+)
        if (strchr(math_operations, str[cur_ptr]) ) {
            while (strchr(whitespace_characters, str[cur_ptr])) {
                cur_ptr++;
            }
            first_variable_letter = 0; 
            // потому что встретился знак, а значит там дальше уже будет другая переменная

            if (strchr(math_operations,str[cur_ptr + 1])  || (str[cur_ptr + 1] == ')') || 
                    str[cur_ptr] == '\0') {
                perror("ERROR: No operand");
                exit(1);
            }
        }

        // Ошибочная ситуация: после открывающейся скобки идет знак мат. операции или закрывающаяся скобка
        // Пример: 1*(-4)
        if (str[cur_ptr] == '(') {
            br_counter++;

            while (strchr(whitespace_characters, str[cur_ptr])) {
                cur_ptr++;
            }

            if (strchr(math_operations, str[cur_ptr + 1]) || str[cur_ptr + 1] == ')') {
                perror("ERROR: Illegal operation");
                exit(1);
            }
        }

        // Ошибочная ситуация: после закрывающейся скобки идёт число или переменная 
        // (хотя может идти только знак мат. операции или закрывающая скобка)
        // Пример: (a+b)2
        if (str[cur_ptr] == ')') {
            if (br_counter == 0) {
                br_imbalance = 1 ; 
            }
            br_counter--;

            while (strchr(whitespace_characters, str[cur_ptr])) {
                cur_ptr++;
            }

            if (!strchr(math_operations, str[cur_ptr + 1]) && str[cur_ptr + 1] != ')') {
                if (str[cur_ptr + 1] == '\0' || str[cur_ptr + 1] == '\n'
                || str[cur_ptr + 1] == ' ') {
                    break;
                } else {
                    perror("ERROR: Missed operation");
                    exit(1);
                }
            }
        }   

        // Первая буква переменной, дальше будут разрешены цифры в имени
        if (is_variable_letter(str[cur_ptr])) {
            first_variable_letter = 1; 
        }

        cur_ptr++;
    }
    if (br_counter > 0) {
        perror("ERROR: Imbalance if brackets : '(' more than ')'");
        exit(1);
    }
    if (br_counter < 0) {
        perror("ERROR: Imbalance of brackets : ')' more than '('");
        exit(1);
    }
    if ((br_counter == 0) && (br_imbalance != 0)) {
        perror("ERROR: Imbalance of brackets");
        exit(1);
    }

    stack_type *head = init();
    int end_ptr = 0;
    cur_ptr = 0;
    char *tmp = (char *)malloc(sizeof(char));
    int num_of_variables = 0;
    int var_flag = 1;

    // разбираем строку, формируем ПОЛИЗ:
    while (str[cur_ptr] != '\0') {

        if (is_variable_letter(str[cur_ptr])) {  
            if (var_flag) {
                num_of_variables++;
            }
            var_flag = 0;
        }

        if (str[cur_ptr] == ')') {
            var_flag = 1;
            while (head->value != '(') {
                tmp[end_ptr] = pop(&head);
                end_ptr++;
                tmp = (char *)realloc(tmp, (end_ptr + 1) * sizeof(char));
                }
            pop(&head);
        }
        if (is_variable_letter(str[cur_ptr]) || strchr(digits, str[cur_ptr])) {  
            tmp[end_ptr] = str[cur_ptr];
            end_ptr++;
            tmp = (char *)realloc(tmp, (end_ptr + 1) * sizeof(char));
            if (!is_variable_letter(str[cur_ptr + 1]) && strchr(digits, str[cur_ptr + 1]) == NULL) {
                tmp[end_ptr] = ',';
                end_ptr++;
                tmp = (char *)realloc(tmp, (end_ptr + 1) * sizeof(char));
            }
            
        }
        if (str[cur_ptr] == '(')
            push(&head, str[cur_ptr]);
        if (strchr(math_operations, str[cur_ptr]) ) {
            var_flag = 1;
            if (head == NULL)
                push(&head, str[cur_ptr]);
            else {
                if (priority(str[cur_ptr]) > priority(head->value))
                    push(&head, str[cur_ptr]);
                else {
                    while (head  && (priority(str[cur_ptr]) <= priority(head->value))) {
                        tmp[end_ptr] = pop(&head);
                        end_ptr++;
                        tmp = (char *)realloc(tmp, (end_ptr + 1) * sizeof(char));
                    }
                    push(&head, str[cur_ptr]);
                }
            }
        }
        cur_ptr++;
    }
  
    while (head ) {
        tmp[end_ptr] = pop(&head);
        end_ptr++;
        tmp = (char *)realloc(tmp, (end_ptr + 1) * sizeof(char));
    }
    clear(head);
    tmp[end_ptr] = '\0';
    cur_ptr = 0;
    end_ptr = 0;
    int i;
    
    // Массивы:
    // if (num_of_variables != 0) { 
        char** var_name = malloc(sizeof(char*)*num_of_variables);
        long int *var_value = malloc(sizeof(long int)*num_of_variables);

        for (i = 0; i < num_of_variables; i++){
            var_value[i] = 0;
        }

        i = 0;
        //int len = 0;
        while (tmp[cur_ptr] != '\0') {

        if (is_variable_letter(tmp[cur_ptr])) {
            var_name[i] = (char *)malloc(sizeof(char));
            while((strchr("+*-/(), \t\v\f\r", tmp[cur_ptr]) == NULL) && tmp[cur_ptr] != '\0') {
                var_name[i][end_ptr] = tmp[cur_ptr];
                    end_ptr++;
                    var_name[i] = (char *)realloc(var_name[i], (end_ptr + 1) * sizeof(char));
                    cur_ptr++;
                }
                var_name[i][end_ptr] = '\0';

                if (strlen(var_name[i]) > 6) {
                    perror("ERROR: Variable name is too long");
                    exit(1);
                }
                i++;
                end_ptr = 0;
            }

            cur_ptr++;
        }

        qsort(var_name, num_of_variables, sizeof(var_name[0]), compar);

        int new_num_of_variables = num_of_variables;
        // Считаю дубликаты
        for (i = 0; i < num_of_variables - 1; i++) {
            if (!strcmp(var_name[i],var_name[i + 1])) {
                new_num_of_variables--; 
            }
        }

        char cur_name[7] = "\0";
        int j = 0;
        for (i = 0; i < num_of_variables; i++) {
            if (strcmp(cur_name, var_name[i])) {
                while (var_name[i][j] != '\0') {
                    cur_name[j] = var_name[i][j];
                    j++;
                }
                j = 0;
                //printf("%s = ", var_name[i]);
                
                if (scanf("%ld", &var_value[i]) <= 0) {
                    perror("ERROR: Wrong variable initilization");
                    exit(2);
                }
                int c;
                if ((c = getchar()) == EOF) {
                    perror("ERROR: Wrong variable initilization");
                    exit(2);
                }
                if (var_value[i] < INT_MIN || var_value[i] > INT_MAX) {
                    perror("ERROR: Value is bigger, than int type");
                    exit(3);
                }
            }
        }
    // }

    cur_ptr = 0;
    end_ptr = 0;
    free(str);
    elem_t *int_head = int_init();
    long int first, second, value = 0, result = 0; // использую тип long int, чтобы понять, помещаются ли значения в int
    while (tmp[cur_ptr] != '\0') {
        if (strchr(digits, tmp[cur_ptr]) ) {
            str = (char *)malloc(sizeof(char));
            while ((strchr(",+*-/() ", tmp[cur_ptr]) == NULL) && tmp[cur_ptr] != '\0') {
                str[end_ptr] = tmp[cur_ptr];
                end_ptr++;
                str = (char *)realloc(str, (end_ptr + 1) * sizeof(char));
                cur_ptr++;    
            }
            str[end_ptr] = '\0';
            sscanf(str, "%ld", &value);

            if (value < INT_MIN || value > INT_MAX) {
                perror("ERROR: Value is bigger, than int type");
                exit(3);
            }

            int_push(&int_head, value);
            end_ptr = 0;
            free(str);
            if (tmp[cur_ptr] == '\0') break;
        } else if (is_variable_letter(tmp[cur_ptr])) {
            str = (char *)malloc(sizeof(char));
            while((strchr("+*-/(), ", tmp[cur_ptr]) == NULL) && tmp[cur_ptr] != '\0') {
                str[end_ptr] = tmp[cur_ptr];
                end_ptr++;
                str = (char *)realloc(str, (end_ptr + 1) * sizeof(char));
                cur_ptr++;
            }
            str[end_ptr] = '\0';
            for (i = 0; i < num_of_variables; i++) {
                if (!strcmp(var_name[i], str)) {
                    value = var_value[i];
                    break;
                }
            }

            int_push(&int_head, value);
            end_ptr = 0;
            free(str);
        } else if (strchr(math_operations, tmp[cur_ptr]) ) {
            first = int_pop(&int_head);
            second = int_pop(&int_head);

            if (tmp[cur_ptr] == '+') {
                result = first + second;
            }
            if (tmp[cur_ptr] == '-') {
                result = second - first;
            }
            if (tmp[cur_ptr] == '*') {
                result = first * second;
            }
            if (tmp[cur_ptr] == '/') {
                if (first == 0) {
                    perror("ERROR: Division by zero");
                    exit(3);
                }
                result = second / first;
            }

            if (result < INT_MIN || result > INT_MAX) {
                perror("ERROR: Value is bigger, than int type");
                exit(3);
            }
            int_push(&int_head, result);
            cur_ptr++;
        } else
            cur_ptr++;
    }
    printf("%d\n", int_pop(&int_head));
    free(tmp);
    free(var_name);
    free(var_value);
    int_clear(int_head);
    return 0;
}
