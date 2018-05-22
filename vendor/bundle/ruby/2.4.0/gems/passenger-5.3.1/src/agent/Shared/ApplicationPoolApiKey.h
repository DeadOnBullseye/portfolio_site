/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2015-2017 Phusion Holding B.V.
 *
 *  "Passenger", "Phusion Passenger" and "Union Station" are registered
 *  trademarks of Phusion Holding B.V.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */
#ifndef _PASSENGER_APPLICATION_POOL2_API_KEY_H_
#define _PASSENGER_APPLICATION_POOL2_API_KEY_H_

#include <oxt/macros.hpp>
#include <string>
#include <cstring>
#include <Exceptions.h>
#include <StaticString.h>
#include <Utils/StrIntUtils.h>

namespace Passenger {
namespace ApplicationPool2 {

using namespace std;


/**
 * An API key is a string that determines which app a particular ApiServer
 * client may access. It is automatically generated by a Group and passed to
 * all application processes that it spawns. Application processes can then
 * use that API key to perform API calls by connecting to the ApiServer.
 *
 * An API key is transient, not persistent. That is, it is only valid during
 * the life time of a Group. It is not pre-generated by the user.
 *
 * An API key is unique per Pool, so may be used to uniquely identify a Group
 * object within a Pool.
 *
 * An API key may be null, which means that it's not given.
 *
 * There also exists a special "super" API key. This API key doesn't have a
 * particular value, so it cannot be used to uniquely identify a Group. Instead,
 * its purpose is to signal that the client is authorized to access all apps in
 * the Pool. Super API keys are associated with ApiServer clients that are
 * authenticated with one of the API accounts.
 */
class ApiKey {
public:
	static const unsigned int SIZE = 16;

private:
	/*
	 * On x86_64 the value is as big as a StaticString,
	 * so might as well directly embed the data instead.
	 */
	char value[SIZE];

	OXT_FORCE_INLINE
	bool validateNonSuperKeyValueChar(const char ch) const {
		return (ch >= 'a' && ch <= 'z')
			|| (ch >= 'A' && ch <= 'Z')
			|| (ch >= '0' && ch <= '9');
	}

	void validateNonSuperKeyValue(const StaticString &value) const {
		const char *data = value.data();
		const char *end  = value.data() + value.size();
		while (data < end) {
			if (!validateNonSuperKeyValueChar(*data)) {
				throw ArgumentException("API keys may only contain the characters a-z, A-Z and 0-9.");
			}
			data++;
		}
	}

public:
	ApiKey() {
		value[0] = '\0';
	}

	ApiKey(const StaticString &other) {
		if (other.size() != SIZE) {
			throw ArgumentException("API keys must be exactly "
				+ Passenger::toString(SIZE) + " characters in length");
		}
		validateNonSuperKeyValue(other);
		memcpy(value, other.data(), SIZE);
	}

	ApiKey(const ApiKey &other) {
		memcpy(value, other.value, SIZE);
	}

	static ApiKey makeSuper() {
		ApiKey key;
		key.value[0] = '!';
		assert(key.isSuper());
		return key;
	}

	bool isNull() const {
		return value[0] == '\0';
	}

	bool isSuper() const {
		return value[0] == '!';
	}

	bool constantTimeCompare(const StaticString &other) const {
		return Passenger::constantTimeCompare(toStaticString(), other);
	}

	bool constantTimeCompare(const ApiKey &other) const {
		return Passenger::constantTimeCompare(toStaticString(), other.toStaticString());
	}

	string toString() const {
		return toStaticString().toString();
	}

	StaticString toStaticString() const {
		if (value[0] == '\0') {
			return StaticString();
		} else if (value[0] == '!') {
			return StaticString("superkey");
		} else {
			return StaticString(value, SIZE);
		}
	}

	bool operator==(const StaticString &other) const {
		return other.size() == SIZE && memcmp(value, other.data(), SIZE) == 0;
	}

	bool operator==(const ApiKey &other) const {
		return memcmp(value, other.value, SIZE) == 0;
	}
};


} // namespace ApplicationPool2
} // namespace Passenger

#endif /* _PASSENGER_APPLICATION_POOL2_API_KEY_H_ */
