/*
Updated 2022-01-12

For every header file in the project, we try to respect the following
declaration order convention:

public | protected | private:
	
	members:
		in no specific order.

	constructors:
		from "simple" to "complicated"

	destructors:
		from "simple" to "complicated"

	methods:
		accessors:
			alphabetical order
		mutators:
			alphabetical order
		logic:
			alphabetical order

	Enclose accessors, mutators and logic in dedicated
	regions.

*/