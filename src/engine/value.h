/*
 * Copyright 2022 René Ferdinand Rivera Morell
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)
 */

#ifndef BOOST_JAM_VALUE_H
#define BOOST_JAM_VALUE_H

#include "config.h"

#include <cstdint>
#include <cstring>
#include <string>

namespace b2 {

struct object
{
	virtual ~object() {}
};

struct value
{
	struct str_view
	{
		const char * str;
		std::size_t size;
	};

	enum class type : char
	{
		null = '\0',
		string = 's',
		number = 'n',
		object = 'o'
	};

	mutable std::uint64_t hash64 = 0;

	virtual ~value() {}
	virtual type get_type() const = 0;
	virtual bool equal_to(const value & o) const = 0;
	virtual str_view as_string() const = 0;
	virtual double as_number() const = 0;
	virtual object * as_object() const = 0;
	inline const char * str() { return as_string().str; }
	static value * make(const char * str, std::size_t size);
	static inline value * make(const char * str)
	{
		return make(str, strlen(str));
	}
	static value * make(object * obj);
	static value * make(double v);
	static inline value * copy(value * v) { return v; }
	static inline void free(value *& v) { v = nullptr; }
	static void done();
};

typedef value * value_ptr;

struct value_ref
{
	inline value_ref(const value_ref & a)
		: val(value::copy(a.val))
	{}

	inline explicit value_ref(value_ptr a)
		: val(value::copy(a))
	{}
	inline explicit value_ref(const char * s)
		: val(value::make(s))
	{}
	inline explicit value_ref(const std::string & s)
		: val(value::make(s.c_str()))
	{}

	inline ~value_ref()
	{
		if (val) value::free(val);
	}
	inline value_ptr release()
	{
		value_ptr r = val;
		val = nullptr;
		return r;
	}

	inline operator value_ptr() const { return val; }
	inline operator std::string() const { return val->str(); }
	inline bool operator==(value_ptr b) const { return val->equal_to(*b); }
	inline value_ptr operator->() const noexcept { return val; };

	private:
	value_ptr val = nullptr;
};

} // namespace b2

#endif
