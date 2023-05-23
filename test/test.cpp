#include <ctest.h>

#include <fstream>
#include <gtk/gtk.h>
#include <iostream>
#include <string>

#include <libsheet/functions.h>

CTEST(checking_text_in_entry_true, test1)
{
    std::string str = "A mojno 5 za kyrsovyu?";
    const int expected = 0;
    const int result = checking_entry(str);
    ASSERT_EQUAL(expected, result);
}

CTEST(checking_text_in_entry_false, test2)
{
    std::string str = "";
    const int expected = 1;
    const int result = checking_entry(str);
    ASSERT_EQUAL(expected, result);
}

CTEST(checking_text_in_entry_false_2, test3)
{
    std::string str = "Ошибка! Поле ввода пустое!";
    const int expected = 1;
    const int result = checking_entry(str);
    ASSERT_EQUAL(expected, result);
}

CTEST(checking_is_open_file_true, test4)
{
    std::string File_name = "tasks.txt";
    std::string str = "Ошибка! Поле ввода пустое!";
    const int expected = 0;
    const int result = checking_is_open_file(File_name);
    ASSERT_EQUAL(expected, result);
}

CTEST(check_text_in_entry_false_, test5)
{
    std::string File_name = "false.txt";
    std::string str = "Ошибка! Поле ввода пустое!";
    const int expected = 1;
    const int result = checking_is_open_file(File_name);
    ASSERT_EQUAL(expected, result);
}