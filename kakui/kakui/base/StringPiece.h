#pragma once

#include "Types.h"

#include <string.h>
#include <iosfwd>
#include <string>

namespace kakui
{
	
class StringArg
{
public:
	StringArg(const char* str): _str(str) {}

	StringArg(const string& str): _str(str.c_str()) {}

	const char* c_str() const { return _str; }

private:
	const char* _str;
};

class StringPiece
{
private:
	const char* _ptr;
	int _len;

public:
	StringPiece(): _ptr(NULL), _len(0) {}

	StringPiece(const char* str): _ptr(str), _len(static_cast<int>(strlen(_ptr))) {}

	StringPiece(const unsigned char* str): _ptr(reinterpret_cast<const char*>(str)), _len(static_cast<int>(strlen(_ptr))) {}

	StringPiece(const string& str): _ptr(str.data()), _len(static_cast<int>(str.size())) {}

	StringPiece(const char* offset, int len): _ptr(offset), _len(len) {}

	const char* data() const { return _ptr; }
	int size() const { return _len; }
	bool empty() const { return _len == 0; }
	const char* begin() const { return _ptr; }
	const char* end() const { return _ptr + _len; }

	void clear() { _ptr = NULL; _len = 0; }
	void set(const char* buffer, int len) { _ptr = buffer; _len = len; }
	void set(const char* str) { _ptr = str; _len = static_cast<int>(strlen(str)); }
	void set(const void* buffer, int len) { _ptr = reinterpret_cast<const char*>(buffer); _len = len; }

	char operator[](int i) const { return _ptr[i]; }
	bool operator==(const StringPiece& sp) const { return ((_len ==  sp._len) && (memcmp(_ptr, sp._ptr, _len) == 0)); }
	bool operator!=(const StringPiece& sp) const { return !(*this == sp); }

	void removePrefix(int n) { _ptr += n; _len -= n; }
	void removeSuffix(int n) { _len -= n; }

	#define STRINGPIECE_COMPARE_TEMPLATE(cmp, auxcmp)					\
	bool operator cmp (const StringPiece& sp) const {					\
		int r = memcmp(_ptr, sp._ptr, _len < sp._len ? _len : sp._len);	\
		return ((r auxcmp 0) || ((r == 0) && (_len cmp sp._len)));		\
	}

	STRINGPIECE_COMPARE_TEMPLATE(<, <);
	STRINGPIECE_COMPARE_TEMPLATE(<=, <);
	STRINGPIECE_COMPARE_TEMPLATE(>, >);
	STRINGPIECE_COMPARE_TEMPLATE(>=, >);
	#undef STRINGPIECE_COMPARE_TEMPLATE

	string toString() const
	{
		return string(data(), size());
	}

	void copyToString(string* target) const 
	{
		target->assign(_ptr, _len);
	}

	bool startsWith(const StringPiece& sp) const
	{
		return ((_len >= sp._len) && (memcmp(_ptr, sp._ptr, sp._len) == 0));
	}

	bool endsWith(const StringPiece& sp) const
	{
		return ((_len >= sp._len) && (memcmp(_ptr+(_len-sp._len), sp._ptr, sp._len) == 0));
	}
};

}

#ifdef HAVE_TYPE_TRAITS
template<> struct __type_traits<kakui::StringPiece> 
{
	typedef __true_type has_trivial_default_constructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type has_trivial_destructor;
	typedef __true_type is_POD_type;
};
#endif

std::ostream& operator<<(std::ostream& o, const kakui::StringPiece& piece);