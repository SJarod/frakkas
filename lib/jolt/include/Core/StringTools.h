// SPDX-FileCopyrightText: 2021 Jorrit Rouwe
// SPDX-License-Identifier: MIT

#pragma once

namespace JPH {

/// Create a formatted text string
string StringFormat(const char *inFMT, ...);

/// Convert type to string
template<typename T>
string ConvertToString(const T &inValue)
{
    ostringstream oss;
    oss << inValue;
    return oss.str();
}

/// Calculate the FNV-1a hash of inString.
/// @see https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
constexpr uint64 HashString(const char *inString)
{
	uint64 hash = 14695981039346656037UL;
	for (const char *c = inString; *c != 0; ++c)
	{
		hash ^= *c;
		hash = hash * 1099511628211UL;
	}
	return hash;
}

/// Replace substring with other string
void StringReplace(string &ioString, string inSearch, string inReplace);

/// Convert a delimited string to an array of strings
void StringToVector(const string &inString, vector<string> &outVector, const string &inDelimiter = ",", bool inClearVector = true);

/// Convert an array strings to a delimited string
void VectorToString(const vector<string> &inVector, string &outString, const string &inDelimiter = ",");

/// Convert a string to lower case
string ToLower(const string &inString);

/// Converts the lower 4 bits of inNibble to a string that represents the number in binary format
const char *NibbleToBinary(uint32 inNibble);

} // JPH