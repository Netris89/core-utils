/*
 *  Copyright (c) 2025, Ludovic Hansen
 *  License: MIT
 *
 *  Description:
 *  This application implements a custom version of the `echo` command in C++.
 *  It handles various escape sequences, such as \a (alert), \b (backspace),
 *  \n (newline), \t (tab), etc. It prints the command-line arguments with the
 *  escape sequences interpreted correctly.
 *
 *  Usage: ./echo <string>
 *
 *  The following escape sequences are supported:
 *      \a      : Alert
 *      \b      : Backspace
 *      \c      : Suppress the <newline> that otherwise follows the final argument in the output. All characters following the '\c' in the arguments shall be ignored.
 *      \f      : Form-feed
 *      \n      : Newline
 *      \t      : Tab
 *      \r      : Carriage return
 *      \v      : Vertical tab
 *      \\      : Backslash
 *      \0num   : 8-bit octal value (num)
 *
 *  Note: More details on options and behavior can be found here:
 *  https://pubs.opengroup.org/onlinepubs/9799919799/utilities/echo.html
 */

#include <cctype>
#include <cstddef>
#include <string>

#include "parser.hpp"

using std::stoi;
using std::string;

Parser::Parser() = default;

auto Parser::ParseOctal(const string& argument, size_t position) -> string
{
    int decimal        = 0; // Decimal value converted from octal
    int backslashIndex = 0; // Position of the \\ in the escape sequence
    string seqToParse;      // Extracted escape sequence to be parse
    string octalSeq;        // Result of the parsing
    string endSeq;          // Anything left after the sequence
    string asciiChar;       // Resulting ASCII character from octal conversion

    // Loops through the whole argument to find where the octal escape sequence is and put it whole in a variable
    for (size_t i = position; i < argument.size() && seqToParse.size() < 4; i++)
    {
        if (argument.at(i) == '\\' || argument.at(i) == '0' || isdigit(argument.at(i)) != 0)
        {
            seqToParse += argument.at(i);
        }
    }

    // Loops through the whole parsed sequence to separate the octal sequence (max 3 numbers) & anything that could be something else after
    for (char character : seqToParse)
    {
        if (character == '\\' || character - '0' < OCTAL)
        {
            octalSeq += character;
        }
        else
        {
            endSeq += character;
        }
    }

    if (octalSeq.size() > 1)
    {
        octalSeq.erase(0, 2); // Removes '\\' & '0' from the sequence

        if (octalSeq.size() != 0)
        {
            decimal   = stoi(octalSeq, nullptr, OCTAL);      // transforms the octal number in decimal
            asciiChar = static_cast<char>(decimal) + endSeq; // Converts the decimal number in ascii char then adds anything remaining at the end
        }
        else if (endSeq.size() == 0)
        {
            asciiChar = "\\0";
        }
    }
    else
    {
        return octalSeq;
    }

    return asciiChar;
}

auto Parser::ParseEscapeCharacter(const string& argument, size_t position) -> string
{
    string parsedEscape; // Result escape sequence after being parse

    switch (argument.at(position + 1))
    {
    case 'a':
        parsedEscape += '\a';
        break;
    case 'b':
        parsedEscape += '\b';
        break;
    case 'c':
        break;
    case 'f':
        parsedEscape += '\f';
        break;
    case 'n':
        parsedEscape += '\n';
        break;
    case 'r':
        parsedEscape += '\r';
        break;
    case 't':
        parsedEscape += '\t';
        break;
    case 'v':
        parsedEscape += '\v';
        break;
    case '\\':
        parsedEscape += '\\';
        break;
    case '0':
        parsedEscape += ParseOctal(argument, position);
        break;
    default:
        parsedEscape += argument.at(position);
        parsedEscape += argument.at(position + 1);
        break;
    }

    return parsedEscape;
}

auto Parser::ParseArgument(const string& argument) -> string
{
    string parsedArg; // Parsed argument being constructed

    for (size_t i = 0; i < argument.size(); i++)
    {
        // If the current character is not \\, just adds it to the output string being constructed
        if (argument.at(i) != '\\')
        {
            parsedArg += argument.at(i);
            continue;
        }

        if (argument.back() == '\\')
        {
            parsedArg += '\\';
            continue;
        }

        // If the current character is not \\ and is not the last character, parse it
        if (argument.at(i + 1) != '\\' && argument.back() != '\\')
        {
            parsedArg += ParseEscapeCharacter(argument, i);

            // If the next character is 0, it is an octal sequence and we need to advance the position counter of the size of the sequence
            if (argument.at(i + 1) == '0')
            {
                size_t position = i + 2;

                while (position < argument.size() && isdigit(argument.at(position)) != 0)
                {
                    if (isdigit(argument.at(position)) == 0)
                    {
                        break;
                    }

                    i++;
                    position++;
                }

                i++;
            }
            else if (argument.at(i + 1) == 'c') // If the next character is C, it is an interrupt sequence so returns the parsed argument as it is
            {
                return parsedArg;
            }
            else
            {
                i++;
            }
        }
        else
        {
            parsedArg += "\\";
        }
    }

    return parsedArg;
}
