#pragma once

/**
 * Checks the passed in array for strings starting with $ and replaces
 * them with their actual values.
 * @param args The array to loop through and check the strings
 * @return void
 */
void expand_env_vars(char* args[]);
