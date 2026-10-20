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
#include <vector>

#include "parser.hpp"

using std::stoi;
using std::string;
using std::vector;

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
    for (size_t i = position; i < argument.size() && seqToParse.size() <= 4; i++)
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
            endSeq += seqToParse.substr(octalSeq.size());
            break;
        }
    }

    if (octalSeq.size() > 1)
    {
        octalSeq.erase(0, 2); // Removes '\\' & '0' from the sequence

        if (!octalSeq.empty())
        {
            decimal   = stoi(octalSeq, nullptr, OCTAL);      // transforms the octal number in decimal
            asciiChar = static_cast<char>(decimal) + endSeq; // Converts the decimal number in ascii char then adds anything remaining at the end
        }
        else
        {
            asciiChar = '\0';
            asciiChar += endSeq;
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
        // Nothing to append: ParseArgument stops the parsing of the argument on \c,
        // but this case prevents the default branch from outputting the sequence itself
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

auto Parser::ParseArgument(const string& argument, bool* suppressNewline) -> string
{
    string parsedArg; // Parsed argument being constructed

    // Copies each character as is, except backslashes: a backslash is either a lone trailing one (kept literally),
    // the start of an escape sequence, or the first half of a pair of backslashes
    for (size_t i = 0; i < argument.size(); i++)
    {
        // If the current character is not a backslash, just adds it to the output string being constructed
        if (argument.at(i) != '\\')
        {
            parsedArg += argument.at(i);
            continue;
        }

        // A backslash in last position has nothing to escape, so it is kept literally.
        // This also guarantees that argument.at(i + 1) below is valid
        if (i == argument.size() - 1)
        {
            parsedArg += '\\';
            continue;
        }

        // The backslash is followed by something other than a backslash: it starts an escape sequence
        if (argument.at(i + 1) != '\\')
        {
            // Converts the escape sequence, unknown sequences are kept as is
            parsedArg += ParseEscapeCharacter(argument, i);

            // If the next character is 0, it is an octal sequence and we need to advance the position counter of the size of the sequence
            if (argument.at(i + 1) == '0')
            {
                size_t position = i + 2; // Stores the index of the first character following the escape prefix.
                size_t start    = i;     // Stores the starting index of the escape sequence.
                size_t maxOctal = 3;     // Limits the octal escape sequence to three digits.

                // Skips at most three octal digits to avoid processing them again.
                while (position < argument.size() && isdigit(argument.at(position)) != 0)
                {
                    // TODO : Reduce cognitive complexity
                    if (position <= start + maxOctal)
                    {
                        i++;
                        position++;
                    }
                    else
                    {
                        i++;
                        break;
                    }
                }

                // Skips the '0' itself, so the for loop resumes right after the sequence
                i++;
            }
            // \c stops the parsing of this argument: everything after it is dropped,
            // and the caller is told that the final newline must be suppressed
            else if (argument.at(i + 1) == 'c')
            {
                if (suppressNewline != nullptr)
                {
                    *suppressNewline = true;
                }

                return parsedArg;
            }
            // Regular escape: ParseEscapeCharacter handled both characters, so skips the escape letter
            else
            {
                i++;
            }
        }
        else
        {
            // The backslash is followed by another backslash (an escaped backslash): appends a single one
            // and skips the second so it is not processed again as the start of a new sequence
            parsedArg += '\\';
            i++;
        }
    }

    return parsedArg;
}

auto Parser::ParseArguments(const vector<string>& arguments) -> string
{
    string arg;                   // Complete output being built: the parsed arguments separated by single spaces, then a newline
    int index            = 0;     // Position of the current argument, used to know whether it is the last one
    bool suppressNewline = false; // Set by ParseArgument when it meets \c, which ends the output without a final newline

    // Goes through the arguments in order: each one has its escape sequences interpreted by ParseArgument
    // and is appended to the output, followed by a single space unless it is the last argument.
    // The loop stops as soon as an argument contains \c: the remaining arguments are ignored
    for (const string& argument : arguments)
    {
        // Interprets the escape sequences of this argument and appends the result to the output
        arg += Parser::ParseArgument(argument, &suppressNewline);

        if (suppressNewline)
        {
            return arg;
        }

        // POSIX: arguments are separated by a single space, and there is none after the last one
        if (index < arguments.size() - 1)
        {
            arg += ' ';
        }

        index++;
    }

    // A newline always follows the last argument. With no argument the loop does not run,
    // so only the newline is written, which is what POSIX requires
    arg += '\n';

    return arg;
}
