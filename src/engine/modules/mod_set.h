/*
Copyright 2019-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)
*/

#ifndef B2_MOD_SET_H
#define B2_MOD_SET_H

#include "../config.h"

#include "../bind.h"
#include "../lists.h"
#include "../optval.h"
#include "../value.h"

#include <string>
#include <unordered_set>
#include <vector>

/* tag::set[]

= `string` module.

end::set[] */

namespace b2 {

/* tag::set[]
== `b2::set`

Set of strings.
end::set[] */
class set : public object
{
	public:
	/* tag::set[]
	== `b2::set::add`

	Set of strings.

	Jam::
	`rule add ( elements * )`
	end::set[] */
	void add(list_cref values);

	void add(const set & value);

	/* tag::set[]
	== `b2::set::contains`

	Does the set contain the given `value`.

	Jam::
	`rule contains ( element )`
	end::set[] */
	bool contains(value_ref value) const;

	/* tag::set[]
	== `b2::set::to_list`

	Return a list with all the elements of the set.

	Jam::
	`rule list ( )`
	end::set[] */
	list_ref to_list() const;

	/* tag::set[]
	== `b2::set::difference`

	Returns the elements of `a` that are not in `b`.

	Jam::
	`rule difference ( set1 * : set2 * )`
	end::set[] */
	static list_ref difference(list_cref a, list_cref b);

	/* tag::set[]
	== `b2::set::intersection`

	Removes all the items appearing in both `a` & `b`.

	Jam::
	`rule intersection ( set1 * : set2 * )`
	end::set[] */
	static list_ref intersection(list_cref a, list_cref b);

	/* tag::set[]
	== `b2::set::equal`

	Returns whether `a` & `b` contain the same elements. Note that this
	ignores any element ordering differences as well as any element
	duplication.

	Jam::
	`rule equal ( set1 * : set2 * )`
	end::set[] */
	static bool equal(list_cref a, list_cref b);

	private:
	using value_set = std::unordered_set<value_ref,
		value_ref::hash_function,
		value_ref::equal_function>;

	value_set elements;
};

struct set_module : b2::bind::module_<set_module>
{
	const char * module_name = "set";

	template <class Binder>
	void def(Binder & binder)
	{
		binder.def(&set::difference, "difference", "set1" * _n | "set2" * _n)
			.def(&set::intersection, "intersection", "set1" * _n | "set2" * _n)
			.def(&set::equal, "equal", "set1" * _n | "set2" * _n);
		binder.def_class("set", type_<set>())
			.def(init_<>())
			.def(static_cast<void (set::*)(b2::list_cref)>(&set::add), "add",
				"elemets" * _n)
			.def(&set::contains, "contains", "element" * _1)
			.def(&set::to_list, "list");
	}
};

} // namespace b2

#endif
