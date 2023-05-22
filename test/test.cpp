#include <ctest.h>

#include <fstream>
#include <gtk/gtk.h>
#include <iostream>
#include <string>

#include <libsheet/functions.h>

CTEST(check_text_in_entry_true, test1)
{
    std::string str = "A mojno 5 za kyrsovyu?";
    const int expected = 0;
    const int result = checking_for_errors(str);
    ASSERT_EQUAL(expected, result);
}

CTEST(check_text_in_entry_false, test2)
{
    std::string str = "";
    const int expected = 1;
    const int result = checking_for_errors(str);
    ASSERT_EQUAL(expected, result);
}
