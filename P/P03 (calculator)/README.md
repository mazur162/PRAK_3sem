# P03

В данной программе необходимо было реализовать кулькулятор. Я выбрала целочисленный (int) вариант.
Унарные операции +- я не делала.
Сначала я читаю выражение, потом анализирую его => если что-то не так, то на стандартный поток ошибок подаю сообщение и завершаю программу с кодом, который требуется по условию.
С помощью многочисленных проверок я выделяю переменные, числа и знаки мат. операций. Переменные запоминаю в массиве, чтобы потом попросить пользователя проинициализировать переменные в лексикографическом порядке, а не по порядку следования. Для этого я сортирую массив переменных с помощью qsort (на одном из семинарских занятий мы уже писали компаратор для сравнения строк и дальнейшей сортировки).

Позже строка-выражение анализируется, отдельные составляющие (числа, значения переменных и знаки) пушатся в стек (ПОЛИЗ).
Потом поочерёдно вычисляются значения, и мы приходим к правильному ответу.
