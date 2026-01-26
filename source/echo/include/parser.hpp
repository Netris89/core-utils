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

#pragma Once

#include <cstddef>
#include <string>

/**
 * @class Parser
 * @brief A utility class for interpreting and converting escape sequences within input strings.
 *
 * The Parser class provides methods to process strings containing C++-style escape sequences,
 * including both standard escape characters (e.g., `\n`, `\t`) and octal escape sequences (e.g., `\012`).
 *
 * It offers a public interface through `ParseArgument()` that parses and converts all valid escape
 * sequences in a given string into their corresponding ASCII characters.
 *
 * Internally, it uses `ParseEscapeCharacter()` to identify and interpret individual escape codes, and
 * `ParseOctal()` to specifically handle octal escape formats.
 *
 * Example usage:
 * @code
 * Parser parser;
 * std::string parsed = parser.ParseArgument("Hello\\nWorld\\041");
 * parsed == "Hello\nWorld!"
 * @endcode
 *
 * @note Octal escapes must begin with `\0` and contain up to three octal digits (0–7).
 *
 * @see Parser::ParseArgument
 * @see Parser::ParseEscapeCharacter
 * @see Parser::ParseOctal
 */
class Parser
{
private:
    static constexpr int OCTAL   = 8;
    static constexpr int MAXSIZE = 5;

    /**
     * @brief Converts an octal escape sequence from the input string into its corresponding ASCII character.
     *
     * This method expects an input string starting with an octal escape sequence (e.g., `\012`).
     * It removes the first two characters (assumed to be the prefix \0`), then parses the remaining
     * digits as an octal number and converts it to the corresponding ASCII character.
     *
     * @param argument A string starting with a prefix followed by octal digits (e.g., "\012").
     * @return A string containing the single character represented by the octal value.
     */
    auto static ParseOctal(const std::string&, size_t) -> std::string;

    /**
     * @brief Parses a single escape sequence in the input string and returns its corresponding character.
     *
     * This method examines the character at the next index of the input string
     * and attempts to interpret it as a valid escape sequence. If the character is recognized
     * as a standard C++ escape code (e.g., `\n`, `\t`, `\0`), it is translated into the corresponding
     * character. Octal sequences starting with `\0` are delegated to the `ParseOctal` method.
     *
     * Supported escape sequences:
     * - `\a` : Alert (bell)
     * - `\b` : Backspace
     * - `\f` : Form feed
     * - `\n` : Newline
     * - `\r` : Carriage return
     * - `\t` : Horizontal tab
     * - `\v` : Vertical tab
     * - `\\` : Backslash
     * - `\0nnn` : Octal escape (handled via `ParseOctal`)
     *
     * If the character following the backslash is not recognized, the method returns an empty string.
     *
     * @param argument The input string that may contain an escape sequence.
     * @return A string containing the corresponding interpreted character, or an empty string if unrecognized.
     *
     * @see Parser::ParseOctal
     */
    auto static ParseEscapeCharacter(const std::string&, size_t) -> std::string;

public:
    Parser();

    /**
     * @brief Parses an input string containing escape sequences and returns the interpreted result.
     *
     * This method processes the input string character by character. When a backslash (`\\`) is encountered,
     * it attempts to parse the following characters as an escape sequence using `ParseEscapeCharacter()`.
     * Otherwise, the character is appended directly to the output string.
     *
     * The method supports both standard escape sequences (e.g., newline, tab) and octal escape sequences
     * starting with `\0`.
     *
     * @param argument The input string that may include escape sequences.
     * @return A string where all valid escape sequences are interpreted and replaced by their actual characters.
     *
     * @see Parser::ParseEscapeCharacter
     */
    auto static ParseArgument(const std::string&) -> std::string;
};
